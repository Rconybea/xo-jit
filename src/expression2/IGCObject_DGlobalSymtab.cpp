/** @file IGCObject_DGlobalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DGlobalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DGlobalSymtab.json5]
**/

#include "symtab/IGCObject_DGlobalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DGlobalSymtab::gco_shallow_move(DGlobalSymtab & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DGlobalSymtab::visit_gco_children(DGlobalSymtab & self, VisitReason reason, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(reason, fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DGlobalSymtab.cpp */
