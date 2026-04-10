/** @file DExpectExprSsm.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/facet/obj.hpp>
#include <xo/indentlog/print/ppindentinfo.hpp>

namespace xo {
    namespace scm {

        class DExpectExprSsm : public DSyntaxStateMachine<DExpectExprSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectExprSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using VisitReason = xo::mm::VisitReason;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DExpectExprSsm(bool allow_defs,
                           bool cxl_on_rightbrace,
                           bool cxl_on_rightparen);

            static DExpectExprSsm * _make(DArena & parser_mm,
                                          bool allow_defs,
                                          bool cxl_on_rightbrace,
                                          bool cxl_on_rightparen);

            /** create fop referring to new DExpectExprSsm **/
            static obj<ASyntaxStateMachine,DExpectExprSsm> make(DArena & parser_mm,
                                                                bool allow_defs,
                                                                bool cxl_on_rightbrace,
                                                                bool cxl_on_rightparen);

            static void start(bool allow_defs,
                              bool cxl_on_rightbrace,
                              bool cxl_on_rightparen,
                              ParserStateMachine * p_psm);
            static void start(ParserStateMachine * p_psm);

            /** @defgroup scm-expectexpr-access-methods access methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectExprSsm"; }
            bool allow_defs() const noexcept { return allow_defs_; }
            bool cxl_on_rightbrace() const noexcept { return cxl_on_rightbrace_; }
            bool cxl_on_rightparen() const noexcept { return cxl_on_rightparen_; }

            ///@}
            /** @defgroup scm-expectexpr-methods general methods **/
            ///@{

            /** update state for this syntax on incoming leftparen token @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state for this syntax on incoming rightparen token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  Generally treats as illegal, but cancels gracefully when
             *  @ref cxl_on_rightparen_ set.
             **/
            void on_rightparen_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            /** update state for this syntax on incoming leftbrace token @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_leftbrace_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state for this syntax on quote token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  Starts nested {progress-ssm, quote-ssm} combination.
             **/
            void on_quote_token(const Token & tk,
                                ParserStateMachine * p_psm);

            /** step state machine for this syntax on incoming boolean literal token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_bool_token(const Token & tk,
                               ParserStateMachine * p_psm);

            /** step state machine for this syntax on incoming nil literal token @p tk
             *  with overall parser state in @p p_psm.
             **/
            void on_nil_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming f64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_f64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming i64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_i64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming string token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_string_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** update state for this syntax on incoming if-token @p tk,
             *  overall parser state in @p p_psm.
             *
             *  action: start nested if-else ssm
             **/
            void on_if_token(const Token & tk,
                             ParserStateMachine * p_psm);

            /** update state for this syntax on incoming lambda token @p tk,
             *  overall parser state in @p p_psm
             *
             *  action: start nested lambda ssm
             **/
            void on_lambda_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectexpr-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error mesages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming symbol-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_def_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax after parsing an expression @p expr,
             *  overall parser state in @p p_psm
             **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            /** update state for this syntax after parsing an expression @p expr
             *  followed by token @p tk
             *  overall parser state in @p p_psm
             **/
            void on_parsed_expression_with_token(obj<AExpression> expr,
                                                 const Token & tk,
                                                 ParserStateMachine * p_psm);

#ifdef NOT_YET
            /** update state for literal @p lit with overall state in @p p_psm.
             *  wraps literal as constant-expr + starts progress-ssm for possible
             *  operator expression.
             **/
            void on_quoted_literal(obj<AGCObject> lit,
                                   ParserStateMachine * p_psm);
#endif

            ///@}
            /** @defgroup scm-define-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
            /** @defgroup scm-expectexprssm-gc-support gc support methods **/
            ///@{

            /** gc support: visit gc-aware child pointers **/
            void visit_gco_children(VisitReason reason, obj<AGCObjectVisitor> gc) noexcept;

            ///@}

        private:
            /** if true: allow a define-expression here; otherwise reject **/
            bool allow_defs_ = false;
            /** if true: expecting either:
             *  1a. expression
             *  1b. right brace '}', in which case no expression
             *  if false: expecting only:
             *  2a. expression
             **/
            bool cxl_on_rightbrace_ = false;
            /** if true: expecting either:
             *  1a. expression
             *  1b. right paren ')', in which case no expression
             **/
            bool cxl_on_rightparen_ = false;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectExprSsm.hpp */
