/* @file ObjectStatistics.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "xo/alloc/ObjectStatistics.hpp"
#include "xo/reflect/Reflect.hpp"
#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/ppstr.hpp"
#include "xo/indentlog/print/tostr.hpp"
#include "xo/indentlog/print/hex.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::ObjectStatistics;
    using xo::gc::PerObjectTypeStatistics;
    using xo::reflect::Reflect;
    using xo::print::ppconfig;

    namespace ut {
        TEST_CASE("PerObjectTypeStatistics", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            PerObjectTypeStatistics stats;

            std::string s = tostr(stats);

            //std::cerr << hex_view(s.c_str(), s.c_str() + s.length(), true /*as_text*/) << std::endl;

            REQUIRE(s == "<PerObjectTypeStatistics :td nullptr :scanned_n 0 :scanned_z 0 :survive_n 0 :survive_z 0>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("PerObjectTypeStatistics-1", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            PerObjectTypeStatistics stats;
            stats.td_ = Reflect::require<bool>();
            stats.scanned_n_ = 4;
            stats.scanned_z_ = 16;
            stats.survive_n_ = 2;
            stats.survive_z_ = 8;

            std::string s = tostr(stats);

            //std::cerr << hex_view(s.c_str(), s.c_str() + s.length(), true /*as_text*/) << std::endl;

            REQUIRE(s == "<PerObjectTypeStatistics :td bool :scanned_n 4 :scanned_z 16 :survive_n 2 :survive_z 8>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("ObjectStatistics", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            ObjectStatistics stats;

            std::string s = tostr(stats);

            REQUIRE(s == "<ObjectStatistics>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("ObjectStatistics-1", "[alloc][gc]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            ObjectStatistics stats;
            stats.per_type_stats_v_.push_back(PerObjectTypeStatistics());

            std::string s = tostr(stats);

            REQUIRE(s == "<ObjectStatistics :[0] <PerObjectTypeStatistics :td nullptr :scanned_n 0 :scanned_z 0 :survive_n 0 :survive_z 0>>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("ObjectStatistics-pretty", "[alloc][gc][pretty]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            std::stringstream ss;
            ppconfig ppc;
            ObjectStatistics stats;

            std::string actual = toppstr2(ppc, stats);
            std::string expected
                = ("<ObjectTypeStatistics :per_type_stats_v []>");

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

        TEST_CASE("ObjectStatistics-pretty-1", "[alloc][gc][pretty]")
        {
            bool saved = tag_config::tag_color_enabled;

            tag_config::tag_color_enabled = false;

            PerObjectTypeStatistics objstats;
            objstats.td_ = Reflect::require<bool>();
            objstats.scanned_n_ = 4;
            objstats.scanned_z_ = 16;
            objstats.survive_n_ = 2;
            objstats.survive_z_ = 8;

            std::stringstream ss;
            ppconfig ppc;
            ObjectStatistics stats;
            stats.per_type_stats_v_.push_back(objstats);

            std::string actual = toppstr2(ppc, stats);

            std::string expected
                = ("<ObjectTypeStatistics\n"
                   "  :per_type_stats_v\n"
                   "    [ <PerObjectTypeStatistics\n"
                   "        :td bool\n"
                   "        :scanned_n 4\n"
                   "        :scanned_z 16\n"
                   "        :survive_n 2\n"
                   "        :survive_z 8> ]>");

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

            tag_config::tag_color_enabled = saved;
        }
    }
} /*namespace xo*/

/* ObjectStatistics.test.cpp */
