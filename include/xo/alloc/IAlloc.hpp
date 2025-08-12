/* file IAlloc.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include <memory>
#include <cstdint>

namespace xo {
    template <typename T>
    using up = std::unique_ptr<T>;

    class Object;

    namespace gc {
        /** @class IAllocator
         *  @brief memory allocation interface with limited garbaga collector support
         **/
        class IAlloc {
        public:
            virtual ~IAlloc() {}

            /** compute padding to add to an allocation of size z to bring it up to
             *  a multiple of word size (8 bytes on x86_64)
             **/
            static std::uint32_t alloc_padding(std::size_t z);
            /** z + alloc_padding(z) **/
            static std::size_t with_padding(std::size_t z);

            /** optional name for this allocator; labelling for diagnostics **/
            virtual const std::string & name() const = 0;
            /** allocator size in bytes (up to reserved limit)
             *  Includes unallocated mmeory
             **/
            virtual std::size_t size() const = 0;
            /** committed size in bytes **/
            virtual std::size_t committed() const = 0;
            /** number of unallocated bytes available (up to soft limit)
             *  from this allocator
             **/
            virtual std::size_t available() const = 0;
            /** number of bytes allocated from this allocator **/
            virtual std::size_t allocated() const = 0;
            /** true iff pointer x comes from this allocator **/
            virtual bool contains(const void * x) const = 0;
            /** true iff object at address @p x was allocated by this allocator,
             *  and before checkpoint
             **/
            virtual bool is_before_checkpoint(const void * x) const = 0;
            /** number of bytes allocated before @ref checkpoint **/
            virtual std::size_t before_checkpoint() const = 0;
            /** number of bytes allocated since @ref checkpoint **/
            virtual std::size_t after_checkpoint() const = 0;
            /** @return true iff debug logging enabled **/
            virtual bool debug_flag() const = 0;

            /** reset allocator to empty state. **/
            virtual void clear() = 0;
            /** remember allocator state.  All currently-allocated addresses xo
             *  will satisfy is_before_checkpoint(x).  Subsequent allocations x
             *  will fail is_before_checkpoint(x), until checkpoint superseded
             *  by @ref clear or another call to @ref checkpoint
             **/
            virtual void checkpoint() = 0;
            /** perform assignment
             *    @code
             *    *lhs = rhs
             *    @endcode
             *  plus additional book keeping if needed (e.g. in @ref GC)
             *  Default implementation just does the assignment.
             **/
            virtual void assign_member(Object * parent, Object ** lhs, Object * rhs);
            /** allocate @p z bytes of memory. returns pointer to first address **/
            virtual std::byte * alloc(std::size_t z) = 0;
            /** allocate @p z bytes for copy of object at @p src.
             *  Only used in @ref GC.  Default implementation asserts and returns nullptr
             **/
            virtual std::byte * alloc_gc_copy(std::size_t z, const void * src);
        };
    } /*namespace gc*/

    class MMPtr {
    public:
        explicit MMPtr(gc::IAlloc * mm) : mm_{mm} {}

        gc::IAlloc * mm_ = nullptr;
    };
} /*namespace xo*/

inline void * operator new (std::size_t z, const xo::MMPtr & mmp) {
    return mmp.mm_->alloc(z);
}

//inline void operator delete (void * p, const MMPtr & mmp) {
//    mmp.mm_->free(reinterpret_cast<std::byte *>(p));
//}

inline void * operator new[] (std::size_t z, const xo::MMPtr & mmp) {
    return mmp.mm_->alloc(z);
}

//inline void operator delete[] (void * p, const MMPtr & mmp) {
//    mmp.mm_->free(reinterpret_cast<std::byte *>(p));
//}


/* end IAlloc.hpp */
