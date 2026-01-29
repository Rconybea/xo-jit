/** @file lambda_xs.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DLambdaSsm.hpp"
#include "ssm/ISyntaxStateMachine_DLambdaSsm.hpp"
#include "DExpectFormalArglistSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArglistSsm.hpp"
#include "ParserStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/arena/DArena.hpp>

#ifdef NOT_YET
#include "define_xs.hpp"
#include "parserstatemachine.hpp"
#include "exprstatestack.hpp"
#include "expect_formal_arglist_xs.hpp"
#include "expect_expr_xs.hpp"
#include "expect_type_xs.hpp"
#include "pretty_expression.hpp"
#include "pretty_variable.hpp"
#include "xo/expression/Lambda.hpp"
#endif

namespace xo {
    using xo::print::APrintable;
    using xo::mm::AAllocator;
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;

    namespace scm {
        const char *
        lambdastatetype_descr(lambdastatetype x) {
            switch(x) {
            case lambdastatetype::invalid: return "invalid";
            case lambdastatetype::lm_0: return "lm_0";
            case lambdastatetype::lm_1: return "lm_1";
            case lambdastatetype::lm_2: return "lm_2";
            case lambdastatetype::lm_3: return "lm_3";
            case lambdastatetype::lm_4: return "lm_4";
            case lambdastatetype::lm_5: return "lm_5";
            default: break;
            }

            return "???lambdastatetype";
        }

        // ----- lambda_xs - ----

        DLambdaSsm::DLambdaSsm()
        {}

        obj<ASyntaxStateMachine,DLambdaSsm>
        DLambdaSsm::make(DArena & parser_mm)
        {
            return obj<ASyntaxStateMachine,DLambdaSsm>(_make(parser_mm));
        }

        DLambdaSsm *
        DLambdaSsm::_make(DArena & parser_mm)
        {
            void * mem = parser_mm.alloc(typeseq::id<DLambdaSsm>(),
                                         sizeof(DLambdaSsm));

            return new (mem) DLambdaSsm();
        }

        void
        DLambdaSsm::start(ParserStateMachine * p_psm)
        {
            p_psm->push_ssm(DLambdaSsm::make(p_psm->parser_alloc()));
            p_psm->on_token(Token::lambda_token());
        }

        syntaxstatetype
        DLambdaSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::lambdaexpr;
        }

        std::string_view
        DLambdaSsm::get_expect_str() const noexcept
        {
            /*
             *   lambda (x : f64) : f64 { ... } ;
             *  ^      ^         ^ ^   ^       ^
             *  |      |         | |   |       lm_5
             *  |      |         | |   lm_4:expect_expression
             *  |      |         | lm_3
             *  |      |         lm_2
             *  |      lm_1:
             *  expect_expression
             */
             switch (this->lmstate_) {
             case lambdastatetype::invalid:
             case lambdastatetype::n_lambdastatetype:
                 assert(false); // impossible
                 break;
             case lambdastatetype::lm_0:
                 return "lambda";
             case lambdastatetype::lm_1:
                 return "lambda-params";
             case lambdastatetype::lm_2:
                 return "colon|lambda-body";
             case lambdastatetype::lm_3:
                 return "type";
             case lambdastatetype::lm_4:
                 return "lambda-body";
             case lambdastatetype::lm_5:
                 return "semicolon";
             }

             return "?expect";
        }

        void
        DLambdaSsm::on_token(const Token & tk,
                             ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {
            case tokentype::tk_lambda:
                this->on_lambda_token(tk, p_psm);
                return;

                // all the not-yet-handled cases
            case tokentype::tk_def:
            case tokentype::tk_if:
            case tokentype::tk_symbol:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_semicolon:
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
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            p_psm->illegal_input_on_token("DLambdaSsm::on_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DLambdaSsm::on_lambda_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            if (lmstate_ == lambdastatetype::lm_0) {
                this->lmstate_ = lambdastatetype::lm_1;

                DExpectFormalArglistSsm::start(p_psm);

                return;
            }

            p_psm->illegal_input_on_token("DLambdaSsm::on_lambda_token",
                                          tk,
                                          this->get_expect_str());
        }


#ifdef NOT_YET
        void
        lambda_xs::on_lambda_token(const token_type & tk,
                                   parserstatemachine * p_psm)
        {
            if (lmxs_type_ == lambdastatetype::lm_0) {
                this->lmxs_type_ = lambdastatetype::lm_1;
                expect_formal_arglist_xs::start(p_psm);
            } else {
                exprstate::on_lambda_token(tk, p_psm);
            }
        }

        void
        lambda_xs::on_formal_arglist(const std::vector<rp<Variable>> & argl,
                                     parserstatemachine * p_psm)
        {
            if (lmxs_type_ == lambdastatetype::lm_1) {
                this->lmxs_type_ = lambdastatetype::lm_2;
                this->parent_env_ = p_psm->top_envframe().promote();
                this->local_env_ = LocalSymtab::make(argl, parent_env_);

                p_psm->push_envframe(local_env_);

                //expect_expr_xs::start(p_psm);
            } else {
                exprstate::on_formal_arglist(argl, p_psm);
            }
        }

        void
        lambda_xs::on_expr_with_semicolon(bp<Expression> expr,
                                          parserstatemachine * p_psm)
        {
            this->on_expr(expr, p_psm);
            this->on_semicolon_token(token_type::semicolon(), p_psm);
        }

        void
        lambda_xs::on_colon_token(const token_type & tk,
                                  parserstatemachine * p_psm)
        {
            constexpr const char * c_self_name = "lambda_xs::on_colon_token";

            if (lmxs_type_ == lambdastatetype::lm_2) {
                this->lmxs_type_ = lambdastatetype::lm_3;
                expect_type_xs::start(p_psm);
                /* control reenters via .on_typedescr() */
            } else {
                this->illegal_input_on_token(c_self_name, tk, this->get_expect_str(), p_psm);
            }
        }

        void
        lambda_xs::on_leftbrace_token(const token_type & tk,
                                      parserstatemachine * p_psm)
        {
            constexpr const char * c_self_name = "lambda_xs::on_leftbrace_token";

            if (lmxs_type_ == lambdastatetype::lm_2)
                this->lmxs_type_ = lambdastatetype::lm_4;

            if (lmxs_type_ == lambdastatetype::lm_4) {
                expect_expr_xs::start(p_psm);
                /* want { to start expr sequence, that finishes on matching } */
                p_psm->on_leftbrace_token(token_type::leftbrace());
            } else {
                this->illegal_input_on_token(c_self_name, tk, this->get_expect_str(), p_psm);
            }
        }
#endif

        void
        DLambdaSsm::on_parsed_typedescr(TypeDescr td,
                                        ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DLambdaSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

#ifdef NOT_YET
        void
        lambda_xs::on_typedescr(TypeDescr td,
                                parserstatemachine * p_psm)
        {
            constexpr const char * c_self_name = "lambda_xs::on_typedescr";
            scope log(XO_DEBUG(p_psm->debug_flag()));

            assert(td);

            if (lmxs_type_ == lambdastatetype::lm_3) {
                this->lmxs_type_ = lambdastatetype::lm_4;
                this->explicit_return_td_ = td;

                this->lambda_td_ = Lambda::assemble_lambda_td(local_env_->argv(),
                                                              explicit_return_td_);

                /* 1. at this point we know function signature (@ref lambda_td_)
                 * 2. if this lambda appears on the rhs of a define,
                 *    propagate function signature to the define.
                 * 3. this makes recursive function definitions like this work
                 *    without relying on type inference:
                 *       def fact = lambda (n : i64) : i64 {
                 *         if (n == 0) then
                 *           1
                 *         else
                 *           n * fact(n - 1)
                 *       }
                 * 4. while parsing the body of the lambda, we want environment
                 *    to already associate the lambda's signature with variable 'fact',
                 *    so that when parser encounters 'fact(n - 1)' the expression has
                 *    known valuetype.
                 */

                if ((p_psm->exprstate_stack_size() >= 3)
                    && (p_psm->lookup_exprstate(1).exs_type() == exprstatetype::expect_rhs_expression)
                    && (p_psm->lookup_exprstate(2).exs_type() == exprstatetype::defexpr)
                    && (p_psm->env_stack_size() >= 2)
                    )
                {
                    const define_xs * def_xs = dynamic_cast<const define_xs*>(&(p_psm->lookup_exprstate(2)));

                    assert(def_xs);

                    bp<Variable> def_var = def_xs->lhs_variable();

                    if (def_var->valuetype() == nullptr) {
                        log && log("assign discovered lambda type T to enclosing define",
                                   xtag("lhs", def_var.get()),
                                   xtag("T", print::unq(this->lambda_td_->canonical_name())));

                        def_var->assign_valuetype(lambda_td_);
                    } else {
                        /* don't need to unify here.  if def already hasa a type,
                         * that's because it was explicitly specified.
                         * will discover any conflict after reporting parsed lambda
                         * to define_xs
                         */
                    }
                }

                expect_expr_xs::start(p_psm);
                /* control reenters via .on_expr() or .on_expr_with_semicolon() */
            } else {
                this->illegal_input_on_type(c_self_name, td, this->get_expect_str(), p_psm);
            }
        }
#endif

        void
        DLambdaSsm::on_parsed_symbol(std::string_view sym,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DLambdaSsm::on_parsed_sybol",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DLambdaSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                        ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression
                ("DLambdaSsm::on_parsed_expression_with_semicolon",
                 expr,
                 this->get_expect_str());
        }

        void
        DLambdaSsm::on_parsed_expression(obj<AExpression> expr,
                                         ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DLambdaSsm::on_parsed_expression",
                                             expr,
                                             this->get_expect_str());
        }

#ifdef NOT_YET
        void
        lambda_xs::on_expr(bp<Expression> expr,
                           parserstatemachine * p_psm)
        {
            constexpr const char * c_self_name = "lambda_xs::on_expr";

            if (lmxs_type_ == lambdastatetype::lm_4) {
                this->lmxs_type_ = lambdastatetype::lm_5;
                this->body_ = expr.promote();
            } else {
                this->illegal_input_on_expr(c_self_name, expr, this->get_expect_str(), p_psm);
            }
        }

        void
        lambda_xs::on_semicolon_token(const token_type & tk,
                                      parserstatemachine * p_psm)
        {
            if (lmxs_type_ == lambdastatetype::lm_5) {
                /* done! */

                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                std::string name = Variable::gensym("lambda");

                /* top env frame recorded arguments to this lambda */
                p_psm->pop_envframe();

                rp<Lambda> lm;

                /* TODO: unify explicit_return_td_ with body_ */

                if (lambda_td_) {
                    lm = Lambda::make(name, lambda_td_, local_env_, body_);
                } else {
                    lm = Lambda::make_from_env(name, local_env_,
                                               explicit_return_td_, body_);
                }

                p_psm->top_exprstate().on_expr(lm, p_psm);
                p_psm->top_exprstate().on_semicolon_token(tk, p_psm);

                return;
            }

            exprstate::on_semicolon_token(tk, p_psm);
        }

        void
        lambda_xs::on_f64_token(const token_type & tk,
                                parserstatemachine * p_psm)
        {
            constexpr const char * c_self_name = "lambda_xs::on_f64_token";

            /* f64 literal can begin lambda body, otherwise illegal.
             * for example:
             *   def foo = lambda (x: bool) 3.14;
             */
            if (lmxs_type_ == lambdastatetype::lm_2) {
                /* omitting return type.
                 * omitting left brace.
                 */
                this->lmxs_type_ = lambdastatetype::lm_4;

                expect_expr_xs::start(p_psm);
                p_psm->on_f64_token(tk);
            } else {
                this->illegal_input_on_token(c_self_name, tk, this->get_expect_str(), p_psm);
            }
        }

        // TODO: on_i64_token, on_bool token

        void
        lambda_xs::print(std::ostream & os) const {
            os << "<lambda_xs"
               << xtag("lmxs_type", lmxs_type_)
               << ">";
        }
#endif

        bool
        DLambdaSsm::pretty(const ppindentinfo & ppii) const
        {
            obj<APrintable> body
                = FacetRegistry::instance().try_variant<APrintable,
                                                        AExpression>(body_);

            if (body) {
                return ppii.pps()->pretty_struct
                    (ppii,
                     "DLambdaSsm",
                     refrtag("lmstate", lmstate_),
                     refrtag("expect", this->get_expect_str()),
                     refrtag("body", body));
            } else {
                return ppii.pps()->pretty_struct
                    (ppii,
                     "DLambdaSsm",
                     refrtag("lmstate", lmstate_),
                     refrtag("expect", this->get_expect_str()));
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DLambdaSsm.cpp */
