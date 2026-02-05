/** @file DLambdaSsm.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DLambdaSsm.hpp"
#include "ssm/ISyntaxStateMachine_DLambdaSsm.hpp"
#include "DExpectFormalArglistSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArglistSsm.hpp"
#include "DExpectTypeSsm.hpp"
#include "DExpectExprSsm.hpp"
#include "ParserStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/expression2/detail/IExpression_DLambdaExpr.hpp>
#include <xo/expression2/DVariable.hpp>
#include <xo/expression2/detail/IExpression_DVariable.hpp>
//#include <xo/expression2/symtab/ISymbolTable_DLocalSymtab.hpp>
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
    using xo::mm::AGCObject;
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
             *   lambda (x : f64) -> f64 { ... } ;
             *  ^      ^         ^  ^   ^       ^
             *  |      |         |  |   |       lm_5
             *  |      |         |  |   lm_4:expect_expression
             *  |      |         |  lm_3
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
                 return "yields|lambda-body";
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

            case tokentype::tk_yields:
                this->on_yields_token(tk, p_psm);
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

            Super::on_token(tk, p_psm);
#ifdef OBSOLETE
            p_psm->illegal_input_on_token("DLambdaSsm::on_token",
                                          tk,
                                          this->get_expect_str());
#endif
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

            Super::on_token(tk, p_psm);
#ifdef OBSOLETE
            p_psm->illegal_input_on_token("DLambdaSsm::on_lambda_token",
                                          tk,
                                          this->get_expect_str());
#endif
        }

        void
        DLambdaSsm::on_yields_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            if (lmstate_ == lambdastatetype::lm_2) {
                this->lmstate_ = lambdastatetype::lm_3;

                DExpectTypeSsm::start(p_psm);

                /* control reenters via .on_parsed_typedescr() */
                return;
            }

            Super::on_token(tk, p_psm);
#ifdef OBSOLETE
            p_psm->illegal_input_on_token("DLambdaSsm::on_yields_token",
                                          tk,
                                          this->get_expect_str());
#endif
        }


        void
        DLambdaSsm::on_parsed_typedescr(TypeDescr td,
                                        ParserStateMachine * p_psm)
        {
            if (lmstate_ == lambdastatetype::lm_3) {
                this->lmstate_ = lambdastatetype::lm_4;
                this->explicit_return_td_ = td;
                this->lambda_td_ = DLambdaExpr::assemble_lambda_td(local_symtab_, td);

                DExpectExprSsm::start(p_psm);
                return;
            }

            Super::on_parsed_typedescr(td, p_psm);
        }

#ifdef NOT_YET
        void
        lambda_xs::on_expr_with_semicolon(bp<Expression> expr,
                                          parserstatemachine * p_psm)
        {
            this->on_expr(expr, p_psm);
            this->on_semicolon_token(token_type::semicolon(), p_psm);
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
        DLambdaSsm::on_parsed_formal_arglist(DArray * arglist,
                                             ParserStateMachine * p_psm)
        {
            if (lmstate_ == lambdastatetype::lm_1) {
                this->lmstate_ = lambdastatetype::lm_2;
                /// something with top env frame ?

                /// TODO: arena-friendly non-gc-aware vector;
                //        use instead of DArray for arglist.
                //        something like DTypedArray<T>

                /// create LocalSymtab from arglist

                DLocalSymtab * symtab
                    = DLocalSymtab::_make_empty(p_psm->expr_alloc(),
                                                p_psm->local_symtab(),
                                                arglist->size());
                assert(symtab);

                for (DArray::size_type i = 0, n = arglist->size(); i < n; ++i) {
                    obj<AGCObject> param = arglist->at(i);

                    // TODO:
                    // sad! runtime poly conversion from obj<AGCObject>
                    // We need this because of (suboptimally) using DArray to store arglist

                    obj<AExpression> param_expr
                        = FacetRegistry::instance().variant<AExpression,AGCObject>(param);
                    obj<AExpression,DVariable> param_var
                        = obj<AExpression,DVariable>::from(param_expr);

                    assert(param_expr.data());
                    assert(param_var.data());

                    Binding b = symtab->append_var(p_psm->expr_alloc(),
                                                   param_var.data()->name(),
                                                   param_var.data()->typeref());

                    assert(b.is_local());

                    this->local_symtab_ = symtab;
                }

                // stash env frame: records local variables while we handle lambda body

                p_psm->push_local_symtab(symtab);

                // control reenters via .on_colon_token() / .on_leftbrace_token()

                return;
            }

            Super::on_parsed_formal_arglist(arglist, p_psm);
#ifdef OBSOLETE
            p_psm->illegal_parsed_formal_arglist("DLambdaSsm::on_parsed_formal_arglist",
                                                 arglist,
                                                 this->get_expect_str());
#endif
        }

        void
        DLambdaSsm::on_parsed_expression(obj<AExpression> expr,
                                         ParserStateMachine * p_psm)
        {
            if (lmstate_ == lambdastatetype::lm_4) {
                this->lmstate_ = lambdastatetype::lm_5;
                this->body_ = expr;

                // assemble lambda

                auto prefix = TypeRef::prefix_type::from_chars("lm");
                TypeRef tref = TypeRef::dwim(prefix, nullptr);

                const DUniqueString * name = p_psm->gensym("lambda");

                auto lm_expr = obj<AExpression,DLambdaExpr>
                    (DLambdaExpr::make(p_psm->expr_alloc(),
                                       tref,
                                       name,
                                       local_symtab_,
                                       body_));

                p_psm->pop_ssm(); // this lambda
                p_psm->on_parsed_expression(lm_expr);
                return;
            }

            Super::on_parsed_expression(expr, p_psm);
        }

        void
        DLambdaSsm::on_parsed_expression_with_token(obj<AExpression> expr,
                                                    const Token & tk,
                                                    ParserStateMachine * p_psm)
        {
            Super::on_parsed_expression_with_token(expr, tk, p_psm);
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
