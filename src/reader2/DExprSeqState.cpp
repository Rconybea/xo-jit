/** @file DExprSeqState.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DExprSeqState.hpp"
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
        DExprSeqState::start_interactive(obj<AAllocator> mm,
                                         ParserStateMachine * p_psm)
        {


            p_psm->push_ssm(make_exprseq_ssm(mm,
                                             exprseqtype::toplevel_interactive));
        }

        void
        DExprSeqState::start_batch(obj<AAllocator> mm,
                                   ParserStateMachine * p_psm)
        {
            (void)mm;
            (void)p_psm;
#ifdef NOT_YET
            p_psm->push_ssm(make_exprseq_ssm(mm,
                                             exprseqtype::toplevel_batch));
#endif
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
        DExprSeqState::on_if_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                assert(false); // DfElseState::start(p_psm);
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
    } /*namespace scm*/
} /*namespace xo*/

/* end DExprSeqState.cpp */
