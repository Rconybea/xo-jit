/** @file ISyntaxStateMachine_DExprSeqState.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExprSeqState.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExprSeqState.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DExprSeqState.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DExprSeqState; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DExprSeqState>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DExprSeqState,
               xo::scm::ISyntaxStateMachine_DExprSeqState>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DExprSeqState
         **/
        class ISyntaxStateMachine_DExprSeqState {
        public:
            /** @defgroup scm-syntaxstatemachine-dexprseqstate-type-traits **/
            ///@{
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dexprseqstate-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DExprSeqState & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DExprSeqState & self) noexcept;

            // non-const methods
            /** update state machine for incoming define-keyworkd-token @p tk **/
            static void on_def_token(DExprSeqState & self, const Token & tk, ParserStateMachine * ps_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */