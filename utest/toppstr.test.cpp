/* @file toppstr.cpp */

#include "xo/indentlog/print/ppstr.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <catch2/catch.hpp>
#include <sstream>

namespace ut {
    using xo::toppstr;
    using xo::toppstr2;
    using xo::print::ppconfig;
    using xo::print::ppstate;

    TEST_CASE("toppstr_1", "[toppstr]") {
        std::string s = toppstr();
        REQUIRE(s.empty());
    }

    TEST_CASE("toppstr_2", "[toppstr]") {
        std::string s = toppstr("hello");
        REQUIRE(s == "hello");
    }

    TEST_CASE("toppstr_3", "[toppstr]") {
        std::string s = toppstr("the", " quick", " brown", " fox", " jumps", " over", " the", " lazy", " dog");
        REQUIRE(s == "the quick brown fox jumps over the lazy dog");
    }

    TEST_CASE("toppstr2_0", "[toppstr2]") {
        ppconfig ppc;
        ppc.right_margin_ = 40;
        ppc.indent_width_ = 0;

        std::string s = toppstr2(ppc, "the", " quick", " brown", " fox", " jumps", " over", " the", " lazy", " dog");
        REQUIRE(s == "the\n quick\n brown\n fox\n jumps\n over\n the\n lazy\n dog");
    }

    TEST_CASE("toppstr2_1", "[toppstr2]") {
        ppconfig ppc;
        ppc.right_margin_ = 40;
        ppc.indent_width_ = 2;

        std::string s = toppstr2(ppc, "the", " quick", " brown", " fox", " jumps", " over", " the", " lazy", " dog");
        REQUIRE(s == "the\n   quick\n   brown\n   fox\n   jumps\n   over\n   the\n   lazy\n   dog");
    }

    TEST_CASE("toppstr2_2", "[toppstr2]") {
        ppconfig ppc;
        ppc.right_margin_ = 40;
        ppc.indent_width_ = 4;

        std::string s = toppstr2(ppc, "the", " quick", " brown", " fox", " jumps", " over", " the", " lazy", " dog");
        REQUIRE(s == "the\n     quick\n     brown\n     fox\n     jumps\n     over\n     the\n     lazy\n     dog");
    }
}
