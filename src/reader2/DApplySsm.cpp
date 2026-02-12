/** @file DApplySsm.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "ApplySsm.hpp"
#include "ExpectExprSsm.hpp"
#include <xo/facet/FacetRegistry.hpp>
#include <xo/alloc2/Arena.hpp>
#include <xo/reflectutil/typeseq.hpp>

//#include "parserstatemachine.hpp"
//#include "expect_expr_xs.hpp"

namespace xo {
    using xo::mm::AGCObject;
    using xo::print::APrintable;
    using xo::reflect::typeseq;

    namespace scm {

        // ----- applyexprstatetype -----

        const char *
        applyexprstatetype_descr(applyexprstatetype x) {
            switch (x) {
            case applyexprstatetype::invalid: return "invalid";
            case applyexprstatetype::apply_0: return "apply_0";
            case applyexprstatetype::apply_1: return "apply_1";
            case applyexprstatetype::apply_2: return "apply_2";
            case applyexprstatetype::apply_3: return "apply_3";
            case applyexprstatetype::N: break;
            }

            return "???applyexprstatetype";
        }

        std::ostream &
        operator<<(std::ostream & os, applyexprstatetype x) {
            os << applyexprstatetype_descr(x);
            return os;
        }

        // ----- DApplySsm -----

        DApplySsm::DApplySsm(applyexprstatetype applystate,
                             obj<AExpression> fn_expr,
                             DArray * args)
        : applystate_{applystate},
          fn_expr_{fn_expr},
          args_expr_v_{args}
        {
            if (fn_expr) {
                this->applystate_ = applyexprstatetype::apply_1;
            }

            assert(args->is_empty());
        }

        DApplySsm *
        DApplySsm::make(DArena & arena,
                        obj<AExpression> fn_expr)
        {
            obj<AAllocator,DArena> mm(&arena);

            void * mem = arena.alloc(typeseq::id<DApplySsm>(),
                                     sizeof(DApplySsm));

            /* allocate room for 8 arguments (during parsing)
             *  will reallocate to expand if needed.
             *
             *  See similar code in DExpectFormalArglistSsm::_make
             */
            DArray * args = DArray::empty(mm, 8);

            applyexprstatetype applystate
                = (fn_expr
                   ? applyexprstatetype::apply_1
                   : applyexprstatetype::apply_0);

            // TODO: revisit if we decide to use flexible array for arguments

            return new (mem) DApplySsm(applystate, fn_expr, args);
        }

        void
        DApplySsm::start(obj<AExpression> fn_expr,
                         ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            DApplySsm * apply_ssm
                = DApplySsm::make(p_psm->parser_alloc(), fn_expr);

            obj<ASyntaxStateMachine,DApplySsm> ssm(apply_ssm);

            p_psm->push_ssm(ssm);
            //OBSOLETE //p_psm->top_exprstate().on_expr(fn_expr.get(), p_psm);
            //OBSOLETE //p_psm->on_token(token_type::leftparen(), p_psm);
        }

        syntaxstatetype
        DApplySsm::ssm_type() const noexcept {
            return syntaxstatetype::apply;
        }


        std::string_view
        DApplySsm::get_expect_str() const noexcept
        {
            switch(applystate_) {
            case applyexprstatetype::invalid: return "invalid";
            case applyexprstatetype::apply_0: return "expr";
            case applyexprstatetype::apply_1: return "lparen";
            case applyexprstatetype::apply_2: return "expr";
            case applyexprstatetype::apply_3: return "comma|rparen";
            case applyexprstatetype::N: break;
            }

            return "?expect";
        }

        void
        DApplySsm::on_token(const Token & tk,
                            ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_leftparen:
                this->on_leftparen_token(tk, p_psm);
                return;
            case tokentype::tk_symbol:
            case tokentype::tk_def:
            case tokentype::tk_if:
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_semicolon:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_invalid:
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
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DApplySsm::on_leftparen_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            if (applystate_ == applyexprstatetype::apply_1) {
                this->applystate_ = applyexprstatetype::apply_2;

                DExpectExprSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DApplySsm::on_parsed_expression_with_token(obj<AExpression> expr,
                                                   const Token & tk,
                                                   ParserStateMachine * p_psm)
        {
            /* maybe we'll find control comes here also on function position?
             * hasn't come up when applyssm recognized via leftparen
             */

            if (applystate_ == applyexprstatetype::apply_2) {
                obj<AGCObject> expr_gco = expr.to_facet<AGCObject>();
                assert(expr_gco);

                obj<AAllocator,DArena> mm(&(p_psm->parser_alloc()));

                if (args_expr_v_->size() == args_expr_v_->capacity()) {
                    // need to expand .args_expr_v_ capacity.
                    // Could use DArena checkpoint to redo this in place,
                    // since argument array must be on the top of the stack.

                    DArray * argv_2x = DArray::empty(mm, 2 * args_expr_v_->capacity());

                    for (DArray::size_type i = 0, n = args_expr_v_->size(); i < n; ++i) {
                        argv_2x->push_back((*args_expr_v_)[i]);
                    }

                    this->args_expr_v_ = argv_2x;
                }

                if (args_expr_v_->size() < args_expr_v_->capacity())
                    args_expr_v_->push_back(expr_gco);

                if (tk.tk_type() == tokentype::tk_rightparen) {
                    // begin assemble_expr()..

                    std::uint32_t n_args = args_expr_v_->size();

                    DApplyExpr * apply
                        = (DApplyExpr::scaffold
                           (mm,
                            TypeRef::dwim(TypeRef::prefix_type::from_chars("apply"),
                                          nullptr),
                            fn_expr_,
                            n_args));

                    for (std::uint32_t i_arg = 0; i_arg < n_args; ++i_arg) {
                        auto arg_expr
                            = args_expr_v_->at(i_arg).to_facet<AExpression>();

                        apply->assign_arg(i_arg, arg_expr);
                    }

                    // ..end assemble_expr()

                    obj<AExpression,DApplyExpr> apply_ex(apply);

                    p_psm->pop_ssm();
                    p_psm->on_parsed_expression(apply_ex);

                    return;
                } else if (tk.tk_type() == tokentype::tk_comma) {
                    // 1. want to remain in state apply_2
                    // 2. expr from nested ssm already incorporated
                    //    into .args_expr_v_

                    DExpectExprSsm::start(p_psm);
                    return;
                }
            }

            Super::on_parsed_expression_with_token(expr, tk, p_psm);
        }

#ifdef NOT_YET
        void
        apply_xs::on_expr(bp<Expression> expr,
                          parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            switch (applyxs_type_) {
            case applyexprstatetype::invalid:
            case applyexprstatetype::n_applyexprstatetype:
                // unreachable
                break;
            case applyexprstatetype::apply_0:
                log && log("stash fn -> new state apply_1");
                this->fn_expr_ = expr.promote();
                this->applyxs_type_ = applyexprstatetype::apply_1;
                return;
            case applyexprstatetype::apply_1:
                log && log("error: was expecting lparen");
                // error, expecting lparen
                break;
            case applyexprstatetype::apply_2:
                log && log(xtag("expr", expr), xtag("do", "stash expr -> new state apply_3"));
                this->args_expr_v_.push_back(expr.promote());
                this->applyxs_type_ = applyexprstatetype::apply_3;
                return;
            case applyexprstatetype::apply_3:
                // error, expecting comma|rparen
                break;
            }

            /* control here --implies-> error state */

            constexpr const char * c_self_name = "apply_xs::on_expr";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_expr(c_self_name, expr, exp, p_psm);
        }

        void
        apply_xs::on_comma_token(const token_type & tk,
                                 parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            if (this->applyxs_type_ == applyexprstatetype::apply_3) {
                this->applyxs_type_ = applyexprstatetype::apply_2;
                expect_expr_xs::start(p_psm);
            } else {
                constexpr const char * c_self_name = "apply_xs::on_comma_token";
                const char * exp = this->get_expect_str();

                this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
            }
        }

        void
        apply_xs::on_rightparen_token(const token_type & tk,
                                      parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("applyxs_type", applyxs_type_);

            if (this->applyxs_type_ == applyexprstatetype::apply_3) {
                /* (done) state */
                log("apply complete -> pop + send expr");

                rp<Apply> apply_expr = Apply::make(this->fn_expr_, this->args_expr_v_);

                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                p_psm->top_exprstate().on_expr(apply_expr, p_psm);
                return;
            }

            constexpr const char * c_self_name = "apply_xs::on_rightparen_token";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }
#endif

        bool
        DApplySsm::pretty(const ppindentinfo & ppii) const
        {
            // TODO: const-correct version of obj<> template
            auto fn_expr = const_cast<DApplySsm*>(this)->fn_expr_.to_facet<APrintable>();
            bool fn_expr_present(fn_expr);

            return ppii.pps()->pretty_struct(ppii,
                                             "DApplySsm",
                                             refrtag("applystate", applystate_),
                                             refrtag("expect", this->get_expect_str()),
                                             refrtag("fn_expr", fn_expr, fn_expr_present));
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DApplySsm.cpp */
