/** @file IGCObject_DClosure.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DClosure.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DClosure.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DClosure.hpp"

namespace xo { namespace scm { class IGCObject_DClosure; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DClosure>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DClosure,
               xo::scm::IGCObject_DClosure>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DClosure
         **/
        class IGCObject_DClosure {
        public:
            /** @defgroup scm-gcobject-dclosure-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dclosure-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using allocator **/
            static Opaque shallow_move(DClosure & self, obj<ACollector> gc) noexcept;
            /** during GC: forward immdiate children **/
            static void forward_children(DClosure & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */