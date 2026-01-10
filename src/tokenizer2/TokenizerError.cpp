/** @file TokenizerError.cpp
 *
 *  @author Roland Conybeare, Jun 2025
 **/

#include "TokenizerError.hpp"

namespace xo {
    namespace scm {

        void
        TokenizerError::print(std::ostream & os) const
        {
            os << "<tokenizer-error"
               << xtag("src-function", src_function_)
               << xtag("message", error_description_)
               << xtag("input", input_state_.current_line())
               << xtag("whitespace", input_state_.whitespace())
               << xtag("error-pos", error_pos_)
               << ">";
        }

        void
        TokenizerError::report(std::ostream & os) const
        {
            using namespace std;

            if (!error_description_.empty()) {
                const char * prefix = "input: ";
                /* input_state.tk_start:    position of first character in token
                 * input_state.current_pos: position of first character following preceding token.
                 * error_pos:               position (relative to start) at which failure detected
                 */
                const size_t tk_start = input_state_.tk_start();
                const size_t tk_indent = (strlen(prefix) + tk_start);
                const size_t error_pos = 1 + tk_start + error_pos_;

                os << "token col: " << tk_start << ", error col: " << error_pos << "\n";
                os << prefix;
                for (const char *p = input_state_.current_line().lo(),
                         *e = input_state_.current_line().hi(); p < e; ++p)
                {
                    os << *p;
                }
                //os << endl;
                os << std::setw(tk_indent) << " ";

                for (size_t i = 0; i < error_pos_; ++i) {
                    os << '_';
                }
                os << '^' << endl;

                os << error_description_ << endl;
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end TokenizerError.cpp */
