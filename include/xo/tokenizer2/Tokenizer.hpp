/* file Tokenizer.hpp
 *
 * author: Roland Conybeare, Jul 2024
 */

#pragma once

#include "Token.hpp"
#include "TkInputState.hpp"
#include "span.hpp"
#include "scan_result.hpp"
#include <xo/arena/DCircularBuffer.hpp>
#include <xo/indentlog/scope.hpp>
#include <xo/indentlog/print/ppdetail_atomic.hpp>
#include <cassert>

namespace xo {
    namespace scm {
        /** @class Tokenizer
         *  @brief Parse a Schematika character stream into lexical tokens
         *
         *  Use:
         *
         *  @code
         *    // see xo-tokenizer2/example/tokenrepl/tokenrepl.cpp
         *    // for exact working code
         *
         *    using tokenizer_type = tokenizer<char>;
         *    using span_type = tokenizer_type::span_type;
         *
         *    tokenizer_type tkz;
         *    span_type input = ...;
         *
         *    while (!input.empty()) {
         *        auto [tk, consumed, error] = tkz.scan(input);
         *
         *        if (tk.is_valid()) {
         *            // do something with tk
         *        } else if (error.is_error()) {
         *            error.report(cout);
         *            break;
         *        }
         *
         *        input = input.after_prefix(consumed);
         *    }
         *
         *    if endofinput {
         *        auto [tk, consumed, error] = tzk.notify_eof()
         *
         *        // do something with (final) tk if tk.is_valid()
         *    }
         *
         *  @endcode
         *
         * See tokentype.hpp for token types
         **/
        class Tokenizer {
        public:
            using CharT = char;
            using token_type = Token;
            using error_type = TokenizerError;
            using DCircularBuffer = xo::mm::DCircularBuffer;
            using CircularBufferConfig = xo::mm::CircularBufferConfig;
            using span_type = xo::mm::span<const CharT>;
            //using input_state_type = TkInputState;
            using result_type = scan_result;

        public:
            /** @defgroup tokenizer-ctors tokenizer constructors **/
            ///@{

            /**
             *  @p config     gives configuration for circular input buffer
             *  @p debug_flag enables tokenizer debug output
             **/
            Tokenizer(const CircularBufferConfig & config = CircularBufferConfig{.name_ = "tkz-input",
                                                                                 .max_capacity_ = 4*1024,
                                                                                 .max_captured_span_ = 128},
                      bool debug_flag = false);

            ///@}

            /** @defgroup tokenizer-access-methods tokenizer access methods **/
            ///@{

#pragma GCC diagnostic push
#ifndef __APPLE__
#pragma GCC diagnostic ignored "-Wchanges-meaning"
#endif
            const TkInputState & input_state() const { return input_state_; }
#pragma GCC diagnostic pop

            ///@}

            /** @defgroup tokenizer-general-methods tokenizer methods **/
            ///@{

            /** identifies punctuation chars.
             *  These are chars that are not permitted to appear within
             *  a symbol token.  Instead they force completion of
             *  a preceding token,  and start a new token with themselves
             **/
            static bool is_1char_punctuation(CharT ch);

            /** more-relaxed version of is_1char_punctuation.
             *  Chars that are not permitted to appear within a symbol token,
             *  but may form token combined with next character
             **/
            static bool is_2char_punctuation(CharT ch);

            /** assemble token from text @p token_text.
             *  @p initial_whitespace   Amount of whitespace input being consumed from input.
             *  @p token_text subset of input_line representing a single token.
             *  @p p_input_state input state containing input_line.  On exit current line cleared
             *                   if error
             *
             *  retval.consumed will represent some possibly-empty prefix of @p input
             **/
            static scan_result assemble_token(std::size_t initial_whitespace,
                                              const span_type & token_text,
                                              TkInputState * p_input_state);

            /** degenerate version of assemble_token() on reaching end-of-file **/
            static scan_result assemble_final_token(const span_type & token_text,
                                                    TkInputState * p_input_state);

            /** true if tokenizer contains stored prefix of
             *  possibly-incomplete token
             **/
            bool has_prefix() const { return !prefix_.empty(); }

            /** copy into buffer the  contents of @p input.
             *  May throw if buffer space exhausted
             **/
            std::pair<input_error, span_type> buffer_input_line(span_type input,
                                                                bool eof_flag);

            /** scan for next input token,  given @p input.
             *  Note:
             *  - tokenizer can consume input (e.g. whitespace)
             *    without completing a token
             *  - input will remember the extent of the last line of input
             *    for which parsing has begun, but not completed.
             *    It's required that at least that portion of the input span
             *    remain valid across scan(), scan2() calls
             *
             *  @return {parsed token, consumed span}
             **/
            scan_result scan(const span_type & input);

            /** discard current line after error.  Just cleans up error-reporting state **/
            void discard_current_line();

            ///@}

        private:
            /** @defgroup tokenizer-instance-vars tokenizer instance variables **/
            ///@{

            /** Buffer input here. vm-aware. uses mmap directly **/
            DCircularBuffer input_buffer_;
            /** track input state (line#,pos,..) for error messages.
             *  There's an ordering problem here:
             *  1. input_state_.skip_leading_whitespace() advances
             *     current line automagically when it sees \n
             *  2. need to capture value of @ref input_state_ _before_ newline
             *  3. but neeed newline to end token
             *  Also recall input_state_type needed for reporting errors.
             **/
            TkInputState input_state_;
            /** Accumulate partial token here.
             *  This will happen if input sent to @ref tokenizer::scan
             *  ends without whitespace such that last available token's
             *  extent is not determined
             **/
            std::string prefix_;

            ///@}
        }; /*tokenizer*/

    } /*namespace scm*/
} /*namespace xo*/

/* end Tokenizer.hpp */
