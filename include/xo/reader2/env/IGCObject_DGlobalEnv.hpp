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
#include <xo/alloc2/GCObject.hpp>
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
            using AGCObjectVisitor = xo::mm::AGCObject::AGCObjectVisitor;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dglobalenv-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using allocator **/
            static Opaque shallow_move(DGlobalEnv & self, obj<ACollector> gc) noexcept;
            /** Invoke fn.visit_child(iface,data) for each child GCObject pointer.
Context: provides address of data pointer so it can be updated in place
when @p fn invokes garbage collector reentry point **/
            static void visit_gco_children(DGlobalEnv & self, obj<AGCObjectVisitor> fn) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */