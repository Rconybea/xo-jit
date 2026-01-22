/** @file ParserStateMachine.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserStateMachine.hpp"
#include "ParserStack.hpp"
#include "SyntaxStateMachine.hpp"
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <xo/indentlog/scope.hpp>
#include <xo/indentlog/print/tostr.hpp>
#include <xo/indentlog/print/tag.hpp>
#include <stdexcept>

namespace xo {
    using xo::facet::with_facet;

    namespace scm {
        ParserStateMachine::ParserStateMachine(const ArenaConfig & config,
                                               size_type max_stringtable_capacity,
                                               obj<AAllocator> expr_alloc)
            : stringtable_{max_stringtable_capacity},
              parser_alloc_{DArena::map(config)},
              expr_alloc_{expr_alloc},
              debug_flag_{config.debug_flag_}
        {

        }

        bool
        ParserStateMachine::is_at_toplevel() const noexcept
        {
            return ((stack_ == nullptr)
                    || (stack_->parent() == nullptr));
        }

        bool
        ParserStateMachine::has_incomplete_expr() const noexcept
        {
            // don't count toplevel expression

            return !(this->is_at_toplevel());
        }

        obj<ASyntaxStateMachine>
        ParserStateMachine::top_ssm() const
        {
            return this->stack_->top();
        }

        void
        ParserStateMachine::establish_toplevel_ssm(obj<ASyntaxStateMachine> ssm)
        {
            scope log(XO_DEBUG(debug_flag_));

            assert(stack_ == nullptr);

            this->stack_ = ParserStack::push(nullptr /*stack*/, parser_alloc_, ssm);
            this->parser_alloc_ckp_ = parser_alloc_.checkpoint();
        }

        void
        ParserStateMachine::push_ssm(obj<ASyntaxStateMachine> ssm)
        {
            scope log(XO_DEBUG(debug_flag_));

            // note: using parser_alloc_ for parser stack, since stacklike behavior

            this->stack_ = ParserStack::push(stack_, parser_alloc_, ssm);
        }

        void
        ParserStateMachine::pop_ssm()
        {
            scope log(XO_DEBUG(debug_flag_));

            assert(this->stack_);

            this->stack_ = ParserStack::pop(stack_, parser_alloc_);
        }

        const DUniqueString *
        ParserStateMachine::intern_string(std::string_view str)
        {
            return stringtable_.intern(str);
        }

        void
        ParserStateMachine::upsert_var(DVariable * var)
        {
            scope log(XO_DEBUG(true), "stub impl");
            log && log(xtag("var", std::string_view(*(var->name()))));
        }

        void
        ParserStateMachine::reset_result()
        {
            this->result_ = ParserResult();
        }

        void
        ParserStateMachine::clear_error_reset()
        {
            this->reset_result();

            while (stack_ && stack_->parent())
                stack_ = stack_->parent();

            this->parser_alloc_.restore(parser_alloc_ckp_);
        }

        void
        ParserStateMachine::on_parsed_symbol(std::string_view sym)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("sym", sym));

            assert(stack_);

            this->stack_->top().on_parsed_symbol(sym, this);
        }

        void
        ParserStateMachine::on_parsed_typedescr(TypeDescr td)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("td", td));

            assert(stack_);

            this->stack_->top().on_parsed_typedescr(td, this);
        }

        void
        ParserStateMachine::on_parsed_expression(obj<AExpression> expr)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("expr", expr));

            assert(stack_);

            this->top_ssm().on_parsed_expression(expr, this);
        }

        void
        ParserStateMachine::on_parsed_expression_with_semicolon(obj<AExpression> expr)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("expr", expr));

            assert(stack_);

            this->top_ssm().on_parsed_expression_with_semicolon(expr, this);
        }

        void
        ParserStateMachine::on_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            if (!stack_) {
                // parsing stack should always have toplevel expression sequence
                throw std::runtime_error(tostr("unexpected empty parsing stack",
                                               xtag("token", tk),
                                               xtag("help", "do it the same. but better!")
                                               ));
            }

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk);
                break;

            case tokentype::tk_def:
                this->on_def_token(tk);
                break;

            case tokentype::tk_if:
                this->on_if_token(tk);
                break;

            case tokentype::tk_colon:
                this->on_colon_token(tk);
                break;

            case tokentype::tk_singleassign:
                this->on_singleassign_token(tk);
                break;

            case tokentype::tk_f64:
                this->on_f64_token(tk);
                break;

            case tokentype::tk_semicolon:
                this->on_semicolon_token(tk);
                break;

            // all the not-yet handled cases
            case tokentype::tk_invalid:
            case tokentype::tk_bool:
            case tokentype::tk_i64:
            case tokentype::tk_string:
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
                throw std::runtime_error(tostr("ParserStateMachin::on_token:",
                                               "NOT IMPLEMENTED",
                                               xtag("token", tk)));

            }
        }

        void
        ParserStateMachine::on_symbol_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_symbol_token(tk, this);
        }

        void
        ParserStateMachine::on_def_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_def_token(tk, this);
        }

        void
        ParserStateMachine::on_if_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_if_token(tk, this);
        }

        void
        ParserStateMachine::on_colon_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_colon_token(tk, this);
        }

        void
        ParserStateMachine::on_singleassign_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_singleassign_token(tk, this);
        }

        void
        ParserStateMachine::on_f64_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_f64_token(tk, this);
        }

        void
        ParserStateMachine::on_semicolon_token(const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            stack_->top().on_semicolon_token(tk, this);
        }

        void
        ParserStateMachine::capture_error(std::string_view ssm_name,
                                          const DString * errmsg)
        {
            this->result_ = ParserResult::error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::illegal_input_on_token(std::string_view ssm_name,
                                                   const Token & tk,
                                                   std::string_view expect_str)
        {
            // TODO:
            // - want to write error message using DArena
            // - need something like log_streambuf and/or tostr() that's arena-aware

            auto errmsg_string = tostr("Unexpected token for parsing state",
                                       xtag("token", tk),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_input_on_token"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::illegal_input_on_symbol(std::string_view ssm_name,
                                                    std::string_view sym,
                                                   std::string_view expect_str)
        {
            // TODO:
            // - want to write error message using DArena
            // - need something like log_streambuf and/or tostr() that's arena-aware

            auto errmsg_string = tostr("Unexpected symbol for parsing state",
                                       xtag("symbol", sym),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_input_on_symbol"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::illegal_input_on_typedescr(std::string_view ssm_name,
                                                       TypeDescr td,
                                                       std::string_view expect_str)
        {
            // TODO:
            // - want to write error message using DArena
            // - need something like log_streambuf and/or tostr() that's arena-aware

            auto errmsg_string = tostr("Unexpected type-description for parsing state",
                                       xtag("td", td),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_input_on_typedescr"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::illegal_parsed_expression(std::string_view ssm_name,
                                                      obj<AExpression> expr,
                                                      std::string_view expect_str)
        {
            // TODO:
            // - want to write error message using DArena
            // - need something like log_streambuf and/or tostr() that's arena-aware

            auto errmsg_string = tostr("Unexpected expression",
                                       xtag("expr", expr),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_parsed_expression"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStateMachine.cpp */
