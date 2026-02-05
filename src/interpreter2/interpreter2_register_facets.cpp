/** @file interpreter2_register_facets.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "interpreter2_register_facets.hpp"

#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"

#include "Closure.hpp"

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

            // VsmStackFrame
            // +- VsmApplyFrame
            // \- VsmEvalArgsFrame

            FacetRegistry::register_impl<AGCObject, DVsmApplyFrame>();
            FacetRegistry::register_impl<APrintable, DVsmApplyFrame>();

            FacetRegistry::register_impl<AGCObject, DVsmEvalArgsFrame>();
            FacetRegistry::register_impl<APrintable, DVsmEvalArgsFrame>();

            // Procedure
            // +- Primitive_gco_2_gco_gco
            // \- Closure

            FacetRegistry::register_impl<AProcedure, DClosure>();

            log && log(xtag("DVsmApplyFrame.tseq", typeseq::id<DVsmApplyFrame>()));
            log && log(xtag("DVsmEvalArgsFrame.tseq", typeseq::id<DVsmEvalArgsFrame>()));
            log && log(xtag("DClosure.tseq", typeseq::id<DClosure>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end interpreter2_register_facets.cpp */
