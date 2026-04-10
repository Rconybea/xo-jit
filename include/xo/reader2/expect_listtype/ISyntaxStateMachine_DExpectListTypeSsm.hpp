/** @file ISyntaxStateMachine_DExpectListTypeSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectListTypeSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectListTypeSsm.json5]
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_Xfer.hpp"
#include "DExpectListTypeSsm.hpp"

namespace xo { namespace scm { class ISyntaxStateMachine_DExpectListTypeSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASyntaxStateMachine,
                                   xo::scm::DExpectListTypeSsm>
        {
            using ImplType = xo::scm::ISyntaxStateMachine_Xfer
              <xo::scm::DExpectListTypeSsm,
               xo::scm::ISyntaxStateMachine_DExpectListTypeSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISyntaxStateMachine_DExpectListTypeSsm
         **/
        class ISyntaxStateMachine_DExpectListTypeSsm {
        public:
            /** @defgroup scm-syntaxstatemachine-dexpectlisttypessm-type-traits **/
            ///@{
            using TypeDescr = xo::scm::ASyntaxStateMachine::TypeDescr;
            using AGCObjectVisitor = xo::scm::ASyntaxStateMachine::AGCObjectVisitor;
            using AGCObject = xo::scm::ASyntaxStateMachine::AGCObject;
            using VisitReason = xo::scm::ASyntaxStateMachine::VisitReason;
            using Copaque = xo::scm::ASyntaxStateMachine::Copaque;
            using Opaque = xo::scm::ASyntaxStateMachine::Opaque;
            ///@}
            /** @defgroup scm-syntaxstatemachine-dexpectlisttypessm-methods **/
            ///@{
            // const methods
            /** identify a type of syntax state machine **/
            static syntaxstatetype ssm_type(const DExpectListTypeSsm & self) noexcept;
            /** text describing expected/allowed input to this ssm in current state **/
            static std::string_view get_expect_str(const DExpectListTypeSsm & self) noexcept;

            // non-const methods
            /** operate state machine for incoming token @p tk **/
            static void on_token(DExpectListTypeSsm & self, const Token & tk, ParserStateMachine * p_psm);
            /** update  stat machine for incoming parsed symbol @p sym **/
            static void on_parsed_symbol(DExpectListTypeSsm & self, std::string_view sym, ParserStateMachine * p_psm);
            /** operate state machine for incoming type description @p td **/
            static void on_parsed_typedescr(DExpectListTypeSsm & self, TypeDescr td, ParserStateMachine * p_psm);
            /** update state machine for type emitted by nested ssm **/
            static void on_parsed_type(DExpectListTypeSsm & self, obj<AType> type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal(DExpectListTypeSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm);
            /** operate state machine for formal emitted by nested ssm **/
            static void on_parsed_formal_with_token(DExpectListTypeSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm);
            /** consume formal arglist emitted by nested ssm **/
            static void on_parsed_formal_arglist(DExpectListTypeSsm & self, DArray * arglist, ParserStateMachine * p_psm);
            /** update state machine for nested parsed expression @p expr **/
            static void on_parsed_expression(DExpectListTypeSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm);
            /** update state machine @p p_psm for incoming parsed expression @p expr followed by token @p tk **/
            static void on_parsed_expression_with_token(DExpectListTypeSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm);
            /** update state machine for nested quoted literal @p lit **/
            static void on_quoted_literal(DExpectListTypeSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm);
            /** gc support: visit immediate gc-aware child pointers with @p gc.  Call gc.visit_child() for each **/
            static void visit_gco_children(DExpectListTypeSsm & self, VisitReason reason, obj<AGCObjectVisitor> gc);
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */