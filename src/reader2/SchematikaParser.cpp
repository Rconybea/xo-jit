/** @file SchematikaParser.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "SchematikaParser.hpp"
#include "ParserStateMachine.hpp"
#include "ParserStack.hpp"
#include "DExprSeqState.hpp"
#include <cstddef>
#include <xo/indentlog/scope.hpp>
#include <stdexcept>

namespace xo {
    using xo::mm::AAllocator;
    using xo::tostr;
    using xo::xtag;

    namespace scm {
        // ----- SchematikaParser -----

        SchematikaParser::SchematikaParser(const ArenaConfig & config,
                                           size_t max_stringtable_capacity,
                                           obj<AAllocator> expr_alloc,
                                           bool debug_flag)
            : psm_{config, max_stringtable_capacity, expr_alloc},
              debug_flag_{debug_flag}
        {
        }

        bool
        SchematikaParser::is_at_toplevel() const
        {
            return psm_.is_at_toplevel();
        }

        bool
        SchematikaParser::has_incomplete_expr() const
        {
            return !(this->is_at_toplevel());
        }

        obj<ASyntaxStateMachine>
        SchematikaParser::top_ssm() const
        {
            return psm_.top_ssm();
        }

        void
        SchematikaParser::begin_interactive_session()
        {
            DExprSeqState::establish_interactive(psm_.expr_alloc(), &psm_);

        }

        void
        SchematikaParser::begin_batch_session()
        {
            DExprSeqState::establish_batch(psm_.expr_alloc(), &psm_);
        }

        const ParserResult &
        SchematikaParser::on_token(const token_type & tk)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("tk", tk));

            if (psm_.stack() == nullptr) {
                throw std::runtime_error(tostr("SchematikaParser::include_token",
                                               ": parser not expecting input"
                                               "(call parser.begin_translation_unit()..?)",
                                               xtag("token", tk)));
            }

            /* stack is non-empty */

            psm_.on_token(tk);

            return psm_.result();
        } /*include_token*/

        void
        SchematikaParser::reset_result()
        {
            psm_.reset_result();
        }

        void
        SchematikaParser::reset_to_idle_toplevel()
        {
            psm_.clear_error_reset();
        } /*reset_to_idle_toplevel*/

        void
        SchematikaParser::print(std::ostream & os) const {
            os << "<SchematikaParser"
               << xtag("debug", debug_flag_)
               << xtag("has_stack", (psm_.stack() != nullptr))
               << ">" << std::endl;
        }

        bool
        SchematikaParser::pretty(const ppindentinfo & ppii) const {
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
                        refrtag("stack", psm_.stack()));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaParser.cpp */
