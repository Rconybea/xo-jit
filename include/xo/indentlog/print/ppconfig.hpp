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
            /** @defgroup ppconfig-instance-vars ppconfig instance variables **/
            ///@{

            /** max line length.
             *  Pretty-printer will introduce newlines if needed
             *  to stay to the left of this margin
             **/
            std::uint32_t right_margin_ = 135;

            /** amount of additional indent per nesting level **/
            std::uint32_t indent_width_ = 2;

            ///@}
        };
    } /*namespace print*/
} /*namespace xo*/
