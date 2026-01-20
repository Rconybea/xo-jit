/** @file DLocalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DLocalSymtab.hpp"
#include "DUniqueString.hpp"
#include <xo/indentlog/scope.hpp>

namespace xo {
    namespace scm {

        Binding
        DLocalSymtab::lookup_binding(const DUniqueString * sym) const noexcept
        {
            scope log(XO_DEBUG(true), "stub impl");
            log && log(xtag("sym", std::string_view(*sym)));

            return Binding();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.cpp */
