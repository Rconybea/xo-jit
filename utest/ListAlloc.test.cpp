/* ListAlloc.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "xo/alloc/ListAlloc.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::ListAlloc;

    namespace ut {
        TEST_CASE("ListAlloc", "[alloc][gc]")
        {
            /** teeny weeny allocator **/
            up<ListAlloc> alloc = ListAlloc::make("test", 16, 32, false);

            REQUIRE(alloc->name() == "test");
            REQUIRE(alloc->size() == 16);
            REQUIRE(alloc->before_checkpoint() == 0);
            REQUIRE(alloc->after_checkpoint() == 0);

            /* will expand */
            std::byte * mem1 = alloc->alloc(20);

            REQUIRE(mem1);
            REQUIRE(alloc->size() == 16 + 32);
            /* round up to multiple of 8 */
            REQUIRE(alloc->before_checkpoint() == 24);
            REQUIRE(alloc->after_checkpoint() == 0);

            alloc->checkpoint();

            std::byte * mem2 = alloc->alloc(30);

            REQUIRE(mem2);
            REQUIRE(alloc->size() == 16 + 32 + 48);
            REQUIRE(alloc->before_checkpoint() == 24);
            /* round up to multiple of 8 */
            REQUIRE(alloc->after_checkpoint() == 32);

            std::byte * mem3 = alloc->alloc(40);

            REQUIRE(mem3);
            REQUIRE(alloc->size() == 16 + 32 + 48 + 80);
            REQUIRE(alloc->before_checkpoint() == 24);
            /* already multiple of 8 */
            REQUIRE(alloc->after_checkpoint() == 32 + 40);

            REQUIRE(alloc->is_before_checkpoint(mem1) == true);
            REQUIRE(alloc->is_before_checkpoint(mem2) == false);
            REQUIRE(alloc->is_before_checkpoint(mem3) == false);
        }

    } /*namespace ut*/
} /*namespace xo*/

/* ListAlloc.test.cpp */
