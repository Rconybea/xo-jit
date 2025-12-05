/* generation.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#pragma once

#include <ostream>
#include <cstdint>
#include <cassert>

namespace xo {
    namespace gc {
        enum class generation {
            nursery,
            tenured,
            N
        };

        constexpr std::size_t gen2int(generation x) { return static_cast<std::size_t>(x); }

        const char * gen2str(generation x);

        inline std::ostream & operator<<(std::ostream & os, generation x) {
            os << gen2str(x);
            return os;
        }

        enum class generation_result {
            nursery,
            tenured,
            not_found
        };

        inline generation valid_genresult2gen(generation_result x) {
            assert(x != generation_result::not_found);

            if (x == generation_result::nursery)
                return generation::nursery;
            else
                return generation::tenured;
        }

        const char * genresult2str(generation_result x);

        inline std::ostream & operator<<(std::ostream & os, generation_result x) {
            os << genresult2str(x);
            return os;
        }

    } /*namespace gc*/
} /*namespace xo*/

/* end generation.hpp */
