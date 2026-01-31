/** @file IExpression_DConstant.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DConstant.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DConstant.json5]
**/

#include "detail/IExpression_DConstant.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DConstant::extype(const DConstant & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DConstant::typeref(const DConstant & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DConstant::valuetype(const DConstant & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DConstant::assign_valuetype(DConstant & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DConstant.cpp */