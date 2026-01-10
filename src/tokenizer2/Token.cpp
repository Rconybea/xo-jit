/** @file token.cpp
 *
 *  author: Roland Conybeare
 **/

#include "Token.hpp"
#include "xo/indentlog/print/tag.hpp"

namespace xo {
    namespace scm {

        bool
        Token::bool_value() const
        {
            if (tk_type_ != tokentype::tk_bool) {
                throw (std::runtime_error
                       (tostr("token::bool_value",
                              ": token with type tk found where tk_bool expected",
                              xtag("tk", tk_type_))));
            }

            if (text_ == "true")
                return true;
            if (text_ == "false")
                return false;

            throw (std::runtime_error
                   (tostr("token::bool_value",
                          ": unexpected input string tk_bool token",
                          xtag("text", text_))));

            return false;
        }

        std::int64_t
        Token::i64_value() const
        {
            if (tk_type_ != tokentype::tk_i64) {
                throw (std::runtime_error
                       (tostr("token::i64_value",
                              ": token with type tk found where tk_i64 expected",
                              xtag("tk", tk_type_))));
            }

            if (text_.empty()) {
                throw (std::runtime_error
                       (tostr("token::i64_value",
                              ": unexpected empty input string for tk_i64 token")));
            }

            int sign = 1;
            int value = 0;
            {
                auto ix = text_.begin();
                auto end_ix = text_.end();

                char ch = *ix;

                if (ch == '+') {
                    ++ix;
                } else if (ch == '-') {
                    sign = -1;
                    ++ix;
                }

                if (ix == end_ix) {
                    throw (std::runtime_error
                           (tostr("token::i64_value",
                                  ": input text found where at least one digit expected",
                                  xtag("text", text_))));
                }

                for (; ix != end_ix; ++ix) {
                    char ch = *ix;

                    if ((ch >= '0') && (ch <= '9')) {
                        value *= 10;
                        value += (ch - '0');
                    } else {
                        throw (std::runtime_error
                               (tostr("token::i64_value",
                                      ": unexpected char ch in integer token",
                                      xtag("ch", ch))));
                    }
                }
            }

            return sign * value;
        } /*i64_value*/

        double
        Token::f64_value() const
        {
            if (tk_type_ != tokentype::tk_f64) {
                throw (std::runtime_error
                       (tostr("token::f64_value",
                              ": token with type tk found where tk_f64 expected",
                              xtag("tk", tk_type_))));
            }

            if (text_.empty()) {
                throw (std::runtime_error
                       (tostr("token::f64_value",
                              ": unexpected empty input string for tk_f64 token")));
            }

            int sign = 1;
            /* integer representing denormalized unsigned mantissa
             * (mantissa scaled by smallest power of 10 sufficient to make
             *  it an integer)
             */
            std::int64_t mantissa = 0;
            /* counts #of digits to the right of decimal point '.' */
            int rh_digits = 0;
            /* sign of exponent */
            int exp_sign = 1;
            /* value of exponenct = integer to the right of 'e' or 'E' */
            int exponent = 0;

            /* floating-point value will represent
             *   sign * mantissa * 10^(sign*exponent - rh_digits)
             */
            {
                auto ix = text_.begin();
                auto end_ix = text_.end();

                char ch = *ix;

                if (ch == '+') {
                    ++ix;
                } else if (ch == '-') {
                    sign = -1;
                    ++ix;
                }

                if (ix == end_ix) {
                    throw (std::runtime_error
                           (tostr("token::f64_value",
                                  ": input text found where at least one digit expected",
                                  xtag("text", text_))));
                }

                /* true iff decimal point '.' present in mantissa */
                bool have_decimal_point = false;
                /* true iff exponent prefix 'e' or 'E' present */
                //bool have_exponent = false;
                /* counts number of digits in mantissa
                 * (both before and after, but not including, any decimal point
                 */
                int m_digits = 0;
                /* digits to the left of decimal point */
                int lh_digits = 0;

                /* loop over mantissa digits */
                for (; ix != end_ix; ++ix) {
                    char ch = *ix;

                    if (ch == '.') {
                        if (have_decimal_point) {
                            throw (std::runtime_error
                                   (tostr("token::f64_value",
                                          ": input text found where at most one decimal point expected",
                                          xtag("text", text_))));
                        }

                        have_decimal_point = true;
                        lh_digits = m_digits;
                    } else if ((ch >= '0') && (ch <= '9')) {
                        mantissa *= 10;
                        mantissa += (ch - '0');
                        ++m_digits;
                    } else if (ch == 'e' || ch == 'E') {
                        //have_exponent = true;
                        break; // done with mantissa
                    } else {
                        throw (std::runtime_error
                               (tostr("token::i64_value",
                                      ": unexpected char ch in integer token",
                                      xtag("ch", ch))));
                    }
                }

                if (have_decimal_point)
                    rh_digits = m_digits - lh_digits;

                if (ix != end_ix) {
                    /* continue to read exponent */

                    /* skip e|E */
                    ++ix;

                    if (ix == end_ix) {
                        throw (std::runtime_error
                               (tostr("token::f64_value",
                                      ": on input text, expect at least one digit following exponent marker e|E",
                                      xtag("text", text_))));
                    }

                    char ch = *ix;

                    if (ch == '+') {
                        ++ix; /*skip*/
                    } else if (ch == '-') {
                        exp_sign = -1;
                        ++ix;
                    }

                    for (; ix != end_ix; ++ix) {
                        char ch = *ix;

                        if ((ch >= '0') && (ch <= '9')) {
                            exponent *= 10;
                            exponent += (ch - '0');
                        } else {
                            throw (std::runtime_error
                                   (tostr("token::f64_value",
                                          "; on input text, expect only digits following"
                                          " (possibly signed) exponenct marker",
                                          xtag("text", text_))));
                        }
                    }
                }
            }

            /* floating-point value will represent
             *   sign * mantissa * 10^(sign*exponent - rh_digits)
             */

            double mantissa_f64 = sign * mantissa;

#ifdef OBSOLETE_DEBUG
            std::cerr << xtag("text", text_)
                      << xtag("rh_digits", rh_digits)
                      << xtag("mantissa_f64", mantissa_f64)
                      << xtag("exp_sign", exp_sign)
                      << xtag("exponent", exponent)
                      << std::endl;
#endif

            double retval = (mantissa_f64
                             * detail::pow10((exp_sign * exponent)
                                             - rh_digits));

            return retval;
        } /*f64_value*/

        void
        Token::print(std::ostream & os) const
        {
            os << "<token"
               << xtag("type", tk_type_);
            if (has_variable_text())
                os << xtag("text", text_);
            os << ">";
        } /*print*/
    } /*namespace scm*/
} /*namespace xo*/

/* end token.cpp */
