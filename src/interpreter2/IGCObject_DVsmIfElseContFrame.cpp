/** @file IGCObject_DVsmIfElseContFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmIfElseContFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmIfElseContFrame.json5]
**/

#include "ifelse/IGCObject_DVsmIfElseContFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmIfElseContFrame::shallow_move(DVsmIfElseContFrame & self, obj<ACollector> gc) noexcept -> Opaque
        {
            return self.shallow_move(gc);
        }
        auto
        IGCObject_DVsmIfElseContFrame::visit_gco_children(DVsmIfElseContFrame & self, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmIfElseContFrame.cpp */
