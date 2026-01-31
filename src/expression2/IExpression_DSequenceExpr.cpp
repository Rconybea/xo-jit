/** @file IExpression_DSequenceExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DSequenceExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DSequenceExpr.json5]
**/

#include "detail/IExpression_DSequenceExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DSequenceExpr::extype(const DSequenceExpr & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DSequenceExpr::typeref(const DSequenceExpr & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DSequenceExpr::valuetype(const DSequenceExpr & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DSequenceExpr::assign_valuetype(DSequenceExpr & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DSequenceExpr.cpp */