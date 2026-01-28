/** @file syntaxstatetype.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "syntaxstatetype.hpp"

namespace xo {
    namespace scm {

        const char *
        syntaxstatetype_descr(syntaxstatetype x) {
            switch (x) {
            case syntaxstatetype::invalid:
                break;
            case syntaxstatetype::expect_toplevel_expression_sequence:
                return "expect-toplevel-expression-sequence";
            case syntaxstatetype::expect_symbol:
                return "expect-symbol";
            case syntaxstatetype::expect_type:
                return "expect-type";
            case syntaxstatetype::expect_rhs_expression:
                return "expect-rhs-expression";
            case syntaxstatetype::defexpr:
                return "defexpr";
            case syntaxstatetype::lambdaexpr:
                return "lambdaexpr";
            case syntaxstatetype::ifelseexpr:
                return "ifelseexpr";
            case syntaxstatetype::progress:
                return "progress";
            case syntaxstatetype::N:
                break;
            }

            return "syntaxstatetype?";
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end syntaxstatetype.cpp */
