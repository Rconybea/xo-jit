/* @file DExpectFormalArglistSsm.cpp
 *
 * @author Roland Conybeare, Jan 2026
 */

#include "DExpectFormalArglistSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArglistSsm.hpp"
#include "DExpectFormalArgSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArgSsm.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

#ifdef NOT_YET
#include "parserstatemachine.hpp"
#include "exprstatestack.hpp"
#include "expect_formal_xs.hpp"
#include "expect_symbol_xs.hpp"
#include "xo/expression/Variable.hpp"
#include "xo/indentlog/print/vector.hpp"
#endif

namespace xo {
    using xo::print::APrintable;
    using xo::print::ppstate;
    using xo::print::ppindentinfo;
    using xo::mm::AGCObject;
    using xo::mm::AAllocator;
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;

    namespace scm {
        const char *
        formalarglstatetype_descr(formalarglstatetype x) {
            switch (x) {
            case formalarglstatetype::invalid:
                return "invalid";
            case formalarglstatetype::argl_0:
                return "argl_0";
            case formalarglstatetype::argl_1a:
                return "argl_1a";
            case formalarglstatetype::argl_1b:
                return "argl_1b";
            case formalarglstatetype::n_formalarglstatetype:
                break;
            }

            return "?formalarglstatetype";
        }

        DExpectFormalArglistSsm::DExpectFormalArglistSsm(DArray * argl) : argl_{argl}
        {}

        DExpectFormalArglistSsm *
        DExpectFormalArglistSsm::_make(DArena & arena)
        {
            obj<AAllocator,DArena> mm(&arena);

            /* out-of-order so argl follows ssm in arena,
             * consistent with any subsequent arglist realloc.
             * Not a load-bearing choice however
             */

            void * mem = arena.alloc(typeseq::id<DExpectFormalArglistSsm>(),
                                     sizeof(DExpectFormalArglistSsm));


            /* allocate room for 8 arguments (during parsing)
             * will re-alloc to expand as needed
             */
            DArray * argl = DArray::empty(mm, 8);

            return new (mem) DExpectFormalArglistSsm(argl);
        }

        obj<ASyntaxStateMachine,DExpectFormalArglistSsm>
        DExpectFormalArglistSsm::make(DArena & arena)
        {
            obj<ASyntaxStateMachine,DExpectFormalArglistSsm> retval(_make(arena));

            return retval;
        }

        void
        DExpectFormalArglistSsm::start(ParserStateMachine * p_psm)
        {
            p_psm->push_ssm(DExpectFormalArglistSsm::make(p_psm->parser_alloc()));
        }

        syntaxstatetype
        DExpectFormalArglistSsm::ssm_type() const noexcept {
            return syntaxstatetype::expect_formal_arglist;
        }

        std::string_view
        DExpectFormalArglistSsm::get_expect_str() const {
            switch (fastate_) {
            case formalarglstatetype::invalid:
            case formalarglstatetype::n_formalarglstatetype:
                assert(false); // impossible
                break;
            case formalarglstatetype::argl_0:
                return "leftparen";
            case formalarglstatetype::argl_1a:
                return "formal-name";
            case formalarglstatetype::argl_1b:
                return "comma|rightparen";
            }

            return "?expect";
        }

        void
        DExpectFormalArglistSsm::on_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {
            case tokentype::tk_leftparen:
                this->on_leftparen_token(tk, p_psm);
                return;

                // all the not-yet-handled cases
            case tokentype::tk_lambda:
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
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            p_psm->illegal_input_on_token("DExpectFormalArglistSsm::on_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectFormalArglistSsm::on_parsed_symbol(std::string_view sym,
                                                  ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExpectFormalArglistSsm::on_parsed_symbol",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DExpectFormalArglistSsm::on_parsed_typedescr(TypeDescr td,
                                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DExpectFormalArglistSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DExpectFormalArglistSsm::on_parsed_expression(obj<AExpression> expr,
                                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DExpectFormalArglistSsm::on_parsed_expression",
                                               expr,
                                               this->get_expect_str());
        }

        void
        DExpectFormalArglistSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DExpectFormalArglistSsm::on_parsed_expression_with_semicolon",
                                             expr,
                                             this->get_expect_str());
        }

#ifdef NOT_YET
        expect_formal_arglist_xs::expect_formal_arglist_xs()
            : exprstate(exprstatetype::expect_formal_arglist),
              farglxs_type_{formalarglstatetype::argl_0}
        {}
#endif

