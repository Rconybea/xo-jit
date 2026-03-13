/** @file reader2_register_types.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "reader2_register_types.hpp"
#include "GlobalEnv.hpp"

#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::facet::impl_for;
    using xo::scope;

    namespace scm {
        bool
        reader2_register_types(obj<ACollector> gc)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            ok &= gc.install_type(impl_for<AGCObject, DGlobalEnv>());

            return ok;
        }
    }
} /*namespace xo*/

/* end reader2_register_types.cpp */
