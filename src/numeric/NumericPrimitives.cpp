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
        NumericPrimitives::s_mul_gco_gco_pm("_mul",
                                            &NumericDispatch::multiply);

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_div_gco_gco_pm("_div",
                                            &NumericDispatch::divide);

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_add_gco_gco_pm("_add",
                                            &NumericDispatch::add);

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_sub_gco_gco_pm("_sub",
                                            &NumericDispatch::subtract);

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_cmpeq_gco_gco_pm("_cmpeq",
                                              &NumericDispatch::cmp_equal);

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_cmpne_gco_gco_pm("_cmpne",
                                              &NumericDispatch::cmp_notequal);

        DPrimitive_gco_2_gco_gco
        NumericPrimitives::s_cmplt_gco_gco_pm("_cmplt",
                                              &NumericDispatch::cmp_less);


    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp */
