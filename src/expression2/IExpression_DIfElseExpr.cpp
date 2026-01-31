/** @file IExpression_DIfElseExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DIfElseExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DIfElseExpr.json5]
**/

#include "detail/IExpression_DIfElseExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DIfElseExpr::extype(const DIfElseExpr & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DIfElseExpr::typeref(const DIfElseExpr & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DIfElseExpr::valuetype(const DIfElseExpr & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DIfElseExpr::assign_valuetype(DIfElseExpr & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DIfElseExpr.cpp */
