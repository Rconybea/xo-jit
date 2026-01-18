/** @file reader2_register_facets.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "reader2_register_facets.hpp"

#include <xo/reader2/ssm/ISyntaxStateMachine_DExprSeqState.hpp>

#include <xo/reader2/ssm/ASyntaxStateMachine.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::facet::FacetRegistry;
    using xo::facet::typeseq;

    namespace scm {
        bool
        reader2_register_facets()
        {
            scope log(XO_DEBUG(true));

            FacetRegistry::register_impl<ASyntaxStateMachine, DExprSeqState>();

            log && log(xtag("DExprSeqState.tseq", typeseq::id<DExprSeqState>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end reader2_register_facets.cpp */
