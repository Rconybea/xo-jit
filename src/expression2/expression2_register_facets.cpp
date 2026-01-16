/** @file expression2_register_facets.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "expression2_register_facets.hpp"

#include <xo/expression2/detail/IGCObject_DUniqueString.hpp>

#include <xo/gc/detail/AGCObject.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::facet::FacetRegistry;
    using xo::facet::typeseq;

    namespace scm {
        bool
        expression2_register_facets()
        {
            scope log(XO_DEBUG(true));

            FacetRegistry::register_impl<AGCObject, DUniqueString>();

            log && log(xtag("DUniqueString.tseq", typeseq::id<DUniqueString>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end expression2_register_facets.cpp */
