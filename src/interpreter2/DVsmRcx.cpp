/** @file DVsmRcx.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmRcx.hpp"
#include "VirtualSchematikaMachine.hpp"

namespace xo {
    using xo::mm::AAllocator;

    namespace scm {

        DVsmRcx::DVsmRcx(VirtualSchematikaMachine * vsm) : vsm_{vsm} {}

        obj<AAllocator>
        DVsmRcx::allocator() const noexcept
        {
            return vsm_->allocator();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmRcx.cpp */
