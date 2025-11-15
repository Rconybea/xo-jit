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
         *  @brief Bump allocator with fixed capacity with dynamic virtual memory commitment.
         *
         *  @text
         *
         *    allocation order:
         *    ----------------------->
         *
         *    <----------------- .size() ------------------>
         *    <----------------- .committed() --------------->
         *
         *    <-------allocated------><--------free-------->  <---uncommitted---->
         *    XXXXXXXXXXXXXXXXXXXXXXXX______________________  ....................
         *    ^       ^               ^                     ^                     ^
         *    lo      checkpoint   free                 limit                    hi
         *
         *                            +- .alloc() ->
         *                                                  +-- .expand() -->
         *   >        < .before_checkpoint()
         *           >                < .after_checkpoint()
         *
         *  @endtext
         *
         *  Design Notes:
         *  - non-copyable, non-moveable
         *  - always heap-allocated
         *  - @ref lo_ <= @ref checkpoint_ <= @ref free_ <= @ref limit_ <= @ref hi_
         *  - memory obtained from mmap(), not heap
         *  - memory addresses are stable. Expand storage by committing VM pages.
         *  - @ref lo_ is aligned on VM page size (guaranteed by mmap())
         *  - @ref lo_ + @ref committed_z_ <= @ref hi_
         *  - @ref limit_ <= @ref lO_ + @ref committed_z_
         *  - @ref committed_z_ is always a multiple of VM page size
         *  - @ref limit_ is not guaranteed to be aligned with VM page size.
         *  - @ref expand increases @ref limit_ and @ref committed_z_ as needed.
         *
         **/
        class ArenaAlloc : public IAlloc {
        public:
            ArenaAlloc(const ArenaAlloc &) = delete;
            ArenaAlloc(ArenaAlloc &&) = delete;
            ~ArenaAlloc();

            /** Create allocator with capacity @p z,
             *  Reserve memory addresses for @p z bytes,
             *  but don't commit them until needed
             **/
            static up<ArenaAlloc> make(const std::string & name,
                                       std::size_t z,
                                       bool debug_flag);

            /** size of virtual address range reserved for this allocator **/
            std::size_t reserved() const { return this->size(); }

            std::size_t page_size() const { return page_z_; }
            std::byte * free_ptr() const { return free_ptr_; }
            void       set_free_ptr(std::byte * x);

            /** if address @p x is allocated from this arena,
             *  return true along with offset relative to base address @ref lo_
             *  otherwise return false with 0
             **/
            std::pair<bool, std::size_t> location_of(const void * x) const;

            /** allocated span **/
            std::pair<const std::byte *, const std::byte *> allocated_span() const {
                return std::make_pair(lo_, free_ptr_);
            }

            /** Reset to empty state; provision at least @p need_z bytes of (committed) space **/
            void reset(std::size_t need_z);

            /** gc support: If used for storing xo::Object instances, scan allocated memory
             *  to populate @p *p_dest.
             **/
            void capture_object_statistics(capture_phase phase,
                                           ObjectStatistics * p_dest) const;

            /** expand available (i.e. committed) space to size at least @p z
             *  In practice will round up to a multiple of @ref page_z_.
             **/
            bool expand(std::size_t z);

            // inherited from IAlloc...

            virtual const std::string & name() const final override;
            virtual std::size_t size() const final override;
            virtual std::size_t committed() const final override;
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

            ArenaAlloc & operator=(const ArenaAlloc &) = delete;
            ArenaAlloc & operator=(ArenaAlloc &&) = delete;

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

            /** size of a VM page (from getpagesize()) **/
            std::size_t page_z_;

            /** allocator owns memory in range [@ref lo_, @ref hi_) **/
            std::byte * lo_ = nullptr;
            /** prefix of this size is actually committed.
             *  Remainder uses uncommitted virtual address space
             **/
            std::size_t committed_z_ = 0;
            /** checkpoint (for GC support); divides objects into
             *  older (addresses below checkpoint)
             *  and younger (addresses above checkpoint)
             **/
            std::byte * checkpoint_;
            /** free pointer. memory in range [@ref free_, @ref limit_) available **/
            std::byte * free_ptr_ = nullptr;
            /** soft limit: end of committed virtual memory **/
            std::byte * limit_ = nullptr;
            /** hard limit: end of reserved virtual memory **/
            std::byte * hi_ = nullptr;
            /** true to enable detailed debug logging **/
            bool debug_flag_ = false;
        };

    } /*namespace gc*/
} /*namespace xo*/


/* end ArenaAlloc.hpp */
