/** @file VirtualSchematikaMachine.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/interpreter2/init_interpreter2.hpp>
#include <xo/interpreter2/VirtualSchematikaMachine.hpp>
#include <xo/interpreter2/Closure.hpp>
#include <xo/object2/Array.hpp>
#include <xo/object2/List.hpp>
#include <xo/object2/Float.hpp>
#include <xo/object2/Integer.hpp>
#include <xo/object2/Boolean.hpp>
#include <xo/object2/RuntimeError.hpp>
#include <xo/stringtable2/UniqueString.hpp>
#include <xo/alloc2/Arena.hpp>
#include <xo/alloc2/abox.hpp>
#include <xo/facet/TypeRegistry.hpp>
#include <xo/indentlog/print/hex.hpp>

#include <catch2/catch.hpp>

namespace xo {
    using xo::scm::DVirtualSchematikaMachine;
    using xo::scm::VsmConfig;
    using xo::scm::VsmResultExt;
    using xo::scm::DClosure;
    using xo::scm::DString;
    using xo::scm::DUniqueString;  // aks Symbol in lisp
    using xo::scm::DList;
    using xo::scm::DArray;
    using xo::scm::DFloat;
    using xo::scm::DBoolean;
    using xo::scm::DInteger;
//    using xo::scm::DRuntimeError;
    using xo::mm::AGCObject;
    using xo::mm::MemorySizeInfo;
    using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::mm::ArenaConfig;
    using xo::facet::FacetRegistry;
    using xo::facet::TypeRegistry;
    using xo::reflect::typeseq;
    using span_type = xo::scm::DVirtualSchematikaMachine::span_type;
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
            : aux_mm_{testname}
            {
                vsm_.adopt(DVirtualSchematikaMachine::make(aux_mm_.to_op(),
                                                           cfg.with_debug_flag(debug_flag),
                                                           aux_mm_.to_op()));
            }

            ~VsmFixture() {}

            bool log_memory_layout(scope * p_log)
            {
                auto visitor = [p_log](const MemorySizeInfo & info) {
                    *p_log && (*p_log)(xtag("resource", info.resource_name_),
                                       xtag("used", info.used_),
                                       xtag("alloc", info.allocated_),
                                       xtag("commit", info.committed_),
                                       xtag("resv", info.reserved_));
                };

                aux_mm_.arena_.visit_pools(visitor);
                TypeRegistry::instance().visit_pools(visitor);
                FacetRegistry::instance().visit_pools(visitor);
                vsm_->visit_pools(visitor);

                return true;
            }

            bool session_with_complete_input(const char * input_text,
                                             std::function<bool (const VsmResultExt & x)> verify_result)
            {
                INFO(xtag("input_text", input_text));

                bool eof_flag = false;

                vsm_->begin_interactive_session();

                VsmResultExt res = vsm_->read_eval_print(span_type::from_cstr(input_text),
                                                         eof_flag);

                REQUIRE(res.is_value());
                REQUIRE(res.value());

                REQUIRE(verify_result(res));

                REQUIRE(res.remaining_.size() == 1);
                REQUIRE(*res.remaining_.lo() == '\n');

                return true;
            }

            ArenaShim aux_mm_;
            abox<AGCObject,DVirtualSchematikaMachine> vsm_;
        };

        void vsm_std_utest_pattern(bool debug_flag,
                                   const std::string & testname,
                                   const char * input,
                                   std::function<bool (const VsmResultExt & x)> verify_fn,
                                   const VsmConfig & cfg = VsmConfig())
        {
            scope log(XO_DEBUG(debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, debug_flag, cfg);

            vsm_fixture.session_with_complete_input(input, verify_fn);

            log && vsm_fixture.log_memory_layout(&log);
        }

        void vsm_multi_utest_pattern(bool debug_flag,
                                     const std::string & testname,
                                     const char * input,
                                     std::vector<std::function<bool (const VsmResultExt &)>> verify_fns,
                                     bool eof_flag = false,
                                     const VsmConfig & cfg = VsmConfig())
        {
            scope log(XO_DEBUG(debug_flag), xtag("test", testname));

            VsmFixture vsm_fixture(testname, debug_flag, cfg);
            auto & vsm = vsm_fixture.vsm_;

            vsm->begin_interactive_session();

            span_type remaining = span_type::from_cstr(input);

            for (std::size_t i = 0; i < verify_fns.size(); ++i) {
                log && log(xtag("i_expr", i), xtag("input", remaining));

                VsmResultExt res = vsm->read_eval_print(remaining, eof_flag);

                REQUIRE(res.is_value());
                REQUIRE(res.value());

                log && log(xtag("res.tseq", res.value()->_typeseq()),
                           xtag("res.type", TypeRegistry::id2name(res.value()->_typeseq())));

                REQUIRE(verify_fns[i](res));

                if (i + 1 == verify_fns.size()) {
                    REQUIRE(res.remaining_.size() == 1);
                    REQUIRE(*res.remaining_.lo() == '\n');
                } else {
                    REQUIRE(res.remaining_.size() > 1);
                }

                remaining = res.remaining_;
            }

            log && vsm_fixture.log_memory_layout(&log);
        }

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

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DFloat>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x._typeseq() == typeseq::id<DFloat>());
                REQUIRE_THAT(x.data()->value(), WithinAbs(3.141592635, 1e-6));

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname, "3.1415926535;", verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-const2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DInteger>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x._typeseq() == typeseq::id<DInteger>());
                REQUIRE(x.data()->value() == 1011);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname, "1011;", verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-arith1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DFloat>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x._typeseq() == typeseq::id<DFloat>());
                REQUIRE(x.data()->value() == 1.570796325);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "3.14159265 * 0.5;",
                                  verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-arith2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = true;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DFloat>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x.data()->value() == 6.2831853);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "3.14159265 / 0.5;", verify_fn);                                  
        }

        TEST_CASE("VirtualSchematikaMachine-cmp1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DBoolean>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x.data()->value() == true);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "123 == 123;",
                                  verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-if", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DString>::from(*res.value());

                REQUIRE(x);
                REQUIRE(strcmp(x.data()->chars(), "equal") == 0);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "if 123 == 123 then \"equal\" else \"notequal\";",
                                  verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-lambda1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DClosure>::from(*res.value());

                REQUIRE(x);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "lambda (x : i64) -> i64 { x * x; }",
                                  verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-apply2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DInteger>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x->value() == 195);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "(lambda (x : i64, y : i64) { x * y; })(13, 15);",
                                  verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-def1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DUniqueString>::from(*res.value());

                REQUIRE(x);
                REQUIRE(strcmp(x->chars(), "foo") == 0);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "def foo = 3.14159;",
                                  verify_fn);
        }

        TEST_CASE("VirtualSchematikaMachine-def2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = true;

            vsm_multi_utest_pattern(
                c_debug_flag, testname,
                "def foo = 3.14159; foo;",
                {
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DUniqueString>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE(strcmp(x->chars(), "foo") == 0);
                        return true;
                    },
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DFloat>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE_THAT(x->value(), WithinAbs(3.14159, 1e-6));
                        return true;
                    }
                });
        }

        TEST_CASE("VirtualSchematikaMachine-def3", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = true;

            vsm_multi_utest_pattern(
                c_debug_flag, testname,
                "def fact = lambda (n) { if (n == 0) then 1 else n * fact(n - 1) };",
                {
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DUniqueString>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE(strcmp(x->chars(), "fact") == 0);
                        return true;
                    }
                },
                /*eof_flag=*/true);
        }

        TEST_CASE("VirtualSchematikaMachine-if2", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = true;

            vsm_multi_utest_pattern(
                c_debug_flag, testname,
                "def n = 4; if (n == 4) then n * 3 else n * 5;",
                {
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DUniqueString>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE(strcmp(x->chars(), "n") == 0);
                        return true;
                    },
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DInteger>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE(x->value() == 12);
                        return true;
                    }
                },
                /*eof_flag=*/true);
        }

        TEST_CASE("VirtualSchematikaMachine-fact0", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            vsm_multi_utest_pattern(
                c_debug_flag, testname,
                "def fact = lambda (n) { if (n == 0) then 1 else n * fact(n - 1) }; fact(4);",
                {
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DUniqueString>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE(strcmp(x->chars(), "fact") == 0);
                        return true;
                    },
                    [](const VsmResultExt & res) {
                        auto x = obj<AGCObject,DInteger>::from(*res.value());
                        REQUIRE(x);
                        REQUIRE(x->value() == 24);
                        return true;
                    }
                },
                /*eof_flag=*/true);
        }

        TEST_CASE("VirtualSchematikaMachine-qliteral1", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = true;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DFloat>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x->value() == 4.5);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "#q{ 4.5 };",
                                  verify_fn,
                                  VsmConfig().with_parser_debug_flag(c_debug_flag));
        }

        TEST_CASE("VirtualSchematikaMachine-qlist", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DList>::from(*res.value());
                REQUIRE(x);
                REQUIRE(x->size() == 2);

                auto x0 = obj<AGCObject,DFloat>::from(x->at(0));
                REQUIRE(x0);
                REQUIRE(x0->value() == 4.5);

                auto x1 = obj<AGCObject,DFloat>::from(x->at(1));
                REQUIRE(x1);
                REQUIRE(x1->value() == 7.2);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "#q{ (4.5 7.2) };",
                                  verify_fn,
                                  VsmConfig().with_parser_debug_flag(c_debug_flag));
        }

        TEST_CASE("VirtualSchematikaMachine-qarray", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = true;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DArray>::from(*res.value());
                REQUIRE(x);
                REQUIRE(x->size() == 2);

                auto x0 = obj<AGCObject,DFloat>::from(x->at(0));
                REQUIRE(x0);
                REQUIRE(x0->value() == 4.5);

                auto x1 = obj<AGCObject,DFloat>::from(x->at(1));
                REQUIRE(x1);
                REQUIRE(x1->value() == 7.2);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "#q{ [4.5 7.2] };",
                                  verify_fn,
                                  VsmConfig().with_parser_debug_flag(c_debug_flag));
        }

        TEST_CASE("VirtualSchematikaMachine-report_memory_use", "[interpreter2][VSM]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();
            constexpr bool c_debug_flag = false;

            auto verify_fn = [](const VsmResultExt & res) {
                auto x = obj<AGCObject,DBoolean>::from(*res.value());

                REQUIRE(x);
                REQUIRE(x->value() == true);

                return true;
            };

            vsm_std_utest_pattern(c_debug_flag, testname,
                                  "report-memory-use();",
                                  verify_fn,
                                  VsmConfig().with_parser_debug_flag(c_debug_flag));
        }
    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
