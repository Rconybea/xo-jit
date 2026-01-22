/** @file ISyntaxStateMachine_DExprSeqState.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
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
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
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
            /** operate state machine for incoming symbol-token @p tk **/
            static void on_symbol_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming define-keyword-token @p tk **/
            static void on_def_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming colon-token @p tk **/
            static void on_colon_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming singleassign-token @p tk **/
            static void on_singleassign_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming f64-token @p tk **/
            static void on_f64_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming semicolon-token @p tk **/
            static void on_semicolon_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExprSeqState & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DExprSeqState & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr **/
            static void on_parsed_expression(DExprSeqState & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr followed by semicolon **/
            static void on_parsed_expression_with_semicolon(DExprSeqState & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */