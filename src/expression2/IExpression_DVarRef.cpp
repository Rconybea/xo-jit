/** @file IExpression_DVarRef.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DVarRef.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DVarRef.json5]
**/

#include "detail/IExpression_DVarRef.hpp"

namespace xo {
    namespace scm {
        auto
        IExpression_DVarRef::extype(const DVarRef & self) noexcept -> exprtype
        {
            return self.extype();
        }

        auto
        IExpression_DVarRef::typeref(const DVarRef & self) noexcept -> TypeRef
        {
            return self.typeref();
        }

        auto
        IExpression_DVarRef::valuetype(const DVarRef & self) noexcept -> TypeDescr
        {
            return self.valuetype();
        }

        auto
        IExpression_DVarRef::assign_valuetype(DVarRef & self, TypeDescr td) noexcept -> void
        {
            self.assign_valuetype(td);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IExpression_DVarRef.cpp */
