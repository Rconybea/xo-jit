/** @file IGCObject_DGlobalEnv.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DGlobalEnv.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DGlobalEnv.json5]
**/

#include "env/IGCObject_DGlobalEnv.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DGlobalEnv::gco_shallow_move(DGlobalEnv & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DGlobalEnv::visit_gco_children(DGlobalEnv & self, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DGlobalEnv.cpp */
