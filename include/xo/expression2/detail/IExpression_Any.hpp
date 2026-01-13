/** @file IExpression_Any.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Expression.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Expression.json5]
 **/

#pragma once

#include "AExpression.hpp"
#include <xo/facet/obj.hpp>

namespace xo { namespace scm { class IExpression_Any; } }

namespace xo {
namespace facet {

template <>
struct FacetImplementation<xo::scm::AExpression,
                           DVariantPlaceholder>
{
    using ImplType = xo::scm::IExpression_Any;
};

}
}

namespace xo {
namespace scm {

    /** @class IExpression_Any
     *  @brief AExpression implementation for empty variant instance
     **/
    class IExpression_Any : public AExpression {
    public:
        /** @defgroup scm-expression-any-type-traits **/
        ///@{

        /** integer identifying a type **/
        using typeseq = xo::facet::typeseq;
        using TypeDescr = AExpression::TypeDescr;

        ///@}
        /** @defgroup scm-expression-any-methods **/
        ///@{

        const AExpression * iface() const { return std::launder(this); }

        // from AExpression

        // const methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        [[noreturn]] exprtype extype(Copaque)  const  noexcept override { _fatal(); }
        [[noreturn]] TypeRef typeref(Copaque)  const  noexcept override { _fatal(); }
        [[noreturn]] TypeDescr valuetype(Copaque)  const  noexcept override { _fatal(); }

        // nonconst methods
        [[noreturn]] void assign_valuetype(Opaque, TypeDescr)  noexcept override { _fatal(); }

        ///@}

    private:
        /** @defgraoup scm-expression-any-private-methods **/
        ///@{

        [[noreturn]] static void _fatal();

        ///@}

    public:
        /** @defgroup scm-expression-any-member-vars **/
        ///@{

        static typeseq s_typeseq;
        static bool _valid;

        ///@}
    };

} /*namespace scm */
} /*namespace xo */

/* IExpression_Any.hpp */