/** @file VsmPrimitives.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "VsmPrimitives.hpp"
#include <xo/numeric/NumericDispatch.hpp>
#include <xo/object2/Boolean.hpp>

namespace xo {
    //using xo::scm::NumericDispatch;
    using xo::mm::MemorySizeInfo;
    using xo::facet::FacetRegistry;
    using xo::facet::TypeRegistry;

    namespace scm {
        // ----- primitive: report_memory_use() -----

        obj<AGCObject>
        xfer_report_memory_use(obj<ARuntimeContext> rcx)
        {
            scope log(XO_DEBUG(true));

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            TypeRegistry::instance().visit_pools(visitor);
            NumericDispatch::instance().visit_pools(visitor);
            rcx.visit_pools(visitor);

            return DBoolean::box<AGCObject>(rcx.allocator(), true);
        }

        DPrimitive_gco_0 *
        VsmPrimitives::make_report_memory_use_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_0::_make(mm,
                                           "report_memory_use",
                                           &xfer_report_memory_use);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end VsmPrimitives.cpp */
