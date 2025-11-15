/* @file GC.test.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "xo/alloc/GC.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::GC;
    using xo::gc::generation;
    using xo::gc::Config;

    namespace ut {

        namespace {
            struct testcase_gc {
                testcase_gc(std::size_t nz, std::size_t tz, std::size_t n_gct, std::size_t t_gct)
                    : nursery_z_{nz}, tenured_z_{tz}, incr_gc_threshold_{n_gct}, full_gc_threshold_{t_gct} {}

                std::size_t nursery_z_;
                std::size_t tenured_z_;
                std::size_t incr_gc_threshold_;
                std::size_t full_gc_threshold_;
            };

            std::vector<testcase_gc>
            s_testcase_v = {
                //            nz    tz  n_gct  t_gct
                testcase_gc(1024, 4096,  1024,  4096)
            };
        }

        TEST_CASE("gc", "[alloc][gc]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                try {
                    const testcase_gc & tc = s_testcase_v[i_tc];

                    up<GC> gc = GC::make(
                        {.initial_nursery_z_ = tc.nursery_z_,
                         .initial_tenured_z_ = tc.tenured_z_,
                         .incr_gc_threshold_ = tc.incr_gc_threshold_,
                         .full_gc_threshold_ = tc.full_gc_threshold_,
                        });

                    REQUIRE(gc.get());
                    REQUIRE(gc->name() == "GC");
                    REQUIRE(gc->nursery_to_allocated() == 0);
                    REQUIRE(gc->nursery_to_committed() >= tc.nursery_z_);
                    REQUIRE(gc->nursery_to_reserved() >= tc.nursery_z_);
                    REQUIRE(gc->nursery_to_reserved() < tc.nursery_z_ + gc->pagesize());
                    REQUIRE(gc->size() >= tc.nursery_z_ + tc.tenured_z_);
                    REQUIRE(gc->size() < tc.nursery_z_ + gc->pagesize() + tc.tenured_z_ + gc->pagesize());
                    REQUIRE(gc->allocated() == 0);
                    REQUIRE(gc->available() == gc->nursery_to_reserved());
                    REQUIRE(gc->before_checkpoint() == 0);
                    // ListAlloc model is that nothing is before checkpoint
                    // until it's first established
                    REQUIRE(gc->after_checkpoint() == 0);

                    REQUIRE(gc->gc_in_progress() == false);
                    REQUIRE(gc->is_gc_enabled() == true);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::nursery)].n_gc_ == 0);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::tenured)].n_gc_ == 0);

                    /* gc with empty state */
                    gc->request_gc(generation::nursery);

                    REQUIRE(gc->gc_in_progress() == false);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::nursery)].n_gc_ == 1);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::tenured)].n_gc_ == 0);

                    /* still empty state */
                    gc->request_gc(generation::tenured);

                    REQUIRE(gc->gc_in_progress() == false);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::nursery)].n_gc_ == 1);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::tenured)].n_gc_ == 1);
                } catch (std::exception &ex) {
                    std::cerr << "caught exception: " << ex.what() << std::endl;
                    REQUIRE(false);
                }
            }
        }

    } /*namespace ut*/
} /*namespace xo*/

/* GC.test.cpp */
