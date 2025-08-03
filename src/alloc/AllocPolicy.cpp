/* AllocPolicy.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "AllocPolicy.hpp"

/* note: inline/.hpp definition not allowed for operator delete */
void operator delete(void * ptr) noexcept {
    xo::xo.free(ptr);
}

/* end AllocPolicy.cpp */
