/** @file INumeric_Any.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Numeric.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Numeric.json5]
 **/

#pragma once

#include "ANumeric.hpp"
#include <xo/facet/obj.hpp>

namespace xo { namespace scm { class INumeric_Any; } }

namespace xo {
namespace facet {

template <>
struct FacetImplementation<xo::scm::ANumeric,
                           DVariantPlaceholder>
{
    using ImplType = xo::scm::INumeric_Any;
};

}
}

namespace xo {
namespace scm {

    /** @class INumeric_Any
     *  @brief ANumeric implementation for empty variant instance
     **/
    class INumeric_Any : public ANumeric {
    public:
        /** @defgroup scm-numeric-any-type-traits **/
        ///@{

        /** integer identifying a type **/
        using typeseq = xo::facet::typeseq;

        ///@}
        /** @defgroup scm-numeric-any-methods **/
        ///@{

        const ANumeric * iface() const { return std::launder(this); }

        // from ANumeric

        // builtin methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        [[noreturn]] void _drop(Opaque) const noexcept override { _fatal(); }

        // const methods

        // nonconst methods

        ///@}

    private:
        /** @defgraoup scm-numeric-any-private-methods **/
        ///@{

        [[noreturn]] static void _fatal();

        ///@}

    public:
        /** @defgroup scm-numeric-any-member-vars **/
        ///@{

        static typeseq s_typeseq;
        static bool _valid;

        ///@}
    };

} /*namespace scm */
} /*namespace xo */

/* INumeric_Any.hpp */
