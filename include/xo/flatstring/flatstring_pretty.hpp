/** @file flatstring_pretty.hpp
 *
 *  Author: Roland Conybeare, Jul 2025
 **/

#pragma once

#include "flatstring.hpp"
#include "flatstring_iostream.hpp"
#include "xo/indentlog/print/ppdetail_atomic.hpp"

namespace xo {
#ifndef ppdetail_atomic
    namespace print {
        struct ppindentinfo;

        template <std::size_t N>
        struct ppdetail<flatstring<N>> {
            static bool print_pretty(const ppindentinfo & ppii,
                                     const flatstring<N> & x) {
                return ppdetail_atomic<flatstring<N>>::print_pretty(ppii, x);
            };
        };
    }
#endif
}

/** end flatstring_pretty.hpp **/
