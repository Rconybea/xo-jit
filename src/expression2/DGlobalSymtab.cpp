/** @file DGlobalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DGlobalSymtab.hpp"
#include "DUniqueString.hpp"
#include <xo/indentlog/scope.hpp>

namespace xo {
    namespace scm {

        Binding
        DGlobalSymtab::lookup_binding(const DUniqueString * sym) const noexcept
        {
            (void)sym;

            scope log(XO_DEBUG(true), "stub");
            log && log(xtag("sym", std::string_view(*sym)));

            return Binding();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.cpp */
