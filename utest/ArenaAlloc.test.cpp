/* @file ArenaAlloc.test.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "xo/alloc/ArenaAlloc.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::ArenaAlloc;

    namespace ut {

        namespace {
            struct testcase_alloc {
                explicit testcase_alloc(std::size_t z)
                    :
                    arena_z_{z} {}

                std::size_t arena_z_;
            };

            std::vector<testcase_alloc>
            s_testcase_v = {
                testcase_alloc(4096)
            };
        }


        TEST_CASE("linearalloc", "[alloc]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                const testcase_alloc & tc = s_testcase_v[i_tc];

                constexpr bool c_debug_flag = false;

                auto alloc = ArenaAlloc::make("linearalloc",
                                              tc.arena_z_, c_debug_flag);
                alloc->expand(tc.arena_z_);

                REQUIRE(alloc.get());
                REQUIRE(alloc->name() == "linearalloc");
                REQUIRE(alloc->size() == std::max(tc.arena_z_, alloc->hugepage_z()));
                REQUIRE(alloc->available() == std::max(tc.arena_z_, alloc->hugepage_z()));
                REQUIRE(alloc->allocated() == 0);
                REQUIRE(alloc->is_before_checkpoint(alloc->free_ptr()) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == 0);

                auto free0 = alloc->free_ptr();

                auto mem = alloc->alloc(std::max(tc.arena_z_, alloc->hugepage_z()));

                REQUIRE(mem != nullptr);

                REQUIRE(mem == free0);

                REQUIRE(alloc->size() == std::max(tc.arena_z_, alloc->hugepage_z()));
                REQUIRE(alloc->available() == 0);
                REQUIRE(alloc->allocated() == std::max(tc.arena_z_, alloc->hugepage_z()));
                REQUIRE(alloc->is_before_checkpoint(mem) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == std::max(tc.arena_z_, alloc->hugepage_z()));

                alloc->clear();

                REQUIRE(alloc->free_ptr() == free0);
                REQUIRE(alloc->available() == std::max(tc.arena_z_, alloc->hugepage_z()));
                REQUIRE(alloc->allocated() == 0);
                REQUIRE(alloc->is_before_checkpoint(free0) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == 0);

                mem = alloc->alloc(1);

                auto used = sizeof(void*);
                REQUIRE(alloc->size() == std::max(tc.arena_z_, alloc->hugepage_z()));
                REQUIRE(alloc->available() == std::max(tc.arena_z_, alloc->hugepage_z()) - used);
                REQUIRE(alloc->allocated() == used);
                REQUIRE(alloc->is_before_checkpoint(free0) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == used);

            }
        }

    } /*namespace ut */
} /*namespace xo*/
