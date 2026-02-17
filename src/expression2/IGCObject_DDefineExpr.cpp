/** @file IGCObject_DDefineExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DDefineExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DDefineExpr.json5]
**/

#include "define/IGCObject_DDefineExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DDefineExpr::shallow_size(const DDefineExpr & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DDefineExpr::shallow_copy(const DDefineExpr & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DDefineExpr::forward_children(DDefineExpr & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DDefineExpr.cpp */
