/** @file NumericPrimitives.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "NumericPrimitives.hpp"
#include "NumericDispatch.hpp"

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_div_gco_gco_pm("_div",
                                            &NumericDispatch::divide);


    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp */
