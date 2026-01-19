/** @file ISyntaxStateMachine_DExpectSymbolSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
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
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExpectSymbolSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** update state machine for incoming define-keyword-token @p tk **/
            static void on_def_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */