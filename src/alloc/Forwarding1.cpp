/* file Forwarding1.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "Forwarding1.hpp"
#include <cstddef>
#include <cassert>

namespace xo {
    namespace obj {
        Forwarding1::Forwarding1(gp<Object> dest)
            : dest_{dest}
        {}

        Object *
        Forwarding1::_offset_destination(Object * src) const
        {
            intptr_t offset = src - static_cast<const Object *>(this);

            return dest_.ptr() + offset;
        }

        std::size_t
        Forwarding1::_shallow_size() const {
            assert(false);
            return 0;
        }

        Object *
        Forwarding1::_shallow_copy() const {
            assert(false);
            return nullptr;
        }

        std::size_t
        Forwarding1::_forward_children() {
            assert(false);
            return 0;
        }

    } /*namespace obj*/
} /*namespace xo*/

/* end Forwarding1.cpp */
