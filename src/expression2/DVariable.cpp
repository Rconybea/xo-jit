/** @file DVariable.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DVariable.hpp"
#include "exprtype.hpp"

namespace xo {
    namespace scm {
        void
        DVariable::assign_valuetype(TypeDescr td) noexcept
        {
            typeref_.resolve(td);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DVariable.cpp */
