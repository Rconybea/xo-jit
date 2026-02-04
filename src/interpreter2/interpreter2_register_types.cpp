/** @file interpreter2_register_types.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "interpreter2_register_types.hpp"

#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"

#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::facet::impl_for;
    //using xo::facet::typeseq;
    using xo::scope;

    namespace scm {
        bool
        interpreter2_register_types(obj<ACollector> gc)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            ok &= gc.install_type(impl_for<AGCObject, DVsmApplyFrame>());
            ok &= gc.install_type(impl_for<AGCObject, DVsmEvalArgsFrame>());

            return ok;
        }
    }
} /*namespace xo*/

/* end interpreter2_register_types.cpp */
