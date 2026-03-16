/** @file interpreter2_register_primitives.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "interpreter2_register_primitives.hpp"
#include "VsmPrimitives.hpp"

namespace xo {
    using xo::mm::AAllocator;

    namespace scm {
        /** TODO: MOVE THESE install_aux() remplates SOMEWHERE COMMON **/

        template <typename PrimitiveRepr>
        bool install_aux(InstallSink sink,
                         PrimitiveRepr * pm,
                         InstallFlags flags)
        {
            scope log(XO_DEBUG(true));

            if ((flags & InstallFlags::f_generalpurpose) == InstallFlags::f_generalpurpose) {
                log && log("create primitive", xtag("name", pm->name()));

                return sink(pm->name(),
                            pm->fn_td(),
                            obj<AProcedure,PrimitiveRepr>(pm),
                            flags);
            } else {
                log && log("skip primitive", xtag("name", pm->name()));

                return true;
            }
        }

        template <typename Primitive>
        bool install_aux(InstallSink sink,
                         obj<AAllocator> mm,
                         std::string_view name,
                         typename Primitive::FunctionPtrType impl,
                         InstallFlags flags)
        {
            if (flags != InstallFlags::f_none) {
                auto pm
                    = Primitive::_make(mm, name, impl);

                return install_aux(sink, pm, flags);
            } else {
                return true;
            }
        }

        bool
        interpreter2_register_primitives(obj<ARuntimeContext> rcx,
                                         //obj<xo::mm::AAllocator> mm,
                                         //StringTable * stbl,
                                         InstallSink sink,
                                         InstallFlags flags)
        {
            scope log(XO_DEBUG(true));

            obj<AAllocator> mm = rcx.allocator();
            StringTable * stbl = rcx.stringtable();

            bool ok = true;

            ok = ok & install_aux(sink,
                                  VsmPrimitives::make_report_memory_use_pm(mm, stbl),
                                  flags);

            return ok;
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end interpreter2_register_primitives.cpp **/
