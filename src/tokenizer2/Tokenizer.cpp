/** @file Tokenizer.cpp
 *
 *  @author Roland Conybeare, Jul 2024
 **/

#include "Tokenizer.hpp"

namespace xo {
    using std::byte;

    namespace scm {
        Tokenizer::Tokenizer(const CircularBufferConfig & config,
                             bool debug_flag)
        : input_buffer_{DCircularBuffer::map(config)},
          input_state_{debug_flag}
        {}

        void
        Tokenizer::discard_current_line()
        {
            this->input_state_.discard_current_line();
        }

        bool
        Tokenizer::is_1char_punctuation(CharT ch)
        {
            switch(ch) {
            case '(':
                return true;
            case ')':
                return true;
            case '[':
                return true;
            case ']':
                return true;
            case '{':
                return true;
            case '}':
                return true;
            case '<':
                /* can't be 1char punctuation -- can begin lessequal token */
                return false;
            case '>':
                /* can't be 1char punctuation -- can begin greatequal token,
                 * and appears in tk_yields token
                 */
                return false;
            case ',':
                return true;
            case ';':
                return true;
            case ':':
                /* can't be 1char punctuation -- can begin assignment token */
                return false;
            case '=':
                /* can't be 1char punctuation -- can begin comparison token '==' */
                return false;
            case '!':
                /* can't be 1char punctuation -- can begin comparison token '!=' */
                return false;
            case '-':
                /* can't be punctuation
                 * - can appear inside f64 token: e.g. 1.23e-9.
                 * - begins tk_yields token: ->
                 */
                return false;
            case '+':
                /* can't be punctuation -- can appear inside f64 token: e.g. 1.23e+4 */
                return false;
            case '*':
                /* not punctuation -- allowed in symbol */
                return false;
            case '/':
                /* not punctuation -- for symmetry with +,- */
                return false;
            case '.':
                /* can't be punctuation -- can appear inside f64 token: e.g. 1.23 */
                return false;
            }

            return false;
        }

        bool
        Tokenizer::is_2char_punctuation(CharT ch)
        {
            /* can't put '-' here, because of the way it appears in numeric literals
             * characters here may not appear in symbol names
             */

            switch(ch) {
            case '<':
                /* can begin <= */
                return true;
            case '>':
                /* can begin >= */
                return true;
            case ':':
                /* can begin := */
                return true;
            case '=':
                /* can begin == */
                return true;
            case '!':
                /* can begin != */
                return true;
            }

            return false;
        }

