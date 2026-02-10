/** @file DExprSeqState.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DExprSeqState.hpp"
#include "ssm/ISyntaxStateMachine_DExprSeqState.hpp"
#include "DDefineSsm.hpp"
#include "DLambdaSsm.hpp"
#include "DProgressSsm.hpp"
#include "DIfElseSsm.hpp"
#include "ParenSsm.hpp"
#include "ExpectExprSsm.hpp"

#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>

#include <xo/object2/DString.hpp>
#include <xo/object2/string/IGCObject_DString.hpp>

#include <xo/object2/DFloat.hpp>
#include <xo/object2/number/IGCObject_DFloat.hpp>

#include <xo/object2/DInteger.hpp>
#include <xo/object2/number/IGCObject_DInteger.hpp>

#include <xo/object2/DBoolean.hpp>
#include <xo/object2/boolean/IGCObject_DBoolean.hpp>

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
        DExprSeqState::on_token(const Token & tk,
                                ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                return;

            case tokentype::tk_def:
                this->on_def_token(tk, p_psm);
                return;

            case tokentype::tk_lambda:
                this->on_lambda_token(tk, p_psm);
                return;

            case tokentype::tk_if:
                this->on_if_token(tk, p_psm);
                return;

            case tokentype::tk_string:
                this->on_string_token(tk, p_psm);
                return;

            case tokentype::tk_f64:
                this->on_f64_token(tk, p_psm);
                return;

            case tokentype::tk_i64:
                this->on_i64_token(tk, p_psm);
                return;

            case tokentype::tk_bool:
                this->on_bool_token(tk, p_psm);
                return;

            case tokentype::tk_leftparen:
                this->on_leftparen_token(tk, p_psm);
                return;

            // all the not-yet handled cases
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
            case tokentype::tk_colon:
                break;
            case tokentype::tk_semicolon:
                assert(false);
                break;
            case tokentype::tk_doublecolon:
            case tokentype::tk_singleassign:
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

            p_psm->illegal_input_on_token("DExprSeqState::on_token",
                                          tk,
                                          this->get_expect_str());
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

            Super::on_token(tk, p_psm);
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
        DExprSeqState::on_lambda_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            (void)tk;

            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                DLambdaSsm::start(p_psm);
                return;
            case exprseqtype::toplevel_batch:
                /* lambda not allowed at top-level in batch mode */
                break;
            case exprseqtype::N:
                assert(false); // unreachable
                break;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DExprSeqState::on_if_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                DIfElseSsm::start(p_psm->parser_alloc(),
                                  p_psm->expr_alloc(),
                                  p_psm);
                return;
            case exprseqtype::toplevel_batch:
                break;
            case exprseqtype::N:
                assert(false); // unreachable
                break;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DExprSeqState::on_string_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                {
                    DString * dstr = DString::from_cstr(p_psm->expr_alloc(),
                                                        tk.text().c_str());
                    obj<AGCObject,DString> str(dstr);
                    obj<AExpression,DConstant> expr = DConstant::make(p_psm->expr_alloc(), str);

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

            Super::on_token(tk, p_psm);
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
                    auto expr = DConstant::make(p_psm->expr_alloc(), f64o);

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

            Super::on_token(tk, p_psm);
        }

        void
        DExprSeqState::on_i64_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                {
                    auto i64o = DInteger::box<AGCObject>(p_psm->expr_alloc(),
                                                         tk.i64_value());
                    auto expr = DConstant::make(p_psm->expr_alloc(), i64o);

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

            Super::on_token(tk, p_psm);
        }

        void
        DExprSeqState::on_bool_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive:
                {
                    auto dvalue = DBoolean::box<AGCObject>(p_psm->expr_alloc(),
                                                           tk.bool_value());
                    auto expr = DConstant::make(p_psm->expr_alloc(), dvalue);

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

            Super::on_token(tk, p_psm);
        }

        void
        DExprSeqState::on_leftparen_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            switch (seqtype_) {
            case exprseqtype::toplevel_interactive: {
                DParenSsm::start(p_psm);
                p_psm->on_token(Token::leftparen_token());

                return;
            }
            case exprseqtype::toplevel_batch:
                break;
            case exprseqtype::N:
                assert(false); // unreachable
                break;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DExprSeqState::on_parsed_expression(obj<AExpression> expr,
                                            ParserStateMachine * p_psm)
        {
            // toplevel expr sequence accepts an arbitrary number of expressions.

            p_psm->capture_result("DExprSeqState::on_parsed_expression", expr);
        }

        void
        DExprSeqState::on_parsed_expression_with_token(obj<AExpression> expr,
                                                       const Token & tk,
                                                       ParserStateMachine * p_psm)
        {
            if (tk.tk_type() == tokentype::tk_semicolon) {
                p_psm->capture_result("DExprSeqState::on_parsed_expression_with_token", expr);
                return;
            }

            Super::on_parsed_expression_with_token(expr, tk, p_psm);
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
