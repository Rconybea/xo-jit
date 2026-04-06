/** @file IGCObject_DSchematikaParser.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DSchematikaParser.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DSchematikaParser.json5]
 **/

#pragma once

#include "GCObject.hpp"
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include "DSchematikaParser.hpp"

namespace xo { namespace scm { class IGCObject_DSchematikaParser; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::mm::AGCObject,
                                   xo::scm::DSchematikaParser>
        {
            using ImplType = xo::mm::IGCObject_Xfer
              <xo::scm::DSchematikaParser,
               xo::scm::IGCObject_DSchematikaParser>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IGCObject_DSchematikaParser
         **/
        class IGCObject_DSchematikaParser {
        public:
            /** @defgroup scm-gcobject-dschematikaparser-type-traits **/
            ///@{
            using size_type = xo::mm::AGCObject::size_type;
            using AAllocator = xo::mm::AGCObject::AAllocator;
            using ACollector = xo::mm::AGCObject::ACollector;
            using AGCObjectVisitor = xo::mm::AGCObject::AGCObjectVisitor;
            using Copaque = xo::mm::AGCObject::Copaque;
            using Opaque = xo::mm::AGCObject::Opaque;
            ///@}
            /** @defgroup scm-gcobject-dschematikaparser-methods **/
            ///@{
            // const methods

            // non-const methods
            /** move instance using collector **/
            static Opaque shallow_move(DSchematikaParser & self, obj<ACollector> gc) noexcept;
            /** Invoke fn.visit_child(iface,data) for each child GCObject pointer.
Context: provides address of data pointer so it can be updated in place
when @p fn invokes garbage collector reentry point **/
            static void visit_gco_children(DSchematikaParser & self, obj<AGCObjectVisitor> fn) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */