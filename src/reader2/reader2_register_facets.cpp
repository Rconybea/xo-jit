/** @file reader2_register_facets.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "reader2_register_facets.hpp"

#include <xo/reader2/ssm/ISyntaxStateMachine_DExprSeqState.hpp>
#include <xo/reader2/ssm/IPrintable_DExprSeqState.hpp>

#include <xo/reader2/ssm/ISyntaxStateMachine_DDefineSsm.hpp>
#include <xo/reader2/ssm/IPrintable_DDefineSsm.hpp>

#include <xo/reader2/ssm/ISyntaxStateMachine_DExpectSymbolSsm.hpp>
#include <xo/reader2/ssm/IPrintable_DExpectSymbolSsm.hpp>

#include <xo/reader2/ssm/ISyntaxStateMachine_DExpectTypeSsm.hpp>
#include <xo/reader2/ssm/IPrintable_DExpectTypeSsm.hpp>

#include <xo/reader2/ssm/ASyntaxStateMachine.hpp>
#include <xo/printable2/detail/APrintable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::typeseq;

    namespace scm {
        bool
        reader2_register_facets()
        {
            scope log(XO_DEBUG(true));

            FacetRegistry::register_impl<ASyntaxStateMachine, DExprSeqState>();
            FacetRegistry::register_impl<APrintable, DExprSeqState>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DDefineSsm>();
            FacetRegistry::register_impl<APrintable, DDefineSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectSymbolSsm>();
            FacetRegistry::register_impl<APrintable, DExpectSymbolSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectTypeSsm>();
            FacetRegistry::register_impl<APrintable, DExpectTypeSsm>();

            log && log(xtag("DExprSeqState.tseq", typeseq::id<DExprSeqState>()));
            log && log(xtag("DDefineSsm.tseq", typeseq::id<DDefineSsm>()));
            log && log(xtag("DExpectSymbolSsm.tseq", typeseq::id<DExpectSymbolSsm>()));
            log && log(xtag("DExpectTypeSsm.tseq", typeseq::id<DExpectTypeSsm>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end reader2_register_facets.cpp */
