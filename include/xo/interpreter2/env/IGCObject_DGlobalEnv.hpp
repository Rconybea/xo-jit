/** @file IGCObject_DGlobalEnv.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DGlobalEnv.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DGlobalEnv.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/gc/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DGlobalEnv.hpp"

namespace xo { namespace scm { class IGCObject_DGlobalEnv; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DGlobalEnv>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DGlobalEnv,
               xo::scm::IGCObject_DGlobalEnv>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DGlobalEnv
         **/
        class IGCObject_DGlobalEnv {
        public:
            /** @defgroup scm-gcobject-dglobalenv-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dglobalenv-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DGlobalEnv & self) noexcept;
            /** copy instance using allocator **/
            static Opaque shallow_copy(const DGlobalEnv & self, obj<AAllocator> mm) noexcept;

            // non-const methods
            /** during GC: forward immdiate children **/
            static size_type forward_children(DGlobalEnv & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */