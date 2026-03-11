/** @file DExpectListTypeSsm.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "ExpectListTypeSsm.hpp"
#include "ExpectTypeSsm.hpp"
#include "syntaxstatetype.hpp"
#include <xo/type/ListType.hpp>
#include <string_view>

namespace xo {
    namespace scm {

        const char *
        ListTypeXst::_descr(enum code x)
        {
            switch (x) {
            case code::invalid: return "?invalid";
            case code::type_0: return "type_0";
            case code::type_1: return "type_1";
            case code::type_2: return "type_2";
            case code::type_3: return "type_3";
            case code::N: break;
            }

            return "?ListTypeXst";
        }

        DExpectListTypeSsm::DExpectListTypeSsm() = default;

        DExpectListTypeSsm *
        DExpectListTypeSsm::_make(DArena & parser_mm)
        {
            void * mem = parser_mm.alloc_for<DExpectListTypeSsm>();

            return new (mem) DExpectListTypeSsm();
        }

        obj<ASyntaxStateMachine, DExpectListTypeSsm>
        DExpectListTypeSsm::make(DArena & parser_mm)
        {
            return obj<ASyntaxStateMachine,DExpectListTypeSsm>(_make(parser_mm));
        }

        void
        DExpectListTypeSsm::start(ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            p_psm->push_ssm(ckp, DExpectListTypeSsm::make(p_psm->parser_alloc()));
        }

        syntaxstatetype
        DExpectListTypeSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::expect_listtype;
        }

        std::string_view
        DExpectListTypeSsm::get_expect_str() const noexcept
        {
            switch(state_.code()) {
            case ListTypeXst::code::invalid:
            case ListTypeXst::code::N:
                break;
            case ListTypeXst::code::type_0:
                return "list";
            case ListTypeXst::code::type_1:
                return "leftangle";
            case ListTypeXst::code::type_2:
                return "type";
            case ListTypeXst::code::type_3:
                return "rightangle";
            }

            return "?ExpectListTypeSsm";
        }

        void
        DExpectListTypeSsm::on_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {

            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                return;

            case tokentype::tk_leftangle:
                this->on_leftangle_token(tk, p_psm);
                return;

            case tokentype::tk_rightangle:
                this->on_rightangle_token(tk, p_psm);
                return;

            // all the {not-yet-handled, illegal} cases
            case tokentype::tk_leftbrace:
            case tokentype::tk_rightbrace:
            case tokentype::tk_quote:
            case tokentype::tk_def:
            case tokentype::tk_deftype:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_semicolon:
            case tokentype::tk_invalid:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_if:
            case tokentype::tk_leftparen:
            case tokentype::tk_rightparen:
            case tokentype::tk_leftbracket:
            case tokentype::tk_rightbracket:
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
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectListTypeSsm::on_symbol_token(const Token & tk,
                                            ParserStateMachine * p_psm)
        {
            if (state_.code() == ListTypeXst::code::type_0) {
                this->state_ = ListTypeXst(ListTypeXst::code::type_1);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectListTypeSsm::on_leftangle_token(const Token & tk,
                                               ParserStateMachine * p_psm)
        {
            if (state_.code() == ListTypeXst::code::type_1) {
                this->state_ = ListTypeXst(ListTypeXst::code::type_2);
                DExpectTypeSsm::start(true /*corrected*/, p_psm);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectListTypeSsm::on_parsed_type(obj<AType> type, ParserStateMachine * p_psm)
        {
            if (state_.code() == ListTypeXst::code::type_2) {
                this->state_ = ListTypeXst(ListTypeXst::code::type_3);
                this->elt_type_ = type;
                return;
            }

            Super::illegal_type(type, p_psm);
        }

        void
        DExpectListTypeSsm::on_rightangle_token(const Token & tk,
                                                ParserStateMachine * p_psm)
        {
            if (state_.code() == ListTypeXst::code::type_3) {
                obj<AType> result = DListType::make(p_psm->expr_alloc(),
                                                    elt_type_);
                p_psm->pop_ssm();
                p_psm->on_parsed_type(result);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        bool
        DExpectListTypeSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DExpectListTypeSsm");
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectListTypeSsm.cpp */
