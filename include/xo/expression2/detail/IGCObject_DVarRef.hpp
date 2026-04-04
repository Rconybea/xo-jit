/** @file IGCObject_DVarRef.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVarRef.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVarRef.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DVarRef.hpp"

namespace xo { namespace scm { class IGCObject_DVarRef; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DVarRef>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DVarRef,
               xo::scm::IGCObject_DVarRef>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DVarRef
         **/
        class IGCObject_DVarRef {
        public:
            /** @defgroup scm-gcobject-dvarref-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dvarref-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using allocator **/
            static Opaque shallow_move(DVarRef & self, obj<ACollector> gc) noexcept;
            /** during GC: forward immdiate children **/
            static void forward_children(DVarRef & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */