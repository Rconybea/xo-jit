/* file Token.hpp
 *
 * author: Roland Conybeare, Jul 2024
 */

#pragma once

#include "tokentype.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <stdexcept>
#include <ostream>
#include <string>
#include <cstdint>

namespace xo {
    namespace scm {
        namespace detail {
            /* compute a * b^p,  p >= 0 */
            constexpr double
            pow_aux(double a, double b, int p) {
                while (p > 0) {
                    if (p % 2 == 1) {
                        /* a * b^p = a * b^(2q + 1) = a.b * 10^(2q) */
                        a *= b;
                        p -= 1;
                    } else {
                        /* a * b^p = a * b^(2q) = a * (b^2)^q */
                        b = b * b;
                        p /= 2;
                    }
                }

                /* a * b^0 = a */
                return a;
            }

            constexpr double
            pow10(int p) {
                if (p >= 0)
                    return pow_aux(1.0, 10.0, p);
                else
                    return 1.0 / pow_aux(1.0, 10.0, -p);
            }
        }

        /** @class token
         *  @brief Represent a Schematika lexical token
         **/
        class Token {
        public:
            /** @defgroup token-ctors token constructors **/
            ///@{

            /** default ctor creates token with type @c tk_invalid **/
            Token() = default;
            /** create token with type @c tk_type and input text @c text **/
            Token(tokentype tk_type, const std::string & text = "")
                : tk_type_{tk_type}, text_{text} {}

            /** create invalid token (same as null ctor, but explicit) **/
            static Token invalid() { return Token(); }
            /** Create token representing a boolean literal from text @p txt
             *  @p txt must be @c true or @c false
             **/
            static Token bool_token(const std::string & txt) {
                return Token(tokentype::tk_bool, txt);
            }
            /** Create token representing 64-bit signed integer literal parsed from decimal @p txt.
             *  The string @p txt must be a decimal integer literal, since @ref i64_value re-parses @p txt.
             **/
            static Token i64_token(const std::string & txt) {
                return Token(tokentype::tk_i64, txt);
            }
            /** create token representing 64-bit floating-point literal parsed from decimal @p txt
             *  The string @p txt must be a decimal floating-point literal, since @ref f64_value re-parses @p txt.
             **/
            static Token f64_token(const std::string & txt) {
                return Token(tokentype::tk_f64, txt);
            }
            /** create token representing literal string parsed from @p txt **/
            static Token string_token(const std::string & txt) {
                return Token(tokentype::tk_string, txt);
            }
            /** create token representing a symbol parsed from @p txt.
             *  Note that not all strings are valid symbol names.
             **/
            static Token symbol_token(const std::string & txt) {
                return Token(tokentype::tk_symbol, txt);
            }
            /** token representing left angle bracket @c "<" **/
            static Token leftangle() { return Token(tokentype::tk_leftangle); }
            /** token representing right angle bracket @c ">" **/
            static Token rightangle() { return Token(tokentype::tk_rightangle); }
            /** token representing left parenthesis @c "(" **/
            static Token leftparen() { return Token(tokentype::tk_leftparen); }
            /** Token representing right parenthesis @c ")" **/
            static Token rightparen() { return Token(tokentype::tk_rightparen); }
            /** token representing left bracket @c "[" **/
            static Token leftbracket() { return Token(tokentype::tk_leftbracket); }
            /** token representing right bracket @c "]" **/
            static Token rightbracket() { return Token(tokentype::tk_rightbracket); }
            /** token representing left brace @c "{" **/
            static Token leftbrace() { return Token(tokentype::tk_leftbrace); }
            /** token representing right brace @c "}" **/
            static Token rightbrace() { return Token(tokentype::tk_rightbrace); }
            /** token representing period @c "." **/
            static Token dot() { return Token(tokentype::tk_dot); }
            /** token representing comma @c "," **/
            static Token comma() { return Token(tokentype::tk_comma); }
            /** token representing colon @c ":" **/
            static Token colon_token() { return Token(tokentype::tk_colon); }
            /** token representing double-colo @c "::" **/
            static Token doublecolon() { return Token(tokentype::tk_doublecolon); }
            /** token representing semicolon @c ";" **/
            static Token semicolon_token() { return Token(tokentype::tk_semicolon); }
            /** token representing single-assignment @c "=" (editor bait: equal_token) **/
            static Token singleassign_token() { return Token(tokentype::tk_singleassign); }
            /** token representing unrestricted assignment @c ":=" **/
            static Token assign_token() { return Token(tokentype::tk_assign); }
            /** token representing indirection @c "->" **/
            static Token yields() { return Token(tokentype::tk_yields); }

