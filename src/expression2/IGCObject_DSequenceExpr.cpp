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
        IGCObject_DSequenceExpr::gco_shallow_move(DSequenceExpr & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DSequenceExpr::visit_gco_children(DSequenceExpr & self, VisitReason reason, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(reason, fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DSequenceExpr.cpp */
