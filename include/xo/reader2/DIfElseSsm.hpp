/** @file DIfElseSsm.hpp
 *
 *  @author Roland Conybeare, Jul 2025
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/expression2/DIfElseExpr.hpp>
#include "syntaxstatetype.hpp"
#include <xo/expression2/detail/IExpression_DIfElseExpr.hpp>
#include <xo/expression2/DIfElseExpr.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /**
         *   if test-expr then then-expr else else-expr ;
         *  ^  ^         ^    ^         ^    ^         ^
         *  |  |         |    |         |    |         |
         *  |  if_1      if_2 if_3      if_4 if_5      if_6
         *  if_0
         *
         *  if_0 --on_if_token()--> if_1
         *  if_1 --on_expr()--> if_2
         *  if_2 --on_then_token()--> if_3
         *  if_3 --on_expr()--> if_4
         *  if_4 --on_else_token()--> if_5
         *       --on_semicolon_token()--> (done)
         *  if_5 --on_expr()-->if_6
         *  if_6 --on_semicolon_token()--> (done)
         **/
        enum class ifexprstatetype {
            invalid = -1,

            if_0,
            if_1,
            if_2,
            if_3,
            if_4,
            if_5,
            if_6,

            N,
        };

        extern const char * ifexprstatetype_descr(ifexprstatetype x);

        std::ostream &
        operator<<(std::ostream & os, ifexprstatetype x);

        /** @class DIfElseSsm
         *  @brief syntax state machine for parsing a conditional expression
         **/
        class DIfElseSsm : public DSyntaxStateMachine<DIfElseSsm> {
        public:
            using Super = DSyntaxStateMachine<DIfElseSsm>;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-ifelsessm-expression-ctors constructors **/
            ///@{
            explicit DIfElseSsm(DIfElseExpr * ifelse_expr);

            /** create instance using memory from @p parser_mm
             *  with initial scaffold @p ifelse_expr.
             **/
            static DIfElseSsm * _make(DArena & parser_mm,
                                      DIfElseExpr * ifelse_expr);

            /** create fop referring to new DIfElseSsm **/
            static obj<ASyntaxStateMachine,DIfElseSsm> make(DArena & parser_mm,
                                                            DIfElseExpr * ifelse_expr);

            /** start nested parser for an if-else expression
             *  on top of parser state machine @p p_psm.
             *  Use @p parser_mm to allocate syntax state machines
             *  (i.e. temporary memory needed only during parsing)
             *  Use @p expr_mm to allocate expressions.
             **/
            static void start(DArena & parser_mm,
                              obj<AAllocator> expr_mm,
                              ParserStateMachine * p_psm);

            static const char * ssm_classname() { return "DIfElseSsm"; }

            DSyntaxStateMachine<DIfElseSsm> * super() { return this; }

            ///@}
            /** @defgroup scm-ifelsessm-expression-methods general methods **/
            ///@{

            /** operate state machine on if-token input @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_if_token(const Token & tk,
                             ParserStateMachine * p_psm);

            /** operate state machine on then-token input @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_then_token(const Token & tk,
                               ParserStateMachine * p_psm);

            /** operate state machine on else-token input @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_else_token(const Token & tk,
                               ParserStateMachine * p_psm);

            /** victory: report completed @ref if_expr_ to parent ssm,
             *  after removing this ssm from parser stack
             **/
            void finish_and_continue(ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-ifelsessm-expression-facet expression facet methods **/
            ///@{

            /** identifies this state machine **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error messages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming semicolon token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_semicolon_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state for this syntax after parsing an expression @p expr,
             *  overall parser state in @p p_psm.
             **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            /** update state for this syntax after parsing an expression @p expr,
             *  followed by semicolon,
             *  with overall parser state in @p p_psm.
             **/
            void on_parsed_expression_with_token(obj<AExpression> expr,
                                                 const Token & tk,
                                                 ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-ifelsessm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

#ifdef NOT_YET
            virtual void on_rightbrace_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;
#endif

        private:
#ifdef NOT_YET
            static std::unique_ptr<if_else_xs> make();

            /** exit this exprstate,
             *  and deliver @ref if_expr_ to parent exprstate
             **/
            void finish_and_continue(parserstatemachine * p_psm);
#endif

        private:
            ifexprstatetype ifstate_ = ifexprstatetype::invalid;
            /** scaffold ifelse-expression here.
             *  This will eventually be the output of this ssm
             *
             *  TODO: can use DIfElseExpr* here.  See xo-object2//DList
             **/
            obj<AExpression,DIfElseExpr> if_expr_;

        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DIfElseSsm.hpp */
