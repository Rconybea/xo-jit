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
        IGCObject_DLambdaExpr::gco_shallow_move(DLambdaExpr & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DLambdaExpr::visit_gco_children(DLambdaExpr & self, VisitReason reason, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(reason, fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DLambdaExpr.cpp */
