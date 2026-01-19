/** @file reader2_register_types.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "reader2_register_types.hpp"

#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::ACollector;
    using xo::scope;

    namespace scm {
        bool
        reader2_register_types(obj<ACollector> /*gc*/)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            /* no gc-aware types yet; scaffold for future use */

            return ok;
        }
    }
} /*namespace xo*/

/* end reader2_register_types.cpp */
