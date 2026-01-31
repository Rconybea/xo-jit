/* @file DSequenceSsm.cpp */

#include "DSequenceSsm.hpp"

#ifdef NOT_YET
#include "expect_expr_xs.hpp"
#include "let1_xs.hpp"
#include "xo/expression/DefineExpr.hpp"
#include "xo/expression/Sequence.hpp"
#include "xo/expression/pretty_expression.hpp"
#endif

namespace xo {
    using xo::scm::DefineExpr;

    namespace scm {
#ifdef NOT_YET
        std::unique_ptr<sequence_xs>
        sequence_xs::make() {
            return std::make_unique<sequence_xs>(sequence_xs());
        }
#endif

        void
        sequence_xs::start(parserstatemachine * p_psm) {
            p_psm->push_exprstate(sequence_xs::make());
            /* want to accept anything that starts an expression,
             * except that } ends it
             */
            expect_expr_xs::start(true /*allow_defs*/,
                                  true /*cxl_on_rightbrace*/,
                                  p_psm);
        }

        sequence_xs::sequence_xs()
            : exprstate(exprstatetype::sequenceexpr)
        {}

        void
        sequence_xs::on_expr(bp<Expression> expr,
                             parserstatemachine * p_psm)
        {
             scope log(XO_DEBUG(p_psm->debug_flag()));

             log && log(xtag("expr", expr.promote()));

            /* TODO: if expr is a DefineExpr,
             *       then need to rewrite...
             *
             *   ...prefix
             *   DefineExpr(lhs_name, rhs)
             *   rest...
             *
             * becomes:
             *
             *   /-- .outer_seq_expr_
             *   v
             *   Sequence(
             *     ...prefix,
             *
             *           /-- .inner_lm_expr_
             *           v
             *     Apply(Lambda(gen999,
             *                  [Variable(lhs_name, type(rhs))],
             *                             /-- .expr_v_
             *                             v
             *                  sequencify(rest...)),
             *           rhs))
             *
             * so amongst other things,
             * helpful to have nested seequence_xs that propagates '}'
             * instead of swallowing it.
             */
            bp<DefineExpr> def_expr = DefineExpr::from(expr);

            if (def_expr) {
                /** nested_start: control returns via
                 *   .on_expr(x)
                 *      with x something like:
                 *    Apply(Lambda(gensym(),
                 *                 [Variable(def_expr->lhs_name(),
                 *                           def_expr->valuetype())],
                 *                 body...))
                 *  followed immediately by
                 *   .on_rightbrace_token()
                 **/
                let1_xs::start(def_expr->lhs_name(),
                               def_expr->rhs(),
                               p_psm);
            } else {
                this->expr_v_.push_back(expr.promote());

                expect_expr_xs::start(true /*allow_defs*/,
                                      true /*cxl_on_rightbrace*/,
                                      p_psm);
            }
        }

        void
        sequence_xs::on_expr_with_semicolon(bp<Expression> expr,
                                            parserstatemachine * p_psm)
        {
            /* sequence continues until right brace */
            this->on_expr(expr, p_psm);
        }

        void
        sequence_xs::on_rightbrace_token(const token_type & /*tk*/,
                                         parserstatemachine * p_psm)
        {
            auto self = p_psm->pop_exprstate();

            /* make sequence from expressions seen at this level,
             * and report it to parent
             */
            auto expr = Sequence::make(this->expr_v_);

            p_psm->top_exprstate().on_expr(expr, p_psm);
        }

        void
        sequence_xs::print(std::ostream & os) const {
            os << "<sequence_xs" << xtag("expr_v.size", expr_v_.size()) << ">";
        }

        bool
        sequence_xs::pretty_print(const xo::print::ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii, "sequence_xs",
                                             xrefrtag("expr_v.size", expr_v_.size()));
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DSequenceSsm.cpp */
