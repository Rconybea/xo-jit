/** @file span.hpp **/

#pragma once

#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/ppdetail_atomic.hpp"
#include <ostream>
#include <cstdint>
#include <cassert>

namespace xo {
    namespace scm {
        /** @class span compression/span.hpp
         *
         *  @brief A contiguous range of characters,  without ownership.
         *
         *  @tparam CharT type for elements referred to by this span.
         **/
        template <typename CharT>
        class span {
        public:
            /** @defgroup span-type-traits span type traits **/
            ///@{

            /** typealias for span size (in units of CharT) **/
            using size_type = std::uint64_t;

            ///@}

        public:
            /** @defgroup span-ctors span constructors **/
            ///@{

            /** null span **/
            span() : lo_{nullptr}, hi_{nullptr} {}

            /** Create span for the contiguous memory range [@p lo, @p hi) **/
            span(CharT * lo, CharT * hi) : lo_{lo}, hi_{hi} {}

            /** explicit conversion from span<U> **/
            template<typename CharU>
            span(const span<CharU> & other,
                 std::enable_if_t<std::is_convertible_v<CharU*, CharT*>
                 && !std::is_same_v<CharU, CharT>> * = nullptr)
                : lo_{other.lo()}, hi_{other.hi()} {}

            /** copy ctor (explicit to avoid ambiguity with template ctor) **/
            span(const span & other) = default;
            span & operator=(const span & other) = default;

            /** Create a null span (i.e. with null @p lo, @p hi pointers)
             *  A null span can be concatenated with any other span
             *  without triggering matching-endpoint asserts.
             **/
            static span make_null() { return span(static_cast<CharT*>(nullptr), static_cast<CharT*>(nullptr)); }

            /** @brief create span for C-style string @p cstr **/
            static span from_cstr(const CharT * cstr) {
                CharT * lo = cstr;
                CharT * hi = cstr ? cstr + strlen(cstr) : nullptr;

                return span(lo, hi);
            }

            /** @brief create span from std::string @p str **/
            static span from_string(const std::string& str) {
                CharT * lo = &(*str.begin());
                CharT * hi = &(*str.end());

                return span(lo, hi);
            }

            /** @brief concatenate two contiguous spans */
            static span concat(const span & span1, const span & span2) {
                if (span1.is_null())
                    return span2;
                if (span2.is_null())
                    return span1;

                if (span1.hi() != span2.lo()) {
                    scope log(XO_DEBUG(true));

                    log && log(xtag("span1.hi", (void*)span1.hi()), xtag("span2.lo", (void*)span2.lo()));
                }

                assert(span1.hi() == span2.lo());

                CharT * lo = span1.lo();
                CharT * hi = span2.hi();

                return span(lo, hi);
            }

            ///@}

            /** @defgroup span-access-methods **/
            ///@{

            CharT * lo() const { return lo_; } /* get member span::lo_ */
            CharT * hi() const { return hi_; } /* get member span::hi_ */

            ///@}

            /** @defgroup span-general-methods **/
            ///@{

            /** @brief strip prefix until first occurence of '\n', including the newline **/
            void discard_until_newline() {
                for (const CharT * p = lo_; p < hi_; ++p) {
                    if (*p == '\n') {
                        lo_ = p + 1;
                        return;
                    }
                }

                lo_ = hi_;
            }

            /** Create new span over supplied type,
             *  with identical (possibly misaligned) endpoints.
             *
             *  @warning
             *  1. New span uses exactly the same memory addresses.
             *     Endpoint pointers may not be aligned.
             *  2. Implementation assumes code compiled with
             *     @code -fno-strict-aliasing @endcode enabled.
             *
             *  @tparam OtherT element type for new span
             **/
            template <typename OtherT>
            span<OtherT>
            cast() const { return span<OtherT>(reinterpret_cast<OtherT *>(lo_),
                                               reinterpret_cast<OtherT *>(hi_)); }

            /** @brief create span including the first @p z members of this span. **/
            span prefix(size_type z) const { return span(lo_, lo_ + z); }

            /** @brief create span representing prefix up to (but not including) @p *p
             **/
            span prefix_upto(CharT * p) const {
                if (p <= hi_)
                    return span(lo_, p);
                else
                    return span(lo_, hi_);
            }

