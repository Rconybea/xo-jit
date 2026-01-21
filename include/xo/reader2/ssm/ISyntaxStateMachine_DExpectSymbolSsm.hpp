/** @file ISyntaxStateMachine_DExpectSymbolSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectSymbolSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectSymbolSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DExpectSymbolSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DExpectSymbolSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DExpectSymbolSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DExpectSymbolSsm,
               xo::scm::ISyntaxStateMachine_DExpectSymbolSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DExpectSymbolSsm
         **/
        class ISyntaxStateMachine_DExpectSymbolSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dexpectsymbolssm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dexpectsymbolssm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DExpectSymbolSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DExpectSymbolSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming symbol-token @p tk **/
            static void on_symbol_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming define-keyword-token @p tk **/
            static void on_def_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming colon-token @p tk **/
            static void on_colon_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExpectSymbolSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DExpectSymbolSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */