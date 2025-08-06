/* @file IAlloc.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "IAlloc.hpp"
#include <cassert>
#include <cstddef>

namespace xo {
    namespace gc {

        std::uint32_t
        IAlloc::alloc_padding(std::size_t z)
        {
            /* word size for alignment */
            constexpr uint32_t c_bpw = sizeof(std::uintptr_t);

            /* round up to multiple of c_bpw, but map 0 -> 0
             * (table assuming c_bpw==8)
             *
             *   z%c_bpw   dz
             *   ------------
             *         0    0
             *         1    7
             *         2    6
             *        ..   ..
             *         7    1
             */
            std::uint32_t dz = (c_bpw - (z % c_bpw)) % c_bpw;
            z += dz;

            assert(z % c_bpw == 0ul);

            return dz;
        }

        std::size_t
        IAlloc::with_padding(std::size_t z)
        {
            return z + alloc_padding(z);
        }

        void
        IAlloc::assign_member(Object * /*parent*/, Object ** lhs, Object * rhs)
        {
            *lhs = rhs;
        }

        // LCOV_EXCL_START
        std::byte *
        IAlloc::alloc_gc_copy(std::size_t /*z*/, const void * /*src*/)
        {
            assert(false);
            return nullptr;
        }
        // LCOV_EXCL_STOP

    } /*namespace gc*/
} /*namespace xo*/

/* end IAlloc.cpp */
