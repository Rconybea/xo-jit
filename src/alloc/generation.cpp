/* generation.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "generation.hpp"

namespace xo {
    namespace gc {
        const char * gen2str(generation x) {
            switch (x) {
            case generation::nursery: return "nursery";
            case generation::tenured: return "tenured";
            case generation::N: break;
            }
            return "?generation";
        }

        const char * genresult2str(generation_result x) {
            switch (x) {
            case generation_result::nursery: return "nursery";
            case generation_result::tenured: return "tenured";
            case generation_result::not_found: return "not-found";
            }
            return "?generation_result";
        }

    } /*namespace gc*/
} /*namespace xo*/

/* generation.cpp */
