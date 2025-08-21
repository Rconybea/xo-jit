/* CircularBuffer.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "xo/alloc/CircularBuffer.hpp"
#include "xo/indentlog/print/vector.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::CircularBuffer;

    namespace ut {
        TEST_CASE("circular_buffer_0", "[circular_buffer]")
        {
            CircularBuffer<std::string> q(10, false /*debug_flag*/);
            q.push_back("a");
            REQUIRE(q.back() == "a");
            q.push_back("b");
            REQUIRE(q.back() == "b");
            q.push_back("c");
            REQUIRE(q.back() == "c");
            REQUIRE(q.location_of(0) == 0);
            REQUIRE(q.location_of(1) == 1);
            REQUIRE(q.location_of(2) == 2);
            //REQUIRE(q.index_of(0) == 0);

            REQUIRE(q.size() == 3);
            REQUIRE(q.front() == "a");
            REQUIRE(q.at(0) == "a");
            REQUIRE(q.at(1) == "b");
            REQUIRE(q.at(2) == "c");

            CircularBuffer<std::string> q2;

            q2 = q;

            q.clear();

            REQUIRE(q2.size() == 3);
            REQUIRE(q2.front() == "a");
            REQUIRE(q2.at(0) == "a");
            REQUIRE(q2.at(1) == "b");
            REQUIRE(q2.at(2) == "c");
        }

        TEST_CASE("circular_buffer_1", "[circular_buffer]")
        {
            CircularBuffer<std::string> q(2, false /*debug_flag*/);
            q.push_back("a");
            REQUIRE(q.back() == "a");
            q.push_back("b");
            REQUIRE(q.back() == "b");
            q.push_back("c");
            REQUIRE(q.back() == "c");
            REQUIRE(q.location_of(0) == 1);
            REQUIRE(q.location_of(1) == 0);
            //REQUIRE(q.index_of(0) == 0);

            REQUIRE(q.size() == 2);
            REQUIRE(q.front() == "b");
            REQUIRE(q.at(0) == "b");
            REQUIRE(q.at(1) == "c");

            {
                std::size_t i = 0;
                for (const auto & qi : q) {
                    REQUIRE(qi == q.at(i));
                    ++i;
                }
            }

            CircularBuffer<std::string> q2 = q;

            q.clear();

            REQUIRE(q2.size() == 2);
            REQUIRE(q2.front() == "b");
            REQUIRE(q2.at(0) == "b");
            REQUIRE(q2.at(1) == "c");

            {
                std::size_t i = 0;
                for (const auto & qi : q) {
                    REQUIRE(qi == q2.at(i));
                    ++i;
                }
            }
        }
    }

    namespace {
        struct Testcase_CircularBuffer {
            explicit Testcase_CircularBuffer(std::size_t capacity,
                                             const std::vector<std::string> & contents)
                : capacity_{capacity},
                  contents_{contents} {}

            std::size_t capacity_ = 0;
            std::vector<std::string> contents_;
        };

        std::vector<Testcase_CircularBuffer>
        s_testcase_v = {
            Testcase_CircularBuffer(0, {}),
            Testcase_CircularBuffer(1, {"a"}),
            Testcase_CircularBuffer(2, {"a", "b"}),
            Testcase_CircularBuffer(2, {"a", "b", "c", "d"})
        };
    }

    namespace ut {
        TEST_CASE("circular_buffer_2", "[circular_buffer]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                const Testcase_CircularBuffer & tc = s_testcase_v[i_tc];

                INFO(tostr(xtag("i_tc", i_tc),
                           xtag("capacity", tc.capacity_),
                           xrtag("contents", tc.contents_)));

                for (std::size_t j_phase = 0; j_phase < 2; ++j_phase) {
                    constexpr bool c_debug_flag = false;

                    CircularBuffer<std::string> q(tc.capacity_, false /*debug_flag*/);

                    REQUIRE(q.empty());
                    REQUIRE(q.size() == 0);
                    REQUIRE(q.begin() == q.end());
                    REQUIRE(q.capacity() == tc.capacity_);

                    std::size_t n = 0;
                    for (const auto & s : tc.contents_) {
                        INFO(tostr(xtag("n0", n), xtag("s", s)));
                        ++n;
                        INFO(xtag("n1", n));

                        q.push_back(s);

                        REQUIRE(q.back() == s);
                        REQUIRE(q.capacity() == tc.capacity_);
                        REQUIRE(q.size() == std::min(n, tc.capacity_));

                        std::size_t i = 0;
                        for (const auto & qi : q) {
                            INFO(tostr(xtag("i", i), xtag("qi", qi)));

                            if (n <= tc.capacity_) {
                                REQUIRE(qi == tc.contents_.at(i));
                                REQUIRE(qi == tc.contents_[i]);
                            } else {
                                REQUIRE(qi == tc.contents_.at(n - tc.capacity_ + i));
                                REQUIRE(qi == tc.contents_[n - tc.capacity_ + i]);
                            }
                            ++i;
                        }

                        REQUIRE(i == std::min(n, tc.capacity_));

                        if (tc.contents_.size() <= tc.capacity_)
                            REQUIRE(q.front() == tc.contents_.at(0));
                    }

                    q.clear();

                    REQUIRE(q.size() == 0);
                    REQUIRE(q.capacity() == tc.capacity_);
                }
            }
        }
    }
}

/* CircularBuffer.test.cpp */
