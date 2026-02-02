/** @file VirtualSchematikaMachine.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/interpreter2/VirtualSchematikaMachine.hpp>

#ifdef NOT_YET
#include <xo/reader2/SchematikaParser.hpp>
#include <xo/reader2/DDefineSsm.hpp>
#include <xo/reader2/DExpectExprSsm.hpp>
#include <xo/reader2/ssm/ISyntaxStateMachine_DExpectExprSsm.hpp>
#include <xo/reader2/ssm/ISyntaxStateMachine_DDefineSsm.hpp>
#endif
#include <xo/interpreter2/init_interpreter2.hpp>
#ifdef NOT_YET
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#endif

#include <catch2/catch.hpp>

namespace xo {
#ifdef NOT_YET
    using xo::scm::SchematikaParser;
    using xo::scm::ASyntaxStateMachine;
    using xo::scm::syntaxstatetype;
//    using xo::scm::DDefineSsm;
    using xo::scm::DExpectExprSsm;
//    using xo::scm::defexprstatetype;
    //using xo::scm::ParserResult;
    //using xo::scm::parser_result_type;
    using xo::scm::Token;
    using xo::scm::DString;
    using xo::mm::ArenaConfig;
    using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::facet::with_facet;
#endif

    static InitEvidence s_init = (InitSubsys<S_interpreter2_tag>::require());

    namespace ut {
        TEST_CASE("VirtualSchematikaMachine-ctor", "[interpreter2][VSM]")
        {
#ifdef NOT_YET
            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            REQUIRE(parser.debug_flag() == false);
            REQUIRE(parser.is_at_toplevel() == true);
#endif
        }

    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
