/** @file IGCObject_DLambdaExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DLambdaExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DLambdaExpr.json5]
**/

#include "detail/IGCObject_DLambdaExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DLambdaExpr::shallow_size(const DLambdaExpr & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DLambdaExpr::shallow_copy(const DLambdaExpr & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DLambdaExpr::forward_children(DLambdaExpr & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DLambdaExpr.cpp */
