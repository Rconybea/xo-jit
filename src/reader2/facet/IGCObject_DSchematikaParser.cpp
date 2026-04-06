/** @file IGCObject_DSchematikaParser.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DSchematikaParser.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DSchematikaParser.json5]
**/

#include "parser/IGCObject_DSchematikaParser.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DSchematikaParser::gco_shallow_move(DSchematikaParser & self, obj<AGCObjectVisitor> gc) noexcept -> Opaque
        {
            return self.gco_shallow_move(gc);
        }
        auto
        IGCObject_DSchematikaParser::visit_gco_children(DSchematikaParser & self, obj<AGCObjectVisitor> fn) noexcept -> void
        {
            self.visit_gco_children(fn);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DSchematikaParser.cpp */
