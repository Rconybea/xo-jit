/** @file DProgressSsm.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserStateMachine.hpp"
#include "syntaxstatetype.hpp"
//#include <xo/expression2/Expression.hpp>
#include <xo/facet/obj.hpp>

#ifdef NOT_YET
#include "exprstate.hpp"
#include "xo/reflect/TypeDescr.hpp"
#include <iostream>
//#include <cstdint>
#endif

namespace xo {
    namespace scm {
        /** represent an infix operator.
         *
         *  See @ref progress_xs::assemble_expr() for translation
         *  to Expression
         **/
        enum class optype {
            invalid = -1,

            /** op:= **/
            op_assign,

            /** op< **/
            op_less,
            /** op<= **/
            op_less_equal,
            /** op== **/
            op_equal,
            /** op!= **/
            op_not_equal,
            /** op> **/
            op_great,
            /** op>= **/
            op_great_equal,

            /** op+ **/
            op_add,
            /** op- **/
            op_subtract,

            /** op* **/
            op_multiply,
            /** op/ **/
            op_divide,

            n_optype
        };

        extern const char *
        optype_descr(optype x);

        /** report operator precedence.
         *  lowest operator precedence is 1
         **/
        extern int
        precedence(optype x);

        inline std::ostream &
        operator<< (std::ostream & os, optype x) {
            os << optype_descr(x);
            return os;
        }

        /** @class DProgressSsm
         *  @brief syntax state machine for parsing a schematica rhs-value-expression
         *
         *  Handles an expression that produces a value, for example appearing on the
         *  right-hand side of a definition.
         *
         *  Deals with:
         *  1. infix operators including operator precedence.
         *  2. generates argument-type-specific arithmetic expressions,
         *     for example using ``Apply::make_add2_f64()`` when adding floating-point numbers
         *
         *  One reason for this to exist is to simulate one-token lookahead.
         *  To look at but not consume a token T, can push a progress_xs instance P,
         *  then send T to P.
         **/
        class DProgressSsm {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DProgressSsm(obj<AExpression> lhs, optype op);

            static DProgressSsm * make(DArena & parser_mm,
                                       obj<AExpression> lhs,
                                       optype op);

            static void start(DArena & parser_mm,
                              obj<AExpression> valex,
                              ParserStateMachine * p_psm);

#ifdef NOT_YET
            static void start(rp<Expression> valex,
                              optype optype,
                              parserstatemachine * p_psm);
#endif

            syntaxstatetype ssm_type() const noexcept;

#ifdef NOT_YET
            bool admits_f64() const;

            void apply_type_error(const char * self_name,
                                  optype op,
                                  bp<Expression> expr1,
                                  bp<Expression> expr2,
                                  parserstatemachine * p_psm) const;
#endif

            std::string_view get_expect_str() const noexcept;

#ifdef NOT_YET
            void on_expr(bp<Expression> expr,
                                 parserstatemachine * p_psm) override;
            void on_expr_with_semicolon(bp<Expression> expr,
                                                parserstatemachine * p_psm) override;
#endif
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);
            void on_def_token(const Token & tk,
                              ParserStateMachine * p_psm);
            void on_if_token(const Token & tk,
                             ParserStateMachine * p_psm);
            void on_colon_token(const Token & tk,
                                ParserStateMachine * p_psm);
            void on_singleassign_token(const Token & tk,
                                       ParserStateMachine * p_psm);
            void on_f64_token(const Token & tk,
                              ParserStateMachine * p_psm);
            void on_semicolon_token(const Token & tk,
                                    ParserStateMachine * p_psm);
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);

            ///@{

            /** @defgroup scm-progressssm-printable-facet printable facet methods **/
            bool pretty(const ppindentinfo & ppii) const;

#ifdef NOT_YET
            void on_comma_token(const token_type & tk,
                                        parserstatemachine * p_psm) final override;
            void on_typedescr(TypeDescr td,
                                      parserstatemachine * p_psm) override;

            void on_semicolon_token(const token_type & tk,
                                            parserstatemachine * p_psm) override;
            void on_assign_token(const token_type & tk,
                                         parserstatemachine * p_psm) final override;
            void on_leftparen_token(const token_type & tk,
                                            parserstatemachine * p_psm) override;
            void on_rightparen_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;
            void on_rightbrace_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;
            void on_then_token(const token_type & tk,
                                       parserstatemachine * p_psm) override;
            void on_else_token(const token_type & tk,
                                       parserstatemachine * p_psm) override;

            /* entry point for an infix operator token */
            void on_operator_token(const token_type & tk,
                                           parserstatemachine * p_psm) final override;

            void on_bool_token(const token_type & tk,
                                       parserstatemachine * p_psm) override;

            void on_i64_token(const token_type & tk,
                                      parserstatemachine * p_psm) override;


            void print(std::ostream & os) const override;

        private:
            /** assemble expression representing
             *  value of
             *  @code
             *    f(lhs_, rhs_)
             *  @endcode
             *
             *  where f determined by @ref op_type_
             **/
            obj<AExpression> assemble_expr(ParserStateMachine * p_psm);
#endif

        private:
            /** populate an expression here, may be followed by an operator **/
            obj<AExpression> lhs_;

            /** infix operator,  if supplied **/
            optype op_type_ = optype::invalid;

            /** populate an expression here, following infix operator */
            obj<AExpression> rhs_;
        };
    } /*namespace scm*/

#ifndef ppdetail_atomic
    namespace print {
        PPDETAIL_ATOMIC(xo::scm::optype);
    }
#endif
} /*namespace xo*/


/* end DProgressSsm.hpp */
