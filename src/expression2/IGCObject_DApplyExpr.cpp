/** @file IGCObject_DApplyExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DApplyExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DApplyExpr.json5]
**/

#include "detail/IGCObject_DApplyExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DApplyExpr::shallow_size(const DApplyExpr & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DApplyExpr::shallow_copy(const DApplyExpr & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DApplyExpr::forward_children(DApplyExpr & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DApplyExpr.cpp */
