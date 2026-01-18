/* file tokentype.cpp
 *
 * author: Roland Conybeare
 */

#include "tokentype.hpp"

namespace xo {
    namespace scm {
        char const *
        tokentype_descr(tokentype tk_type)
        {
#define CASE(x) case tokentype::x: return STRINGIFY(x)

            switch(tk_type) {
                CASE(tk_bool);
                CASE(tk_i64);
                CASE(tk_f64);
                CASE(tk_string);
                CASE(tk_symbol);
                CASE(tk_leftparen);

                CASE(tk_rightparen);
                CASE(tk_leftbracket);
                CASE(tk_rightbracket);
                CASE(tk_leftbrace);
                CASE(tk_rightbrace);

                CASE(tk_leftangle);
                CASE(tk_rightangle);
                CASE(tk_lessequal);
                CASE(tk_greatequal);
                CASE(tk_dot);
                CASE(tk_comma);
                CASE(tk_colon);

                CASE(tk_doublecolon);
                CASE(tk_semicolon);
                CASE(tk_singleassign);
                CASE(tk_assign);
                CASE(tk_yields);

                CASE(tk_plus);
                CASE(tk_minus);
                CASE(tk_star);
                CASE(tk_slash);

                CASE(tk_cmpeq);
                CASE(tk_cmpne);

                CASE(tk_type);
                CASE(tk_def);
                CASE(tk_lambda);
                CASE(tk_if);
                CASE(tk_then);
                CASE(tk_else);
                CASE(tk_let);

                CASE(tk_in);
                CASE(tk_end);

            case tokentype::tk_invalid:
            case tokentype::N:
                return "?tokentype";
            }

#undef CASE

            return "???";
        } /*tokentype_descr*/
    } /*namespace scm*/
} /*namespace xo*/

/* end tokentype.cpp */
