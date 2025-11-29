/** @file Allocator.hpp
 *
 *  @author Roland Conybeare, Nov 2025
 **/

#pragma once

#include "xo/alloc/ArenaAlloc.hpp"

namespace xo {
    namespace gc {
        /** @class allocator
         *  @brief c++ allocator with allocator traits
         *
         *  Can use ArenaAllocT with std::map etc.
         **/
        template <typename T>
        class ArenaAllocT {
        public:
            using value_type = T;
            /** copy assignment: leave lhs allocator in place **/
            using propagate_on_container_copy_assignment = std::false_type;
            /** move assignment: adopt rhs allocator
             *  (Forced: cannot mix allocations from different allocators
             *  within a container)
             **/
            using propagate_on_container_move_assignment = std::true_type;
            /** swap: also swap allocators
             *  (Forced: cannot mix allocations from different allocators
             *  within a containers)
             **/
            using propagate_on_container_swap = std::true_type;
            /** An ArenaAlloc instance is unique owner of its own memory:
             *  no other instance can dealloc
             **/
            using is_always_equal = std::false_type;

        public:
            explicit ArenaAllocT(ArenaAlloc * mm) : mm_{mm} {}
            ArenaAllocT(const ArenaAllocT & other) = default;

            /** rebind ctor. Allows container to use supplied allocator
             *  for multiple types
             **/
            template <typename U>
            ArenaAllocT(const ArenaAllocT<U> & other) noexcept : mm_{other.mm_} {}

            T * allocate(size_t n) {
                void * mem = mm_->alloc(n * sizeof(T));

                return reinterpret_cast<T *>(mem);
            }

            void deallocate(T * p, size_t n) noexcept {
                assert(mm_->contains(p));
                assert(n == 0 || mm_->contains(p + n - 1));

                //arena_->deallocate(p, n * sizeof(T));
            }

            bool operator==(const ArenaAllocT & other) const {
                return mm_ == other.mm_;
            }

            bool operator!=(const ArenaAllocT & other) const {
                return mm_ != other.mm_;
            }

            ArenaAlloc * mm_ = nullptr;
        };
    } /*namespace gc*/
} /*namespace xo*/

/* end Allocator.hpp */
