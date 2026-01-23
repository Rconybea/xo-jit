/** @file ISyntaxStateMachine_DProgressSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DProgressSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DProgressSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DProgressSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DProgressSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DProgressSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DProgressSsm,
               xo::scm::ISyntaxStateMachine_DProgressSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DProgressSsm
         **/
        class ISyntaxStateMachine_DProgressSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dprogressssm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dprogressssm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DProgressSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DProgressSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming symbol-token @p tk **/
            static void on_symbol_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming define-keyword-token @p tk **/
            static void on_def_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming colon-token @p tk **/
            static void on_colon_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming singleassign-token @p tk **/
            static void on_singleassign_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming f64-token @p tk **/
            static void on_f64_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming i64-token @p tk **/
            static void on_i64_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming bool-token @p tk **/
            static void on_bool_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming semicolon-token @p tk **/
            static void on_semicolon_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DProgressSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DProgressSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr **/
            static void on_parsed_expression(DProgressSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr followed by semicolon **/
            static void on_parsed_expression_with_semicolon(DProgressSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */