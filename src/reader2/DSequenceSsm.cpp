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

        void
        DSequenceSsm::on_token(const Token & tk,
                               ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_rightbrace:
                this->on_rightbrace_token(tk, p_psm);
                return;
            case tokentype::tk_symbol:
            case tokentype::tk_def:
            case tokentype::tk_if:
            case tokentype::tk_then:
            case tokentype::tk_else:
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

            // default = illegal token error
            DSyntaxStateMachine<DSequenceSsm>::on_token(tk, p_psm);
        }

        void
        DSequenceSsm::on_rightbrace_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            (void)tk;

            /** rightbrace ends DSequenceSsm **/

            obj<AExpression,DSequenceExpr> expr(seq_expr_);

            p_psm->pop_ssm();

            /* make sequence from expressions seen at this level,
             * and report it to parent
             */
            p_psm->top_ssm().on_parsed_expression(expr, p_psm);
        }

        void
        DSequenceSsm::on_parsed_expression(obj<AExpression> expr,
                                           ParserStateMachine * p_psm)
        {
             scope log(XO_DEBUG(p_psm->debug_flag()));

             // TODO: stream inserter that sets up pretty-printing.
             //       Or integrate with indentlog.
             //       Maybe trouble is that indentlog doesn't #include Printable ?
             //
             log && log(xtag("expr", expr));

#ifdef NOT_YET
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
#endif

            this->seq_expr_->push_back(p_psm->expr_alloc(),
                                       expr);
        }

#ifdef NOT_YET
        void
        sequence_xs::on_expr_with_semicolon(bp<Expression> expr,
                                            parserstatemachine * p_psm)
        {
            /* sequence continues until right brace */
            this->on_expr(expr, p_psm);
        }
#endif

        bool
        DSequenceSsm::pretty(const xo::print::ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "DSequenceSsm",
                        refrtag("seq_expr.size", seq_expr_->size()),
                        refrtag("expect", this->get_expect_str()));
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DSequenceSsm.cpp */