        void
        DExpectFormalArglistSsm::on_leftparen_token(const Token & tk,
                                                    ParserStateMachine * p_psm)
        {
            if (fastate_ == formalarglstatetype::argl_0) {
                this->fastate_ = formalarglstatetype::argl_1a;

                DExpectFormalArgSsm::start(p_psm);
                return;
            }

            p_psm->illegal_input_on_token("DExpectFormalArglistSsm::on_token",
                                          tk,
                                          this->get_expect_str());
        }

#ifdef NOT_YET
        void
        expect_formal_arglist_xs::on_formal(const rp<Variable> & formal,
                                            parserstatemachine * p_psm)
        {
            if (farglxs_type_ == formalarglstatetype::argl_1a) {
                this->farglxs_type_ = formalarglstatetype::argl_1b;
                this->argl_.push_back(formal);
            } else {
                exprstate::on_formal(formal, p_psm);
            }
        }

        void
        expect_formal_arglist_xs::on_comma_token(const token_type & tk,
                                                 parserstatemachine * p_psm)
        {
            if (farglxs_type_ == formalarglstatetype::argl_1b) {
                this->farglxs_type_ = formalarglstatetype::argl_1a;
                expect_formal_xs::start(p_psm);
            } else {
                exprstate::on_comma_token(tk, p_psm);
            }
        }

        void
        expect_formal_arglist_xs::on_rightparen_token(const token_type & tk,
                                                      parserstatemachine * p_psm)
        {
            if (farglxs_type_ == formalarglstatetype::argl_1b) {
                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                p_psm->top_exprstate().on_formal_arglist(this->argl_, p_psm);
            } else {
                exprstate::on_rightparen_token(tk, p_psm);
            }
        }

        void
        expect_formal_arglist_xs::print(std::ostream & os) const {
            os << "<expect_formal_arglist_xs"
               << xtag("type", farglxs_type_);
            os << xtag("farglxs_type", farglxs_type_);
            os << xtag("argl", argl_);
            os << ">";
        }
#endif

        bool
        DExpectFormalArglistSsm::pretty(const ppindentinfo & ppii) const
        {
            ppstate * pps = ppii.pps();

            if (ppii.upto()) {
                if (!pps->print_upto("<DExpectFormalArglistSsm"))
                    return false;

                if (!pps->print_upto(xrefrtag("fastate", fastate_)))
                    return false;

                if (!pps->print_upto(xrefrtag("expect", this->get_expect_str())))
                    return false;

                if (!pps->print_upto(xrefrtag("n_args", n_args_)))
                    return false;

                for (size_type i_arg = 0; i_arg < n_args_; ++i_arg) {
                    char buf[80];
                    snprintf(buf, sizeof(buf), "arg[%ud]", i_arg);

                    auto arg_gco = argl_->at(i_arg);
                    obj<APrintable> arg_pr
                        = FacetRegistry::instance().try_variant<APrintable,AGCObject>(arg_gco);

                    if (!pps->print_upto(xrefrtag(buf, arg_pr)))
                        return false;
                }

                pps->write(">");

                return true;
            } else {
                pps->write("<DExpectFormalArglistSsm");

                pps->newline_indent(ppii.ci1());
                pps->pretty(refrtag("fastate", fastate_));

                pps->newline_indent(ppii.ci1());
                pps->pretty(refrtag("expect", this->get_expect_str()));

                pps->newline_indent(ppii.ci1());
                pps->pretty(refrtag("n_args", n_args_));

                for (size_type i_arg = 0; i_arg < n_args_; ++i_arg) {
                    char buf[80];
                    snprintf(buf, sizeof(buf), "arg[%ud]", i_arg);

                    auto arg_gco = argl_->at(i_arg);
                    obj<APrintable> arg_pr
                        = FacetRegistry::instance().try_variant<APrintable,AGCObject>(arg_gco);

                    pps->newline_indent(ppii.ci1());
                    pps->pretty(refrtag(buf, arg_pr));
                }

                pps->write(">");

                return false;
            }
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DExpectFormalArglistSsm.cpp */
