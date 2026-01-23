/** @file ISyntaxStateMachine_DDefineSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DDefineSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DDefineSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DDefineSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DDefineSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DDefineSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DDefineSsm,
               xo::scm::ISyntaxStateMachine_DDefineSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DDefineSsm
         **/
        class ISyntaxStateMachine_DDefineSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-ddefinessm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-ddefinessm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DDefineSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DDefineSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming symbol-token @p tk **/
            static void on_symbol_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming define-keyword-token @p tk **/
            static void on_def_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming if-keyword-token @p tk **/
            static void on_if_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming colon-token @p tk **/
            static void on_colon_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming singleassign-token @p tk **/
            static void on_singleassign_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming f64-token @p tk **/
            static void on_f64_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming i64-token @p tk **/
            static void on_i64_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming bool-token @p tk **/
            static void on_bool_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming string-token @p tk **/
            static void on_string_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for incoming semicolon-token @p tk **/
            static void on_semicolon_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DDefineSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DDefineSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr **/
            static void on_parsed_expression(DDefineSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine for incoming parsed expression @p expr followed by semicolon **/
            static void on_parsed_expression_with_semicolon(DDefineSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */