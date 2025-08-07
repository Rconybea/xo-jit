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
                testcase_alloc(std::size_t rz, std::size_t z)
                    :
                    arena_z_{z} {}

                std::size_t arena_z_;

            };

            std::vector<testcase_alloc>
            s_testcase_v = {
                testcase_alloc(0, 4096)
            };
        }


        TEST_CASE("linearalloc", "[alloc]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                const testcase_alloc & tc = s_testcase_v[i_tc];

                constexpr bool c_debug_flag = false;

                auto alloc = ArenaAlloc::make("linearalloc",
                                              tc.arena_z_, c_debug_flag);

                REQUIRE(alloc.get());
                REQUIRE(alloc->name() == "linearalloc");
                REQUIRE(alloc->size() == tc.arena_z_);
                REQUIRE(alloc->available() == tc.arena_z_);
                REQUIRE(alloc->allocated() == 0);
                REQUIRE(alloc->is_before_checkpoint(alloc->free_ptr()) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == 0);

                auto free0 = alloc->free_ptr();

                auto mem = alloc->alloc(tc.arena_z_);

                REQUIRE(mem != nullptr);

                REQUIRE(mem == free0);

                REQUIRE(alloc->size() == tc.arena_z_);
                REQUIRE(alloc->available() == 0);
                REQUIRE(alloc->allocated() == tc.arena_z_);
                REQUIRE(alloc->is_before_checkpoint(mem) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == tc.arena_z_);

                alloc->clear();

                REQUIRE(alloc->free_ptr() == free0);
                REQUIRE(alloc->available() == tc.arena_z_);
                REQUIRE(alloc->allocated() == 0);
                REQUIRE(alloc->is_before_checkpoint(free0) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == 0);

                mem = alloc->alloc(1);

                auto used = sizeof(void*);
                REQUIRE(alloc->size() == tc.arena_z_);
                REQUIRE(alloc->available() == tc.arena_z_ - used);
                REQUIRE(alloc->allocated() == used);
                REQUIRE(alloc->is_before_checkpoint(free0) == false);
                REQUIRE(alloc->before_checkpoint() == 0);
                REQUIRE(alloc->after_checkpoint() == used);

            }
        }

    } /*namespace ut */
} /*namespace xo*/
