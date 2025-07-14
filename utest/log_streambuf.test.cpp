/* @file log_streambuf.test.cpp */

#include "xo/indentlog/log_streambuf.hpp"
#include "xo/indentlog/print/tag.hpp"
#include "xo/indentlog/print/quoted.hpp"
#include <catch2/catch.hpp>
//#include <sstream>

namespace ut {
    using xo::log_streambuf;

    TEST_CASE("log_streamhuf", "[log_streambuf]") {
        std::size_t z = 16;
        log_streambuf<char, std::char_traits<char>> sbuf(z);
        std::ostream ss(&sbuf);

        REQUIRE(sbuf.capacity() == z);
        REQUIRE(sbuf.pos() == 0);
        REQUIRE(sbuf.lpos() == 0);

        ss << '\n';

        REQUIRE(sbuf.capacity() == z);
        REQUIRE(sbuf.pos() == 1);
        REQUIRE(sbuf.lpos() == 1);
    }

    // write test cases with some random strings.
    // for each string a list of tuples {ch,pos,lpos}

    struct recd {
        char const * text_ = nullptr;
        std::size_t expect_pos_ = 0;
        std::size_t expect_lpos_ = 0;
    };

    struct test_case {
        std::size_t buf_capacity_;
        std::vector<recd> steps_;
    };

    std::vector<test_case> s_testcase_v = {
        {64, {{"\n", 1, 0},
              {"abcd", 5, 4},
              {"abcd\nefg\nhij", 17, 3},
              {"klmnopqrstuvwxyz", 33, 19},
            }},
        {32, {{"\n", 1, 0},
              {"abcd", 5, 4},
              {"abcd\nefg\nhij", 17, 3},
              {"klmnopqrstuvwxyz", 33, 19},
            }},
        {16, {{"\n", 1, 0},
              {"abcd", 5, 4},
              {"abcd\nefg\nhij", 17, 3},
              {"klmnopqrstuvwxyz", 33, 19},
            }},
        {8, {{"\n", 1, 0},
              {"abcd", 5, 4},
              {"abcd\nefg\nhij", 17, 3},
              {"klmnopqrstuvwxyz", 33, 19},
            }},
        {4, {{"\n", 1, 0},
              {"abcd", 5, 4},
              {"abcd\nefg\nhij", 17, 3},
              {"klmnopqrstuvwxyz", 33, 19},
            }},
    };

    TEST_CASE("log_streambuf2", "[log_streambuf]") {
        using xo::xtag;
        using xo::print::quot;
        using xo::print::unq;

        for (std::size_t i = 0; i < s_testcase_v.size(); ++i) {
            INFO(tostr(xtag("i", i)));

            const test_case & tc = s_testcase_v[i];

            std::size_t z = tc.buf_capacity_;
            log_streambuf<char> sbuf(z, true /*debug_flag*/);
            std::ostream ss(&sbuf);

            REQUIRE(sbuf.capacity() == z);
            REQUIRE(sbuf.pos() == 0);
            REQUIRE(sbuf.lpos() == 0);
            REQUIRE(sbuf.lo() + sbuf.capacity() == sbuf.hi());

            std::size_t j = 0;
            for (const recd & r : tc.steps_) {
                INFO(tostr(xtag("j", j), xtag("text", unq(r.text_))));

                ss << r.text_;

                REQUIRE(ss.good());
                REQUIRE(sbuf.capacity() >= z);
                REQUIRE(sbuf.lo() + sbuf.capacity() == sbuf.hi());
                REQUIRE(sbuf.pos() == r.expect_pos_);
                REQUIRE(sbuf.lpos() == r.expect_lpos_);

                ++j;
            }
        }
    }
}

/* end log_streambuf.test.cpp */
