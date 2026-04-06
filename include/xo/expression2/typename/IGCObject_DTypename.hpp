/** @file IGCObject_DTypename.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DTypename.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DTypename.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include "DTypename.hpp"

namespace xo { namespace scm { class IGCObject_DTypename; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DTypename>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DTypename,
               xo::scm::IGCObject_DTypename>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DTypename
         **/
        class IGCObject_DTypename {
        public:
            /** @defgroup scm-gcobject-dtypename-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using AGCObjectVisitor = xo::mm::AGCObject::AGCObjectVisitor;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dtypename-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using object visitor.
Arguably abusing the word 'visitor' here **/
            static Opaque gco_shallow_move(DTypename & self, obj<AGCObjectVisitor> gc) noexcept;
            /** Invoke fn.visit_child(iface,data) for each child GCObject pointer.
Context: provides address of data pointer so it can be updated in place
when @p fn invokes garbage collector reentry point **/
            static void visit_gco_children(DTypename & self, obj<AGCObjectVisitor> fn) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */