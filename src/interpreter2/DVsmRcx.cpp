/** @file DVsmRcx.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmRcx.hpp"
#include "VirtualSchematikaMachine.hpp"
#include <xo/facet/FacetRegistry.hpp>

namespace xo {
    using xo::mm::AAllocator;
    using xo::mm::ACollector;

    namespace scm {

        DVsmRcx::DVsmRcx(DVirtualSchematikaMachine * vsm) : vsm_{vsm} {}

        obj<AAllocator>
        DVsmRcx::allocator() const noexcept
        {
            return vsm_->allocator();
        }

        obj<ACollector>
        DVsmRcx::collector() const noexcept
        {
            return vsm_->allocator().try_to_facet<ACollector>();
        }

        obj<AAllocator>
        DVsmRcx::error_allocator() const noexcept
        {
            return vsm_->error_allocator();
        }

        StringTable *
        DVsmRcx::stringtable() const noexcept
        {
            return vsm_->stringtable();
        }

        void
        DVsmRcx::visit_pools(const MemorySizeVisitor & visitor) const
        {
            vsm_->visit_pools(visitor);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmRcx.cpp */
