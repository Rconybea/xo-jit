/** @file DApplySsm.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "ApplySsm.hpp"
#include <xo/reflectutil/typeseq.hpp>

//#include "parserstatemachine.hpp"
//#include "expect_expr_xs.hpp"

namespace xo {
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

        DApplySsm::DApplySsm(obj<AExpression> fn_expr)
            : applystate_{applyexprstatetype::apply_0},
              fn_expr_{fn_expr}
        {
            if (fn_expr) {
                this->applystate_ = applyexprstatetype::apply_1;
            }
        }

        DApplySsm *
        DApplySsm::make(DArena & mm,
                        obj<AExpression> fn_expr)
        {
            void * mem = mm.alloc(typeseq::id<DApplySsm>(),
                                  sizeof(DApplySsm));

            // TODO: revisit if we use flexible array for
            //   arguments

            return new (mem) DApplySsm(fn_expr);
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
        apply_xs::on_leftparen_token(const token_type & tk,
                                     parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("applyxs_type", applyxs_type_);

            if (this->applyxs_type_ == applyexprstatetype::apply_1) {
                this->applyxs_type_ = applyexprstatetype::apply_2;
                expect_expr_xs::start(p_psm);
            } else {
                constexpr const char * c_self_name = "apply_xs::on_leftparen_token";
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

#ifdef NOT_YET
        void
        apply_xs::print(std::ostream & os) const
        {
            os << "<apply_xs"
               << xtag("this", (void*)this)
               << xtag("applyxs_type", applyxs_type_);
            os << ">";
        }

        bool
        apply_xs::pretty_print(const xo::print::ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii, "apply_xs",
                                             refrtag("applyxs_type", applyxs_type_),
                                             refrtag("fn_expr", fn_expr_),
                                             refrtag("args_expr_v", args_expr_v_));
        }

#endif
    } /*namespace scm*/
} /*namespace xo*/


/* end DApplySsm.cpp */
