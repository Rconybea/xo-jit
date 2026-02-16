/** @file reader2_register_facets.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "reader2_register_facets.hpp"

#include "SchematikaParser.hpp"
#include "ExprSeqState.hpp"
#include "DefineSsm.hpp"
#include "LambdaSsm.hpp"
#include "IfElseSsm.hpp"
#include "ApplySsm.hpp"
#include "SequenceSsm.hpp"
#include "ParenSsm.hpp"
#include "ExpectFormalArglistSsm.hpp"
#include "ExpectFormalArgSsm.hpp"
#include "ExpectSymbolSsm.hpp"
#include "ExpectTypeSsm.hpp"
#include "ExpectExprSsm.hpp"
#include "ProgressSsm.hpp"
#include "SyntaxStateMachine.hpp"

#include <xo/printable2/detail/APrintable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::TypeRegistry;
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

            FacetRegistry::register_impl<ASyntaxStateMachine, DLambdaSsm>();
            FacetRegistry::register_impl<APrintable, DLambdaSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DIfElseSsm>();
            FacetRegistry::register_impl<APrintable, DIfElseSsm>();

            FacetRegistry::register_impl<APrintable, DApplySsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DSequenceSsm>();
            FacetRegistry::register_impl<APrintable, DSequenceSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectFormalArglistSsm>();
            FacetRegistry::register_impl<APrintable, DExpectFormalArglistSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectFormalArgSsm>();
            FacetRegistry::register_impl<APrintable, DExpectFormalArgSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectSymbolSsm>();
            FacetRegistry::register_impl<APrintable, DExpectSymbolSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectTypeSsm>();
            FacetRegistry::register_impl<APrintable, DExpectTypeSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectExprSsm>();
            FacetRegistry::register_impl<APrintable, DExpectExprSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DProgressSsm>();
            FacetRegistry::register_impl<APrintable, DProgressSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DParenSsm>();
            FacetRegistry::register_impl<APrintable, DParenSsm>();

            // misc types showing up in aux arena
            TypeRegistry::register_type<SchematikaParser>();
            // misc types showing up in parser stack arena
            TypeRegistry::register_type<ParserStack>();

            log && log(xtag("DExprSeqState.tseq", typeseq::id<DExprSeqState>()));
            log && log(xtag("DDefineSsm.tseq", typeseq::id<DDefineSsm>()));
            log && log(xtag("DLambdaSsm.tseq", typeseq::id<DLambdaSsm>()));
            log && log(xtag("DIfElseSsm.tseq", typeseq::id<DIfElseSsm>()));
            log && log(xtag("DExpectFormalArglistSsm.tseq", typeseq::id<DExpectFormalArglistSsm>()));
            log && log(xtag("DExpectFormalArgSsm.tseq", typeseq::id<DExpectFormalArgSsm>()));
            log && log(xtag("DExpectSymbolSsm.tseq", typeseq::id<DExpectSymbolSsm>()));
            log && log(xtag("DExpectTypeSsm.tseq", typeseq::id<DExpectTypeSsm>()));
            log && log(xtag("DExpectExprSsm.tseq", typeseq::id<DExpectExprSsm>()));
            log && log(xtag("DProgressSsm.tseq", typeseq::id<DProgressSsm>()));
            log && log(xtag("DParenSsm.tseq", typeseq::id<DParenSsm>()));
            log && log(xtag("ASyntaxStateMachine.tseq", typeseq::id<ASyntaxStateMachine>()));

            return true;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end reader2_register_facets.cpp */
