/** @file IGCObject_DLambdaExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DLambdaExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DLambdaExpr.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/gc/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DLambdaExpr.hpp"

namespace xo { namespace scm { class IGCObject_DLambdaExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DLambdaExpr>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DLambdaExpr,
               xo::scm::IGCObject_DLambdaExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DLambdaExpr
         **/
        class IGCObject_DLambdaExpr {
        public:
            /** @defgroup scm-gcobject-dlambdaexpr-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dlambdaexpr-methods **/
            ///@{
            // const methods
            /** memory consumption for this instance **/
            static size_type shallow_size(const DLambdaExpr & self) noexcept;
            /** copy instance using allocator **/
            static Opaque shallow_copy(const DLambdaExpr & self, obj<AAllocator> mm) noexcept;

            // non-const methods
            /** during GC: forward immdiate children **/
            static size_type forward_children(DLambdaExpr & self, obj<ACollector> gc) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */