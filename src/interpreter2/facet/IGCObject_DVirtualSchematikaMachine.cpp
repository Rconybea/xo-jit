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
        IGCObject_DVirtualSchematikaMachine::shallow_size(const DVirtualSchematikaMachine & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DVirtualSchematikaMachine::shallow_move(DVirtualSchematikaMachine & self, obj<ACollector> gc) noexcept -> Opaque
        {
            return self.shallow_move(gc);
        }
        auto
        IGCObject_DVirtualSchematikaMachine::forward_children(DVirtualSchematikaMachine & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVirtualSchematikaMachine.cpp */
