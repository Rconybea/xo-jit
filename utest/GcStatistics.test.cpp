/* @file GcStatistics.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "xo/alloc/GcStatistics.hpp"
#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/tostr.hpp"
#include "xo/indentlog/print/hex.hpp"
#include <catch2/catch.hpp>
#include <ranges>

namespace xo {
    using xo::gc::GcStatistics;
    using xo::gc::GcStatisticsExt;
    using xo::gc::PerGenerationStatistics;
    using xo::print::ppconfig;

    namespace ut {
        TEST_CASE("PerGenerationStatistics", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            PerGenerationStatistics stats;

            std::string s = tostr(stats);

            //std::cerr << hex_view(s.c_str(), s.c_str() + s.length(), true /*as_text*/) << std::endl;

            REQUIRE(s == "<PerGenerationStatistics :used 0 :n_gc 0 :new_alloc_z 0 :scanned_z 0 :survive_z 0 :promote_z 0>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("GcStatistics", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            GcStatistics stats;

            std::string s = tostr(stats);

            REQUIRE(s ==
                    "<GcStatistics"
                    /**/" :gen_v ["
                    /***/ "<PerGenerationStatistics"
                    /****/ " :used 0"
                    /****/ " :n_gc 0"
                    /****/ " :new_alloc_z 0"
                    /****/ " :scanned_z 0"
                    /****/ " :survive_z 0"
                    /****/ " :promote_z 0"
                    /***/ ">"
                    /***/ " <PerGenerationStatistics"
                    /****/ " :used 0"
                    /****/ " :n_gc 0"
                    /****/ " :new_alloc_z 0"
                    /****/ " :scanned_z 0"
                    /****/ " :survive_z 0"
                    /****/ " :promote_z 0"
                    /***/ ">]"
                    /**/ " :total_allocated 0"
                    /**/ " :total_promoted_sab 0"
                    ">");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("GcStatisticsExt", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            GcStatisticsExt stats({});

            std::string s = tostr(stats);

            REQUIRE(s == "<GcStatisticsExt :gen_v [<PerGenerationStatistics :used 0 :n_gc 0 :new_alloc_z 0 :scanned_z 0 :survive_z 0 :promote_z 0> <PerGenerationStatistics :used 0 :n_gc 0 :new_alloc_z 0 :scanned_z 0 :survive_z 0 :promote_z 0>] :total_allocated 0 :total_promoted_sab 0 :nursery_z 0 :nursery_before_ckp_z 0 :nursery_after_ckp_z 0 :tenured_z 0 :n_mutation 0 :n_logged_mutation 0 :n_xgen_mutation 0 :n_xckp_mutation 0>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("GcStatistics-pretty", "[alloc][gc][pretty]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            std::stringstream ss;
            ppconfig ppc;
            GcStatistics stats;

            std::string actual = toppstr2(ppc, stats);
            std::string expected
                = ("<GcStatistics\n"
                   "  :gen_v\n"
                   "    [ <PerGenerationStatistics\n"
                   "        :used_z 0\n"
                   "        :n_gc 0\n"
                   "        :new_alloc_z 0\n"
                   "        :scanned_z 0\n"
                   "        :survive_z 0\n"
                   "        :promote_z 0>,\n"
                   "      <PerGenerationStatistics\n"
                   "        :used_z 0\n"
                   "        :n_gc 0\n"
                   "        :new_alloc_z 0\n"
                   "        :scanned_z 0\n"
                   "        :survive_z 0\n"
                   "        :promote_z 0> ]\n"
                   "  :total_allocated 0\n"
                   "  :total_promoted_sab 0\n"
                   "  :total_promoted 0\n"
                   "  :n_mutation 0\n"
                   "  :n_logged_mutation 0\n"
                   "  :n_xgen_mutation 0\n"
                   "  :n_xckp_mutation 0>");

            if (actual != expected) {
                CHECK(actual == expected);
                CHECK(actual.length() == expected.length());

                auto a_ix = actual.begin();
                auto e_ix = expected.begin();

                std::size_t pos = 0;
                while (a_ix != actual.end() && e_ix != expected.end()) {
                    INFO(xtag("pos", pos));
                    INFO(xtag("matching_prefix", std::string(actual.c_str(), pos)));

                    REQUIRE(*a_ix == *e_ix);

                    ++a_ix;
                    ++e_ix;
                    ++pos;
                }
            }

            REQUIRE(actual == expected);

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("GcStatisticsExt-pretty", "[alloc][gc][pretty]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            std::stringstream ss;
            ppconfig ppc;
            GcStatisticsExt stats({});

            std::string actual = toppstr2(ppc, stats);
            std::string expected
                = ("<GcStatisticsExt\n"
                   "  :gen_v\n"
                   "    [ <PerGenerationStatistics\n"
                   "        :used_z 0\n"
                   "        :n_gc 0\n"
                   "        :new_alloc_z 0\n"
                   "        :scanned_z 0\n"
                   "        :survive_z 0\n"
                   "        :promote_z 0>,\n"
                   "      <PerGenerationStatistics\n"
                   "        :used_z 0\n"
                   "        :n_gc 0\n"
                   "        :new_alloc_z 0\n"
                   "        :scanned_z 0\n"
                   "        :survive_z 0\n"
                   "        :promote_z 0> ]\n"
                   "  :total_allocated 0\n"
                   "  :total_promoted_sab 0\n"
                   "  :total_promoted 0\n"
                   "  :n_mutation 0\n"
                   "  :n_logged_mutation 0\n"
                   "  :n_xgen_mutation 0\n"
                   "  :n_xckp_mutation 0\n"
                   "  :nursery_z 0\n"
                   "  :nursery_before_checkpoint_z 0\n"
                   "  :nursery_after_checkpoint_z 0\n"
                   "  :tenured_z 0>");

            if (actual != expected) {
                CHECK(actual == expected);
                CHECK(actual.length() == expected.length());

                auto a_ix = actual.begin();
                auto e_ix = expected.begin();

                std::size_t pos = 0;
                while (a_ix != actual.end() && e_ix != expected.end()) {
                    INFO(xtag("pos", pos));
                    INFO(xtag("matching_prefix", std::string(actual.c_str(), pos)));

                    REQUIRE(*a_ix == *e_ix);

                    ++a_ix;
                    ++e_ix;
                    ++pos;
                }
            }

            REQUIRE(actual == expected);

            tag_config::tag_color_enabled = saved;
        }
    }
} /*namespace xo*/

/* GcStatistics.test.cpp */
