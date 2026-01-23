/* file scan_result.hpp
 *
 * author: Roland Conybeare, Jun 2025
 */

#pragma once

#include "Token.hpp"
#include "TokenizerError.hpp"
#include "TkInputState.hpp"

namespace xo {
    namespace scm {
        /** @class scan_result
         *  @brief Represent result of parsing one input token.
         *
         * @code
         *  Possible outcomes fall into several categories
         *  (with T: @c token_.is_valid(), E: @cerror_.is_error())
         *
         *  | T     | E     | description                         |
         *  |-------+-------+-------------------------------------|
         *  | false | false | end of input, including end of line |
         *  | true  | false | parsed token in T                   |
         *  | false | true  | parse error in E                    |
         *
         * @endcode
         **/
        class scan_result {
        public:
            //using CharT = char;
            using token_type = Token;
            using span_type = xo::mm::span<const char>;
            using error_type = TokenizerError;
            using input_state_type = TkInputState;

        public:
            scan_result(const Token & token,
                        const span_type & consumed,
                        const TokenizerError & error = TokenizerError())
                : token_{token}, consumed_{consumed}, error_{error} {}

            static scan_result make_whitespace(const span_type & prefix_input);
            static scan_result make_partial(const span_type & prefix_input);
            /**
             *  @p error_src can be __FUNCTION__ from site where error generated.
             *  @p error_msg error message
             *  @p error_pos error position, relative to start of token
             *  @p input_state_ref input state object;
             *  copied into scan_result, and leaving input_state_ref.current_line cleared
             **/
            static scan_result make_error_consume_current_line(const char * error_src,
                                                               std::string error_msg,
                                                               size_t error_pos,
                                                               input_state_type & input_state_ref);

            bool is_eof_or_ambiguous() const { return token_.is_invalid() && error_.is_not_an_error(); }
            bool is_token() const { return token_.is_valid(); }
            bool is_error() const { return error_.is_error(); }

            const Token & get_token() const { return token_; }
            const span_type & consumed() const { return consumed_; }
            const TokenizerError & error() const { return error_; }

        public:
            /** Successfully parsed token, whenever tk_type != tokentype::tk_invalid.
             *  Will be tokentype::tk_invalid in normal cause of events for valid input,
             *  when consuming whitespace
             **/
            token_type token_;
            /** input span represented by .token, on success. Otherwise not defined **/
            span_type consumed_;
            /** error description, whenever .error_.is_error() is true **/
            TokenizerError error_;
        };


    } /*namespace scm*/
} /*namespace xo*/

/* end scan_result.hpp */
