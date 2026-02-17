/* @file DExpectFormalArglistSsm.cpp
 *
 * @author Roland Conybeare, Jan 2026
 */

#include "DExpectFormalArglistSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArglistSsm.hpp"
#include "DExpectFormalArgSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArgSsm.hpp"
#include <xo/expression2/DVariable.hpp>
#include <xo/expression2/detail/IGCObject_DVariable.hpp>
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
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            p_psm->push_ssm(ckp, DExpectFormalArglistSsm::make(p_psm->parser_alloc()));
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

            case tokentype::tk_comma:
                this->on_comma_token(tk, p_psm);
                return;

            case tokentype::tk_rightparen:
                this->on_rightparen_token(tk, p_psm);
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
            case tokentype::tk_leftbracket:
            case tokentype::tk_rightbracket:
            case tokentype::tk_leftbrace:
            case tokentype::tk_rightbrace:
            case tokentype::tk_leftangle:
            case tokentype::tk_rightangle:
            case tokentype::tk_lessequal:
            case tokentype::tk_greatequal:
            case tokentype::tk_dot:
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

            Super::on_token(tk, p_psm);
        }


        void
        DExpectFormalArglistSsm::_accept_formal(obj<AAllocator> expr_alloc,
                                                DArena & parser_alloc,
                                                const DUniqueString * param_name,
                                                TypeDescr param_type)
        {
            /* note: param_type can be nullptr */
            TypeRef typeref
                = TypeRef::dwim(TypeRef::prefix_type::from_chars("formal"), param_type);

            DVariable * var = DVariable::make(expr_alloc,
                                              param_name,
                                              typeref);

            // need AGCObject facet to use DArray here.
            // May want to have gc feature that allows it to use
            // FacetRegistry on memory that stores obj<AExpression,..>
            //
            // In this case doesn't matter since DExpectFormalArglistSsm not actually collected!

            obj<AGCObject,DVariable> var_o(var);

            if (argl_->size() == argl_->capacity()) {
                // need to expand argl_ capacity.
                // If DArena were to allow it (i.e. offer a realloc() feature,
                // could do this in place since this SSM is at the top of the parser stack.

                obj<AAllocator,DArena> mm(&parser_alloc);
                DArray * argl_2x = DArray::empty(mm, 2 * argl_->capacity());

                for (DArray::size_type i = 0, n = argl_->size(); i < n; ++i) {
                    // TODO: prefer non-bounds-checked access here
                    argl_2x->push_back(argl_->at(i));
                }

                // update in place
                this->argl_ = argl_2x;
            }

            this->argl_->push_back(var_o);
        }

        void
        DExpectFormalArglistSsm::on_parsed_formal(const DUniqueString * param_name,
                                                  TypeDescr param_type,
                                                  ParserStateMachine * p_psm)
        {
            if (fastate_ == formalarglstatetype::argl_1a) {
                this->fastate_ = formalarglstatetype::argl_1b;

                this->_accept_formal(p_psm->expr_alloc(),
                                     p_psm->parser_alloc(),
                                     param_name,
                                     param_type);
                return;
            }

            Super::on_parsed_formal(param_name, param_type, p_psm);
        }

        void
        DExpectFormalArglistSsm::on_parsed_formal_with_token(const DUniqueString * param_name,
                                                             TypeDescr param_type,
                                                             const Token & tk,
                                                             ParserStateMachine * p_psm)
        {
            if (fastate_ == formalarglstatetype::argl_1a) {
                this->fastate_ = formalarglstatetype::argl_1b;

                this->_accept_formal(p_psm->expr_alloc(),
                                     p_psm->parser_alloc(),
                                     param_name,
                                     param_type);

                this->on_token(tk, p_psm);
                return;
            }

            Super::on_parsed_formal_with_token(param_name, param_type, tk, p_psm);
        }

        void
        DExpectFormalArglistSsm::on_leftparen_token(const Token & tk,
                                                    ParserStateMachine * p_psm)
        {
            if (fastate_ == formalarglstatetype::argl_0) {
                this->fastate_ = formalarglstatetype::argl_1a;

                DExpectFormalArgSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DExpectFormalArglistSsm::on_comma_token(const Token & tk,
                                                ParserStateMachine * p_psm)
        {
            if (fastate_ == formalarglstatetype::argl_1b) {
                this->fastate_ = formalarglstatetype::argl_1a;

                DExpectFormalArgSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DExpectFormalArglistSsm::on_rightparen_token(const Token & tk,
                                                     ParserStateMachine * p_psm)
        {
            if (fastate_ == formalarglstatetype::argl_1b) {
                DArray * args = argl_;

                p_psm->pop_ssm();
                p_psm->on_parsed_formal_arglist(args);
                return;
            }

            Super::on_token(tk, p_psm);
        }

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

                if (!pps->print_upto(xrefrtag("n_args", argl_->size())))
                    return false;

                for (size_type i_arg = 0; i_arg < argl_->size(); ++i_arg) {
                    char buf[80];
                    snprintf(buf, sizeof(buf), "arg[%u]", i_arg);

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
                pps->pretty(refrtag("n_args", argl_->size()));

                for (size_type i_arg = 0, n_arg = argl_->size(); i_arg < n_arg; ++i_arg) {
                    char buf[80];
                    snprintf(buf, sizeof(buf), "arg[%u]", i_arg);

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
