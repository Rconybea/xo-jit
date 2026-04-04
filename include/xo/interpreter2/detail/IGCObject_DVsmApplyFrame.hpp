/** @file IGCObject_DVsmApplyFrame.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmApplyFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmApplyFrame.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DVsmApplyFrame.hpp"

namespace xo { namespace scm { class IGCObject_DVsmApplyFrame; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DVsmApplyFrame>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DVsmApplyFrame,
               xo::scm::IGCObject_DVsmApplyFrame>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DVsmApplyFrame
         **/
        class IGCObject_DVsmApplyFrame {
        public:
            /** @defgroup scm-gcobject-dvsmapplyframe-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dvsmapplyframe-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DVsmApplyFrame & self) noexcept;

            // non-const methods
            /** move instance using allocator **/
            static Opaque shallow_move(DVsmApplyFrame & self, obj<ACollector> gc) noexcept;
            /** during GC: forward immdiate children **/
            static void forward_children(DVsmApplyFrame & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */