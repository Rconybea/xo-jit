/* generation.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "xo/alloc/generation.hpp"
#include <catch2/catch.hpp>
#include <cstring>

namespace xo {
    namespace gc {
        TEST_CASE("generation", "[gc]") {
            REQUIRE(::strcmp(gen2str(generation::nursery), "nursery") == 0);
            REQUIRE(::strcmp(gen2str(generation::tenured), "tenured") == 0);
            REQUIRE(::strcmp(gen2str(generation::N), "?generation") == 0);

            {
                std::stringstream ss;
                ss << generation::nursery;
                REQUIRE(ss.str() == "nursery");
            }

            {
                std::stringstream ss;
                ss << generation::tenured;
                REQUIRE(ss.str() == "tenured");
            }

            {
                std::stringstream ss;
                ss << generation::N;
                REQUIRE(ss.str() == "?generation");
            }
        }
    } /*namespace gc*/
} /*namespace xo*/

/* generation.test.cpp */
