/** @file tokentype.hpp
 *
 *  author: Roland Conybeare, Jul 2024
 **/

#pragma once

#include "xo/indentlog/print/tag.hpp" // for STRINGIFY
#include "xo/indentlog/print/ppdetail_atomic.hpp"
#include <ostream>

namespace xo {
    namespace scm {
        /** @enum tokentype
         *  Enum to identify different schematika input token types
         *
         *  Schematica code examples:
         *
         *  @code
         *    type point :: { xcoord : f64, ycoord : f64 };
         *    type matrix :: array<double, 2>;  // 2-d array
         *
         *    decl hypot(x : f64, y : f64) -> f64;
         *
         *    def hypot(x : f64, y : f64) {
         *      let
         *        x2 = (x * x);
         *        y2 = (y * y);
         *        hypot2 = (x2 + y2);
         *      in
         *        sqrt(hypot2);
         *    };
         *
         *    def someconst 4;
         *
         *    def foo(v : vec<i32>) {
         *      def (pi : f64) = 3.1415926;
         *      def (h : (f64,f64) -> f64) = hypot;
         *
         *      h = hypot3;
         *    };
         *
         *    def matrixproduct(x : matrix, y : matrix) {
         *      [i, j : x.row(i) * y.col(j)];
         *    };
         *  @endcode
         **/
        enum class tokentype {
            /** sentinel value **/
            tk_invalid = -1,

            /** a boolean constant **/
            tk_bool,

            /** an integer constant (signed 64-bit integer) **/
            tk_i64,

            /** a 64-bit floating-point constant **/
            tk_f64,

            /** a string literal **/
            tk_string,

            /** a symbol **/
            tk_symbol,

            /** left-hand parenthesis @c '(' **/
            tk_leftparen,

            /** right-hand parenthesis @c ')' **/
            tk_rightparen,

            /** left-hand bracket @c '[' **/
            tk_leftbracket,

            /** right-hand bracket @c ']' **/
            tk_rightbracket,

            /** left-hand brace @c '{' **/
            tk_leftbrace,

            /** right-hand brace @c '}' **/
            tk_rightbrace,

            /** left-hand angle bracket @c '<' **/
            tk_leftangle,

            /** right-hand angle bracket @c '>' **/
            tk_rightangle,

            /** less-equal @c '<=' **/
            tk_lessequal,

            /** great-equal @c '>=' **/
            tk_greatequal,

            /** dot @c '.' **/
            tk_dot,

            /** comma @c ',' **/
            tk_comma,

            /** colon @c ':' **/
            tk_colon,

            /** double-colon @c '::' **/
            tk_doublecolon,

            /** semi-colon @c ';' **/
            tk_semicolon,

            /** single equals sign @c '=' **/
            tk_singleassign,

            /** assignment @c ':=' **/
            tk_assign,

            /** indirection @c '->' **/
            tk_yields,

            /** note: operators not treated as punctuation
             *  'do-always' is a legal variable name,
             *  as is 'maybe*2', 'maybe+1', 'path/to/foo'
             **/

            /** operator @c '+' **/
            tk_plus,
            /** operator @c '-' **/
            tk_minus,
            /** operator @c '*' **/
            tk_star,
            /** operator @c '/' **/
            tk_slash,

            /** operator @c '==' **/
            tk_cmpeq,
            /** operator @c '!=' **/
            tk_cmpne,

            /** keyword @c 'type' **/
            tk_type,

            /** keyword @c 'def' **/
            tk_def,

            /** keyword @c 'lambda' **/
            tk_lambda,

            /** keyword @c 'if' **/
            tk_if,

            /** keyworkd @c 'then' **/
            tk_then,

            /** keyword @c 'else' **/
            tk_else,

            /** keyword @c 'let' **/
            tk_let,

            /** keyword @c 'in' **/
            tk_in,

            /** keyword @c 'end' **/
            tk_end,

            /** comes last, counts number of entries **/
            N
        }; /*tokentype*/

        /** String representation for enum value.
         *  For example @c tokentype_descr(tokentype::tk_if) -> @c "if"
         **/
        extern char const *
        tokentype_descr(tokentype tk_type);

        /** Print enum value for @p tk_type on stream @p os **/
        inline std::ostream &
        operator<< (std::ostream & os, tokentype tk_type) {
            os << tokentype_descr(tk_type);
            return os;
        }
    } /*namespace scm*/

#ifndef ppdetail_atomic
    namespace print {
        PPDETAIL_ATOMIC(xo::scm::tokentype);
    } /*namespace print*/
#endif
} /*namespace xo*/

/* end tokentype.hpp */
