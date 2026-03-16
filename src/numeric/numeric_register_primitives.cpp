/** @file numeric_register_primitives.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "numeric_register_primitives.hpp"
#include "NumericPrimitives.hpp"
#include "NumericDispatch.hpp"
#include <xo/procedure2/Primitive_gco_2_gco_gco.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AAllocator;
    using xo::scope;

    namespace scm {
        namespace {
            bool install_aux(InstallSink sink,
                             DPrimitive_gco_2_gco_gco * pm,
                             InstallFlags flags)
            {
                if (flags != InstallFlags::f_none) {
                    return sink(pm->name(),
                                pm->fn_td(),
                                obj<AProcedure,DPrimitive_gco_2_gco_gco>(pm),
                                flags);
                } else {
                    return true;
                }
            }

            bool install_aux(InstallSink sink,
                             obj<AAllocator> mm,
                             std::string_view name,
                             obj<AGCObject> (*impl)(obj<ARuntimeContext> rcx,
                                                    obj<AGCObject> x,
                                                    obj<AGCObject> y),
                             InstallFlags flags)
            {
                if (flags != InstallFlags::f_none) {
                    auto pm
                        = DPrimitive_gco_2_gco_gco::_make(mm, name, impl);

                    return install_aux(sink, pm, flags);
                } else {
                    return true;
                }
            }
        }

        bool
        numeric_register_primitives(obj<AAllocator> mm, StringTable * stbl,
                                    InstallSink sink, InstallFlags flags)
        {
            (void)stbl;

            scope log(XO_DEBUG(true));

            bool ok = true;

            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_multiply_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_divide_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_add_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_subtract_pm(mm),
                                  flags & InstallFlags::f_essential);

            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_cmpeq_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_cmpne_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_cmplt_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink,
                                  NumericPrimitives::make_cmple_pm(mm),
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink, mm, "_cmpgt", &NumericDispatch::cmp_greater,
                                  flags & InstallFlags::f_essential);
            ok = ok & install_aux(sink, mm, "_cmpge", &NumericDispatch::cmp_greatequal,
                                  flags & InstallFlags::f_essential);

            return ok;
        }
    }
} /*namespace xo*/

/* end numeric_register_primitives.cpp */
