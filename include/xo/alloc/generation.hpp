/* generation.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include <cstdint>

namespace xo {
    namespace gc {
        enum class generation {
            nursery,
            tenured,
            N
        };

        constexpr std::size_t gen2int(generation x) { return static_cast<std::size_t>(x); }

        enum class generation_result {
            nursery,
            tenured,
            not_found
        };
    } /*namespace gc*/
} /*namespace xo*/

/* end generation.hpp */
