/* Stack.hpp
 *
 * author: Roland Conybeare, jul 2025
 */

#pragma once

#include <vector>

namespace xo {
    namespace gc {
        /** Simple stack implementation
         **/
        template <typename T>
        class Stack {
        public:
            explicit Stack(std::size_t capacity) {
                this->contents_.reserve(capacity);
            }

            bool is_empty() const { return contents_.empty(); }
            std::size_t available() const { return contents_.capacity() - contents_.size(); }
            void drop() { contents_.resize(contents_.size() - 1); }
            void push(const T & x) { contents_.push_back(x); }
            T pop() {
                T retval = contents_[contents_.size() - 1];
                this->drop();
                return retval;
            }
            const T & top() const {
                return this->lookup(0);
            }
            const T & lookup(std::size_t i) const {
                return contents_.at(contents_.size() - 1 - i);
            }
            void clear() { contents_.clear(); }
            void reset_to(std::size_t z) { contents_.resize(z); }

            std::size_t n_elements() const { return contents_.size(); }
            std::size_t capacity() const { return contents_.capacity(); }

        private:
            std::vector<T> contents_;
        };

    } /*namespace gc*/
} /*namespace xo*/

/* end Stack.hpp */
