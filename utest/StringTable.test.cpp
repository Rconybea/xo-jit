/** @file StringTable.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/expression2/StringTable.hpp>
#include <catch2/catch.hpp>
#include <cstring>

namespace xo {
    using xo::scm::StringTable;
    using xo::scm::DUniqueString;
    //using xo::scm::DString;

    namespace ut {
        TEST_CASE("StringTable-lookup-empty", "[expression2][StringTable]")
        {
            StringTable table(1024);

            // lookup on empty table returns nullptr
            REQUIRE(table.lookup("foo") == nullptr);
            REQUIRE(table.lookup("") == nullptr);
        }

        TEST_CASE("StringTable-intern", "[expression2][StringTable]")
        {
            StringTable table(1024);

            const DUniqueString * s1 = table.intern("hello");

            REQUIRE(s1 != nullptr);
            REQUIRE(std::strcmp(s1->chars(), "hello") == 0);
            REQUIRE(s1->size() == 5);
        }

        TEST_CASE("StringTable-intern-idempotent", "[expression2][StringTable]")
        {
            StringTable table(1024);

            const DUniqueString * s1 = table.intern("hello");
            const DUniqueString * s2 = table.intern("hello");

            // same key returns same pointer
            REQUIRE(s1 != nullptr);
            REQUIRE(s2 != nullptr);
            REQUIRE(s1 == s2);
        }

        TEST_CASE("StringTable-lookup-after-intern", "[expression2][StringTable]")
        {
            StringTable table(1024);

            REQUIRE(table.lookup("hello") == nullptr);

            const DUniqueString * s1 = table.intern("hello");

            const DUniqueString * s2 = table.lookup("hello");

            REQUIRE(s2 != nullptr);
            REQUIRE(s1 == s2);
        }

        TEST_CASE("StringTable-multiple-strings", "[expression2][StringTable]")
        {
            StringTable table(1024);

            const DUniqueString * s1 = table.intern("apple");
            const DUniqueString * s2 = table.intern("banana");
            const DUniqueString * s3 = table.intern("cherry");

            // all different pointers
            REQUIRE(s1 != s2);
            REQUIRE(s2 != s3);
            REQUIRE(s1 != s3);

            // correct contents
            REQUIRE(std::strcmp(s1->chars(), "apple") == 0);
            REQUIRE(std::strcmp(s2->chars(), "banana") == 0);
            REQUIRE(std::strcmp(s3->chars(), "cherry") == 0);

            // lookup still works
            REQUIRE(table.lookup("apple") == s1);
            REQUIRE(table.lookup("banana") == s2);
            REQUIRE(table.lookup("cherry") == s3);
            REQUIRE(table.lookup("date") == nullptr);
        }

        TEST_CASE("StringTable-intern-empty-string", "[expression2][StringTable]")
        {
            StringTable table(1024);

            const DUniqueString * s1 = table.intern("");

            REQUIRE(s1 != nullptr);
            REQUIRE(s1->size() == 0);
            REQUIRE(s1->chars()[0] == '\0');

            // idempotent for empty string too
            const DUniqueString * s2 = table.intern("");
            REQUIRE(s1 == s2);
        }

        TEST_CASE("StringTable-verify_ok", "[expression2][StringTable]")
        {
            StringTable table(4096);

            {
                INFO("1. empty table");

                // empty table passes verify_ok
                REQUIRE(table.verify_ok());
            }

            // after interning strings, still passes
            {
                INFO("2. intern(hello)");

                table.intern("hello");
                REQUIRE(table.verify_ok());
            }

            {
                INFO("3. intern(world)");

                table.intern("world");
                REQUIRE(table.verify_ok());
            }

            {
                INFO("4. intern(foo)");

                table.intern("foo");
                REQUIRE(table.verify_ok());
            }

            {
                INFO("5. intern(bar)");

                table.intern("bar");
                REQUIRE(table.verify_ok());
            }

            // idempotent intern doesn't break invariants
            {
                INFO("6. intern(hello)");

                table.intern("hello");
                REQUIRE(table.verify_ok());
            }

            {
                INFO("7. intern(world)");

                table.intern("world");
                REQUIRE(table.verify_ok());
            }
        }
    } /*namespace ut*/
} /*namespace xo*/

/* end StringTable.test.cpp */
