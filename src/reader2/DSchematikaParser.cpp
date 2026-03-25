/** @file DSchematikaParser.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "SchematikaParser.hpp"
#include "ParserStateMachine.hpp"
#include "ParserStack.hpp"
#include "DToplevelSeqSsm.hpp"
#include <cstddef>
#include <xo/indentlog/scope.hpp>
#include <stdexcept>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AAllocator;
    using xo::mm::AGCObject;
    using xo::mm::MemorySizeInfo;
    using xo::tostr;
    using xo::xtag;

    namespace scm {
        // ----- SchematikaParser -----

        DSchematikaParser::DSchematikaParser(const ParserConfig & cfg,
                                             obj<AAllocator> expr_alloc,
                                             obj<AAllocator> aux_alloc)
        : psm_{
                cfg.parser_arena_config_,
                cfg.symtab_var_config_,
                cfg.symtab_types_config_,
                cfg.max_stringtable_capacity_,
                cfg.pm_install_flags_,
                expr_alloc,
                aux_alloc
            },
          debug_flag_{cfg.debug_flag_}
        {
        }

        DSchematikaParser *
        DSchematikaParser::_make(obj<AAllocator> mm,
                                 const ParserConfig & cfg,
                                 obj<AAllocator> expr_alloc,
                                 obj<AAllocator> aux_alloc)
        {
            void * mem = mm.alloc_for<DSchematikaParser>();

            return new (mem) DSchematikaParser(cfg, expr_alloc, aux_alloc);
        }

        obj<AGCObject,DSchematikaParser>
        DSchematikaParser::make(obj<AAllocator> mm,
                                const ParserConfig & cfg,
                                obj<AAllocator> expr_alloc,
                                obj<AAllocator> aux_alloc)
        {
            return obj<AGCObject,DSchematikaParser>(_make(mm, cfg, expr_alloc, aux_alloc));
        }

        DGlobalSymtab *
        DSchematikaParser::global_symtab() const noexcept
        {
            return psm_.global_symtab();
        }

        DGlobalEnv *
        DSchematikaParser::global_env() const noexcept
        {
            return psm_.global_env();
        }

        bool
        DSchematikaParser::is_at_toplevel() const
        {
            return psm_.is_at_toplevel();
        }

        bool
        DSchematikaParser::has_incomplete_expr() const
        {
            return psm_.has_incomplete_expr();
        }

        obj<ASyntaxStateMachine>
        DSchematikaParser::top_ssm() const
        {
            return psm_.top_ssm();
        }

        const ParserResult &
        DSchematikaParser::result() const
        {
            return psm_.result();
        }

        void
        DSchematikaParser::visit_pools(const MemorySizeVisitor & visitor) const
        {
            return psm_.visit_pools(visitor);
        }

        void
        DSchematikaParser::begin_interactive_session()
        {
            DToplevelSeqSsm::establish_interactive(psm_.parser_alloc(), &psm_);

        }

        void
        DSchematikaParser::begin_batch_session()
        {
            DToplevelSeqSsm::establish_batch(psm_.parser_alloc(), &psm_);
        }

        const DUniqueString *
        DSchematikaParser::intern_string(std::string_view str)
        {
            return psm_.intern_string(str);
        }

        const ParserResult &
        DSchematikaParser::on_token(const token_type & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            if (psm_.stack() == nullptr) {
                throw std::runtime_error(tostr("DSchematikaParser::include_token",
                                               ": parser not expecting input"
                                               "(call parser.begin_translation_unit()..?)",
                                               xtag("token", tk)));
            }

            /* stack is non-empty */

            psm_.on_token(tk);

            log && log(xtag("parser", this));
            log && log(xtag("result", psm_.result()));

            return psm_.result();
        } /*include_token*/

        void
        DSchematikaParser::reset_result()
        {
            psm_.reset_result();
        }

        void
        DSchematikaParser::reset_to_idle_toplevel()
        {
            psm_.clear_error_reset();
        } /*reset_to_idle_toplevel*/

        void
        DSchematikaParser::print(std::ostream & os) const {
            os << "<SchematikaParser"
               << xtag("debug", debug_flag_)
               << xtag("has_stack", (psm_.stack() != nullptr))
               << ">" << std::endl;
        }

        bool
        DSchematikaParser::pretty(const ppindentinfo & ppii) const {
            auto * pps = ppii.pps();

            if (ppii.upto())
                return false;

            // TODO: consider printing:
            //   psm.stringtable_
            //   psm.parser_alloc_
            //   psm.parser_alloc_ckp_
            //   psm.expr_alloc_
            //   psm.result_
            //   psm.debug_flag_
            //

            return pps->pretty_struct
                       (ppii,
                        "SchematikaParser",
                        refrtag("stack", psm_.stack())
                           );
        }

        std::size_t
        DSchematikaParser::shallow_size() const noexcept
        {
            return sizeof(DSchematikaParser);
        }

        DSchematikaParser *
        DSchematikaParser::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            (void)mm;

            assert(false);
            return nullptr;
        }

        std::size_t
        DSchematikaParser::forward_children(obj<ACollector> gc) noexcept
        {
            psm_.forward_children(gc);

            return this->shallow_size();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaParser.cpp */
