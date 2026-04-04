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
        IGCObject_DVsmApplyFrame::shallow_move(DVsmApplyFrame & self, obj<ACollector> gc) noexcept -> Opaque
        {
            return self.shallow_move(gc);
        }
        auto
        IGCObject_DVsmApplyFrame::forward_children(DVsmApplyFrame & self, obj<ACollector> gc) noexcept -> void
        {
            self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmApplyFrame.cpp */
