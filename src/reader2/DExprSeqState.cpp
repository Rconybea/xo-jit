/** @file DExprSeqState.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DExprSeqState.hpp"
#include "DDefineSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExprSeqState.hpp"
#include <xo/reader2/DProgressSsm.hpp>
#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/object2/DFloat.hpp>
#include <xo/object2/number/IGCObject_DFloat.hpp>
#include <xo/gc/GCObject.hpp>

namespace xo {
    using xo::scm::DProgressSsm;
    using xo::scm::DConstant;
    using xo::scm::DFloat;
    using xo::mm::AGCObject;
    using xo::mm::AAllocator;
    using xo::facet::with_facet;
    using xo::reflect::typeseq;

    namespace scm {
        const char *
        exprseqtype_descr(exprseqtype x)
        {
            switch (x) {
            case exprseqtype::toplevel_interactive:
                return "toplevel-interactive";
            case exprseqtype::toplevel_batch:
                return "toplevel-batch";
            case exprseqtype::N:
                break;
            }

            return "exprseqtype?";
        }

        DExprSeqState::DExprSeqState(exprseqtype ty) : seqtype_{ty}
        {}

        namespace {
            obj<ASyntaxStateMachine>
            make_exprseq_ssm(obj<AAllocator> mm,
                             exprseqtype seqtype)
            {
                void * mem = mm.alloc(typeseq::id<DExprSeqState>(),
                                      sizeof(DExprSeqState));

                DExprSeqState * ssm = new (mem) DExprSeqState(seqtype);

                return with_facet<ASyntaxStateMachine>::mkobj(ssm);
            }
        }

        void
        DExprSeqState::establish_interactive(obj<AAllocator> mm,
                                             ParserStateMachine * p_psm)
        {
            p_psm->establish_toplevel_ssm(make_exprseq_ssm
                                          (mm,
                                           exprseqtype::toplevel_interactive));
        }

        void
        DExprSeqState::establish_batch(obj<AAllocator> mm,
                                       ParserStateMachine * p_psm)
        {
            p_psm->establish_toplevel_ssm(make_exprseq_ssm
                                          (mm,
                                           exprseqtype::toplevel_batch));
        }

        // SyntaxStateMachine facet methods

        syntaxstatetype
        DExprSeqState::ssm_type() const noexcept
        {
            return syntaxstatetype::expect_toplevel_expression_sequence;
        }

        std::string_view
        DExprSeqState::get_expect_str() const noexcept
        {
            // TODO: provisional.  Will expand as more syntax implemented

            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                return "def|expression|...";
            case exprseqtype::toplevel_batch:
                return "def|...";
            case exprseqtype::N:
                break;
            }

            assert(false);
            return "impossible-DExprSeqState::get_expr_str";
        }

        void
        DExprSeqState::on_symbol_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                {
#ifdef NOT_YET
                    obj<AExpression,DVariable> var = p_psm->lookup_var(tk.text());

                    if (var) {
                        DProgressSsm::start(var, p_psm);
                    } else {
                        p_psm->unknown_variable_error("DExprSeqState::on_symbol_token",
                                                      tk,
                                                      this->get_expect_str(),
                                                      p_psm);
                    }
#endif
                }
                break;
            case exprseqtype::toplevel_batch:
                break;
            case exprseqtype::N:
                assert(false); // unreachable
                break;
            }

            p_psm->illegal_input_on_token("DExprSeqState::on_symbol_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExprSeqState::on_def_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            (void)tk;

            DDefineSsm::start(p_psm->parser_alloc(),
                              p_psm->expr_alloc(),
                              p_psm);

            /* keyword 'def' introduces a definition:
             *   def pi : f64 = 3.14159265
             *   def sq(x : f64) -> f64 { (x * x) }
             */
        }

        void
        DExprSeqState::on_if_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                p_psm->illegal_input_on_token("DExprSeqState::on_if_token",
                                              tk,
                                              this->get_expect_str());
                //assert(false); // DfElseState::start(p_psm);
                break;
            case exprseqtype::toplevel_batch:
                p_psm->illegal_input_on_token("DExprSeqState::on_if_token",
                                              tk,
                                              this->get_expect_str());
                break;
            case exprseqtype::N:
                assert(false); // unreachable
                break;
            }
        }

        void
        DExprSeqState::on_colon_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExprSeqState::on_colon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExprSeqState::on_singleassign_token(const Token & tk,
                                             ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExprSeqState::on_singleassign_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExprSeqState::on_f64_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                {
                    auto f64o = DFloat::box<AGCObject>(p_psm->expr_alloc(),
                                                       tk.f64_value());
                    auto * dconst = DConstant::make(p_psm->expr_alloc(), f64o);
                    auto expr = with_facet<AExpression>::mkobj(dconst);

                    DProgressSsm::start(p_psm->parser_alloc(),
                                        expr,
                                        p_psm);
                    return;
                }
            case exprseqtype::toplevel_batch:
                break;
            case exprseqtype::N:
                assert(false); // unreachable
                break;
            }

            p_psm->illegal_input_on_token("DExprSeqState::on_f64_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExprSeqState::on_semicolon_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExprSeqState::on_semicolon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExprSeqState::on_parsed_symbol(std::string_view sym,
                                        ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExprSeqState::on_parsed_symbol",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DExprSeqState::on_parsed_typedescr(TypeDescr td,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DExprSeqState::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DExprSeqState::on_parsed_expression(obj<AExpression> expr,
                                            ParserStateMachine * p_psm)
        {
            // toplevel expr sequence accepts an arbitrary number of expressions.

            p_psm->capture_result("DExprSeqState::on_parsed_expression", expr);
        }

        void
        DExprSeqState::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                           ParserStateMachine * p_psm)
        {
            p_psm->capture_result("DExprSeqState::on_parsed_expression_with_semicolon", expr);
        }

        bool
        DExprSeqState::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DExprSeqState",
                 refrtag("seqtype", seqtype_));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DExprSeqState.cpp */
