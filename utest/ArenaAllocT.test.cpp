/** @file ArenaAllocT.test.cpp
 *
 *  @author Roland Conybeare, Nov 2025
 **/

#include "xo/alloc/ArenaAllocT.hpp"
#include <catch2/catch.hpp>
#include <map>

namespace xo {
    using xo::gc::ArenaAllocT;
    using xo::gc::ArenaAlloc;

    namespace ut {

        namespace {
            struct testcase_ArenaAllocT {
                std::size_t arena_z_;
                std::vector<std::pair<std::string, std::string>> kv_pairs_;
            };

            std::vector<testcase_ArenaAllocT>
            s_testcase_v = {
                { 4096, {} },
                { 4096, {{"a", "apple"}} },
                { 4096, {{"a", "apple"}, {"b", "banana"}, {"c", "carrot"}} },
                { 4096, {{"a", "apple"}, {"b", "banana"}, {"c", "carrot"}, {"e", "eggplant"}} },
            };
        }

        TEST_CASE("ArenaAllocT", "[alloc][traits]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                const testcase_ArenaAllocT & tc = s_testcase_v[i_tc];

                constexpr bool c_debug_flag = false;

                auto arena = ArenaAlloc::make("arena", tc.arena_z_, c_debug_flag);
                auto alloc = ArenaAllocT<std::pair<const std::string, std::string>>(arena.get());

                using TestMapType = std::map<std::string,
                                             std::string,
                                             std::less<std::string>,
                                             ArenaAllocT<std::pair<const std::string, std::string>>>;

                TestMapType test_map(alloc);

                size_t n = 0;
                for (const auto & kv_ix : tc.kv_pairs_) {
                    test_map[kv_ix.first] = kv_ix.second;
                    ++n;

                    REQUIRE(test_map.size() == n);

                    for (const auto & map_ix : test_map) {
                        // verify alloc was used for both Key + Value.
                        REQUIRE(arena->contains(&map_ix.first));
                        REQUIRE(arena->contains(&map_ix.second));
                    }
                }

            }
        }
    }
} /*namespace xo*/

/* end ArenaAllocT.test.cpp */
