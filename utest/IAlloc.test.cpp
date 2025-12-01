/* @file IAlloc.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "xo/allocutil/IAlloc.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::IAlloc;

    namespace ut {
        TEST_CASE("ialloc", "[alloc]")
        {
            REQUIRE(IAlloc::alloc_padding(0) == 0);
            REQUIRE(IAlloc::alloc_padding(1) == 7);
            REQUIRE(IAlloc::alloc_padding(2) == 6);
            REQUIRE(IAlloc::alloc_padding(3) == 5);
            REQUIRE(IAlloc::alloc_padding(4) == 4);
            REQUIRE(IAlloc::alloc_padding(5) == 3);
            REQUIRE(IAlloc::alloc_padding(6) == 2);
            REQUIRE(IAlloc::alloc_padding(7) == 1);
            REQUIRE(IAlloc::alloc_padding(8) == 0);
            REQUIRE(IAlloc::alloc_padding(9) == 7);
        }
    } /*namespace ut*/
} /*namespace xo*/

/* end IAlloc.test.cpp */
