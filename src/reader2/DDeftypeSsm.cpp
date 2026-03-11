/** @file DDeftypeSsm.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "DeftypeSsm.hpp"
#include "syntaxstatetype.hpp"
#include "ExpectSymbolSsm.hpp"
#include "ExpectTypeSsm.hpp"
#include "Constant.hpp"
#include <xo/object2/Boolean.hpp>
#include <string_view>

namespace xo {
    namespace scm {

        extern const char *
        DeftypeXst::_descr(enum DeftypeXst::code x)
        {
            switch (x) {
            case code::invalid: return "invalid";
            case code::def_0: return "def_0";
            case code::def_1: return "def_1";
            case code::def_2: return "def_2";
            case code::def_3: return "def_3";
            case code::def_4: return "def_4";
            case code::N: break;
            }

            return "?DeftypeXst";
        }

        std::ostream &
        operator<<(std::ostream & os, DeftypeXst x)
        {
            os << DeftypeXst::_descr(x.code());
            return os;
        }

        DDeftypeSsm::DDeftypeSsm()
            : deftype_xst_{DeftypeXst::code::def_0}
        {}

        DDeftypeSsm *
        DDeftypeSsm::_make(DArena & parser_mm)
        {
            void * mem = parser_mm.alloc_for<DDeftypeSsm>();

            return new (mem) DDeftypeSsm();
        }

        obj<ASyntaxStateMachine,DDeftypeSsm>
        DDeftypeSsm::make(DArena & parser_mm)
        {
            return obj<ASyntaxStateMachine,DDeftypeSsm>(_make(parser_mm));
        }

        void
        DDeftypeSsm::start(DArena & mm,
                           ParserStateMachine * p_psm)
        {
            assert(p_psm->stack());

            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            auto deftype_ssm = DDeftypeSsm::make(mm);

            p_psm->push_ssm(ckp, deftype_ssm);
        }

        syntaxstatetype
        DDeftypeSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::deftypeexpr;
        }

        std::string_view
        DDeftypeSsm::get_expect_str() const noexcept
        {
            switch (this->deftype_xst_.code()) {
            case DeftypeXst::code::invalid:
                break;
            case DeftypeXst::code::def_0:
                return "deftype";
            case DeftypeXst::code::def_1:
                return "symbol";
            case DeftypeXst::code::def_2:
                return "doublecolon";
            case DeftypeXst::code::def_3:
                return "type";
            case DeftypeXst::code::def_4:
                return "semicolon";
            case DeftypeXst::code::N:
                break;

            }

            return "?expect";
        }

        void
        DDeftypeSsm::on_token(const Token & tk,
                              ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {

            case tokentype::tk_deftype:
                this->on_deftype_token(tk, p_psm);
                return;

            case tokentype::tk_doublecolon:
                this->on_doublecolon_token(tk, p_psm);
                return;

            case tokentype::tk_semicolon:
                this->on_semicolon_token(tk, p_psm);
                return;

            // all the not-yet handled cases
            case tokentype::tk_symbol:
            case tokentype::tk_def:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_invalid:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_if:
            case tokentype::tk_quote:
            case tokentype::tk_leftparen:
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
            case tokentype::tk_assign:
            case tokentype::tk_yields:
            case tokentype::tk_plus:
            case tokentype::tk_minus:
            case tokentype::tk_star:
            case tokentype::tk_slash:
            case tokentype::tk_cmpeq:
            case tokentype::tk_cmpne:
            case tokentype::tk_nil:
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
        DDeftypeSsm::on_deftype_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            if (deftype_xst_.code() == DeftypeXst::code::def_0) {
                this->deftype_xst_ = DeftypeXst(DeftypeXst::code::def_1);

                DExpectSymbolSsm::start(p_psm);

                /* continue in .on_parsed_symbol() */

                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DDeftypeSsm::on_parsed_symbol(std::string_view sym_name,
                                      ParserStateMachine * p_psm)
        {
            if (deftype_xst_.code() == DeftypeXst::code::def_1) {
                this->deftype_xst_ = DeftypeXst(DeftypeXst::code::def_2);
                this->lhs_symbol_ = p_psm->intern_string(sym_name);

                return;
            }

            Super::illegal_parsed_symbol(sym_name, p_psm);
        }

        void
        DDeftypeSsm::on_doublecolon_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            if (deftype_xst_.code() == DeftypeXst::code::def_2) {
                this->deftype_xst_ = DeftypeXst(DeftypeXst::code::def_3);

                DExpectTypeSsm::start(true /*corrected*/, p_psm);

                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DDeftypeSsm::on_parsed_type(obj<AType> type,
                                    ParserStateMachine * p_psm)
        {
            if (deftype_xst_.code() == DeftypeXst::code::def_3) {
                this->deftype_xst_ = DeftypeXst(DeftypeXst::code::def_4);

                DLocalSymtab * local = p_psm->local_symtab();

                if (local) {
                    local->append_type(p_psm->expr_alloc(),
                                       lhs_symbol_,
                                       type);
                } else {
                    DGlobalSymtab * global = p_psm->global_symtab();

                    DTypename * tname = DTypename::_make(p_psm->expr_alloc(),
                                                         lhs_symbol_,
                                                         type);

                    global->upsert_typename(p_psm->expr_alloc(), tname);
                }

                return;
            }

            Super::illegal_type(type, p_psm);
        }

        void
        DDeftypeSsm::on_semicolon_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            if (deftype_xst_.code() == DeftypeXst::code::def_4) {
                p_psm->pop_ssm();

                // Reporting a placeholder expression to preserve policy
                // that 'every toplevel statement produces an expression'
                //
                auto result_expr
                    = DConstant::make(p_psm->expr_alloc(),
                                      DBoolean::box<AGCObject>(p_psm->expr_alloc(),
                                                               true));
                p_psm->on_parsed_expression(result_expr);

                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        bool
        DDeftypeSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DDeftypeSsm",
                                             refrtag("deftypestate", deftype_xst_),
                                             refrtag("expect", this->get_expect_str()));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DDeftypeSsm.cpp */
