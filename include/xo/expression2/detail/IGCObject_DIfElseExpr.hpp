/** @file IGCObject_DIfElseExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DIfElseExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DIfElseExpr.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/gc/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DIfElseExpr.hpp"

namespace xo { namespace scm { class IGCObject_DIfElseExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DIfElseExpr>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DIfElseExpr,
               xo::scm::IGCObject_DIfElseExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DIfElseExpr
         **/
        class IGCObject_DIfElseExpr {
        public:
            /** @defgroup scm-gcobject-difelseexpr-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-difelseexpr-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DIfElseExpr & self) noexcept;
            /** copy instance using allocator **/
            static Opaque shallow_copy(const DIfElseExpr & self, obj<AAllocator> mm) noexcept;

            // non-const methods
            /** during GC: forward immdiate children **/
            static size_type forward_children(DIfElseExpr & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */