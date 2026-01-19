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
                                               obj<AAllocator> * expr_alloc)
            : parser_alloc_{DArena::map(config)},
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

        void
        ParserStateMachine::establish_toplevel_ssm(obj<ASyntaxStateMachine> ssm)
        {
            scope log(XO_DEBUG(debug_flag_));

            assert(stack_ == nullptr);

            auto alloc = with_facet<AAllocator>::mkobj(&parser_alloc_);

            this->stack_ = ParserStack::push(nullptr /*stack*/, alloc, ssm);
            this->parser_alloc_ckp_ = parser_alloc_.checkpoint();
        }

        void
        ParserStateMachine::push_ssm(obj<ASyntaxStateMachine> ssm)
        {
            scope log(XO_DEBUG(debug_flag_));

            // note: using parser_alloc_ for parser stack, since stacklike behavior

            auto alloc = with_facet<AAllocator>::mkobj(&parser_alloc_);

            this->stack_ = ParserStack::push(stack_, alloc, ssm);
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
            case tokentype::tk_def:
                this->on_def_token(tk);
                break;

            case tokentype::tk_if:
                this->on_if_token(tk);
                break;

            // all the not-yet handled cases
            case tokentype::tk_invalid:
            case tokentype::tk_bool:
            case tokentype::tk_i64:
            case tokentype::tk_f64:
            case tokentype::tk_string:
            case tokentype::tk_symbol:
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
            case tokentype::tk_colon:
            case tokentype::tk_doublecolon:
            case tokentype::tk_semicolon:
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

            auto errmsg = DString::from_view(*expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStateMachine.cpp */
