/** @file ISyntaxStateMachine_DQuoteSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DQuoteSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DQuoteSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DQuoteSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DQuoteSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DQuoteSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DQuoteSsm,
               xo::scm::ISyntaxStateMachine_DQuoteSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DQuoteSsm
         **/
        class ISyntaxStateMachine_DQuoteSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dquotessm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using AGCObject = xo::scm::ASyntaxStateMachine::AGCObject;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dquotessm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DQuoteSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DQuoteSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming token @p tk **/
            static void on_token(DQuoteSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DQuoteSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DQuoteSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal(DQuoteSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal_with_token(DQuoteSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm);
            /** consume formal arglist emitted by nested ssm **/
            static void on_parsed_formal_arglist(DQuoteSsm & self, DArray * arglist, ParserStateMachine * p_psm);
            /** update state machine for nested parsed expression @p expr **/
            static void on_parsed_expression(DQuoteSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine @p p_psm for incoming parsed expression @p expr followed by token @p tk **/
            static void on_parsed_expression_with_token(DQuoteSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for nested quoted literal @p lit **/
            static void on_quoted_literal(DQuoteSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */