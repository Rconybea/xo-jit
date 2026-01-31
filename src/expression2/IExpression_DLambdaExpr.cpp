/** @file IExpression_DLambdaExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DLambdaExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DLambdaExpr.json5]
**/

#include "detail/IExpression_DLambdaExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DLambdaExpr::extype(const DLambdaExpr & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DLambdaExpr::typeref(const DLambdaExpr & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DLambdaExpr::valuetype(const DLambdaExpr & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DLambdaExpr::assign_valuetype(DLambdaExpr & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DLambdaExpr.cpp */
