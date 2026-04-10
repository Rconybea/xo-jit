/** @file IGCObject_DVirtualSchematikaMachine.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVirtualSchematikaMachine.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVirtualSchematikaMachine.json5]
**/

#include "vsm/IGCObject_DVirtualSchematikaMachine.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVirtualSchematikaMachine::gco_shallow_move(DVirtualSchematikaMachine & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DVirtualSchematikaMachine::visit_gco_children(DVirtualSchematikaMachine & self, VisitReason reason, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(reason, fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVirtualSchematikaMachine.cpp */
