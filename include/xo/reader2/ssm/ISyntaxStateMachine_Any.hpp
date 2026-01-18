/** @file ISyntaxStateMachine_Any.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/SyntaxStateMachine.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/SyntaxStateMachine.json5]
 **/

#pragma once

#include "ASyntaxStateMachine.hpp"
#include <xo/facet/obj.hpp>

namespace xo { namespace scm { class ISyntaxStateMachine_Any; } }

namespace xo {
namespace facet {

template <>
struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                           DVariantPlaceholder>
{
    using ImplType = xo::scm::ISyntaxStateMachine_Any;
};

}
}

namespace xo {
namespace scm {

    /** @class ISyntaxStateMachine_Any
     *  @brief ASyntaxStateMachine implementation for empty variant instance
     **/
    class ISyntaxStateMachine_Any : public ASyntaxStateMachine {
    public:
        /** @defgroup scm-syntaxstatemachine-any-type-traits **/
        ///@{

        /** integer identifying a type **/
        using typeseq = xo::facet::typeseq;

        ///@}
        /** @defgroup scm-syntaxstatemachine-any-methods **/
        ///@{

        const ASyntaxStateMachine * iface() const { return std::launder(this); }

        // from ASyntaxStateMachine

        // const methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        [[noreturn]] syntaxstatetype ssm_type(Copaque)  const  noexcept override { _fatal(); }
        [[noreturn]] std::string_view get_expect_str(Copaque)  const  noexcept override { _fatal(); }

        // nonconst methods
        [[noreturn]] void on_if_token(Opaque, const Token &, ParserStateMachine *)  override;

        ///@}

    private:
        /** @defgraoup scm-syntaxstatemachine-any-private-methods **/
        ///@{

        [[noreturn]] static void _fatal();

        ///@}

    public:
        /** @defgroup scm-syntaxstatemachine-any-member-vars **/
        ///@{

        static typeseq s_typeseq;
        static bool _valid;

        ///@}
    };

} /*namespace scm */
} /*namespace xo */

/* ISyntaxStateMachine_Any.hpp */