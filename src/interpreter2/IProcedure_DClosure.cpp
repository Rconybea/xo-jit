/** @file IProcedure_DClosure.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IProcedure_DClosure.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IProcedure_DClosure.json5]
**/

#include "detail/IProcedure_DClosure.hpp"

namespace xo {
    namespace scm {
        auto
        IProcedure_DClosure::is_nary(const DClosure & self) noexcept -> bool
        {
            return self.is_nary();
        }

        auto
        IProcedure_DClosure::n_args(const DClosure & self) noexcept -> std::int32_t
        {
            return self.n_args();
        }

        auto
        IProcedure_DClosure::apply_nocheck(DClosure & self, obj<ARuntimeContext> rcx, const DArray * args) -> obj<AGCObject>
        {
            return self.apply_nocheck(rcx, args);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IProcedure_DClosure.cpp */
