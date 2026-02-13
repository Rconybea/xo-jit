/** @file interpreter2_register_facets.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "interpreter2_register_facets.hpp"

#include "DPrimitive_gco_2_gco_gco.hpp"
#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"
#include "VsmApplyClosureFrame.hpp"
#include "VsmSeqContFrame.hpp"
#include "Primitive_gco_2_gco_gco.hpp"
#include "Closure.hpp"
#include "LocalEnv.hpp"
#include "VsmRcx.hpp"

#include <xo/printable2/detail/APrintable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;
    using xo::xtag;

    namespace scm {
        bool
        interpreter2_register_facets()
        {
            scope log(XO_DEBUG(true));


            // VsmStqackFrame
            // +- VsmApplyFrame
            // +- VsmEvalArgsFrame
            // +- VsmApplyClosureFrame
            // \- VsmSeqContFrame

            FacetRegistry::register_impl<AGCObject, DVsmApplyFrame>();
            FacetRegistry::register_impl<APrintable, DVsmApplyFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmEvalArgsFrame>();
            FacetRegistry::register_impl<APrintable, DVsmEvalArgsFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmApplyClosureFrame>();
            FacetRegistry::register_impl<APrintable, DVsmApplyClosureFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmSeqContFrame>();
            FacetRegistry::register_impl<APrintable, DVsmSeqContFrame>();

            // LocalEnv

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
            log && log(xtag("DVsmSeqContFrame.tseq", typeseq::id<DVsmSeqContFrame>()));
            log && log(xtag("DClosure.tseq", typeseq::id<DClosure>()));
            log && log(xtag("DLocalEnv.tseq", typeseq::id<DLocalEnv>()));
            log && log(xtag("DVsmRcx.tseq", typeseq::id<DVsmRcx>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end interpreter2_register_facets.cpp */
