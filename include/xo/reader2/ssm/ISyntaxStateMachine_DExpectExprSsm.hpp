/** @file ISyntaxStateMachine_DExpectExprSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectExprSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectExprSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DExpectExprSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DExpectExprSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DExpectExprSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DExpectExprSsm,
               xo::scm::ISyntaxStateMachine_DExpectExprSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DExpectExprSsm
         **/
        class ISyntaxStateMachine_DExpectExprSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dexpectexprssm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dexpectexprssm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DExpectExprSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DExpectExprSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming symbol-token @p tk **/
            static void on_symbol_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming define-keyword-token @p tk **/
            static void on_def_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming colon-token @p tk **/
            static void on_colon_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming singleassign-token @p tk **/
            static void on_singleassign_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming f64-token @p tk **/
            static void on_f64_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming semicolon-token @p tk **/
            static void on_semicolon_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExpectExprSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DExpectExprSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr **/
            static void on_parsed_expression(DExpectExprSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */