/** @file scan_result.cpp
 *
 *  @author Roland Conybeare, 2025
 **/

#include "scan_result.hpp"

namespace xo {
    namespace scm {
        scan_result
        scan_result::make_whitespace(const span_type& whitespace_input)
        {
            return scan_result(token_type::invalid(), whitespace_input /*consumed*/);
        }

        scan_result
        scan_result::make_partial(const span_type& prefix_input)
        {
            return scan_result(token_type::invalid(), prefix_input /*consumed*/);
        }

        scan_result
        scan_result::make_error_consume_current_line(const char * error_src,
                                                     std::string error_msg,
                                                     size_t error_pos,
                                                     input_state_type & input_state_ref)
        {
            /* report+consume entire input line */

            /* copy before altered by .consume_current_line() */
            input_state_type input_state_copy = input_state_ref;

            return scan_result(token_type::invalid(),
                               input_state_ref.consume_current_line(),
                               error_type(error_src,
                                          error_msg,
                                          input_state_copy,
                                          error_pos));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end scan_result.cpp */
