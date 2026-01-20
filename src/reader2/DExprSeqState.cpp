/** @file DExprSeqState.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DExprSeqState.hpp"
#include "DDefineSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExprSeqState.hpp"

namespace xo {
    using xo::mm::AAllocator;
    using xo::facet::with_facet;
    using xo::reflect::typeseq;

    namespace scm {
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
        DExprSeqState::on_parsed_symbol(std::string_view sym,
                                        ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExprSeqState::on_parsed_symbol",
                                           sym,
                                           this->get_expect_str());
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DExprSeqState.cpp */
