/** @file SetupReader2.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "SetupReader2.hpp"

#include "SchematikaParser.hpp"
#include "ToplevelSeqSsm.hpp"
#include "DefineSsm.hpp"
#include "DeftypeSsm.hpp"
#include "LambdaSsm.hpp"
#include "IfElseSsm.hpp"
#include "ApplySsm.hpp"
#include "SequenceSsm.hpp"
#include "ParenSsm.hpp"
#include "QuoteSsm.hpp"
#include "ProgressSsm.hpp"
#include "SyntaxStateMachine.hpp"
#include "ExpectFormalArglistSsm.hpp"
#include "ExpectFormalArgSsm.hpp"
#include "ExpectSymbolSsm.hpp"
#include "ExpectTypeSsm.hpp"
#include "ExpectListTypeSsm.hpp"
#include "ExpectExprSsm.hpp"
#include "ExpectQLiteralSsm.hpp"
#include "ExpectQListSsm.hpp"
#include "ExpectQArraySsm.hpp"

#include <xo/printable2/detail/APrintable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::facet::FacetRegistry;
    using xo::facet::TypeRegistry;
    using xo::facet::typeseq;
    using xo::facet::impl_for;

    namespace scm {
        bool
        SetupReader2::register_facets()
        {
            scope log(XO_DEBUG(true));

            // GlobalEnv

            FacetRegistry::register_impl<AGCObject, DGlobalEnv>();
            FacetRegistry::register_impl<APrintable, DGlobalEnv>();

            // SyntaxStateMachine

            FacetRegistry::register_impl<ASyntaxStateMachine, DToplevelSeqSsm>();
            FacetRegistry::register_impl<APrintable, DToplevelSeqSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DDefineSsm>();
            FacetRegistry::register_impl<APrintable, DDefineSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DDeftypeSsm>();
            FacetRegistry::register_impl<APrintable, DDeftypeSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DLambdaSsm>();
            FacetRegistry::register_impl<APrintable, DLambdaSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DIfElseSsm>();
            FacetRegistry::register_impl<APrintable, DIfElseSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DApplySsm>();
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

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectListTypeSsm>();
            FacetRegistry::register_impl<APrintable, DExpectListTypeSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectExprSsm>();
            FacetRegistry::register_impl<APrintable, DExpectExprSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DProgressSsm>();
            FacetRegistry::register_impl<APrintable, DProgressSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DParenSsm>();
            FacetRegistry::register_impl<APrintable, DParenSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DQuoteSsm>();
            FacetRegistry::register_impl<APrintable, DQuoteSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectQLiteralSsm>();
            FacetRegistry::register_impl<APrintable, DExpectQLiteralSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectQListSsm>();
            FacetRegistry::register_impl<APrintable, DExpectQListSsm>();

            FacetRegistry::register_impl<ASyntaxStateMachine, DExpectQArraySsm>();
            FacetRegistry::register_impl<APrintable, DExpectQArraySsm>();

            // misc types showing up in aux arena
            TypeRegistry::register_type<SchematikaParser>();
            // misc types showing up in parser stack arena
            TypeRegistry::register_type<ParserStack>();

            log && log(xtag("DGlobalEnv.tseq", typeseq::id<DGlobalEnv>()));
            log && log(xtag("DToplevelSeqSsm.tseq", typeseq::id<DToplevelSeqSsm>()));
            log && log(xtag("DDefineSsm.tseq", typeseq::id<DDefineSsm>()));
            log && log(xtag("DDeftypeSsm.tseq", typeseq::id<DDeftypeSsm>()));
            log && log(xtag("DLambdaSsm.tseq", typeseq::id<DLambdaSsm>()));
            log && log(xtag("DIfElseSsm.tseq", typeseq::id<DIfElseSsm>()));
            log && log(xtag("DExpectFormalArglistSsm.tseq", typeseq::id<DExpectFormalArglistSsm>()));
            log && log(xtag("DExpectFormalArgSsm.tseq", typeseq::id<DExpectFormalArgSsm>()));
            log && log(xtag("DExpectSymbolSsm.tseq", typeseq::id<DExpectSymbolSsm>()));
            log && log(xtag("DExpectTypeSsm.tseq", typeseq::id<DExpectTypeSsm>()));
            log && log(xtag("DExpectExprSsm.tseq", typeseq::id<DExpectExprSsm>()));
            log && log(xtag("DExpectQLiteralSsm.tseq", typeseq::id<DExpectQLiteralSsm>()));
            log && log(xtag("DExpectQListSsm.tseq", typeseq::id<DExpectQListSsm>()));
            log && log(xtag("DExpectQArraySsm.tseq", typeseq::id<DExpectQArraySsm>()));

            log && log(xtag("DProgressSsm.tseq", typeseq::id<DProgressSsm>()));
            log && log(xtag("DParenSsm.tseq", typeseq::id<DParenSsm>()));
            log && log(xtag("DQuoteSsm.tseq", typeseq::id<DQuoteSsm>()));
            log && log(xtag("ASyntaxStateMachine.tseq", typeseq::id<ASyntaxStateMachine>()));

            return true;
        }

        bool
        SetupReader2::register_types(obj<ACollector> gc)
        {
            scope log(XO_DEBUG(true));

            bool ok = true;

            ok &= gc.install_type(impl_for<AGCObject, DGlobalEnv>());

            return ok;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end SetupReader2.cpp */
