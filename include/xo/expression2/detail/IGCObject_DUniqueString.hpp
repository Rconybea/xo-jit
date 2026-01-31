/** @file IGCObject_DUniqueString.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DUniqueString.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DUniqueString.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/gc/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DUniqueString.hpp"

namespace xo { namespace scm { class IGCObject_DUniqueString; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DUniqueString>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DUniqueString,
               xo::scm::IGCObject_DUniqueString>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DUniqueString
         **/
        class IGCObject_DUniqueString {
        public:
            /** @defgroup scm-gcobject-duniquestring-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-duniquestring-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DUniqueString & self) noexcept;
            /** copy instance using allocator **/
            static Opaque shallow_copy(const DUniqueString & self, obj<AAllocator> mm) noexcept;

            // non-const methods
            /** during GC: forward immdiate children **/
            static size_type forward_children(DUniqueString & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */