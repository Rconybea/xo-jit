/** @file DExpectExprSsm.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DExpectExprSsm.hpp"
#include "ParserStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
#include "ssm/ISyntaxStateMachine_DExpectExprSsm.hpp"
#include "ssm/ISyntaxStateMachine_DProgressSsm.hpp"
#include "syntaxstatetype.hpp"
#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/object2/DBoolean.hpp>
#include <xo/object2/boolean/IGCObject_DBoolean.hpp>
#include <xo/object2/DInteger.hpp>
#include <xo/object2/number/IGCObject_DInteger.hpp>
#include <xo/object2/DFloat.hpp>
#include <xo/object2/number/IGCObject_DFloat.hpp>
#include <xo/object2/DString.hpp>
#include <xo/object2/string/IGCObject_DString.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/facet/facet_implementation.hpp>

#ifdef NOT_YET
#include "exprstatestack.hpp"
#include "define_xs.hpp"
#include "lambda_xs.hpp"
#include "if_else_xs.hpp"
#include "paren_xs.hpp"
#include "sequence_xs.hpp"
#include "progress_xs.hpp"
#include "xo/expression/Lambda.hpp"
#include "xo/expression/Constant.hpp"
#include "xo/expression/pretty_expression.hpp"
#endif

namespace xo {
#ifdef NOT_YET
    using xo::scm::Constant;
#endif
    using xo::scm::DFloat;
    using xo::mm::AGCObject;

    using xo::reflect::typeseq;
    using xo::facet::with_facet;

    namespace scm {

        DExpectExprSsm::DExpectExprSsm(bool allow_defs,
                                       bool cxl_on_rightbrace)
            : allow_defs_{allow_defs},
              cxl_on_rightbrace_{cxl_on_rightbrace}
        {
        }

        DExpectExprSsm *
        DExpectExprSsm::make(DArena & mm,
                             bool allow_defs,
                             bool cxl_on_rightbrace)
        {
            void * mem = mm.alloc(typeseq::id<DExpectExprSsm>(),
                                  sizeof(DExpectExprSsm));

            return new (mem) DExpectExprSsm(allow_defs,
                                            cxl_on_rightbrace);
        }

        void
        DExpectExprSsm::start(DArena & mm,
                              bool allow_defs,
                              bool cxl_on_rightbrace,
                              ParserStateMachine * p_psm)
        {
            DExpectExprSsm * exp_expr
                = DExpectExprSsm::make(mm,
                                       allow_defs,
                                       cxl_on_rightbrace);
            obj<ASyntaxStateMachine> ssm
                = with_facet<ASyntaxStateMachine>::mkobj(exp_expr);

            p_psm->push_ssm(ssm);
        }

        void
        DExpectExprSsm::start(ParserStateMachine * p_psm)
        {
            start(p_psm->parser_alloc(),
                  false /*!allow_defs*/,
                  false /*!cxl_on_rightbrace*/,
                  p_psm);
        }

        syntaxstatetype
        DExpectExprSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::expect_rhs_expression;
        }

        std::string_view
        DExpectExprSsm::get_expect_str() const noexcept
        {
            if (allow_defs_) {
                return "def|lambda|lparen|lbrace|literal|var";
            } else {
                return "lambda|lparen|lbrace|literal|var";
            }
        }

        void
        DExpectExprSsm::on_token(const Token & tk,
                                 ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                break;

            case tokentype::tk_def:
                this->on_def_token(tk, p_psm);
                break;

            case tokentype::tk_if:
                this->on_if_token(tk, p_psm);
                break;

            case tokentype::tk_colon:
                this->on_colon_token(tk, p_psm);
                break;

            case tokentype::tk_singleassign:
                this->on_singleassign_token(tk, p_psm);
                break;

            case tokentype::tk_string:
                this->on_string_token(tk, p_psm);
                break;

            case tokentype::tk_f64:
                this->on_f64_token(tk, p_psm);
                break;

            case tokentype::tk_i64:
                this->on_i64_token(tk, p_psm);
                break;

            case tokentype::tk_bool:
                this->on_bool_token(tk, p_psm);
                break;

            case tokentype::tk_semicolon:
                this->on_semicolon_token(tk, p_psm);
                break;

            // all the not-yet handled cases
            case tokentype::tk_invalid:
            case tokentype::tk_leftparen:
            case tokentype::tk_rightparen:
            case tokentype::tk_leftbracket:
            case tokentype::tk_rightbracket:
            case tokentype::tk_leftbrace:
            case tokentype::tk_rightbrace:
            case tokentype::tk_leftangle:
            case tokentype::tk_rightangle:
            case tokentype::tk_lessequal:
            case tokentype::tk_greatequal:
            case tokentype::tk_dot:
            case tokentype::tk_comma:
            case tokentype::tk_doublecolon:
            case tokentype::tk_assign:
            case tokentype::tk_yields:
            case tokentype::tk_plus:
            case tokentype::tk_minus:
            case tokentype::tk_star:
            case tokentype::tk_slash:
            case tokentype::tk_cmpeq:
            case tokentype::tk_cmpne:
            case tokentype::tk_type:
            case tokentype::tk_lambda:
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                p_psm->illegal_input_on_token("DExpectExprSsm::on_token",
                                              tk,
                                              this->get_expect_str());
                break;
            }
        }

        void
        DExpectExprSsm::on_symbol_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectExprSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectExprSsm::on_def_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectExprSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectExprSsm::on_if_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectExprSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectExprSsm::on_colon_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectExprSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectExprSsm::on_singleassign_token(const Token & tk,
                                              ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectExprSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectExprSsm::on_bool_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            auto flag = DBoolean::box<AGCObject>(p_psm->expr_alloc(),
                                                 tk.bool_value());

            auto expr = DConstant::make(p_psm->expr_alloc(), flag);

            // DProgressSsm responsible for resolving cases like
            //  true;
            //  true && false;

            DProgressSsm::start(p_psm->parser_alloc(),
                                expr,
                                p_psm);
        }

        void
        DExpectExprSsm::on_f64_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            auto f64o = DFloat::box<AGCObject>(p_psm->expr_alloc(),
                                               tk.f64_value());

            auto expr = DConstant::make(p_psm->expr_alloc(), f64o);

            // DProgressSsm responsible for resolving cases like
            //   1.9,
            //   1.9;
            //   1.9 + 2;
            //   1.9 + 2 ..  // could be followed by infix
            //   1.9 + 2 * 3;
            //   1.9 + 2 * 3 ..  // could be followed by infix
            //   1.9 * (2 + 3)

            DProgressSsm::start(p_psm->parser_alloc(),
                                expr,
                                p_psm);
        }

        void
        DExpectExprSsm::on_i64_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            auto i64o = DInteger::box<AGCObject>(p_psm->expr_alloc(),
                                                 tk.i64_value());

            auto expr = DConstant::make(p_psm->expr_alloc(), i64o);

            // DProgressSsm responsible for resolving cases like
            //   1,
            //   1;
            //   1 + 2;
            //   1 + 2 ..  // could be followed by infix
            //   1 + 2 * 3;
            //   1 + 2 * 3 ..  // could be followed by infix
            //   1 * (2 + 3)

            DProgressSsm::start(p_psm->parser_alloc(),
                                expr,
                                p_psm);
        }

        void
        DExpectExprSsm::on_string_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            auto str = DString::from_str(p_psm->expr_alloc(),
                                         tk.text());
            auto str_o = obj<AGCObject,DString>(str);

            auto expr = DConstant::make(p_psm->expr_alloc(), str_o);

            /* e.g.
             *   def msg = "hello, world";
             *              \----tk----/
             *
             * DProgressSsm responsible for operators that apply to string
             *   "foo";
             *   "foo" <= "bar"
             *   "foo" + ", she said";
             */

            DProgressSsm::start(p_psm->parser_alloc(),
                                expr,
                                p_psm);
        }

        void
        DExpectExprSsm::on_semicolon_token(const Token & tk,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectExprSsm::on_semicolon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectExprSsm::on_parsed_symbol(std::string_view sym,
                                         ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExpectExprSsm::on_parsed_symbol",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DExpectExprSsm::on_parsed_typedescr(TypeDescr td,
                                            ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DExpectExprSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DExpectExprSsm::on_parsed_formal(const DUniqueString * param_name,
                                         TypeDescr param_type,
                                         ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_formal("DExpectExprSsm::on_parsed_formal",
                                         param_name,
                                         param_type,
                                         this->get_expect_str());
        }

        void
        DExpectExprSsm::on_parsed_formal_arglist(DArray * arglist,
                                                 ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_formal_arglist("DExpectExprSsm::on_parsed_formal_arglist",
                                                 arglist,
                                                 this->get_expect_str());
        }

        void
        DExpectExprSsm::on_parsed_expression(obj<AExpression> expr,
                                             ParserStateMachine * p_psm)
        {
            p_psm->pop_ssm();
            p_psm->on_parsed_expression(expr);
        }

        void
        DExpectExprSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                            ParserStateMachine * p_psm)
        {
            // expression (reported by nested ProgressSsm)
            // completes this DExpectExprSsm's assignment

            p_psm->pop_ssm();
            p_psm->on_parsed_expression_with_semicolon(expr);
        }

        bool
        DExpectExprSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DExpectExprSsm",
                 refrtag("allow_defs", allow_defs_),
                 refrtag("cxl_on_rightbrace", cxl_on_rightbrace_)
                    );
        }

#ifdef NOT_YET
        void
        expect_expr_xs::on_def_token(const token_type & tk,
                                     parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            if (allow_defs_) {
                define_xs::start(p_psm);
            } else {
                exprstate::on_def_token(tk, p_psm);
            }
        }

        void
        expect_expr_xs::on_lambda_token(const token_type & /*tk*/,
                                        parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            //constexpr const char * self_name = "exprstate::on_leftparen";

            lambda_xs::start(p_psm);
        }

        void
        expect_expr_xs::on_if_token(const token_type & /*tk*/,
                                    parserstatemachine * p_psm)
        {
            if_else_xs::start(p_psm);
        }

        void
        expect_expr_xs::on_leftparen_token(const token_type & /*tk*/,
                                           parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            //constexpr const char * self_name = "exprstate::on_leftparen";

            /* push lparen_0 to remember to look for subsequent rightparen. */
            paren_xs::start(p_psm);
        }

        void
        expect_expr_xs::on_leftbrace_token(const token_type & /*tk*/,
                                           parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            /* push lparen_0 to remember to look for subsequent rightparen. */
            sequence_xs::start(p_psm);
        }

        void
        expect_expr_xs::on_rightbrace_token(const token_type & tk,
                                            parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            if (cxl_on_rightbrace_) {
                auto self = p_psm->pop_exprstate();

                /* do not call .on_expr(), since '}' cancelled */

                p_psm->on_rightbrace_token(tk);
            } else {
                exprstate::on_rightbrace_token(tk, p_psm);
            }
        }

        void
        expect_expr_xs::on_symbol_token(const token_type & tk,
                                        parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("tk", tk));

            constexpr const char * c_self_name = "expect_expr_xs::on_symbol_token";

            /* various possibilities when looking for rhs expression:
             *
             *   x := y       // (1)
             *   x := f(a)    // (2)
             *   x := f(a,b)  // (3)
             *
             * need lookahead token following symbol to distinguish
             * between (1) (symbol completes rhs expression)
             * and {(2), (3)} (symbol is function call)
             */

            bp<Variable> var = p_psm->lookup_var(tk.text());

            if (!var) {
                this->unknown_variable_error(c_self_name, tk, p_psm);
                return;
            }

            /* e.g.
             *   def pi = 3.14159265;
             *   def mypi = pi;
             *              ^
             *   def pi2 = pi * 2;
             *             ^
             *   def y = foo(pi2);
             *           ^
             */
            progress_xs::start(var.promote(), p_psm);

 #ifdef NOT_YET
            p_stack->push_exprstate(exprstate(exprstatetype::expr_progress,
                                              Variable::make(name, type)));
