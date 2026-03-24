/** @file ISyntaxStateMachine_DExpectQListSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectQListSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectQListSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DExpectQListSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DExpectQListSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DExpectQListSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DExpectQListSsm,
               xo::scm::ISyntaxStateMachine_DExpectQListSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DExpectQListSsm
         **/
        class ISyntaxStateMachine_DExpectQListSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dexpectqlistssm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using ACollector = xo::scm::ASyntaxStateMachine::ACollector;
            using AGCObject = xo::scm::ASyntaxStateMachine::AGCObject;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dexpectqlistssm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DExpectQListSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DExpectQListSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming token @p tk **/
            static void on_token(DExpectQListSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExpectQListSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DExpectQListSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for type emitted by nested ssm **/
            static void on_parsed_type(DExpectQListSsm & self, obj<AType> type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal(DExpectQListSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal_with_token(DExpectQListSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm);
            /** consume formal arglist emitted by nested ssm **/
            static void on_parsed_formal_arglist(DExpectQListSsm & self, DArray * arglist, ParserStateMachine * p_psm);
            /** update state machine for nested parsed expression @p expr **/
            static void on_parsed_expression(DExpectQListSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine @p p_psm for incoming parsed expression @p expr followed by token @p tk **/
            static void on_parsed_expression_with_token(DExpectQListSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for nested quoted literal @p lit **/
            static void on_quoted_literal(DExpectQListSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm);
            /** gc support: move immediate children to to-space and sub forwarding pointer **/
            static void forward_children(DExpectQListSsm & self, obj<ACollector> gc);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */