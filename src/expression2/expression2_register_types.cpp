/** @file expression2_register_types.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "expression2_register_types.hpp"

#include "DefineExpr.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Typename.hpp"
//#include "detail/IGCObject_DDefineExpr.hpp"   // when avail
//#include "detail/IGCObject_DApplyExpr.hpp"    // when avail
#include "LambdaExpr.hpp"
#include "IfElseExpr.hpp"
#include "SequenceExpr.hpp"
#include "LocalSymtab.hpp"
#include "GlobalSymtab.hpp"
//#include "detail/IGCObject_DLocalSymtab.hpp"  // when avail
//#include "detail/IGCObject_DUniqueString.hpp"

//#include "detail/IPrintable_DUniqueString.hpp" // when avail

#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::facet::impl_for;
    using xo::scope;

    namespace scm {
        bool
        expression2_register_types(obj<ACollector> gc)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            ok &= gc.install_type(impl_for<AGCObject, DConstant>());
            ok &= gc.install_type(impl_for<AGCObject, DVariable>());
            ok &= gc.install_type(impl_for<AGCObject, DTypename>());
            ok &= gc.install_type(impl_for<AGCObject, DDefineExpr>());
            //ok &= gc.install_type(impl_for<AGCObject, DApplyExpr>());   // when avail
            ok &= gc.install_type(impl_for<AGCObject, DLambdaExpr>());
            ok &= gc.install_type(impl_for<AGCObject, DIfElseExpr>());
            ok &= gc.install_type(impl_for<AGCObject, DSequenceExpr>());

            ok &= gc.install_type(impl_for<AGCObject, DLocalSymtab>());
            ok &= gc.install_type(impl_for<AGCObject, DGlobalSymtab>());

            return ok;
        }
    }
} /*namespace xo*/

/* end expression2_register_types.cpp */
