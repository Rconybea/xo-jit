/** @file buffer.hpp **/

#pragma once

#include "span.hpp"
#include <utility>
#include <cstdint>
#include <cassert>
#include <new>

namespace xo {
    namespace scm {
        /**
         * @class buffer buffer.hpp
         *
         * @brief Container for a (possibly owned) FIFO queue of chars
         *
         * @tparam CharT.  buffer element type.
         *
         * @code
         *  .buf
         *
         *    +------------------------------------------+
         *    |  |  ...  |  | X|  ... | X|  |    ...  |  |
         *    +------------------------------------------+
         *     ^             ^            ^               ^
         *     0             .lo          .hi             .buf_z
         *
         *                   <-contents-><----avail----->
         * @endcode
         *
         * Buffer does not support wrapped content:
         * content that has not been consumed always occupies contiguous memory.
         *
         * Example:
         * @code
         * // 1.
         *   buffer<char> buf(64*1024);
         *   buf.empty() -> true
         *   buf.buf_z() -> 65536
         *   buf.lo_pos() -> 0
         *   buf.hi_pos() -> 65536
         *   buf.contents() -> empty span
         *   buf.avail() -> span entire buffer memory
         *
         *   // write to (a prefix of) buf.avail()
         *   ::strncpy(buf.buf(), "hello, world\n", 13);
         *   buf.produce(span_type(buf.buf(), buf.buf() + 13));
         *
         *   buf.lo_pos() -> 0
         *   buf.hi_pos() -> 13
         *   buf.contents() -> "hello, world\n";
         *
         *
         *   // examine stored content (does not change buffer state)
         *   auto span = buf.contents();
         *   cerr << string_view(span.lo(), span.hi());  // "hello, world\n"
         *
         *   // consume (a prefix of) stored content
         *   buf.consume(span.prefix(7);
         *
         *   buf.lo_pos() -> 7
         *   buf.hi_pos() -> 13
         *   buf.contents() -> "world\n"
         *
         *   // consuming all remain content resets to original state
         *   buf.consume(buf.contents());
         *
         *   buf.empty() -> true
         *   buf.hi_pos() -> 0     // not 13!
         *
         * // 2.
         *   buffer<char> buf;
         *   buf.empty() -> true
         *   buf.buf_z() -> 0
         *   buf.lo_pos() -> 0
         *   buf.hi_pos() -> 0
         *   buf.contents() -> empty span
         *   buf.avail() -> empty span
         *
         *   // allocate memory separately from ctor
         *   buf.alloc(64*1024);
         * @endcode
         **/
        template <typename CharT>
        class buffer {
        public:
            /** @brief typealias for span of CharT **/
            using span_type = span<CharT>;
            /** @brief typealias for buffer size (counts CharT's, not bytes) **/
            using size_type = std::uint64_t;

        public:
            /** @brief create empty buffer.

                Does not allocate any storage;  @see alloc
            **/
            buffer() = default;
            /** @brief create empty buffer,  and possibly allocate storage.

                @param buf_z    Buffer size.  allocate storage (owned by this buffer) if >0.
                @param align_z  Align to this value,  e.g. 8 to align storage on an 8-byte boundary
            **/
            buffer(size_type buf_z,
                   size_type align_z = sizeof(char))
                : is_owner_{true},
                  buf_{buf_z ? (new (std::align_val_t(align_z)) CharT [buf_z]) : nullptr},
                  buf_z_{buf_z},
                  lo_pos_{0},
                  hi_pos_{0}
                {}
            /** @brief buffer is not copyable **/
            buffer(buffer const & x) = delete;
            /** @brief destructor.  Release storage if owned **/
            ~buffer() { this->reset(); }

            /** @name Access methods **/
            ///@{

            /** @brief start of buffer memory **/
            CharT * buf() const { return buf_; }
            /** @brief buffer size (number of characters) **/
            size_type buf_z() const { return buf_z_; }
            /** @brief current start position within buffer **/
            size_type lo_pos() const { return lo_pos_; }
            /** @brief current end position within buffer **/
            size_type hi_pos() const { return hi_pos_; }

            ///@}

            /** @brief readonly access to a single buffer element.

                Relative to start of buffer (ignores current consume position)
            **/
            CharT const & operator[](size_type i) const { return buf_[i]; }

            /** @brief return span for current buffer contents **/
            span_type contents() const { return span_type(buf_ + lo_pos_,
                                                          buf_ + hi_pos_); }
            /** @brief returns span for writable buffer contents (unused prefix following produce position **/
            span_type avail() const { return span_type(buf_ + hi_pos_,
                                                       buf_ + buf_z_); }

            /** @brief @c true iff buffer is empty **/
            bool empty() const { return lo_pos_ == hi_pos_; }


            /**
               @brief update buffer produce position, after (independently) writing contents of span to it

               @pre left endpoint of @p span equals buffer produce position (@c .hi_pos)
               @pre right endpoint of @p span within bounds of buffer memory range
               @post right endpoint of @p span equals buffer produce position.
            **/
            void produce(span_type const & span) {
                assert(span.lo() == buf_ + hi_pos_);

                hi_pos_ += span.size();
            }

