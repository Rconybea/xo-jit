/** @file SchematikaParser.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "SchematikaParser.hpp"
#include "ParserStateMachine.hpp"
#include "ParserStack.hpp"
#include "DExprSeqState.hpp"
#include <xo/indentlog/scope.hpp>
#include <stdexcept>

namespace xo {
    using xo::tostr;
    using xo::xtag;

    namespace scm {
        // ----- SchematikaParser -----

        SchematikaParser::SchematikaParser(const ArenaConfig & config, bool debug_flag)
            : psm_{config},
              debug_flag_{debug_flag}
        {
        }

        bool
        SchematikaParser::is_at_toplevel() const {
            return psm_.stack() == nullptr;
        }

        bool
        SchematikaParser::has_incomplete_expr() const {
            /* (don't count toplevel exprseq) */
            ParserStack * stack = psm_.stack();
            if (!stack)
                return false;
            return stack->parent() != nullptr;
        }

        void
        SchematikaParser::begin_interactive_session() {
            DExprSeqState::start_interactive(psm_.expr_alloc(), &psm_);
        }

        void
        SchematikaParser::begin_translation_unit() {
            DExprSeqState::start_batch(psm_.expr_alloc(), &psm_);
        }

        const ParserResult &
        SchematikaParser::include_token(const token_type & tk)
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
            psm_.reset_stack();
            psm_.reset_result();
        } /*reset_to_idle_toplevel*/

        void
        SchematikaParser::print(std::ostream & os) const {
            os << "<SchematikaParser"
               << xtag("debug", debug_flag_)
               << xtag("has_stack", (psm_.stack() != nullptr))
               << ">" << std::endl;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaParser.cpp */
