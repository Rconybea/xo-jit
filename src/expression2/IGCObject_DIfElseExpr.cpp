/** @file IGCObject_DIfElseExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DIfElseExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DIfElseExpr.json5]
**/

#include "detail/IGCObject_DIfElseExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DIfElseExpr::shallow_move(DIfElseExpr & self, obj<ACollector> gc) noexcept -> Opaque
        {
            return self.shallow_move(gc);
        }
        auto
        IGCObject_DIfElseExpr::forward_children(DIfElseExpr & self, obj<ACollector> gc) noexcept -> void
        {
            self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DIfElseExpr.cpp */
