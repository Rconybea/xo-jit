/** @file NumericPrimitives.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "NumericPrimitives.hpp"
#include "NumericDispatch.hpp"
#include <xo/type/AtomicType.hpp>
#include <xo/type/FunctionType.hpp>

namespace xo {
    using xo::mm::AAllocator;
    //using xo::mm::AGCObject;

    namespace scm {

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_multiply_pm(obj<AAllocator> mm,
                                            StringTable * stbl)
        {
            (void)stbl;

            // TODO: want to expand this to record schedule based on argument types.
            //
            // e.g. f64 x f64 -> f64

            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op+: numeric x numeric -> numeric
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, numeric_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_mul", pm_ty,
                                                   &NumericDispatch::multiply);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_divide_pm(obj<AAllocator> mm,
                                          StringTable * stbl)
        {
            (void)stbl;

            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op+: numeric x numeric -> numeric
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, numeric_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_div", pm_ty,
                                                   &NumericDispatch::divide);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_add_pm(obj<AAllocator> mm,
                                       StringTable * stbl)
        {
            (void)stbl;

            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op+: numeric x numeric -> numeric
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, numeric_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_add", pm_ty,
                                                   &NumericDispatch::add);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_subtract_pm(obj<AAllocator> mm,
                                            StringTable * stbl)
        {
            (void)stbl;

            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op+: numeric x numeric -> numeric
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, numeric_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_sub", pm_ty,
                                                   &NumericDispatch::subtract);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmpeq_pm(obj<AAllocator> mm,
                                         StringTable * stbl)
        {
            (void)stbl;

            auto booleic_ty = DAtomicType::make(mm, Metatype::t_booleic());
            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op==: numeric x numeric -> booleic
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, booleic_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmpeq", pm_ty,
                                                   &NumericDispatch::cmp_equal);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmpne_pm(obj<AAllocator> mm,
                                         StringTable * stbl)
        {
            (void)stbl;

            auto booleic_ty = DAtomicType::make(mm, Metatype::t_booleic());
            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op!=: numeric x numeric -> booleic
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, booleic_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmpne", pm_ty,
                                                   &NumericDispatch::cmp_notequal);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmplt_pm(obj<AAllocator> mm,
                                         StringTable * stbl)
        {
            (void)stbl;

            auto booleic_ty = DAtomicType::make(mm, Metatype::t_booleic());
            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op!=: numeric x numeric -> booleic
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, booleic_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmplt", pm_ty,
                                                   &NumericDispatch::cmp_less);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmple_pm(obj<AAllocator> mm,
                                         StringTable * stbl)
        {
            (void)stbl;

            auto booleic_ty = DAtomicType::make(mm, Metatype::t_booleic());
            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op!=: numeric x numeric -> booleic
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, booleic_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmple", pm_ty,
                                                   &NumericDispatch::cmp_lessequal);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmpgt_pm(obj<AAllocator> mm,
                                         StringTable * stbl)
        {
            (void)stbl;

            auto booleic_ty = DAtomicType::make(mm, Metatype::t_booleic());
            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op!=: numeric x numeric -> booleic
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, booleic_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmpgt", pm_ty,
                                                   &NumericDispatch::cmp_greater);
        }

        DPrimitive_gco_2_gco_gco *
        NumericPrimitives::make_cmpge_pm(obj<AAllocator> mm,
                                         StringTable * stbl)
        {
            (void)stbl;

            auto booleic_ty = DAtomicType::make(mm, Metatype::t_booleic());
            auto numeric_ty = DAtomicType::make(mm, Metatype::t_numeric());
            // #op!=: numeric x numeric -> booleic
            auto pm_ty = obj<AType,DFunctionType>
                (DFunctionType::_make(mm, booleic_ty, numeric_ty, numeric_ty));

            return DPrimitive_gco_2_gco_gco::_make(mm, "_cmpge", pm_ty,
                                                   &NumericDispatch::cmp_greatequal);
        }



    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp */
