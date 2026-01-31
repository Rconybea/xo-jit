/** @file IExpression_DVariable.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DVariable.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DVariable.json5]
**/

#include "detail/IExpression_DVariable.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DVariable::extype(const DVariable & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DVariable::typeref(const DVariable & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DVariable::valuetype(const DVariable & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DVariable::assign_valuetype(DVariable & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DVariable.cpp */