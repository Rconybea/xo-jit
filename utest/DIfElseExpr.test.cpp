/** @file DIfElseExpr.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include <xo/expression2/DIfElseExpr.hpp>
#include <xo/expression2/detail/IExpression_DIfElseExpr.hpp>
#include <xo/expression2/detail/IPrintable_DIfElseExpr.hpp>
#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>

#include <xo/object2/DFloat.hpp>
#include <xo/object2/DBoolean.hpp>
#include <xo/object2/number/IGCObject_DFloat.hpp>
#include <xo/object2/boolean/IGCObject_DBoolean.hpp>

#include <xo/gc/CollectorTypeRegistry.hpp>
#include <xo/gc/DX1Collector.hpp>
#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/gc/detail/ICollector_DX1Collector.hpp>

#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflect/Reflect.hpp>

#include <xo/indentlog/scope.hpp>

#include <catch2/catch.hpp>

#include <sstream>

namespace ut {
    using xo::S_expression2_tag;
    using xo::scm::DIfElseExpr;
    using xo::scm::DConstant;
    using xo::scm::DFloat;
    using xo::scm::DBoolean;
    using xo::scm::AExpression;
    using xo::mm::CollectorTypeRegistry;
    using xo::mm::AAllocator;
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::mm::DX1Collector;
    using xo::mm::CollectorConfig;
    using xo::mm::ArenaConfig;
    using xo::print::APrintable;
    using xo::print::ppstate_standalone;
    using xo::print::ppconfig;
    using xo::facet::FacetRegistry;
    using xo::facet::with_facet;
    using xo::facet::obj;
    using xo::reflect::Reflect;
    using xo::InitEvidence;
    using xo::InitSubsys;
    using xo::scope;

    static InitEvidence s_init = InitSubsys<S_expression2_tag>::require();

    TEST_CASE("DIfElseExpr-init", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());
    }

    TEST_CASE("DIfElseExpr-make", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_make_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        // Create test expression: constant true
        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, true);
        auto test_expr = DConstant::make(alloc, bval);
        REQUIRE(test_expr.data() != nullptr);

        // Create when_true expression: constant 1.0
        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);
        REQUIRE(when_true_expr.data() != nullptr);

        // Create when_false expression: constant 2.0
        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);
        REQUIRE(when_false_expr.data() != nullptr);

        // Create if-else expression: if true then 1.0 else 2.0
        auto ifexpr = DIfElseExpr::make(alloc, test_expr, when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);
    }

    TEST_CASE("DIfElseExpr-test", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_test_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, true);
        auto test_expr = DConstant::make(alloc, bval);

        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);

        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);

        auto ifexpr = DIfElseExpr::make(alloc, test_expr, when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);

        obj<AExpression> test = ifexpr.data()->test();
        REQUIRE(test.data() != nullptr);
        REQUIRE(test.extype() == xo::scm::exprtype::constant);
    }

    TEST_CASE("DIfElseExpr-when-true", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_when_true_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, true);
        auto test_expr = DConstant::make(alloc, bval);

        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);

        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);

        auto ifexpr = DIfElseExpr::make(alloc, test_expr, when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);

        obj<AExpression> wt = ifexpr.data()->when_true();
        REQUIRE(wt.data() != nullptr);
        REQUIRE(wt.extype() == xo::scm::exprtype::constant);
    }

    TEST_CASE("DIfElseExpr-when-false", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_when_false_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, false);
        auto test_expr = DConstant::make(alloc, bval);

        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);

        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);

        auto ifexpr = DIfElseExpr::make(alloc, test_expr, when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);

        obj<AExpression> wf = ifexpr.data()->when_false();
        REQUIRE(wf.data() != nullptr);
        REQUIRE(wf.extype() == xo::scm::exprtype::constant);
    }

    TEST_CASE("DIfElseExpr-extype", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_extype_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, true);
        auto test_expr = DConstant::make(alloc, bval);

        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);

        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);

        auto ifexpr = DIfElseExpr::make(alloc, test_expr, when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);
        REQUIRE(ifexpr.data()->extype() == xo::scm::exprtype::ifexpr);
    }

    TEST_CASE("DIfElseExpr-valuetype", "[expression2][DIfElseExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_valuetype_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, true);
        auto test_expr = DConstant::make(alloc, bval);

        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);

        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);

        auto ifexpr = DIfElseExpr::make(alloc, test_expr, when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);

        // valuetype may be null before type resolution
        // just verify we can call it
        ifexpr.data()->valuetype();
    }

    TEST_CASE("DIfElseExpr-pretty", "[expression2][DIfElseExpr][pp]")
    {
        scope log(XO_DEBUG(true));

        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "difelseexpr_pretty_test",
            .arena_config_ = ArenaConfig{
                .size_ = 8192,
                .store_header_flag_ = true},
            .object_types_z_ = 16384,
            .gc_trigger_v_{{4096, 4096}},
            .debug_flag_ = false,
        };

        DX1Collector gc(cfg);
        auto alloc = with_facet<AAllocator>::mkobj(&gc);
        auto coll = with_facet<ACollector>::mkobj(&gc);

        bool ok = CollectorTypeRegistry::instance().install_types(coll);
        REQUIRE(ok);

        obj<AGCObject> bval = DBoolean::box<AGCObject>(alloc, true);
        auto test_expr = DConstant::make(alloc, bval);

        obj<AGCObject> fval1 = DFloat::box<AGCObject>(alloc, 1.0);
        auto when_true_expr = DConstant::make(alloc, fval1);

        obj<AGCObject> fval2 = DFloat::box<AGCObject>(alloc, 2.0);
        auto when_false_expr = DConstant::make(alloc, fval2);

        auto ifexpr = DIfElseExpr::make(alloc,
                                        test_expr,
                                        when_true_expr, when_false_expr);
        REQUIRE(ifexpr.data() != nullptr);

        std::stringstream ss;
        ppconfig ppc;
        ppstate_standalone pps(&ss, 0, &ppc);

        obj<APrintable,DIfElseExpr> ifexpr_pr(ifexpr.data());
        pps.pretty(ifexpr_pr);

        std::string output = ss.str();

        log && log(output);

        CHECK(output.find("DIfElseExpr") != std::string::npos);
    }
}

/* end DIfElseExpr.test.cpp */
