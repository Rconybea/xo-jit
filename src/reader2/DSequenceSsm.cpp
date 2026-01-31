/* @file DSequenceSsm.cpp */

#include "DSequenceSsm.hpp"
#include "ssm/ISyntaxStateMachine_DSequenceSsm.hpp"
#include "DExpectExprSsm.hpp"

#ifdef NOT_YET
#include "expect_expr_xs.hpp"
#include "let1_xs.hpp"
#include "xo/expression/DefineExpr.hpp"
#include "xo/expression/Sequence.hpp"
#include "xo/expression/pretty_expression.hpp"
#endif

namespace xo {
#ifdef NOT_YET
    using xo::scm::DDefineExpr;
#endif
    using xo::facet::typeseq;

    namespace scm {
        void
        DSequenceSsm::start(ParserStateMachine * p_psm)
        {
            p_psm->push_ssm(DSequenceSsm::make(p_psm->parser_alloc(),
                                               p_psm->expr_alloc()));
            /* want to accept anything that starts an expression,
             * except that rightbrace '}' ends it
             */
            DExpectExprSsm::start(p_psm->parser_alloc(),
                                  true /*allow_defs*/,
                                  true /*cxl_on_rightbrace*/,
                                  p_psm);
        }

        obj<ASyntaxStateMachine,DSequenceSsm>
        DSequenceSsm::make(DArena & mm,
                           obj<AAllocator> expr_mm)
        {
            return obj<ASyntaxStateMachine,DSequenceSsm>(_make(mm, expr_mm));
        }

        DSequenceSsm *
        DSequenceSsm::_make(DArena & mm,
                            obj<AAllocator> expr_mm)
        {
            void * mem = mm.alloc(typeseq::id<DSequenceSsm>(),
                                  sizeof(DSequenceSsm));

            DSequenceExpr * seq_expr = DSequenceExpr::_make_empty(expr_mm);

            return new (mem) DSequenceSsm(seq_expr);
        }

        DSequenceSsm::DSequenceSsm(DSequenceExpr * seq_expr) : seq_expr_{seq_expr}
        {}

#ifdef NOT_YET
        sequence_xs::sequence_xs()
            : exprstate(exprstatetype::sequenceexpr)
        {}
#endif

        syntaxstatetype
        DSequenceSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::sequence;
        }

        std::string_view
        DSequenceSsm::get_expect_str() const noexcept
        {
            return "expr|semicolon|rightbrace";
        }

#ifdef NOT_YET
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
#endif

        bool
        DSequenceSsm::pretty(const xo::print::ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "SequenceSsm",
                        xrefrtag("seq_expr.size", seq_expr_->size()),
                        xrefrtag("expect", this->get_expect_str()));
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DSequenceSsm.cpp */
