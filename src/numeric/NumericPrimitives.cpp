/** @file NumericPrimitives.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "NumericPrimitives.hpp"
#include "NumericDispatch.hpp"

namespace xo {
    using xo::mm::AAllocator;
    //using xo::mm::AGCObject;

    namespace scm {

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_multiply_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_mul",
                                                   &NumericDispatch::multiply);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_divide_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_div",
                                                   &NumericDispatch::divide);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_add_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_add",
                                                   &NumericDispatch::add);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_subtract_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_sub",
                                                   &NumericDispatch::subtract);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmpeq_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmpeq",
                                                   &NumericDispatch::cmp_equal);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmpne_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmpne",
                                                   &NumericDispatch::cmp_notequal);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmplt_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmplt",
                                                   &NumericDispatch::cmp_less);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmple_pm(obj<AAllocator> mm)
        {
            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmple",
                                                   &NumericDispatch::cmp_lessequal);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp */
