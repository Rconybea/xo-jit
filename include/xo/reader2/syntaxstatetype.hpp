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
