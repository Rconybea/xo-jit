/** @file IGCObject_DVsmApplyClosureFrame.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmApplyClosureFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmApplyClosureFrame.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DVsmApplyClosureFrame.hpp"

namespace xo { namespace scm { class IGCObject_DVsmApplyClosureFrame; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DVsmApplyClosureFrame>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DVsmApplyClosureFrame,
               xo::scm::IGCObject_DVsmApplyClosureFrame>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DVsmApplyClosureFrame
         **/
        class IGCObject_DVsmApplyClosureFrame {
        public:
            /** @defgroup scm-gcobject-dvsmapplyclosureframe-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using AGCObjectVisitor = xo::mm::AGCObject::AGCObjectVisitor;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dvsmapplyclosureframe-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using object visitor.
Arguably abusing the word 'visitor' here **/
            static Opaque gco_shallow_move(DVsmApplyClosureFrame & self, obj<AGCObjectVisitor> gc) noexcept;
            /** Invoke fn.visit_child(iface,data) for each child GCObject pointer.
Context: provides address of data pointer so it can be updated in place
when @p fn invokes garbage collector reentry point **/
            static void visit_gco_children(DVsmApplyClosureFrame & self, obj<AGCObjectVisitor> fn) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */