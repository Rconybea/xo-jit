/** @file IGCObject_DSequenceExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DSequenceExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DSequenceExpr.json5]
**/

#include "detail/IGCObject_DSequenceExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DSequenceExpr::shallow_size(const DSequenceExpr & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DSequenceExpr::shallow_copy(const DSequenceExpr & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DSequenceExpr::forward_children(DSequenceExpr & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DSequenceExpr.cpp */
