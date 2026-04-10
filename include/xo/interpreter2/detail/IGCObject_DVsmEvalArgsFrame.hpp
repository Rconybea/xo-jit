/** @file IGCObject_DVsmEvalArgsFrame.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmEvalArgsFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmEvalArgsFrame.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DVsmEvalArgsFrame.hpp"

namespace xo { namespace scm { class IGCObject_DVsmEvalArgsFrame; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DVsmEvalArgsFrame>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DVsmEvalArgsFrame,
               xo::scm::IGCObject_DVsmEvalArgsFrame>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DVsmEvalArgsFrame
         **/
        class IGCObject_DVsmEvalArgsFrame {
        public:
            /** @defgroup scm-gcobject-dvsmevalargsframe-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using AGCObjectVisitor = xo::mm::AGCObject::AGCObjectVisitor;
            using VisitReason = xo::mm::AGCObject::VisitReason;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dvsmevalargsframe-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using object visitor.
Arguably abusing the word 'visitor' here **/
            static Opaque gco_shallow_move(DVsmEvalArgsFrame & self, obj<AGCObjectVisitor> gc) noexcept;
            /** Invoke fn.visit_child(iface,data) for each child GCObject pointer.
Context: provides address of data pointer so it can be updated in place
when @p fn invokes garbage collector reentry point **/
            static void visit_gco_children(DVsmEvalArgsFrame & self, VisitReason reason, obj<AGCObjectVisitor> fn) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */