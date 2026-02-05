/** @file IProcedure_DClosure.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IProcedure_DClosure.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IProcedure_DClosure.json5]
 **/

#pragma once

#include "Procedure.hpp"
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/procedure2/detail/IRuntimeContext_Xfer.hpp>
#include <xo/procedure2/Procedure.hpp>
#include <xo/procedure2/detail/IProcedure_Xfer.hpp>
#include "DClosure.hpp"

namespace xo { namespace scm { class IProcedure_DClosure; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AProcedure,
                                   xo::scm::DClosure>
        {
            using ImplType = xo::scm::IProcedure_Xfer
              <xo::scm::DClosure,
               xo::scm::IProcedure_DClosure>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IProcedure_DClosure
         **/
        class IProcedure_DClosure {
        public:
            /** @defgroup scm-procedure-dclosure-type-traits **/
            ///@{
            using AGCObject = xo::scm::AProcedure::AGCObject;
            using Copaque = xo::scm::AProcedure::Copaque;
            using Opaque = xo::scm::AProcedure::Opaque;
            ///@}
            /** @defgroup scm-procedure-dclosure-methods **/
            ///@{
            // const methods
            /** true iff procedure takes n arguments **/
            static bool is_nary(const DClosure & self) noexcept;
            /** number of arguments. -1 for n-ary **/
            static std::int32_t n_args(const DClosure & self) noexcept;

            // non-const methods
            /** invoke procedure; assume arguments satisfy type system **/
            static obj<AGCObject> apply_nocheck(DClosure & self, obj<ARuntimeContext> rcx, const DArray * args);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */