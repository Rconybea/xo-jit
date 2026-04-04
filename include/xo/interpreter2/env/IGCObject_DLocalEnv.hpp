/** @file IGCObject_DLocalEnv.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DLocalEnv.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DLocalEnv.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DLocalEnv.hpp"

namespace xo { namespace scm { class IGCObject_DLocalEnv; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DLocalEnv>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DLocalEnv,
               xo::scm::IGCObject_DLocalEnv>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DLocalEnv
         **/
        class IGCObject_DLocalEnv {
        public:
            /** @defgroup scm-gcobject-dlocalenv-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dlocalenv-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using allocator **/
            static Opaque shallow_move(DLocalEnv & self, obj<ACollector> gc) noexcept;
            /** during GC: forward immdiate children **/
            static void forward_children(DLocalEnv & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */