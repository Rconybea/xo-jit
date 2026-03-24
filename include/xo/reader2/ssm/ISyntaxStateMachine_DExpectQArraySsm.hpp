/** @file ISyntaxStateMachine_DExpectQArraySsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectQArraySsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectQArraySsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DExpectQArraySsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DExpectQArraySsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DExpectQArraySsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DExpectQArraySsm,
               xo::scm::ISyntaxStateMachine_DExpectQArraySsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DExpectQArraySsm
         **/
        class ISyntaxStateMachine_DExpectQArraySsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dexpectqarrayssm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using ACollector = xo::scm::ASyntaxStateMachine::ACollector;
            using AGCObject = xo::scm::ASyntaxStateMachine::AGCObject;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dexpectqarrayssm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DExpectQArraySsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DExpectQArraySsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming token @p tk **/
            static void on_token(DExpectQArraySsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExpectQArraySsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DExpectQArraySsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for type emitted by nested ssm **/
            static void on_parsed_type(DExpectQArraySsm & self, obj<AType> type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal(DExpectQArraySsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal_with_token(DExpectQArraySsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm);
            /** consume formal arglist emitted by nested ssm **/
            static void on_parsed_formal_arglist(DExpectQArraySsm & self, DArray * arglist, ParserStateMachine * p_psm);
            /** update state machine for nested parsed expression @p expr **/
            static void on_parsed_expression(DExpectQArraySsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine @p p_psm for incoming parsed expression @p expr followed by token @p tk **/
            static void on_parsed_expression_with_token(DExpectQArraySsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for nested quoted literal @p lit **/
            static void on_quoted_literal(DExpectQArraySsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm);
            /** gc support: move immediate children to to-space and sub forwarding pointer **/
            static void forward_children(DExpectQArraySsm & self, obj<ACollector> gc);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */