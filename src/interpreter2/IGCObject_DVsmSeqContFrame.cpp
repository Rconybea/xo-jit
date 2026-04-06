/** @file IGCObject_DVsmSeqContFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmSeqContFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmSeqContFrame.json5]
**/

#include "sequence/IGCObject_DVsmSeqContFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmSeqContFrame::gco_shallow_move(DVsmSeqContFrame & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DVsmSeqContFrame::visit_gco_children(DVsmSeqContFrame & self, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmSeqContFrame.cpp */
