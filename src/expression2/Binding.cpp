/** @file Binding.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "Binding.hpp"
#include <cassert>

namespace xo {
    namespace scm {

        Binding
        Binding::relative(int32_t i_link, Binding def)
        {
            if (def.i_link_ == Binding::c_link_global) {
                // for globally defined vars, i_link always -1
                return def;
            } else if (def.i_link_ >= 0) {
                return Binding(i_link + def.i_link_, def.j_slot_);
            } else {
                assert(false);
                return Binding();
            }
        }

        void
        Binding::print(std::ostream & os) const
        {
            if (i_link_ == c_link_global) {
                os << "{path:global:" << j_slot_ << "}";
            } else if (i_link_ == c_link_sentinel) {
                os << "{path}";
            } else {
                os << "{path:" << i_link_ << ":" << j_slot_ << "}";
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end Binding.cpp */
