/* @file pretty_vector.test.cpp */

#include "xo/indentlog/print/pretty.hpp"
#include "xo/indentlog/print/pretty_vector.hpp"
#include <catch2/catch.hpp>
#include <sstream>

namespace ut {
    using xo::print::ppconfig;
    using xo::print::ppstate_standalone;

    TEST_CASE("print-upto", "[pretty]") {
        std::stringstream ss;
        ppconfig cfg;
        ppstate_standalone pps(&ss, 0, &cfg);

        REQUIRE(pps.pos() == 0);
        REQUIRE(pps.lpos() == 0);
        REQUIRE(pps.avail_margin() == cfg.right_margin_);
        REQUIRE(pps.has_margin());
        REQUIRE(pps.has_budget(cfg.right_margin_));
        REQUIRE(pps.scan_no_newline(0));
    }

    TEST_CASE("pretty", "[pretty]") {
        ppconfig ppc;
        ppc.right_margin_ = 40;
        ppc.indent_width_ = 2;

        std::stringstream ss;
        ppstate_standalone pps(&ss, 0, &ppc);

        pps.pretty("hello");

        REQUIRE(ss.str() == "hello");
    }

    TEST_CASE("prettyvec", "[pretty]") {
        ppconfig ppc;
        ppc.right_margin_ = 20;
        ppc.indent_width_ = 2;

        std::stringstream ss;
        ppstate_standalone pps(&ss, 0, &ppc);

        std::vector<int64_t> test = {1, 2, 3, 4, 5, 6};

        pps.pretty(test);

        REQUIRE(ss.str() == "[1, 2, 3, 4, 5, 6]");
    }

    TEST_CASE("prettyvec2", "[pretty]") {
        ppconfig ppc;
        ppc.right_margin_ = 10;
        ppc.indent_width_ = 2;

        std::stringstream ss;
        ppstate_standalone pps(&ss, 0, &ppc);

        std::vector<int64_t> test = {1, 2, 3, 4, 5, 6};

        pps.pretty(test);

        REQUIRE(ss.str() == "[\n  1,\n  2,\n  3,\n  4,\n  5,\n  6\n]");
    }

    TEST_CASE("prettyvec3", "[pretty]") {
        ppconfig ppc;
        ppc.right_margin_ = 20;
        ppc.indent_width_ = 2;

        std::stringstream ss;
        ppstate_standalone pps(&ss, 0, &ppc);

        std::vector<std::vector<int64_t>> test = {{1, 2, 3, 4}, {4, 5, 6, 7}};

        pps.pretty(test);

        REQUIRE(ss.str() == "[\n  [1, 2, 3, 4],\n  [4, 5, 6, 7]\n]");
    }

    TEST_CASE("prettyvec4", "[pretty]") {
        ppconfig ppc;
        ppc.right_margin_ = 10;
        ppc.indent_width_ = 2;

        std::stringstream ss;
        ppstate_standalone pps(&ss, 0, &ppc);

        std::vector<std::vector<int64_t>> test = {{1, 2, 3, 4}, {4, 5, 6, 7}};

        pps.pretty(test);

        REQUIRE(ss.str() == "[\n  [\n    1,\n    2,\n    3,\n    4\n  ],\n  [\n    4,\n    5,\n    6,\n    7\n  ]\n]");
    }
}
