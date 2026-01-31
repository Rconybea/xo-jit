/** @file expression2_register_types.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "expression2_register_types.hpp"

#include "detail/IGCObject_DConstant.hpp"
#include "detail/IGCObject_DVariable.hpp"
//#include "detail/IGCObject_DDefineExpr.hpp"   // when avail
//#include "detail/IGCObject_DApplyExpr.hpp"    // when avail
//#include "detail/IGCObject_DLambdaExpr.hpp"   // when avail
#include "detail/IGCObject_DIfElseExpr.hpp"
#include "detail/IGCObject_DSequenceExpr.hpp"
//#include "detail/IGCObject_DLocalSymtab.hpp"  // when avail
#include "detail/IGCObject_DUniqueString.hpp"

//#include "detail/IPrintable_DUniqueString.hpp" // when avail

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

            ok &= gc.install_type(impl_for<AGCObject, DConstant>());
            ok &= gc.install_type(impl_for<AGCObject, DVariable>());
            //ok &= gc.install_type(impl_for<AGCObject, DDefineExpr>());  // when avail
            //ok &= gc.install_type(impl_for<AGCObject, DApplyExpr>());   // when avail
            //ok &= gc.install_type(impl_for<AGCObject, DLambdaExpr>());  // when avail
            ok &= gc.install_type(impl_for<AGCObject, DIfElseExpr>());
            ok &= gc.install_type(impl_for<AGCObject, DSequenceExpr>());

            //ok &= gc.install_type(impl_for<AGCObject, DLocalSymtab>()); // when avail

            ok &= gc.install_type(impl_for<AGCObject, DUniqueString>());

            return ok;
        }
    }
} /*namespace xo*/

/* end expression2_register_types.cpp */
