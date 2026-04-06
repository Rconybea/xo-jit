/** @file IGCObject_DVsmEvalArgsFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmEvalArgsFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmEvalArgsFrame.json5]
**/

#include "detail/IGCObject_DVsmEvalArgsFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmEvalArgsFrame::shallow_move(DVsmEvalArgsFrame & self, obj<ACollector> gc) noexcept -> Opaque
        {
            return self.shallow_move(gc);
        }
        auto
        IGCObject_DVsmEvalArgsFrame::visit_gco_children(DVsmEvalArgsFrame & self, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmEvalArgsFrame.cpp */
