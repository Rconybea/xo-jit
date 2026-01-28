/** @file syntaxstatetype.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <ostream>

namespace xo {
    namespace scm {
        /** @enum syntaxstatemachine
         *  @brief Label a specialized parsing state machine
         *
         *  Label for a schematika syntax state machine
         *  dedicated to some particular piece of syntax
         **/
        enum class syntaxstatetype {
            invalid = -1,

            /** toplevel of some translation unit. See @ref DExprSeqState **/
            expect_toplevel_expression_sequence,

            /** expecting a formal argument list (sub-syntax within lambda-expression) **/
            expect_formal_arglist,

            /** expecting a formal argument (sub-syntax within formal-arglist) **/
            expect_formal_arg,

            /** expecting a s symbol. See @ref DExpectSymbolSsm **/
            expect_symbol,

            /** expecting a type. See @ref DExpectTypeSsm **/
            expect_type,

            /** expecting a rhs expression. See @ref DExpectExprSsm **/
            expect_rhs_expression,

            /** handle define-expression. See @ref DDefineSsm **/
            defexpr,

            /** handle lambda-expression. See @ref DLambdaSsm **/
            lambdaexpr,

            /** handle ifelse-expression. See @ref DIfElseSsm **/
            ifelseexpr,

            /** rhs expression.  state exists to achieve 1-token lookahead **/
            progress,

            /** comes lasts, counts number of valid enums **/
            N
        };

        const char * syntaxstatetype_descr(syntaxstatetype x);

        inline std::ostream &
        operator<< (std::ostream & os, syntaxstatetype x) {
            os << syntaxstatetype_descr(x);
            return os;
        }
    }
} /*namespace xo*/

/* end syntaxstatetype.hpp */
