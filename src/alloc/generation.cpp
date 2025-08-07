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

    } /*namespace gc*/
} /*namespace xo*/

/* generation.cpp */
