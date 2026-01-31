/** @file IExpression_DDefineExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DDefineExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DDefineExpr.json5]
**/

#include "detail/IExpression_DDefineExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DDefineExpr::extype(const DDefineExpr & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DDefineExpr::typeref(const DDefineExpr & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DDefineExpr::valuetype(const DDefineExpr & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DDefineExpr::assign_valuetype(DDefineExpr & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DDefineExpr.cpp */