            /** @brief create span with first @p z members of this span removed **/
            span after_prefix(size_type z) const {
                if (lo_ + z > hi_)
                    z = hi_ - lo_;

                return span(lo_ + z, hi_);
            }

            /** @brief create span with @p prefix of this span removed **/
            span after_prefix(const span & prefix) const {
                if (!prefix.is_null() && (prefix.lo() != lo_)) {
                    throw std::runtime_error
                        ("after_prefix: expected prefix of this span");
                }

                return after_prefix(prefix.size());
            }

            /** Create span starting with position @p p.
             *  Does boundary checking; will return empty span if @p p is outside @c [lo_,hi)
             **/
            span suffix_from(CharT * p) const {
                if ((lo_ <= p) && (p <= hi_))
                    return span(p, hi_);
                else
                    return span(hi_, hi_);
            }

            /** true iff this span is null.  distinct from empty. **/
            bool is_null() const { return lo_ == nullptr && hi_ == nullptr; }
            /** true iff this span is empty (comprises 0 elements). **/
            bool empty() const { return lo_ == hi_; }
            /** report the number of elements (of type CharT) in this span. **/
            size_type size() const { return hi_ - lo_; }

            /** increase extent of this spans to include @p x.
             *  Requires @c hi() == @c x.lo()
             **/
            span & operator+=(const span & x) {
                if (hi_ == x.lo_) {
                    hi_ = x.hi_;
                } else if (!x.is_null()) {
                    assert(false);
                }

                return *this;
            }

            /** print representation for this span on stream @p os **/
            void print(std::ostream & os) const {
                os << "<span"
                   << xtag("addr", (void*)lo_)
                   << xtag("size", size())
                   << " :text " << xo::print::quot(std::string_view(lo_, hi_))
                   << ">";
            }
            ///@}

        private:
            /** @defgroup span-instance-vars **/
            ///@{

            /** start of span.
                Span comprises memory address between @p lo (inclusive) and @p hi (exclusive)
            **/
            CharT * lo_ = nullptr;

            /** @brief end of span.
                Span comprises memory address between @p lo (inclusive) and @p hi (exclusive)
            **/
            CharT * hi_ = nullptr;

            ///@}
        }; /*span*/

        /** @defgroup span-operators **/
        ///@{

        /** compare spans for equality.
         *  Two spans are equal iff both endpoints match exactly.
         **/
        template <typename CharT>
        inline bool
        operator==(const span<CharT> & lhs, const span<CharT> & rhs) {
            return ((lhs.lo() == rhs.lo())
                    && (lhs.hi() == rhs.hi()));
        }

        /** compare spans for inequality.
         *  Two spans are unequal if either paired endpoint differs.
         **/
        template <typename CharT>
        inline bool
        operator!=(const span<CharT> & lhs, const span<CharT> & rhs) {
            return ((lhs.lo() != rhs.lo())
                    || (lhs.hi() != rhs.hi()));
        }

        /** print a summary of @p x on stream @p os. Intended for diagnostics **/
        template <typename CharT>
        inline std::ostream &
        operator<<(std::ostream & os,
                   const span<CharT> & x) {
            x.print(os);
            return os;
        }

        ///@}
    } /*namespace scm*/

    namespace print {
        template <typename CharT>
        class printspan_impl {
        public:
            printspan_impl(xo::scm::span<CharT> x) : span_{x} {}

            xo::scm::span<CharT> span_;
        };

        template <typename CharT>
        printspan_impl<CharT> printspan(const xo::scm::span<CharT>& span) {
            return printspan_impl<CharT>(span);
        }

        template <typename CharT>
        inline std::ostream &
        operator<< (std::ostream & os,
                    const printspan_impl<CharT> & x)
        {
            for (const CharT * p = x.span_.lo(); p < x.span_.hi(); ++p)
                os << *p;

            return os;
        }

#ifndef ppdetail_atomic
        template <typename CharT>        \
        PPDETAIL_ATOMIC_BODY(printspan_impl<CharT>);

        template <typename CharT>        \
        PPDETAIL_ATOMIC_BODY(xo::scm::span<CharT>);
#endif

    }
} /*namespace xo*/
