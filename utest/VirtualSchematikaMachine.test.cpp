/** @file VirtualSchematikaMachine.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/interpreter2/VirtualSchematikaMachine.hpp>
#include <xo/interpreter2/Closure.hpp>
#include <xo/object2/Float.hpp>
//#include <xo/object2/number/IGCObject_DFloat.hpp>
#include <xo/object2/Integer.hpp>
//#include <xo/object2/number/IGCObject_DInteger.hpp>

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
#include <xo/indentlog/print/hex.hpp>

#include <catch2/catch.hpp>

namespace xo {
    using xo::scm::VirtualSchematikaMachine;
    using xo::scm::VsmConfig;
    using xo::scm::VsmResultExt;
    using xo::scm::DClosure;
    using xo::scm::DFloat;
    using xo::scm::DInteger;
    using xo::mm::AGCObject;
    using xo::mm::MemorySizeInfo;
    using xo::facet::FacetRegistry;
    using span_type = xo::scm::VirtualSchematikaMachine::span_type;
    using Catch::Matchers::WithinAbs;

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
    using std::cout;
    using std::endl;

    static InitEvidence s_init = (InitSubsys<S_interpreter2_tag>::require());

    namespace ut {
        TEST_CASE("VirtualSchematikaMachine-ctor", "[interpreter2][VSM]")
        {
            scope log(XO_DEBUG(true));

            VsmConfig cfg;
            VirtualSchematikaMachine vsm(cfg);

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            vsm.visit_pools(visitor);
        }

        TEST_CASE("VirtualSchematikaMachine-const1", "[interpreter2][VSM]")
        {
            scope log(XO_DEBUG(true));

            VsmConfig cfg;
            VirtualSchematikaMachine vsm(cfg);

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res = vsm.read_eval_print(span_type::from_cstr("3.141592635;"), eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            auto x = obj<AGCObject,DFloat>::from(*res.value());

            REQUIRE(x);
            REQUIRE_THAT(x.data()->value(), WithinAbs(3.141592635, 1e-6));

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            vsm.visit_pools(visitor);
        }

        TEST_CASE("VirtualSchematikaMachine-const2", "[interpreter2][VSM]")
        {
            scope log(XO_DEBUG(true));

            VsmConfig cfg;
            VirtualSchematikaMachine vsm(cfg);

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res = vsm.read_eval_print(span_type::from_cstr("1011;"), eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DInteger>::from(*res.value());

            REQUIRE(x);
            REQUIRE(x.data()->value() == 1011);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            vsm.visit_pools(visitor);
        }

        TEST_CASE("VirtualSchematikaMachine-arith1", "[interpreter2][VSM]")
        {
            scope log(XO_DEBUG(true));

            VsmConfig cfg;
            VirtualSchematikaMachine vsm(cfg);

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res = vsm.read_eval_print(span_type::from_cstr("3.14159265 * 0.5;"), eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DFloat>::from(*res.value());

            REQUIRE(x);
            REQUIRE(x.data()->value() == 1.570796325);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            vsm.visit_pools(visitor);
        }

        TEST_CASE("VirtualSchematikaMachine-lambda1", "[interpreter2][VSM]")
        {
            scope log(XO_DEBUG(true));

            VsmConfig cfg;
            VirtualSchematikaMachine vsm(cfg);

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res = vsm.read_eval_print(span_type::from_cstr("lambda (x : i64) -> i64 { x * x; }"), eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DClosure>::from(*res.value());

            REQUIRE(x);
            //REQUIRE(x.data()->value() == 1.570796325);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            vsm.visit_pools(visitor);
        }

        TEST_CASE("VirtualSchematikaMachine-apply2", "[interpreter2][VSM]")
        {
            scope log(XO_DEBUG(true));

            VsmConfig cfg;
            VirtualSchematikaMachine vsm(cfg);

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res
                = vsm.read_eval_print(span_type::from_cstr
                                          ("(lambda (x : i64, y : i64) { x * y; })(13, 15);"),
                                      eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DClosure>::from(*res.value());

            REQUIRE(x);
            //REQUIRE(x.data()->value() == 1.570796325);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            auto visitor = [&log](const MemorySizeInfo & info) {
                log && log(xtag("resource", info.resource_name_),
                           xtag("used", info.used_),
                           xtag("alloc", info.allocated_),
                           xtag("commit", info.committed_),
                           xtag("resv", info.reserved_));
            };

            FacetRegistry::instance().visit_pools(visitor);
            vsm.visit_pools(visitor);
        }
    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
