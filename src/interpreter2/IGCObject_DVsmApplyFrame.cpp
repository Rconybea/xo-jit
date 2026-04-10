/** @file IGCObject_DVsmApplyFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmApplyFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmApplyFrame.json5]
**/

#include "detail/IGCObject_DVsmApplyFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmApplyFrame::gco_shallow_move(DVsmApplyFrame & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DVsmApplyFrame::visit_gco_children(DVsmApplyFrame & self, VisitReason reason, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(reason, fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmApplyFrame.cpp */
