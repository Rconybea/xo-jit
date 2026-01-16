/** @file expression2_register_types.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "expression2_register_types.hpp"

#include "detail/IGCObject_DUniqueString.hpp"

//#include "detail/IPrintable_DUniqueString.hpp"

//#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::facet::impl_for;
    using xo::facet::typeseq;
    using xo::scope;

    namespace scm {
        bool
        expression2_register_types(obj<ACollector> gc)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            ok &= gc.install_type(impl_for<AGCObject, DUniqueString>());

            return ok;
        }
    }
} /*namespace xo*/

/* end expression2_register_types.cpp */
