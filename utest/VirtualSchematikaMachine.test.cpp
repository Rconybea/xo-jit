/** @file VirtualSchematikaMachine.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/interpreter2/VirtualSchematikaMachine.hpp>
#include <xo/interpreter2/Closure.hpp>
#include <xo/expression2/UniqueString.hpp>
#include <xo/object2/Float.hpp>
#include <xo/object2/Integer.hpp>
#include <xo/object2/Boolean.hpp>
#include <xo/object2/RuntimeError.hpp>
#include <xo/alloc2/Arena.hpp>
#include <xo/facet/TypeRegistry.hpp>

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
    using xo::scm::DString;
    using xo::scm::DUniqueString;  // aks Symbol in lisp
    using xo::scm::DFloat;
    using xo::scm::DBoolean;
    using xo::scm::DInteger;
    using xo::scm::DRuntimeError;
    using xo::mm::AGCObject;
    using xo::mm::MemorySizeInfo;
    using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::mm::ArenaConfig;
    using xo::facet::FacetRegistry;
    using xo::facet::TypeRegistry;
    using span_type = xo::scm::VirtualSchematikaMachine::span_type;
    using Catch::Matchers::WithinAbs;

    using std::cout;
    using std::endl;

    static InitEvidence s_init = (InitSubsys<S_interpreter2_tag>::require());

    namespace ut {
        struct ArenaShim {
            explicit ArenaShim(const std::string & name, std::size_t size = 16*1024)
            : arena_(ArenaConfig().with_name(name).with_size(size))
            {
            }

            obj<AAllocator,DArena> to_op() { return obj<AAllocator,DArena>(&arena_); }

            DArena arena_;
        };

        struct VsmFixture {
            explicit VsmFixture(const std::string & testname,
                                bool debug_flag,
                                const VsmConfig & cfg = VsmConfig())
            : aux_mm_{testname},
              vsm_{cfg.with_debug_flag(debug_flag), aux_mm_.to_op()}
            {}

            bool log_memory_layout(scope * p_log) {
                auto visitor = [p_log](const MemorySizeInfo & info) {
                    *p_log && (*p_log)(xtag("resource", info.resource_name_),
                                       xtag("used", info.used_),
                                       xtag("alloc", info.allocated_),
                                       xtag("commit", info.committed_),
                                       xtag("resv", info.reserved_));
                };

                aux_mm_.arena_.visit_pools(visitor);
                FacetRegistry::instance().visit_pools(visitor);
                TypeRegistry::instance().visit_pools(visitor);
                vsm_.visit_pools(visitor);

                return true;
            }

            ArenaShim aux_mm_;
            VirtualSchematikaMachine vsm_;
        };

        TEST_CASE("VirtualSchematikaMachine-ctor", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-const1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);

            auto & vsm = vsm_fixture.vsm_;

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

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-const2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);

            auto & vsm = vsm_fixture.vsm_;

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

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-arith1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

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

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-cmp1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res
                = vsm.read_eval_print(span_type::from_cstr("123 == 123;"),
                                      eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DBoolean>::from(*res.value());

            REQUIRE(x);
            REQUIRE(x.data()->value() == true);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-if", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res
                = vsm.read_eval_print(span_type::from_cstr("if 123 == 123 then \"equal\" else \"notequal\";"),
                                      eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DString>::from(*res.value());

            REQUIRE(x);
            REQUIRE(strcmp(x.data()->chars(), "equal") == 0);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-lambda1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res
                = vsm.read_eval_print(span_type::from_cstr("lambda (x : i64) -> i64 { x * x; }"),
                                      eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            auto x = obj<AGCObject,DClosure>::from(*res.value());

            REQUIRE(x);
            //REQUIRE(x.data()->value() == 1.570796325);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-apply2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res
                = vsm.read_eval_print(span_type::from_cstr
                                          ("(lambda (x : i64, y : i64) { x * y; })(13, 15);"),
                                      eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()));

            // currently get not-implemented error
            auto x = obj<AGCObject,DInteger>::from(*res.value());

            REQUIRE(x);
            REQUIRE(x->value() == 195);

            //log && log("runtime-error", xtag("ex.src", x->src_function()), xtag("ex.descr", x->error_descr()));

            //REQUIRE(x.data()->value() == 1.570796325);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-def1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

            bool eof_flag = false;

            vsm.begin_interactive_session();
            VsmResultExt res
                = vsm.read_eval_print(span_type::from_cstr("def foo = 3.14159;"),
                                      eof_flag);

            REQUIRE(res.is_value());
            REQUIRE(res.value());

            log && log(xtag("res.tseq", res.value()->_typeseq()),
                       xtag("res.type", TypeRegistry::id2name(res.value()->_typeseq())));

            // currently get not-implemented error
            auto x = obj<AGCObject,DUniqueString>::from(*res.value());

            REQUIRE(x);
            REQUIRE(strcmp(x->chars(), "foo") == 0);

            //log && log("runtime-error", xtag("ex.src", x->src_function()), xtag("ex.descr", x->error_descr()));

            //REQUIRE(x.data()->value() == 1.570796325);

            REQUIRE(res.remaining_.size() == 1);
            REQUIRE(*res.remaining_.lo() == '\n');

            log && vsm_fixture.log_memory_layout(&log);
        }

        TEST_CASE("VirtualSchematikaMachine-def2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, c_debug_flag);
            auto & vsm = vsm_fixture.vsm_;

            bool eof_flag = false;

            vsm.begin_interactive_session();

            span_type input = span_type::from_cstr("def foo = 3.14159; foo");

            for (int i_expr = 0; i_expr < 2; ++i_expr) {
                VsmResultExt res
                    = vsm.read_eval_print(input, eof_flag);

                REQUIRE(res.is_value());
                REQUIRE(res.value());

                log && log(xtag("res.tseq", res.value()->_typeseq()),
                           xtag("res.type", TypeRegistry::id2name(res.value()->_typeseq())));

                if (i_expr == 0) {
                    auto x = obj<AGCObject,DUniqueString>::from(*res.value());
                    REQUIRE(x);
                    REQUIRE(strcmp(x->chars(), "foo") == 0);

                    REQUIRE(res.remaining_.size() > 1);

                    input = res.remaining_;
                } else if (i_expr == 1) {
                    auto x = obj<AGCObject,DFloat>::from(*res.value());
                    REQUIRE(x);
                    REQUIRE(x->value() == 3.14159);

                    REQUIRE(res.remaining_.size() == 1);
                    REQUIRE(*res.remaining_.lo() == '\n');
                    input = res.remaining_;
                }

                ++i_expr;
            }

            log && vsm_fixture.log_memory_layout(&log);

        }

    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
