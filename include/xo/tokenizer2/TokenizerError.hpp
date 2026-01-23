/* file TokenizerError.hpp
 *
 * author: Roland Conybeare, Jun 2025
 */

#pragma once

#include "TkInputState.hpp"
#include "tokentype.hpp"
#include "span.hpp"
#include <iomanip>

namespace xo {
    namespace scm {
        /** @class tokenizer_error
         *  @brief represent a lexing error, with context
         *
         *  @tparam CharT  representation for single characters
         **/
        class TokenizerError {
        public:
            using CharT = char;
            using span_type = xo::mm::span<const CharT>;

        public:
            /** @defgroup tokenizer-error-ctors **/
            ///@{

            /** Default ctor represents a not-an-error sentinel object **/
            TokenizerError() = default;
            /** Constructor to capture parsing error context
             *  @p tk_start   current position on entry to scanner
             *  @p error_pos  error location relative to token start
             **/
            TokenizerError(std::string_view src_function,
                           std::string error_description,
                           const TkInputState & input_state,
                           size_t error_pos)
                : src_function_{src_function},
                  error_description_{std::move(error_description)},
                  input_state_{input_state},
                  error_pos_{error_pos}
                {
                    scope log(XO_DEBUG(input_state.debug_flag()));

                    log && log(xtag("input_state.current_pos", input_state.current_pos()),
                               xtag("error_pos", error_pos));
                }

            TokenizerError with_error(std::string_view error_src_fn,
                                      std::string error_msg) {
                return TokenizerError(error_src_fn,
                                      std::string(error_msg),
                                      this->input_state_,
                                      0 /*error_pos*/);
            }
            ///@}

            /** @defgroup tokenizer-error-access-methods **/
            ///@{

            std::string_view src_function() const { return src_function_; }
            const std::string & error_description() const { return error_description_; }
#pragma GCC diagnostic push
#ifndef __APPLE__
#pragma GCC diagnostic ignored "-Wchanges-meaning"
#endif
            const TkInputState & input_state() const { return input_state_; }
#pragma GCC diagnostic pop
            size_t tk_start() const { return input_state_.current_pos(); }
            size_t whitespace() const { return input_state_.whitespace(); }
            size_t error_pos() const { return error_pos_; }

            ///@}

            /** @defgroup tokenizer-error-general-methods **/
            ///@{

            /** true, except for a sentinel error object **/
            bool is_error() const { return !error_description_.empty(); }
            /** false except for object in sentinel state **/
            bool is_not_an_error() const { return error_description_.empty(); }

            /** Print representation to stream @p os. Intended for tokenizer diagnostics.
             *  For Schematika errors prefer @ref report
             **/
            void print(std::ostream & os) const;

            /** Print human-oriented error report on @p os. **/
            void report(std::ostream & os) const;

            ///@}

        private:
            /** @defgroup tokenizer-error-vars **/
            ///@{

            /** source location (in tokenizer) at which error identified **/
            std::string_view src_function_;
            /** error description **/
            std::string error_description_;
            /** input state associated with this error.
             *  Sufficient to precisely locate it with context.
             **/
            TkInputState input_state_;
            /** position (relative to @ref tk_entry_) of error **/
            size_t error_pos_ = 0;

            ///@}
        };

        inline std::ostream &
        operator<< (std::ostream & os,
                    const TokenizerError & tkerr)
        {
            tkerr.print(os);
            return os;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end tokenizer_error.hpp */
