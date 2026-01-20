/** @file expression2_register_facets.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "expression2_register_facets.hpp"

#include <xo/expression2/detail/IGCObject_DUniqueString.hpp>
#include <xo/expression2/detail/IPrintable_DUniqueString.hpp>

#include <xo/expression2/detail/IPrintable_DDefineExpr.hpp>

#include <xo/expression2/detail/IPrintable_DVariable.hpp>

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

            FacetRegistry::register_impl<APrintable, DDefineExpr>();

            FacetRegistry::register_impl<APrintable, DVariable>();

            log && log(xtag("DUniqueString.tseq", typeseq::id<DUniqueString>()));
            log && log(xtag("DDefineExpr.tseq", typeseq::id<DDefineExpr>()));
            log && log(xtag("DVariable.tseq", typeseq::id<DVariable>()));

            log && log(xtag("AExpression.tqseq", typeseq::id<AExpression>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end expression2_register_facets.cpp */
