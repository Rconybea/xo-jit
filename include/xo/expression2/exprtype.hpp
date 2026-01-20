/** @file exprtype.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include <ostream>
//#include <cstdint>

namespace xo {
    namespace scm {
        /** @enum exprtype
         *  @brief enum to identify subclasses of xo::scm::Expression.
         *
         **/
        enum class exprtype {
            /** sentinel value **/
            invalid = -1,

            /** literal constant.  must satisfy both standard_layout_type + trivial **/
            constant,
#ifdef NOT_YET
            /** a literal constant that refers to a linkable named function **/
            primitive,
#endif
            /** variable/function definition **/
            define,
#ifdef NOT_YET
            /** variable assignment **/
            assign,
            /** function call **/
            apply,
            /** function definition **/
            lambda,
#endif
            /** variable reference **/
            variable,
#ifdef NOT_YET
            /** if-then-else **/
            ifexpr,
            /** sequence **/
            sequence,
            /** type conversion **/
            convert,
#endif

            /** not an expression.  comes last, counts entries **/
            N
        };

        inline const char *
        expr2str(exprtype x)
        {
            switch(x) {
            case exprtype::invalid: return "?exprtype";
            case exprtype::constant: return "constant";
#ifdef NOT_YET
            case exprtype::primitive: return "primitive";
#endif
            case exprtype::define: return "define";
#ifdef NOT_YET
            case exprtype::assign: return "assign";
            case exprtype::apply: return "apply";
            case exprtype::lambda: return "lambda";
            case exprtype::variable: return "variable";
            case exprtype::ifexpr: return "if_expr";
            case exprtype::sequence: return "sequence";
            case exprtype::convert: return "convert";
#endif
            default: break;
            }

            return "???exprtype???";
        }

        /** @brief number of built-in expression types,  repr convenient for array sizing **/
        static constexpr std::size_t n_exprtype = static_cast<std::size_t>(exprtype::N);

        inline std::ostream &
        operator<<(std::ostream & os,
                   exprtype x)
        {
            os << expr2str(x);
            return os;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end exprtype.hpp */
