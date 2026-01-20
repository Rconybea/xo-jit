/* @file log_streambuf.test.cpp */

#include "scope.hpp"
#include "log_streambuf.hpp"
#include "print/tag.hpp"
#include "print/quoted.hpp"
#include <catch2/catch.hpp>
#include <string_view>
//#include <sstream>

namespace ut {
    using xo::xtag;
    using xo::scope;
    using xo::log_streambuf;
    using std::string_view;
    using std::cerr;
    using std::endl;

    TEST_CASE("log_streamhuf", "[log_streambuf]")
    {
        constexpr bool c_debug_flag = false;
        //scope log(XO_DEBUG(c_debug_flag), "log_streambuf test");

        std::size_t z = 16;
        log_streambuf<char, std::char_traits<char>> sbuf(z, c_debug_flag);
        std::ostream ss(&sbuf);

        REQUIRE(sbuf.debug_flag() == c_debug_flag);

        if (c_debug_flag) {
            cerr << "empty log_streambuf" << endl;
            cerr << "sbuf.lo=" << (void*)sbuf.lo() << endl;
            cerr << "sbuf.hi=" << (void*)sbuf.hi() << endl;
            cerr << "sbuf._color_escape_chars=" << sbuf._color_escape_chars() << endl;
        }

        //REQUIRE(sbuf.lo() == sbuf.pbase());

        REQUIRE(sbuf.capacity() == z);
        REQUIRE(sbuf.pos() == 0);
        REQUIRE(sbuf._solpos() == 0);
        REQUIRE(sbuf.lpos() == 0);

        // note: log_streambuf doesn't get control on every character

        ss << '\n';

        if (c_debug_flag) {
            cerr << "after single newline" << endl;
            cerr << "sbuf.lo=" << (void*)sbuf.lo() << endl;
            cerr << "sbuf.hi=" << (void*)sbuf.hi() << endl;
            cerr << "sbuf._color_escape_chars=" << sbuf._color_escape_chars() << endl;
        }

        REQUIRE(sbuf.capacity() == z);
        REQUIRE(sbuf.pos() == 1);
        // we don't know what sbuf._solpos is.  Could be 0 or 1 depending on
        // ostream implementation
        bool ok = (sbuf._solpos() == 0) || (sbuf._solpos() == 1);
        REQUIRE(ok);
        REQUIRE(sbuf.lpos() == 0);  // at least on OSX
    }

    TEST_CASE("log_streambuf_xtag", "[log_streambuf][xtag]")
    {
        constexpr bool c_debug_flag = false;
        scope log(XO_DEBUG(c_debug_flag), "log_streambuf_xtag test");

        std::size_t z = 16;
        log_streambuf<char, std::char_traits<char>> sbuf(z, false);
        std::ostream ss(&sbuf);

        ss << "empty log_streambuf";
        ss << xtag("sbuf.lo", (void*)nullptr);
        ss << xtag("sbuf.hi", (void*)nullptr);
        ss << xtag("sbuf.color_escape_chars", sbuf._color_escape_chars());

        //REQUIRE(sbuf.lo() == sbuf.pbase());

        auto expected = string_view("empty log_streambuf :sbuf.lo 0 :sbuf.hi 0 :sbuf.color_escape_chars 0");

        REQUIRE(string_view(sbuf) == expected);

        /* sbuf size will have been expanded */
        REQUIRE(sbuf.capacity() == 128);
        REQUIRE(sbuf.pos() == expected.size());
        REQUIRE(sbuf._solpos() == 0);
        REQUIRE(sbuf.lpos() == expected.size());

        // note: log_streambuf doesn't get control on every character

        ss << '\n';

        REQUIRE(sbuf.capacity() == 128);
        REQUIRE(sbuf.pos() == expected.size() + 1);
        REQUIRE(sbuf.lpos() == 0);
        // note: solpos: updated b/c of call to lazy sbuf.lpos()
        REQUIRE(sbuf._solpos() == expected.size() + 1);
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
            log_streambuf<char> sbuf(z, false /*debug_flag*/);
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
