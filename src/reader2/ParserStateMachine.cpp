/** @file ParserStateMachine.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserStateMachine.hpp"
#include "ParserStack.hpp"
#include "SyntaxStateMachine.hpp"
#include <xo/object2/array/IPrintable_DArray.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>
#include <xo/indentlog/print/tostr.hpp>
#include <xo/indentlog/print/tag.hpp>
#include <stdexcept>

namespace xo {
    using xo::mm::MemorySizeInfo;
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::with_facet;

    namespace scm {
        ParserStateMachine::ParserStateMachine(const ArenaConfig & config,
                                               size_type max_stringtable_capacity,
                                               obj<AAllocator> expr_alloc,
                                               obj<AAllocator> aux_alloc)
            : stringtable_{max_stringtable_capacity},
              parser_alloc_{DArena::map(config)},
              expr_alloc_{expr_alloc},
              aux_alloc_{aux_alloc},
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
        ParserStateMachine::visit_pools(const MemorySizeVisitor & visitor) const
        {
            stringtable_.visit_pools(visitor);
            parser_alloc_.visit_pools(visitor);

            // not counting {expr_alloc_, fixed_alloc_}. We don't consider
            // either to be owned by ParserStateMachine
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

        const DUniqueString *
        ParserStateMachine::gensym(std::string_view str)
        {
            return stringtable_.gensym(str);
        }

        DVarRef *
        ParserStateMachine::lookup_varref(std::string_view symbolname)
        {
            scope log(XO_DEBUG(debug_flag_));

            // TODO:
            // 1. check global symtab
            // 2. combine local+global symtab into indept struct
            // 3. move lookup_varref implementation there.
            //

            if (local_symtab_) {
                const DUniqueString * ustr = stringtable_.lookup(symbolname);

                if (ustr) {
                    DLocalSymtab * symtab = local_symtab_;

                    // count #of nested scopes to cross, to reach symbol
                    //
                    int32_t link_count = 0;

                    while (symtab) {
                        Binding b = symtab->lookup_binding(ustr);

                        if (b.is_local()) {
                            assert(b.i_link() == 0);

                            DVariable * vardef = symtab->lookup_var(b);
                            assert(vardef);


                            /** ascii diagram here
                             **/

                            return DVarRef::make(expr_alloc_,
                                                 vardef,
                                                 link_count);
                        } else {
                            assert(b.is_null());
                        }

                        ++link_count;
                        symtab = symtab->parent();
                    }
                } else {
                    // if we don't already know the symbol,
                    //  -> can't be a valid variable reference
                    //     (whether global or local)

                    return nullptr;
                }
            }

            // TODO: check global symtab also

            log.retroactively_enable();
            log("STUB: check global symtab");

            return nullptr;
        }

        void
        ParserStateMachine::push_local_symtab(DLocalSymtab * symtab)
        {
            this->local_symtab_ = symtab;
        }

        void
        ParserStateMachine::pop_local_symtab()
        {
            assert(local_symtab_);

            this->local_symtab_ = local_symtab_->parent();
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
        ParserStateMachine::on_parsed_formal(const DUniqueString * sym,
                                             TypeDescr td)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("sym", std::string_view(*sym)), xtag("td", td));

            assert(stack_);

            this->stack_->top().on_parsed_formal(sym, td, this);
        }

        void
        ParserStateMachine::on_parsed_formal_arglist(DArray * arglist)
        {
            scope log(XO_DEBUG(debug_flag_),
                      xtag("arglist", obj<APrintable,DArray>(arglist)));

            assert(stack_);

            this->stack_->top().on_parsed_formal_arglist(arglist, this);
        }

        void
        ParserStateMachine::on_parsed_expression(obj<AExpression> expr)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("expr", expr));

            assert(stack_);

            this->top_ssm().on_parsed_expression(expr, this);
        }

        void
        ParserStateMachine::on_parsed_expression_with_token(obj<AExpression> expr,
                                                            const Token & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("expr", expr), xtag("tk", tk));

            assert(stack_);

            this->top_ssm().on_parsed_expression_with_token(expr, tk, this);
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

            stack_->top().on_token(tk, this);
        }

        void
        ParserStateMachine::capture_result(std::string_view ssm_name,
                                           obj<AExpression> expr)
        {
            this->result_ = ParserResult::expression(ssm_name, expr);
        }

        void
        ParserStateMachine::capture_error(std::string_view ssm_name,
                                          const DString * errmsg)
        {
            if (result_.is_error()) {
                /* in case one error triggers another, remmber just the first one */
            } else {
                this->result_ = ParserResult::error(ssm_name, errmsg);
            }
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
        ParserStateMachine::illegal_parsed_formal(std::string_view ssm_name,
                                                  const DUniqueString * param_name,
                                                  TypeDescr param_type,
                                                  std::string_view expect_str)
        {
            // TODO:
            // - want to write error message using DArena
            // - need something like log_streambuf and/or tostr() that's arena-aware

            auto errmsg_string = tostr("Unexpected formal",
                                       xtag("param_name", std::string_view(*param_name)),
                                       xtag("param_type", param_type),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_parsed_formal"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::illegal_parsed_formal_arglist(std::string_view ssm_name,
                                                          DArray * arglist,
                                                          std::string_view expect_str)
        {
            obj<APrintable,DArray> arglist_pr(arglist);

            auto errmsg_string = tostr("Unexpected formal arglist",
                                       xtag("arglist", arglist_pr),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_parsed_formal_arglist"));

            assert(expr_alloc_);

            auto errmsg = DString::from_str(expr_alloc_, errmsg_string);

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

            auto expr_pr = expr.to_facet<APrintable>();
            //= FacetRegistry::instance().variant<APrintable,AExpression>(expr);
            assert(expr_pr);

            /** TODO
             *  problem here: we have pretty() support for obj<AExpression>,
             *  but not "ordinary printing" support.  So expression doesn't get printed
             **/
            auto errmsg_string = tostr("Unexpected expression",
                                       xtag("expr", expr_pr),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_parsed_expression"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::illegal_parsed_expression_with_token(std::string_view ssm_name,
                                                                 obj<AExpression> expr,
                                                                 const Token & tk,
                                                                 std::string_view expect_str)
        {
            // TODO:
            // - want to write error message using DArena
            // - need something like log_streambuf and/or tostr() that's arena-aware

            obj<APrintable> expr_pr
                = FacetRegistry::instance().variant<APrintable,AExpression>(expr);
            assert(expr_pr);

            /** TODO
             *  problem here: we have pretty() support for obj<AExpression>,
             *  but not "ordinary printing" support.  So expression doesn't get printed
             **/
            auto errmsg_string = tostr("Unexpected expression",
                                       xtag("expr", expr_pr),
                                       xtag("tk", tk),
                                       xtag("expecting", expect_str),
                                       xtag("ssm", ssm_name),
                                       xtag("via", "ParserStateMachine::illegal_parsed_expression"));

            assert(expr_alloc_);

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

        void
        ParserStateMachine::error_unbound_variable(std::string_view ssm_name,
                                                   std::string_view sym)
        {
            auto errmsg_string = tostr("No binding for symbol",
                                       xtag("symbol", sym),
                                       xtag("ssm", ssm_name));

            auto errmsg = DString::from_view(expr_alloc_,
                                             std::string_view(errmsg_string));

            this->capture_error(ssm_name, errmsg);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStateMachine.cpp */
