/** @file SetupNumeric.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "SetupNumeric.hpp"
#include "NumericDispatch.hpp"
#include "Numeric.hpp"
#include "NumericPrimitives.hpp"
#include "NumericDispatch.hpp"

#include "FloatIntegerOps.hpp"
#include "FloatOps.hpp"
#include "float/INumeric_DFloat.hpp"

#include "IntegerOps.hpp"
#include "integer/INumeric_DInteger.hpp"

#include <xo/procedure2/Primitive_gco_2_gco_gco.hpp>

#include <xo/object2/DFloat.hpp>
#include <xo/object2/Integer.hpp>

#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AAllocator;
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;

    namespace scm {
        bool
        SetupNumeric::register_facets()
        {
            scope log(XO_DEBUG(true));

            FacetRegistry::register_impl<ANumeric, DInteger>();

            NumericDispatch::instance().register_impl<DFloat, DFloat>
                (&FloatOps::multiply,
                 &FloatOps::divide,
                 &FloatOps::add,
                 &FloatOps::subtract,
                 &FloatOps::cmp_equal,
                 &FloatOps::cmp_notequal,
                 &FloatOps::cmp_less,
                 &FloatOps::cmp_lessequal,
                 &FloatOps::cmp_greater,
                 &FloatOps::cmp_greatequal);

            NumericDispatch::instance().register_impl<DFloat, DInteger>
                (&FloatIntegerOps::multiply,
                 &FloatIntegerOps::divide,
                 &FloatIntegerOps::add,
                 &FloatIntegerOps::subtract,
                 &FloatIntegerOps::cmp_equal,
                 &FloatIntegerOps::cmp_notequal,
                 &FloatIntegerOps::cmp_less,
                 &FloatIntegerOps::cmp_lessequal,
                 &FloatIntegerOps::cmp_greater,
                 &FloatIntegerOps::cmp_greatequal);

            NumericDispatch::instance().register_impl<DInteger, DFloat>
                (&IntegerFloatOps::multiply,
                 &IntegerFloatOps::divide,
                 &IntegerFloatOps::add,
                 &IntegerFloatOps::subtract,
                 &IntegerFloatOps::cmp_equal,
                 &IntegerFloatOps::cmp_notequal,
                 &IntegerFloatOps::cmp_less,
                 &IntegerFloatOps::cmp_lessequal,
                 &IntegerFloatOps::cmp_greater,
                 &IntegerFloatOps::cmp_greatequal);

            NumericDispatch::instance().register_impl<DInteger, DInteger>
                (&IntegerOps::multiply,
                 &IntegerOps::divide,
                 &IntegerOps::add,
                 &IntegerOps::subtract,
                 &IntegerOps::cmp_equal,
                 &IntegerOps::cmp_notequal,
                 &IntegerOps::cmp_less,
                 &IntegerOps::cmp_lessequal,
                 &IntegerOps::cmp_greater,
                 &IntegerOps::cmp_greatequal);

            log && log(xtag("ANumeric.tseq", typeseq::id<ANumeric>()));

            return true;
        }

        bool
        SetupNumeric::register_primitives(obj<ARuntimeContext> rcx,
                                          InstallSink sink,
                                          InstallFlags flags)
        {
            obj<AAllocator> mm = rcx.allocator();
            StringTable * stbl = rcx.stringtable();

            scope log(XO_DEBUG(false));

            bool ok = true;

            /* "_mul" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_multiply_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_div" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_divide_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_add" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_add_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_sub" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_subtract_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_cmpeq" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_cmpeq_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_cmpne" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_cmpne_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_cmplt" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_cmplt_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_cmple" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_cmple_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_cmpgt" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_cmpgt_pm(mm, stbl),
                        flags & InstallFlags::f_essential));
            /* "_cmpge" */
            ok = ok & (PrimitiveRegistry::install_aux
                       (sink,
                        NumericPrimitives::make_cmpge_pm(mm, stbl),
                        flags & InstallFlags::f_essential));

            return ok;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end SetupNumeric.cpp */
