/** @file IGCObject_DLocalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DLocalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DLocalSymtab.json5]
**/

#include "symtab/IGCObject_DLocalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DLocalSymtab::gco_shallow_move(DLocalSymtab & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DLocalSymtab::visit_gco_children(DLocalSymtab & self, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DLocalSymtab.cpp */
