/* @file ppconfig.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include <cstdint>

namespace xo {
    namespace print {

        /** @class ppconfig
         *  @brief hold pretty-printer control parameters
         *
         *  Need one read-only instance of this to invoke pretty printer
         **/
        struct ppconfig {
            /** @defgroup ppconfig-ctors ppconfig constructors **/
            ///@{

            /** config to use pretty printer in degenerate form:
             *  In this form right margin is absurdly far away,
             *  so should not be forced to use multiple lines.
             *
             *  Note this won't prevent a printer from returning -1
             **/
            static inline ppconfig ugly() {
                return ppconfig { .right_margin_ = 99999999, .indent_width_ = 0, .assert_indent_threshold = 10 };
            }

            ///@}

            /** @defgroup ppconfig-instance-vars ppconfig instance variables **/
            ///@{

            /** max line length.
             *  Pretty-printer will introduce newlines if needed
             *  to stay to the left of this margin
             **/
            std::uint32_t right_margin_ = 80;

            /** amount of additional indent per nesting level **/
            std::uint32_t indent_width_ = 2;

            /** assert if attempting this much indent **/
            std::uint32_t assert_indent_threshold = 10000;

            ///@}
        };
    } /*namespace print*/
} /*namespace xo*/
