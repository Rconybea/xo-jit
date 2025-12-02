/* @file IAlloc.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

//#include "xo/allocutil/IAlloc.hpp"
#include "xo/alloc/ArenaAlloc.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::IAlloc;
    using xo::gc::ArenaAlloc;

    namespace ut {
        TEST_CASE("ialloc", "[alloc]")
        {
            static_assert((sizeof(std::uintptr_t) == 8) && "possibly fine if this fails, but would want to know");

            REQUIRE(IAlloc::alloc_padding(0) == 0);

            for (std::size_t i = 1; i < sizeof(std::uintptr_t); ++i) {
                REQUIRE(IAlloc::alloc_padding(i) + i == IAlloc::c_alloc_alignment);
            }

            REQUIRE(IAlloc::alloc_padding(IAlloc::c_alloc_alignment) == 0);

            for (std::size_t i = 1; i < sizeof(std::uintptr_t); ++i) {
                REQUIRE(IAlloc::alloc_padding(IAlloc::c_alloc_alignment + i) + i == IAlloc::c_alloc_alignment);
            }
        }

        /* although xo::gc::allocator<T> is intended for
         * IAlloc derivatives (so T is ArenaAlloc | GC),
         * 
         * it only relies on allocate() and deallocate() methods
         */

        namespace {
            struct TestCase {
                explicit TestCase(size_t arena_z, size_t n, size_t n2) : arena_z_{arena_z}, n_{n}, n2_{n2} {}

                size_t arena_z_ = 0;
                size_t n_ = 0;
                size_t n2_ = 0;
            };

            std::vector<TestCase> s_testcase_v = { TestCase{1024*1024, 9, 13} };
        }

        TEST_CASE("gc.allocator", "[alloc]")
        {
            using xo::gc::allocator;

            constexpr bool c_debug_flag = false;

            for (size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                INFO(xtag("i_tc", i_tc));

                const TestCase & tc = s_testcase_v[i_tc];

                up<ArenaAlloc> mm1 = ArenaAlloc::make("arena1",
                                                      tc.arena_z_,
                                                      c_debug_flag);
                up<ArenaAlloc> mm2 = ArenaAlloc::make("arena2",
                                                      tc.arena_z_,
                                                      c_debug_flag);

                REQUIRE(mm1.get());
                REQUIRE(mm1->allocated() == 0);

                allocator<int32_t> alloc1(mm1.get());
                allocator<double> alloc1a(mm1.get());

                REQUIRE(mm2.get());
                REQUIRE(mm2->allocated() == 0);

                allocator<int32_t> alloc2(mm2.get());

                SECTION("IAlloc identity determines allocator equality") {
                    REQUIRE(alloc1 == alloc1a);
                    REQUIRE(alloc1 != alloc2);
                }

                int * p1 = nullptr;
                size_t z1 = 0;

                SECTION("alloc space for ints") {
                    p1 = alloc1.allocate(tc.n_);

                    REQUIRE(p1 != nullptr);

                    // note: allowing for alignment
                    REQUIRE(mm1->allocated() >= sizeof(int32_t) * tc.n_);
                    REQUIRE(mm1->allocated() <  sizeof(int32_t) * tc.n_ + IAlloc::c_alloc_alignment);
                    z1 = mm1->allocated();

                    // deallocate exists..
                    alloc1.deallocate(p1, tc.n_);

                    // ..but is a no-op
                    REQUIRE(mm1->allocated() == z1);
                }

                int * p2 = nullptr;

                SECTION("allocator independence") {
                    REQUIRE(mm2->allocated() == 0);
                  
                    p2 =  alloc2.allocate(tc.n2_);

                    REQUIRE(p2 != nullptr);
                    REQUIRE(p1 != p2);

                    REQUIRE(mm2->allocated() >= sizeof(int32_t) * tc.n2_);
                    REQUIRE(mm2->allocated() < sizeof(int32_t) * tc.n2_ + IAlloc::c_alloc_alignment);

                    // mm1 unaffected by mm2 allocation 
                    REQUIRE(mm1->allocated() == z1);
                }
            }
        }

    } /*namespace ut*/
} /*namespace xo*/

/* end IAlloc.test.cpp */
