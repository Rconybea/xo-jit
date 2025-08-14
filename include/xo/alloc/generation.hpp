/* generation.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#pragma once

#include <ostream>
#include <cstdint>

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

    } /*namespace gc*/
} /*namespace xo*/

/* end generation.hpp */
