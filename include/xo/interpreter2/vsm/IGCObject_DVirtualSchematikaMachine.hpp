/** @file IGCObject_DVirtualSchematikaMachine.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVirtualSchematikaMachine.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVirtualSchematikaMachine.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DVirtualSchematikaMachine.hpp"

namespace xo { namespace scm { class IGCObject_DVirtualSchematikaMachine; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DVirtualSchematikaMachine>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DVirtualSchematikaMachine,
               xo::scm::IGCObject_DVirtualSchematikaMachine>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DVirtualSchematikaMachine
         **/
        class IGCObject_DVirtualSchematikaMachine {
        public:
            /** @defgroup scm-gcobject-dvirtualschematikamachine-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dvirtualschematikamachine-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DVirtualSchematikaMachine & self) noexcept;

            // non-const methods
            /** copy instance using allocator **/
            static Opaque shallow_copy(DVirtualSchematikaMachine & self, obj<AAllocator> mm) noexcept;
            /** during GC: forward immdiate children **/
            static size_type forward_children(DVirtualSchematikaMachine & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */