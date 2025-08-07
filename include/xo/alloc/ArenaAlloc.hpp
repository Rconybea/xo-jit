/* file ArenaAlloc.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "IAlloc.hpp"
#include "ObjectStatistics.hpp"

namespace xo {
    namespace gc {
        /** @class ArenaAlloc
         *  @brief Bump allocator with fixed capacity
         *
         *  @text
         *
         *    <-----allocated----> <--------free------->
         *    XXXXXXXXXXXXXXXXXXXX______________________
         *    ^                   ^                     ^
         *    lo                  free                  hi
         *                                              limit
         *  @endtext
         **/
        class ArenaAlloc : public IAlloc {
        public:
            ~ArenaAlloc();

            /** create allocator with capacity @p z,
             **/
            static up<ArenaAlloc> make(const std::string & name,
                                        std::size_t z,
                                        bool debug_flag);

            std::byte * free_ptr() const { return free_ptr_; }
            void       set_free_ptr(std::byte * x);

            void capture_object_statistics(capture_phase phase,
                                           ObjectStatistics * p_dest) const;

            // inherited from IAlloc...

            virtual const std::string & name() const final override;
            virtual std::size_t size() const final override;
            virtual std::size_t available() const final override;
            virtual std::size_t allocated() const final override;
            virtual bool contains(const void * x) const final override;
            virtual bool is_before_checkpoint(const void * x) const final override;
            virtual std::size_t before_checkpoint() const final override;
            virtual std::size_t after_checkpoint() const final override;
            virtual bool debug_flag() const final override;

            virtual void clear() final override;
            virtual void checkpoint() final override;
            virtual std::byte * alloc(std::size_t z) final override;

        private:
            ArenaAlloc(const std::string & name,
                       std::size_t z, bool debug_flag);

        private:
            /**
             *  Invariants:
             *  - @ref free_ always a multiple of word size (assumed to be sizeof(void*))
             **/

            /** optional instance name, for diagnostics **/
            std::string name_;

            /** allocator owns memory in range [@ref lo_, @ref hi_) **/
            std::byte * lo_ = nullptr;
            /** checkpoint (for GC support); divides objects into
             *  older (addresses below checkpoint)
             *  and younger (addresses above checkpoint)
             **/
            std::byte * checkpoint_;
            /** free pointer. memory in range [@ref free_, @ref limit_) available **/
            std::byte * free_ptr_ = nullptr;
            /** soft limit: end of released memory **/
            std::byte * limit_ = nullptr;
            /** hard limit: end of allocated memory **/
            std::byte * hi_ = nullptr;
            /** true to enable detailed debug logging **/
            bool debug_flag_ = false;
        };

    } /*namespace gc*/
} /*namespace xo*/


/* end ArenaAlloc.hpp */
