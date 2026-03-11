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
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dtypename-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DTypename & self) noexcept;
            /** copy instance using allocator **/
            static Opaque shallow_copy(const DTypename & self, obj<AAllocator> mm) noexcept;

            // non-const methods
            /** during GC: forward immdiate children **/
            static size_type forward_children(DTypename & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */