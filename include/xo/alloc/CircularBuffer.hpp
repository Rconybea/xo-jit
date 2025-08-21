/* CircularBuffer.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#pragma once

#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/tostr.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <vector>
#include <cstdint>
#include <cassert>
//#include <concepts>

namespace xo {
    namespace gc {
        /** @class CircularBuffer
         *  @brief A circular buffer
         *
         *  push operations may overwrite prior contents,
         *  i.e. buffer behavior on overflow
         *  old
         *
         *  @tparam T is type for buffer elements.
         **/
        template <typename T>
        class CircularBuffer {
        public:
            using value_type = T;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type &;
            using const_reference = const value_type &;
            using pointer = value_type *;
            using const_pointer = const value_type *;

            template <typename Parent, typename TT>
            class _iterator {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = TT;
                using difference_type = std::ptrdiff_t;
                using pointer = value_type *;
                using reference = value_type &;

                _iterator(Parent * p, std::int64_t ix) : parent_{p}, index_{ix} {}

                reference  operator* () const { return (*parent_)[index_]; }
                pointer    operator->() const { return &(*parent_)[index_]; }
                _iterator & operator++() { ++index_; return *this; }
                _iterator   operator++(int) { _iterator retval(parent_, index_); ++index_; return retval; }

                auto operator<=>(const _iterator & other) {
                    if (parent_ == other.parent_)
                        return index_ <=> other.index_;
                    else
                        return std::partial_ordering::unordered;
                }

                bool operator==(const _iterator & other) const = default;

            private:
                Parent * parent_ = nullptr;
                /** index position
                 *  (-1 = just before front = rend, 0 = front, z-1 = back, z = just after back = end)
                 **/
                std::int64_t index_ = 0;
            };

            using iterator = _iterator<CircularBuffer<T>, T>;
            using const_iterator = _iterator<const CircularBuffer<T>, const T>;

        public:
            explicit CircularBuffer(std::size_t capacity = 0, bool debug_flag = false);
            CircularBuffer(const CircularBuffer& other) = default;
            CircularBuffer(CircularBuffer&& other) noexcept = default;
            ~CircularBuffer() = default;

            static constexpr std::int64_t npos = -1;

            /** @return location of i'th element. i: 0=front, 1=second etc **/
            std::size_t location_of(std::size_t i) const;
            /** @return ordinal index (relative to front) of location @p loc;
             *  npos if not used
             **/
            //std::int64_t index_of(std::size_t loc) const; // not implemented yet

            // standard container methods
            bool empty() const noexcept { return size_ == 0; }
            size_type size() const noexcept { return size_; }
            size_type max_size() const noexcept { return contents_.size(); }
            // void reserve(size_type new_capacity); // not implemented
            size_type capacity() const noexcept { return contents_.size(); }
            // void shrink_to_fit(); // not implemented

            reference at(size_type pos) {
                if ((pos < 0) || (pos >= size_)) {
                    throw std::out_of_range(tostr("CircularBuffer::at: index out of range",
                                                  xtag("pos", pos), xtag("size", size_)));
                }

                return contents_[this->location_of(pos)];
            }

            const_reference at(size_type pos) const {
                reference retval = const_cast<CircularBuffer*>(this)->at(pos);
                return retval;
            }

            reference operator[](size_type pos) {
                return contents_[this->location_of(pos)];
            }

            const_reference operator[](size_type pos) const {
                return contents_[this->location_of(pos)];
            }

            reference front() { return contents_[front_ix_]; }
            const_reference front() const {
                reference retval = const_cast<CircularBuffer*>(this)->front();
                return retval;
            }

            reference back() { return contents_[location_of(size_ - 1)]; }
            const_reference back() const {
                reference retval = const_cast<CircularBuffer*>(this)->back();
                return retval;
            }

            iterator begin() { return iterator(this, 0); }
            iterator end() { return iterator(this, size_); }
            const_iterator begin() const { return const_iterator(this, 0); }
            const_iterator end() const { return const_iterator(this, size_); }

            // reverse_iterator rbegin();
            // reverse_iterator rend();
            // const_reverse_iterator rbegin() const;
            // const_reverse_iterator rend() const;

            // General Methods

            void clear() {
                size_ = 0;
                front_ix_ = 0;
                std::size_t capacity = contents_.size();
                contents_.clear();
                contents_.resize(capacity);
            }

            /** push @p x on to the end of this buffer.
             *  If buffer is at capacity, overwrites the oldest element
             **/
            CircularBuffer & push_back(const T & x);

            // template<typename... Args>
            //reference emplace_back(Args&&... args);

            CircularBuffer & pop_back();

            // push_front();
            // pop_front();

            CircularBuffer& operator=(const CircularBuffer& other) = default;
            CircularBuffer& operator=(CircularBuffer&& other) noexcept = default;

        private:
            /** number of elements in buffer.  Not the same as @code contents_.size();
             *  the latter represents buffer capacity.
             *
             *  Promise:
             *    size_ <= contents_.size()
             **/
            std::size_t size_ = 0;
            /** first element is @code contents_.at(front_ix_) **/
            std::size_t front_ix_ = 0;
            /** buffer contents.  contents_.size() represents buffer capacity
             *  first element stored in @code contents_.at(front_)
             *  last element stored in @code contents_.at((front_ + size_ - 1) % contents_.size())
             **/
            std::vector<T> contents_;
            /** true to enable debug logging **/
            bool debug_flag_ = false;
        };

        template <typename T>
        CircularBuffer<T>::CircularBuffer(std::size_t capacity, bool debug_flag)
            : size_{0}, front_ix_{0}, contents_{capacity}, debug_flag_{debug_flag}
        {
        }

        template <typename T>
        std::size_t
        CircularBuffer<T>::location_of(std::size_t i) const
        {
            if (size_ == 0)
                return 0;
            else
                return (front_ix_ + i) % size_;
        }

        template <typename T>
        CircularBuffer<T> &
        CircularBuffer<T>::push_back(const T & x) {
            scope log(XO_DEBUG(debug_flag_), rtag("x", x), xrtag("size", size_));

            if (size_ < contents_.size()) {
                ++size_;
                /* _after_ incr .size_ */
                std::size_t back_ix = location_of(size_ - 1);

                this->contents_[back_ix] = x;

                log && log(xtag("back_ix", back_ix), xtag("+size", size_));
            } else {
                std::size_t back_ix = location_of(size_);

                this->contents_[back_ix] = x;
                /* buffer was full,  so oldest element replaced */
                this->front_ix_ = (this->front_ix_ + 1) % contents_.size();

                log && log(xtag("back_ix", back_ix), xtag("+front", front_ix_));
            }

            return *this;
        }

        template <typename T>
        CircularBuffer<T> &
        CircularBuffer<T>::pop_back() {
            if (size_ > 0) {
                std::size_t back_ix = location_of(size_ - 1);

                this->contents_[back_ix] = T();
                --(this->size_);
            } else {
                assert(false);
            }

            return *this;
        }
    } /*namespace gc*/
} /*namespace xo*/

/* CircularBuffer.hpp */
