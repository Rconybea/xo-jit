/* @file cond.test.cpp */

#include "xo/indentlog/print/cond.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <catch2/catch.hpp>
#include <sstream>

using namespace xo;

namespace ut {
    TEST_CASE("cond", "[cond]") {
        tag_config::tag_color = color_spec_type::none();

        {
            std::stringstream ss;
            ss << cond(true, "yes", "no");
            REQUIRE(ss.str() == "yes");
        }

        {
            std::stringstream ss;
            ss << cond(false, "yes", "no");
            REQUIRE(ss.str() == "no");
        }

        {
            std::stringstream ss;
            ss << cond(true, 42, "none");
            REQUIRE(ss.str() == "42");
        }

        {
            std::stringstream ss;
            ss << cond(false, 42, "none");
            REQUIRE(ss.str() == "none");
        }

        {
            std::stringstream ss;

            int * ptr = nullptr;

            ss << cond(ptr, xtag("ptr", 123), xtag("ptr", "null"));

            REQUIRE(ss.str() == " :ptr null");
        }

        {
            std::stringstream ss;

            int * ptr = nullptr;

            ss << xtag("ptr", cond(ptr, 123, "null"));

            REQUIRE(ss.str() == " :ptr null");
        }
    }
}

/* end cond.test.cpp */
