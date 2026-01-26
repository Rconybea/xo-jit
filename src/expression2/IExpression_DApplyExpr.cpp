/** @file IExpression_DApplyExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DApplyExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DApplyExpr.json5]
**/

#include "detail/IExpression_DApplyExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DApplyExpr::extype(const DApplyExpr & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DApplyExpr::typeref(const DApplyExpr & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DApplyExpr::valuetype(const DApplyExpr & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DApplyExpr::assign_valuetype(DApplyExpr & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DApplyExpr.cpp */