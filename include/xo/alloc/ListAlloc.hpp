/* file ListAlloc.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "IAlloc.hpp"
#include "ObjectStatistics.hpp"
#include <list>
#include <memory>
#include <cstdint>

namespace xo {
    namespace gc {
        class ArenaAlloc;

        /** GC-compatible allocator using a linked list of buckets.
         *
         *  - all allocs done from first allocator in list
         *  GC Support:
         *  - reserved memory, released after call to @ref release_redline_memory.
         *
         *  TODO: reserve address space using mmap,
         *        but don't commit until alloc requires it.
         **/
        class ListAlloc : public IAlloc {
        public:
            ListAlloc(std::unique_ptr<ArenaAlloc> hd,
                      ArenaAlloc * marked,
                      std::size_t cz, std::size_t nz, std::size_t tz,
#ifdef REDLINE_MEMORY
                      bool use_redline,
#endif
                      bool debug_flag);
            ~ListAlloc();

            static up<ListAlloc> make(const std::string & name, std::size_t cz, std::size_t nz, bool debug_flag);

            /** reset to have at least @p z bytes of storage **/
            bool reset(std::size_t z);

            /** expand bucket list to accomodate a requrest of size @p z **/
            bool expand(std::size_t z);

            /** current free pointer **/
            std::byte * free_ptr() const;

            /** scan space (must not contain forwarding pointers, because loses size info)
             *  + gather stats by object type
             *
             *  See @ref Object::self_tp
             **/
            void capture_object_statistics(capture_phase phase,
                                           ObjectStatistics * p_dest) const;

            // inherited from IAlloc..

            virtual const std::string & name() const final override;
            virtual std::size_t size() const final override;
            virtual std::size_t available() const final override;
            virtual std::size_t allocated() const final override;
            virtual bool contains(const void * x) const final override;
            virtual bool is_before_checkpoint(const void * x) const final override;
            virtual std::size_t before_checkpoint() const final override;
            virtual std::size_t after_checkpoint() const final override;

            virtual void clear() final override;
            virtual void checkpoint() final override;
            virtual std::byte * alloc(std::size_t z) final override;
#ifdef REDLINE_MEMORY
            virtual void release_redline_memory() final override;
#endif

        private:
            /** **/
            std::size_t start_z_ = 0;
            /** all new allocs from this list **/
            std::unique_ptr<ArenaAlloc> hd_;
            /** allocator that was in @ref hd_ when @ref checkpoint last called **/
            ArenaAlloc * marked_ = nullptr;
            /** overflow allocs (expect list to be short);
             *  from trying to converge on app working set size
             **/
            std::list<std::unique_ptr<ArenaAlloc>> full_l_;
            /** size of current arena @ref hd_ **/
            std::size_t current_z_ = 0;
            /** if @ref hd_ fills, size of next arena to allocate **/
            std::size_t next_z_ = 0;
            /** total size of @ref hd_ + contents of @ref full_l_ **/
            std::size_t total_z_ = 0;
#ifdef REDLINE_MEMORY
            bool use_redline_ = false;
            bool redlined_flag_ = false;
#endif

            /** true to enable debug logging **/
            bool debug_flag_ = false;
        };
    } /*namespace gc*/
} /*namespace xo*/

/* end ListAlloc.hpp */
