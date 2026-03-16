/** @file SetupInterpreter2.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "SetupInterpreter2.hpp"
#include "VsmPrimitives.hpp"
#include "DPrimitive_gco_2_gco_gco.hpp"
#include "VsmDefContFrame.hpp"
#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"
#include "VsmApplyClosureFrame.hpp"
#include "VsmIfElseContFrame.hpp"
#include "VsmSeqContFrame.hpp"
#include "Primitive_gco_2_gco_gco.hpp"
#include "Closure.hpp"
#include "GlobalEnv.hpp"
#include "LocalEnv.hpp"
#include "VsmRcx.hpp"

#include <xo/printable2/detail/APrintable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AAllocator;
    using xo::mm::AGCObject;
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::impl_for;
    using xo::reflect::typeseq;
    using xo::xtag;

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
        SetupInterpreter2::register_primitives(obj<ARuntimeContext> rcx,
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

        bool
        SetupInterpreter2::register_facets()
        {
            scope log(XO_DEBUG(true));


            // VsmStqackFrame
            // +- VsmApplyFrame
            // +- VsmEvalArgsFrame
            // +- VsmApplyClosureFrame
            // +- VsmDefContFrame
            // +- VsmIfElseContFrame
            // \- VsmSeqContFrame

            FacetRegistry::register_impl<AGCObject, DVsmApplyFrame>();
            FacetRegistry::register_impl<APrintable, DVsmApplyFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmEvalArgsFrame>();
            FacetRegistry::register_impl<APrintable, DVsmEvalArgsFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmApplyClosureFrame>();
            FacetRegistry::register_impl<APrintable, DVsmApplyClosureFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmDefContFrame>();
            FacetRegistry::register_impl<APrintable, DVsmDefContFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmIfElseContFrame>();
            FacetRegistry::register_impl<APrintable, DVsmIfElseContFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmSeqContFrame>();
            FacetRegistry::register_impl<APrintable, DVsmSeqContFrame>();

            // LocalEnv (see xo-reader2/ for GlobalEnv)

            FacetRegistry::register_impl<AGCObject, DLocalEnv>();
            FacetRegistry::register_impl<APrintable, DLocalEnv>();

            // Procedure
            // \- Primitive_gco_2_gco_gco

            FacetRegistry::register_impl<AProcedure, DPrimitive_gco_2_gco_gco>();
            FacetRegistry::register_impl<AGCObject, DPrimitive_gco_2_gco_gco>();
            FacetRegistry::register_impl<APrintable, DPrimitive_gco_2_gco_gco>();

            // Closure

//            FacetRegistry::register_impl<AProcedure, DClosure>();  // if/when provided
            FacetRegistry::register_impl<AGCObject, DClosure>();
            FacetRegistry::register_impl<APrintable, DClosure>();


            // RuntimeContext
            // \- VsmRcx

            FacetRegistry::register_impl<ARuntimeContext, DVsmRcx>();

            log && log(xtag("DVsmApplyFrame.tseq", typeseq::id<DVsmApplyFrame>()));
            log && log(xtag("DVsmEvalArgsFrame.tseq", typeseq::id<DVsmEvalArgsFrame>()));
            log && log(xtag("DVsmApplyClosureFrame.tseq", typeseq::id<DVsmApplyClosureFrame>()));
            log && log(xtag("DVsmDefContFrame.tseq", typeseq::id<DVsmDefContFrame>()));
            log && log(xtag("DVsmDefContFrame.tseq", typeseq::id<DVsmDefContFrame>()));
            log && log(xtag("DVsmIfElseContFrame.tseq", typeseq::id<DVsmIfElseContFrame>()));
            log && log(xtag("DVsmSeqContFrame.tseq", typeseq::id<DVsmSeqContFrame>()));
            log && log(xtag("DClosure.tseq", typeseq::id<DClosure>()));
            log && log(xtag("DGlobalEnv.tseq", typeseq::id<DGlobalEnv>()));
            log && log(xtag("DLocalEnv.tseq", typeseq::id<DLocalEnv>()));
            log && log(xtag("DVsmRcx.tseq", typeseq::id<DVsmRcx>()));

            return true;
        }

        bool
        SetupInterpreter2::register_types(obj<ACollector> gc)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            ok &= gc.install_type(impl_for<AGCObject, DVsmApplyFrame>());
            ok &= gc.install_type(impl_for<AGCObject, DVsmEvalArgsFrame>());

            return ok;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end SetupInterpreter2.cpp */