            /** token for @c "+" **/
            static Token plus_token() { return Token(tokentype::tk_plus); }
            /** token for @c "-" **/
            static Token minus_token() { return Token(tokentype::tk_minus); }
            /** token for @c "*" **/
            static Token star_token() { return Token(tokentype::tk_star); }
            /** token for @c "/" **/
            static Token slash_token() { return Token(tokentype::tk_slash); }

            /** token representing keyword @c type **/
            static Token type() { return Token(tokentype::tk_type); }
            /** token representing keyword @c def **/
            static Token def_token() { return Token(tokentype::tk_def); }
            /** token representing keyword @c lambda **/
            static Token lambda() { return Token(tokentype::tk_lambda); }
            /** token representing keyword @c if **/
            static Token if_token() { return Token(tokentype::tk_if); }
            /** token representing keyword @c else **/
            static Token else_token() { return Token(tokentype::tk_else); }
            /** token representing keyword @c let **/
            static Token let() { return Token(tokentype::tk_let); }
            /** token representing keyword @c in **/
            static Token in() { return Token(tokentype::tk_in); }
            /** token representing keyword @c end **/
            static Token end() { return Token(tokentype::tk_end); }

            ///@}

            /** @defgroup token-access-methods **/
            ///@{

            tokentype tk_type() const { return tk_type_; }
            const std::string & text() const { return text_; }

            ///@}

            /** @defgroup token-general-methods **/
            ///@{

            /** true if token understood to represent valid input
             *  i.e. any token type except @c tk_invalid
             **/
            bool is_valid() const { return tk_type_ != tokentype::tk_invalid; }
            /** true for sentinel token with type tk_invalid **/
            bool is_invalid() const { return tk_type_ == tokentype::tk_invalid; }

            /** true for tokens with variable text.  false for those with fixed textual representation **/
            bool has_variable_text() const { return (tk_type_ == tokentype::tk_i64
                                                     || tk_type_ == tokentype::tk_f64
                                                     || tk_type_ == tokentype::tk_string
                                                     || tk_type_ == tokentype::tk_symbol); }

            /** expect input matching @c true or @c false **/
            bool bool_value() const;

            /** expect input matching @c [+|-][0-9][0-9]* **/
            std::int64_t i64_value() const;

            /** expect input matching @c [+|-][0-9]*[.][0-9]*[e|E][+|-][0-9]* **/
            double f64_value() const;

            /** print human-readable token representation on stream @p os **/
            void print(std::ostream & os) const;

            ///@}

        private:
            /** @defgroup token-instance-vars **/
            ///@{

            /** category for this token **/
            tokentype tk_type_ = tokentype::tk_invalid;

            /** characters comprising this token.
             *  only provided for certain token types:
             *
             *    tk_i64
             *    tk_f64
             *    tk_string
             *    tk_symbol
             **/
            std::string text_;

            ///@}
        };

        inline std::ostream &
        operator<< (std::ostream & os,
                    const Token & tk)
        {
            tk.print(os);
            return os;
        }
    } /*namespace scm*/

#ifndef ppdetail_atomic
    namespace print {
        PPDETAIL_ATOMIC(xo::scm::token<char>);
    }
#endif

} /*namespace xo*/

/* end Token.hpp */