#endif

#ifdef LATER
            p_psm->pop_exprstate();
            p_psm->top_exprstate().on_symbol(tk.text(),
                                               p_stack, p_emit_expr);
#endif
            return;
        }

        void
        expect_expr_xs::on_expr(bp<Expression> expr,
                                parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("exstype", this->exs_type_),
                       xtag("expr", expr.promote()));
            log && log("pop expect_expr_xs, forward to parent");

            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            p_psm->on_expr(expr);
        } /*on_expr*/

        void
        expect_expr_xs::on_expr_with_semicolon(bp<Expression> expr,
                                               parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("exstype", this->exs_type_),
                       xtag("expr", expr.promote()));
            log && log("pop expect_expr_xs, forward to parent");

            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            p_psm->on_expr_with_semicolon(expr);
        } /*on_expr_with_semicolon*/

        void
        expect_expr_xs::print(std::ostream & os) const {
            os << "<expect_expr_xs"
               << xtag("allow_defs", allow_defs_)
               << xtag("cxl_on_rightbrace", cxl_on_rightbrace_)
               << ">";
        }

        bool
        expect_expr_xs::pretty_print(const xo::print::ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii, "expect_expr_xs",
                                             refrtag("allow_defs", allow_defs_),
                                             refrtag("cxl_on_rightbrace", cxl_on_rightbrace_));
        }
#endif

    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectExprSsm.cpp */