        auto
        Tokenizer::assemble_token(std::size_t initial_whitespace,
                                  const span_type & token_text,
                                  TkInputState * p_input_state) -> result_type
        {
            /* literal|pretty|streamlined */
            log_config::style = function_style::streamlined;

            scope log(XO_DEBUG(p_input_state->debug_flag()));
            log && log(xtag("token_text", token_text),
                       xtag("initial_whitespace", initial_whitespace),
                       xtag("input_state", *p_input_state));

            tokentype tk_type = tokentype::tk_invalid;
            std::string tk_text;

            const CharT * tk_start = token_text.lo();
            const CharT * tk_end = token_text.hi();

            const CharT * ix = tk_start;

            /* switch here applies to the first character in a token */
            switch (*ix) {
            case '-':
            case '+':
                if (token_text.size() == 1) {
                    /* standalone '+' or '-' */
                    if (*ix == '+')
                        tk_type = tokentype::tk_plus;
                    else if(*ix == '-')
                        tk_type = tokentype::tk_minus;
                }

                /** fall through to numeric literal code below **/
                [[fallthrough]];
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                /* examples of valid floating-point numbers:
                 *   .0
                 *   1e0
                 *   1e
                 *   0.
                 *   +1e0
                 *   -1e0
                 *   +1E+2
                 *   -1E+2
                 *   -0.123e-10
                 * non-examples:
                 *   .
                 *   -
                 *   +
                 *   e0
                 *   .e0
                 *   -.e-0
                 *   +.e+0
                 *
                 * in particular: to be recognized as a number,
                 * must contain at least one digit
                 */

                log && log("possible number-token");

                /* true if initial sign -/+ encountered */
                bool sign_flag = false;
                /* true if '.' encountered */
                bool period_flag = false;
                /* true if 'e' | 'E' encountered.
                 */
                bool exponent_flag = false;
                /* true when sign '-' | '+' precedes exponenct digits */
                bool exponent_sign_flag = false;
                /* true when at least one digit follows exponent marker */
                bool exponent_digit_flag = false;
                /* true if at least one digit encountered */
                bool number_flag = false;

                log && log(xtag("*ix", *ix),
                           xtag("tk.length", token_text.size()));
                if (log && (ix + 1 < tk_end))
                    log(xtag("*(ix+1)", *(ix + 1)));

                if ((*ix == '-') && (ix + 2 == token_text.hi()) && (*(ix + 1) == '>')) {
                    /* composing exactly '->' */
                    tk_type = tokentype::tk_yields;
                } else {
                    /* token (if valid) will be one of: {tk_i64, tk_f64, tk_dot}: */
                    for (; ix != token_text.hi(); ++ix) {
                        if ((*ix == '-') || (*ix == '+')) {
                            /* sign allowed:
                             * 1. before period and before first digit
                             * 2. after exponent
                             */
                            if (!period_flag && !number_flag && !sign_flag) {
                                sign_flag = true;
                            } else if (exponent_flag && !exponent_digit_flag) {
                                exponent_sign_flag = true;
                            } else {
                                return result_type::make_error_consume_current_line
                                    (__FUNCTION__ /*src_function*/,
                                     "improperly placed sign indicator",
                                     (ix - tk_start),
                                     *p_input_state);
                            }
                        } else if (*ix == '.') {
                            if (period_flag) {
                                return result_type::make_error_consume_current_line
                                    (__FUNCTION__ /*src_function*/,
                                     "duplicate decimal point in numeric literal",
                                     (ix - tk_start),
                                     *p_input_state);
                            }

                            period_flag = true;
                        } else if ((*ix == 'e') || (*ix == 'E')) {
                            if (exponent_flag) {
                                return result_type::make_error_consume_current_line
                                    (__FUNCTION__ /*src_function*/,
                                     "duplicate exponent marker in numeric literal",
                                     (ix - tk_start),
                                     *p_input_state);
                            }

                            exponent_flag = true;
                        } else if (isdigit(*ix)) {
                            if (exponent_flag) {
                                /* need digit before exponent to recognize as number */
                                exponent_digit_flag = true;
                            } else {
                                number_flag = true;
                            }
                        } else {
                            return result_type::make_error_consume_current_line
                                (__FUNCTION__ /*src_function*/,
                                 "unexpected character in numeric constant" /*error_description*/,
                                 (ix - tk_start),
                                 *p_input_state);
                        }
                    }

                    if (number_flag) {
                        if (period_flag || exponent_flag) {
                            tk_type = tokentype::tk_f64;
                        } else {
                            tk_type = tokentype::tk_i64;
                        }
                    } else if (period_flag && !exponent_flag) {
                        tk_type = tokentype::tk_dot;
                    } else {
                        /* not a valid token */
                    }

                    log && log(xtag("sign_flag", sign_flag));
                    log && log(xtag("period_flag", period_flag),
                               xtag("exponent_flag", exponent_flag),
                               xtag("exponent_sign_flag", exponent_sign_flag),
                               xtag("number_flag", number_flag));
                    log && log(xtag("tk_type", tk_type));
                }

                break;
            }
            case '*':
                if (token_text.size() == 1) {
                    /* standalone '*' */
                    tk_type = tokentype::tk_star;
                    ++ix;
                } else {
                    /* '*' isn't punctuation -- but may allow appearance in a longer token
                     *
                     * thinking that x*y is a symbol with an embedded '*' character;
                     * in particular want to support kebab-case symbols like 'foo-config'
                     */
                }
                break;
            case '/':
                if (token_text.size() == 1) {
                    /* standalone '/' */
                    tk_type = tokentype::tk_slash;
                    ++ix;
                }
                break;
            case '=':
                log && log("singleassign or cmpeq token");

                if (*(ix + 1) == '=') {
                    tk_type = tokentype::tk_cmpeq;
                    ++ix;
                    ++ix;
                } else {
                    /* standalone '=' */
                    tk_type = tokentype::tk_singleassign;
                    ++ix;
                }
                break;
            case '!':
                if (*(ix + 1) == '=') {
                    tk_type = tokentype::tk_cmpne;
                    ++ix;
                    ++ix;
                } else {
                    /* standlone '!' */

                    // TODO
                }
                break;
            case '"':
            {
                log && log("recognize string-token");

                tk_type = tokentype::tk_string;

                tk_text.reserve(token_text.hi() - token_text.lo());

                ++ix; /*skip initial " char*/

                /* true on final " */
                bool endofstring = false;

                for (; ix != token_text.hi(); ++ix) {
                    log && log(xtag("*ix", *ix));

                    switch(*ix) {
                    case '"':
                        endofstring = true;

                        /* skip final " char, don't capture */
                        ++ix;

                        break;
                    case '\\':
                        /* skip escape char, don't capture */
                        ++ix;

                        if (ix == token_text.hi()) {
                            return result_type::make_error_consume_current_line
                                (__FUNCTION__ /*src_function*/,
                                 "expecting key following escape character \\",
                                 (ix - tk_start),
                                 *p_input_state);
                        }

                        switch(*ix) {
                        case '\\':
                            log && log(xtag("*ix", *ix), xtag("escaped", "t"));
                            tk_text.push_back(*ix);
                            break;
                        case 'n':
                            log && log(xtag("*ix", *ix), xtag("newline", "t"));
                            tk_text.push_back('\n');
                            break;
                        case 't':
                            log && log(xtag("*ix", *ix), xtag("tab", "t"));
                            tk_text.push_back('\t');
                            break;
                        case 'r':
                            log && log(xtag("*ix", *ix), xtag("cr", "t"));
                            tk_text.push_back('\r');
                            break;
                        case '"':
                            log && log(xtag("*ix", *ix), xtag("quote", "t"));
                            tk_text.push_back('"');
                            break;
                        default:
                            return result_type::make_error_consume_current_line
                                (__FUNCTION__ /*src_function*/,
                                 "expecting one of n|r|\"|\\ following escape \\",
                                 (ix - tk_start),
                                 *p_input_state);
                        }
                        break;
                    default:
                        tk_text.push_back(*ix);
                        break;
                    }

                    if (endofstring)
                        break;
                }

                if (!endofstring) {
                    return result_type::make_error_consume_current_line
                        (__FUNCTION__ /*src_function*/,
                         "missing terminating '\"' to complete literal string",
                         (ix - tk_start),
                         *p_input_state);
                }

                log && log(tostr("tokenizer::assemble_token",
                                 xtag("tk_text", tk_text)));

                break;
            }
            case 'a': case 'A':
            case 'b': case 'B':
            case 'c': case 'C':
            case 'd': case 'D':
            case 'e': case 'E':
            case 'f': case 'F':
            case 'g': case 'G':
            case 'h': case 'H':
            case 'i': case 'I':
            case 'j': case 'J':
            case 'k': case 'K':
            case 'l': case 'L':
            case 'm': case 'M':
            case 'n': case 'N':
            case 'o': case 'O':
            case 'p': case 'P':
            case 'q': case 'Q':
            case 'r': case 'R':
            case 's': case 'S':
            case 't': case 'T':
            case 'u': case 'U':
            case 'v': case 'V':
            case 'w': case 'W':
            case 'x': case 'X':
            case 'y': case 'Y':
            case 'z': case 'Z':
            {
                /* symbol/identifier must begin with a letter?
                 * we want to accept some other chars too.
                 * specifically want to allow identifiers:
                 *   this-is-the-way
                 *   this+is+also+the+way
                 *   how/much/is/that/doggy
                 *   put*an*asterisk*in*that
                 *   something%special%
                 *
                 * like pure lisp,  we don't allow:
                 * - identifier beginning with digit
                 * - period .
                 *
                 * unlike pure lisp,  we don't allow anywhere in a symbol:
                 * - colon     :
                 * - semicolon ;
                 * - comma     ,
                 *
                 * also we don't allow symbols to begin with special chars
                 */

                tk_type = tokentype::tk_symbol;
                break;
            }
            case '<':
            {
                log && log("leftangle or lessequal token");

                if (*(ix + 1) == '=') {
                    tk_type = tokentype::tk_lessequal;
                    ++ix;
                    ++ix;
                } else {
                    tk_type = tokentype::tk_leftangle;
                    ++ix;
                }
                break;
            }
            case '>':
            {
                log && log("rightangle or greatequal token");

                if (*(ix + 1) == '=') {
                    tk_type = tokentype::tk_greatequal;
                    ++ix;
                    ++ix;
                } else {
                    tk_type = tokentype::tk_rightangle;
                    ++ix;
                }
                break;
            }
            case '(':
                tk_type = tokentype::tk_leftparen;
                ++ix;
                break;
            case ')':
                tk_type = tokentype::tk_rightparen;
                ++ix;
                break;
            case '[':
                tk_type = tokentype::tk_leftbracket;
                ++ix;
                break;
            case ']':
                tk_type = tokentype::tk_rightbracket;
                ++ix;
                break;
            case '{':
                tk_type = tokentype::tk_leftbrace;
                ++ix;
                break;
            case '}':
                tk_type = tokentype::tk_rightbrace;
                ++ix;
                break;
            case ',':
                tk_type = tokentype::tk_comma;
                ++ix;
                break;
            case ';':
                tk_type = tokentype::tk_semicolon;
                ++ix;
                break;
            case ':':
            {
                log && log("colon or assignment token");

                if (*(ix + 1) == '=') {
                    tk_type = tokentype::tk_assign;
                    ++ix;
                    ++ix;
                } else {
                     tk_type = tokentype::tk_colon;
                     ++ix;
                }
                break;
            }
            default:
                break;
            }

            if (tk_type == tokentype::tk_invalid) {
                return result_type::make_error_consume_current_line
                    (__FUNCTION__ /*src_function*/,
                     "illegal input character",
                     (ix - tk_start),
                     *p_input_state);
            }

            if ((tk_type == tokentype::tk_i64)
                || (tk_type == tokentype::tk_f64)
                || (tk_type == tokentype::tk_symbol))
            {
                /* note: capturing token text here;
                 *       for numeric literals will re-parse in token::i64_value() / token::f64_value()
                 */
                tk_text = std::string(tk_start, tk_end);
            } else if (tk_type == tokentype::tk_string) {
                ; /* nothing to do here -- desired tk_text already constructed */
            }

            if (tk_type == tokentype::tk_symbol) {
                /* check for keywords */

                bool keep_text = false;

                if ((tk_text == "true") || (tk_text == "false")) {
                    tk_type = tokentype::tk_bool;
                    keep_text = true;
                } else if (tk_text == "type") {
                    tk_type = tokentype::tk_type;
                } else if (tk_text == "def") {
                    tk_type = tokentype::tk_def;
                } else if (tk_text == "lambda") {
                    tk_type = tokentype::tk_lambda;
                } else if (tk_text == "if") {
                    tk_type = tokentype::tk_if;
                } else if (tk_text == "then") {
                    tk_type = tokentype::tk_then;
                } else if (tk_text == "else") {
                    tk_type = tokentype::tk_else;
                } else if (tk_text == "let") {
                    tk_type = tokentype::tk_let;
                } else if (tk_text == "in") {
                    tk_type = tokentype::tk_in;
                } else if (tk_text == "end") {
                    tk_type = tokentype::tk_end;
                } else {
                    /* keep as symbol */
                    keep_text = true;
                }

                if (!keep_text)
                    tk_text.clear();
            }

            // TOOD: report tk_text as span,
            //       but must pin / unpin

            /* input.prefix(0):
             * require caller preserves current input line until it's entirely exhausted
             */
            return result_type(Token(tk_type, std::move(tk_text)),
                               p_input_state->current_line().prefix(0));
        } /*assemble_token*/

        auto
        Tokenizer::assemble_final_token(const span_type & token_text,
                                        TkInputState * p_input_state) -> result_type
        {
            return assemble_token(0 /*initial_whitespace*/,
                                  token_text,
                                  p_input_state);
        }

        auto
        Tokenizer::buffer_input_line(const char * input_cstr,
                                     bool eof_flag) -> std::pair<input_error, span_type>
        {
            scope log(XO_DEBUG(input_state_.debug_flag()));

            log && log(xtag("input", input_cstr));

            auto buf_input_0 = input_buffer_.input_range().hi();

            auto remainder = input_buffer_.append
                                 (DCircularBuffer::const_span_type::from_cstr(input_cstr));
            auto remainder2 = input_buffer_.append
                                  (DCircularBuffer::const_span_type::from_cstr("\n"));

            if (!remainder.empty() || !remainder2.empty()) {
                throw std::runtime_error(tostr("Tokenizer::buffer_line: line too long!",
                                               xtag("remainder.size", remainder.size())));
            }

            auto buf_input_1 = input_buffer_.input_range().hi();

            span_type input = span_type(buf_input_0,
                                        buf_input_1);

            return this->input_state_.capture_current_line(input, eof_flag);
        }

        auto
        Tokenizer::scan(const span_type & input) -> result_type
        {
            scope log(XO_DEBUG(input_state_.debug_flag()));

            /* - Always at beginning of token when scan() invoked
             * - scan will not report any portion of line as consumed until it has
             *   emitted all tokens in that line.
             *   rationale: caller is allowed to discard storage that
             *   scan() reports as consumed. But will be holding that line
             *   until all tokens have been read.
             * - this means caller will typically call scan()
             *   with the same input span multiple times
             */

            const CharT * ix = this->input_state_.skip_leading_whitespace();

            if(ix == input.hi()) {
                log && log("end input -> consume current line");

                /* entirety of current line has been tokenized
                 *  -> caller may consume it
                 */
                return result_type::make_whitespace(this->input_state_.consume_current_line());
            }

            /* ix: if ix < input.hi: first non-whitespace character after input_state_.current_pos_ */

            // TODO:
            // 1. hoist complete_flag up here
            // 2. use in each branch
            // 3. common check for prefix-capturing after if-cascade below done

            /* here: *ix is not whitespace */

            auto whitespace_z = input_state_.whitespace();

            log && log(xtag("whitespace_z", whitespace_z));

            /* tk_start points to known beginning of token
             * (after any whitespace)
             *
             * goal is to leave ix pointing to 1 char past the end of the token
             */
            const CharT * tk_start = ix;

            if (is_1char_punctuation(*ix)) {
                /* 1-character token */
                ++ix;
            } else if (is_2char_punctuation(*ix)) {
                CharT ch1 = *ix;

                (void)ch1;

                ++ix;

#ifdef OBSOLETE // no longer a thing. either input ends in whitespace, or ends translation unit
                if (ix == input.hi()) {
                    /* need more input to know if/when token complete */
                    this->prefix_ += std::string(tk_start, input.hi());

                    log && log(xtag("captured-prefix1", this->prefix_));
                } else
#endif
                    {
                    CharT ch2 = *ix;

                    if (((ch2 >= '0') && (ch2 <= '9'))
                        || ((ch2 >= 'A') && (ch2 <= 'Z'))
                        || ((ch2 >= 'a') && (ch2 <= 'z')))
                    {
                        /* treat as 1 char punctuation */
                        ;
                    } else {
                        /* include next char */
                        ++ix;
                    }
                }
            } else if (*ix == '"') {
                bool complete_flag = false;

                /* 1. embedded space/tab allowed in string literal.
                 * 2. embedded newline/cr not allowed.
                 */
                CharT prev_ch = '"';

                ++ix;

                for (; ix != input.hi(); ++ix) {
                    /* looking for unescaped " char to end literal */
                    if (*ix == '"') {
                        if (prev_ch != '\\') {
                            ++ix;  /* include terminating " for assemble_token */
                            complete_flag = true;
                            break;
                        }
                    } else if ((*ix == '\n') || (*ix == '\r')) {
                        log && log ("string literal with naked newline or CR");

                        return result_type::make_error_consume_current_line
                            (__FUNCTION__ /*src_function*/,
                             "must use \\n or \\r to encode newline/cr in string literal",
                             (ix - tk_start),
                             this->input_state_);
                    }

                    prev_ch = *ix;
                }

                if (!complete_flag) {
                    log && log("unterminated string literal");

                    return result_type::make_error_consume_current_line
                               (__FUNCTION__ /*src_function*/,
                                "unterminated string literal",
                                (ix - tk_start),
                                this->input_state_);
                }
            } else {
                /* ix is start of some token */

                if (*ix == '-') {
                    /* this section load-bearing for input '->' scanning from beginning of token */
                    ++ix;

                    if (ix == input.hi()) {
                        /* need more input to know if/when token complete -- see captured-prefix5 below */
                    } else {
                        CharT ch2 = *ix;

                        if (ch2 == '>') {
                            /* include next char and complete token */
                            ++ix;

                            log && log("complete '->' token");

                            this->input_state_.advance_until(ix);

                            return assemble_token(whitespace_z,
                                                  span_type(tk_start, ix) /*token*/,
                                                  &(this->input_state_));
                        }

                        /* here: -123, -.5e-21 for example */
                    }
                } else if (*ix == '>') {
                    /* this section load-bearing for input '>=' scanning from beginning of token.
                     * Need this because '>' necessarily excluded from is_1char_punctuation()
                     */
                    ++ix;

                    if (ix == input.hi()) {
                        /* need more input to know if/when token complete -- see captured-prefix5 below */
                    } else {
                        CharT ch2 = *ix;

                        if (ch2 != '=') {
                            log && log("complete '>=' token");

                            this->input_state_.advance_until(ix);

                            /* ignore next char and complete token */
                            return assemble_token(whitespace_z,
                                                  span_type(tk_start, ix) /*token*/,
                                                  &(this->input_state_));
                        }

                        /* here: >= for example */
                    }
                }

                /* scan until:
                 * - whitespace
                 * - punctuation
                 */
                for (; ix != input.hi(); ++ix) {
                    if (TkInputState::is_whitespace(*ix)
                        || is_1char_punctuation(*ix)
                        || is_2char_punctuation(*ix))
                    {
                        break;
                    }

                    /* this section load-bearing for input '>' after beginning of a token, e.g. p> */
                    if ((ix > tk_start) && (*ix == '>'))
                        break;

                    /* this section load-bearing for input '->' at the end of another token, e.g. p->q */
                    if (*ix == '-') {
                        if (ix + 1 == input.hi()) {
                            /* need more input to know if/when token complete
                             *
                             *   apple-banana   parses as: {tk_symbol: apple-banana}
                             *   apple->        parses as: {tk_symbol: apple} {tk_yields}
                             *   apple-         illegal (may not end symbol with '-')
                             */
                            break;
                        }

                        if (*(ix + 1) == '>') {
                            /* treat '->' as punctuation;  complete preceding token */
                            break;
                        }
                    }
                }
            }

            log && log("assemble token z", xtag("token_z", ix - tk_start));

            assert(tk_start < ix);

            this->input_state_.advance_until(ix);

            return assemble_token(whitespace_z,
                                  span_type(tk_start, ix) /*token*/,
                                  &(this->input_state_));
        } /*_scan_aux*/
    } /*namespace scm*/
} /*namespace xo*/

/* end Tokenizer.cpp */