            /**
               @brief update buffer consume position,  when done with contents of span

               @pre left endpoint of @p span equals buffer consume position (@c .lo_pos)
               @pre right endpoint of @p span within bounds of buffer memory range
               @post Either
               buffer is empty, with @c .lo_pos = @c .hi_pos = @c 0.
               buffer is non-empty, right endpoint of @p span equals new buffer consume position.
            **/
            void consume(span_type const & span) {
                if (span.size()) {
                    assert(span.lo() == buf_ + lo_pos_);

                    lo_pos_ += span.size();
                } else {
                    /* since .consume() that arrives at empty contents also resets .lo_pos .hi_pos,
                     * we don't want to blow up when called with an empty span -- argument
                     * may represent some pre-reset location in buffer
                     */
                }

                if (lo_pos_ == hi_pos_) {
                    lo_pos_ = 0;
                    hi_pos_ = 0;
                }
            }

            /**
               @brief allocate buffer with desired amount of memory

               @param buf_z     desired buffer size
               @param align_z   alignment;  buffer memory will be aligned on this byte-boundary.
            **/
            void alloc(size_type buf_z, size_type align_z = sizeof(char)) {
                /* properly reset (+ discard) any existing state */
                this->reset();

                is_owner_ = true;
                if (buf_z)
                    buf_ = new (std::align_val_t(align_z)) CharT [buf_z];
                buf_z_ = buf_z;
                lo_pos_ = 0;
                hi_pos_ = 0;
            }

            /**
               @brief attach buffer to (unowned)  range of @p buf_z bytes starting at @p buf[0]

               Buffer is not responsible for managing storage.

               @post
               1. buffer is empty
               @post
               2. buffer read position = buffer write position = 0
            **/
            void setbuf(CharT * buf, size_type buf_z) {
                /* properly reset (+ discard) any existing state */
                this->reset();

                is_owner_ = false;
                lo_pos_ = 0;
                hi_pos_ = 0;
                buf_ = buf;
                buf_z_ = buf_z;
            }

            /**
               @brief revert buffer to empty state and possibly zero it

               @param zero_buffer_flag   Zero buffer contents iff this is true

               @post
               1. buffer is empty
               @post
               2. buffer read position = buffer write position = 0
            **/
            void clear2empty(bool zero_buffer_flag) {
                if (buf_ && zero_buffer_flag)
                    explicit_bzero(buf_, buf_z_ * sizeof(CharT));

                lo_pos_ = 0;
                hi_pos_ = 0;
            }

            /**
               @brief swap representation with another buffer instance.
            **/
            void swap (buffer & x) {
                std::swap(is_owner_, x.is_owner_);
                std::swap(buf_, x.buf_);
                std::swap(buf_z_, x.buf_z_);
                std::swap(lo_pos_, x.lo_pos_);
                std::swap(hi_pos_, x.hi_pos_);
            }

            /**
               @brief reset buffer to an empty state and recover owned storage
            **/
            void reset() {
                if (is_owner_ && buf_)
                    delete [] buf_;

                is_owner_ = false;
                buf_ = nullptr;
                buf_z_ = 0;
                lo_pos_ = 0;
                hi_pos_ = 0;
            }

            /**
               @brief move-assignment operator.
               @param x   right-hand-side to move from.

               @post
               @p x is in a valid, empty,
            **/
            buffer & operator= (buffer && x) {
                is_owner_ = x.is_owner_;
                buf_ = x.buf_;
                buf_z_ = x.buf_z_;
                lo_pos_ = x.lo_pos_;
                hi_pos_ = x.hi_pos_;

                x.is_owner_ = false;
                x.lo_pos_ = 0;
                x.hi_pos_ = 0;
                x.buf_ = nullptr;
                x.buf_z_ = 0;

                return *this;
            }

            /** @brief buffer is not assignable */
            buffer & operator= (buffer & x) = delete;

        private:
            /** @brief true iff buffer is responsible for freeing storage at @c buf_ **/
            bool is_owner_ = false;
            /** @brief buffer contents.  buffer memory comprises @c buf_[0] to @c buf_[buf_z_] **/
            CharT * buf_ = nullptr;
            /** @brief buffer size (in units of CharT) **/
            size_type buf_z_ = 0;

            /** @brief buffer read (consume) position

                @invariant
                0 <= lo_pos_ <= hi_pos_ < buf_z_
            **/
            size_type lo_pos_ = 0;
            /** @brief buffer write (produce) position

                @invariant
                0 <= hi_pos_ < hi_pos_ < buf_z_
            **/
            size_type hi_pos_ = 0;
        };

        /** @brief Overload for @c swap,  so that @c buffer<CharT> swappable **/
        template <typename CharT>
        inline void
        swap(buffer<CharT> & lhs,
             buffer<CharT> & rhs) {
            lhs.swap(rhs);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end buffer.hpp */
