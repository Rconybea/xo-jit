/* @file DExpectQLiteralSsm.cpp
 *
 * @author Roland Conybeare, Mar 2026
 */

#include "ExpectQLiteralSsm.hpp"
#include <xo/object2/Float.hpp>
//#include "DExpectFormalArgSsm.hpp"
//#include "ssm/ISyntaxStateMachine_DExpectFormalArgSsm.hpp"
//#include <xo/expression2/DVariable.hpp>
//#include <xo/expression2/detail/IGCObject_DVariable.hpp>
//#include <xo/printable2/Printable.hpp>
//#include <xo/alloc2/arena/IAllocator_DArena.hpp>
//#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
//    using xo::print::APrintable;
//    using xo::print::ppstate;
//    using xo::print::ppindentinfo;
    using xo::mm::AGCObject;
//    using xo::mm::AAllocator;
//    using xo::facet::FacetRegistry;
//    using xo::reflect::typeseq;

    namespace scm {
#ifdef NOT_USING
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
#endif

        DExpectQLiteralSsm::DExpectQLiteralSsm()
        {}

        DExpectQLiteralSsm *
        DExpectQLiteralSsm::_make(DArena & arena)
        {
            /* out-of-order so argl follows ssm in arena,
             * consistent with any subsequent arglist realloc.
             * Not a load-bearing choice however
             */

            void * mem = arena.alloc_for<DExpectQLiteralSsm>();

            return new (mem) DExpectQLiteralSsm();
        }

        obj<ASyntaxStateMachine,DExpectQLiteralSsm>
        DExpectQLiteralSsm::make(DArena & arena)
        {
            obj<ASyntaxStateMachine,DExpectQLiteralSsm> retval(_make(arena));

            return retval;
        }

        void
        DExpectQLiteralSsm::start(ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            p_psm->push_ssm(ckp, DExpectQLiteralSsm::make(p_psm->parser_alloc()));
        }

        syntaxstatetype
        DExpectQLiteralSsm::ssm_type() const noexcept {
            return syntaxstatetype::expect_qliteral;
        }

        std::string_view
        DExpectQLiteralSsm::get_expect_str() const
        {
            return "leftparen|leftbracket|leftbrace|string|f64|i64|bool";
        }

        void
        DExpectQLiteralSsm::on_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {
            case tokentype::tk_f64:
                this->on_f64_token(tk, p_psm);
                return;

            case tokentype::tk_leftparen:
            case tokentype::tk_comma:
            case tokentype::tk_rightparen:
            case tokentype::tk_lambda:
            case tokentype::tk_def:
            case tokentype::tk_if:
            case tokentype::tk_symbol:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_string:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_semicolon:
            case tokentype::tk_invalid:
            case tokentype::tk_quote:
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
        DExpectQLiteralSsm::on_f64_token(const Token & tk,
                                         ParserStateMachine * p_psm)
        {
            auto literal = DFloat::box<AGCObject>(p_psm->expr_alloc(),
                                                  tk.f64_value());

            p_psm->pop_ssm();
            p_psm->on_quoted_literal(literal);
        }

#ifdef NOT_YET
        void
        DExpectQLiteralSsm::_accept_formal(obj<AAllocator> expr_alloc,
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
            // In this case doesn't matter since DExpectQLiteralSsm not actually collected!

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
#endif

#ifdef NOT_YET
        void
        DExpectQLiteralSsm::on_leftparen_token(const Token & tk,
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
        DExpectQLiteralSsm::on_comma_token(const Token & tk,
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
        DExpectQLiteralSsm::on_rightparen_token(const Token & tk,
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
#endif

        bool
        DExpectQLiteralSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DExpectQLiteralSsm",
                                             refrtag("expect", this->get_expect_str()));
        }
    } /*namespace scm*/
} /*namespace xo*/


/* end DExpectQLiteralSsm.cpp */
