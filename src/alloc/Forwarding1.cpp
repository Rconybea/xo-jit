/* file Forwarding1.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "Forwarding1.hpp"
#include "xo/reflect/Reflect.hpp"
#include <cstddef>
#include <cassert>

namespace xo {
    using xo::reflect::Reflect;
    using xo::reflect::TaggedPtr;

    namespace obj {
        Forwarding1::Forwarding1(gp<Object> dest)
            : dest_{dest}
        {}

        TaggedPtr
        Forwarding1::self_tp() const
        {
            return Reflect::make_tp(const_cast<Forwarding1*>(this));
        }

        void
        Forwarding1::display(std::ostream & os) const
        {
            os << "<fwd" << xtag("dest-td", dest_->self_tp().td()->short_name()) << ">";
        }

        Object *
        Forwarding1::_offset_destination(Object * src) const
        {
            intptr_t offset = src - static_cast<const Object *>(this);

            return dest_.ptr() + offset;
        }

        Object *
        Forwarding1::_destination() {
            return dest_.ptr();
        }

        // LCOV_EXCL_START
        std::size_t
        Forwarding1::_shallow_size() const {
            assert(false);
            return 0;
        }
        // LCOV_EXCL_STOP

        // LCOV_EXCL_START
        Object *
        Forwarding1::_shallow_copy(gc::IAlloc *) const {
            assert(false);
            return nullptr;
        }
        // LCOV_EXCL_STOP

        // LCOV_EXCL_START
        std::size_t
        Forwarding1::_forward_children(gc::IAlloc *) {
            assert(false);
            return 0;
        }
        // LCOV_EXCL_STOP

    } /*namespace obj*/
} /*namespace xo*/

/* end Forwarding1.cpp */
