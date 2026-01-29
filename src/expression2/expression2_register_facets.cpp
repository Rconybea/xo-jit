/** @file expression2_register_facets.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "expression2_register_facets.hpp"

#include <xo/expression2/detail/IGCObject_DUniqueString.hpp>
#include <xo/expression2/detail/IPrintable_DUniqueString.hpp>

#include <xo/expression2/detail/IExpression_DDefineExpr.hpp>
#include <xo/expression2/detail/IPrintable_DDefineExpr.hpp>

#include <xo/expression2/detail/IExpression_DVariable.hpp>
#include <xo/expression2/detail/IGCObject_DVariable.hpp>
#include <xo/expression2/detail/IPrintable_DVariable.hpp>

#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/expression2/detail/IPrintable_DConstant.hpp>

#include <xo/expression2/detail/IExpression_DApplyExpr.hpp>
#include <xo/expression2/detail/IPrintable_DApplyExpr.hpp>

#include <xo/expression2/detail/IExpression_DLambdaExpr.hpp>
#include <xo/expression2/detail/IPrintable_DLambdaExpr.hpp>

#include <xo/expression2/detail/IExpression_DIfElseExpr.hpp>
#include <xo/expression2/detail/IPrintable_DIfElseExpr.hpp>

#include <xo/gc/detail/AGCObject.hpp>
#include <xo/printable2/detail/APrintable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::typeseq;

    namespace scm {
        bool
        expression2_register_facets()
        {
            scope log(XO_DEBUG(true));

            FacetRegistry::register_impl<AGCObject, DUniqueString>();
            FacetRegistry::register_impl<APrintable, DUniqueString>();

            // Expression
            // +- Constant
            // +- Variable
            // +- DefineExpr
            // +- ApplyExpr
            // +- LambdaExpr
            // \- IfElseExpr

            FacetRegistry::register_impl<AExpression, DConstant>();
            FacetRegistry::register_impl<APrintable, DConstant>();

            FacetRegistry::register_impl<AExpression, DVariable>();
            FacetRegistry::register_impl<AGCObject, DVariable>();
            FacetRegistry::register_impl<APrintable, DVariable>();

            FacetRegistry::register_impl<AExpression, DDefineExpr>();
            FacetRegistry::register_impl<APrintable, DDefineExpr>();

            FacetRegistry::register_impl<AExpression, DApplyExpr>();
            FacetRegistry::register_impl<APrintable, DApplyExpr>();

            FacetRegistry::register_impl<AExpression, DLambdaExpr>();
            FacetRegistry::register_impl<APrintable, DLambdaExpr>();

            FacetRegistry::register_impl<AExpression, DIfElseExpr>();
            FacetRegistry::register_impl<APrintable, DIfElseExpr>();

            log && log(xtag("DUniqueString.tseq", typeseq::id<DUniqueString>()));
            log && log(xtag("DDefineExpr.tseq", typeseq::id<DDefineExpr>()));
            log && log(xtag("DVariable.tseq", typeseq::id<DVariable>()));
            log && log(xtag("DConstant.tseq", typeseq::id<DConstant>()));
            log && log(xtag("DApplyExpr.tseq", typeseq::id<DApplyExpr>()));
            log && log(xtag("DLambdaExpr.tseq", typeseq::id<DLambdaExpr>()));
            log && log(xtag("DIfElseExpr.tseq", typeseq::id<DIfElseExpr>()));

            log && log(xtag("AExpression.tqseq", typeseq::id<AExpression>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end expression2_register_facets.cpp */
