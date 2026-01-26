/** @file DApplyExpr.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include <xo/expression2/DApplyExpr.hpp>
#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DApplyExpr.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/expression2/detail/IPrintable_DApplyExpr.hpp>
#include <xo/expression2/detail/IPrintable_DConstant.hpp>

#include <xo/procedure2/init_primitives.hpp>
#include <xo/procedure2/detail/IGCObject_DPrimitive_gco_2_gco_gco.hpp>
#include <xo/procedure2/detail/IPrintable_DPrimitive_gco_2_gco_gco.hpp>

#include <xo/object2/DFloat.hpp>
#include <xo/object2/number/IGCObject_DFloat.hpp>

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
    using xo::scm::DApplyExpr;
    using xo::scm::DConstant;
    using xo::scm::DFloat;
    using xo::scm::AExpression;
    using xo::scm::TypeRef;
    using xo::scm::Primitives;
    using xo::scm::DPrimitive_gco_2_gco_gco;
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

    TEST_CASE("DApplyExpr-init", "[expression2][DApplyExpr]")
    {
        REQUIRE(s_init.evidence());
    }

    TEST_CASE("DApplyExpr-make2", "[expression2][DApplyExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dapplyexpr_make2_test",
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

        // wrap primitive as GCObject, then as expression
        obj<AGCObject> prim_gco = with_facet<AGCObject>::mkobj(&Primitives::s_mul_gco_gco_pm);
        obj<AExpression,DConstant> fn_expr = DConstant::make(alloc, prim_gco);
        REQUIRE(fn_expr.data() != nullptr);

        // create argument expressions
        obj<AGCObject> val1 = DFloat::box<AGCObject>(alloc, 3.0);
        obj<AGCObject> val2 = DFloat::box<AGCObject>(alloc, 7.0);
        obj<AExpression,DConstant> arg1 = DConstant::make(alloc, val1);
        obj<AExpression,DConstant> arg2 = DConstant::make(alloc, val2);
        REQUIRE(arg1.data() != nullptr);
        REQUIRE(arg2.data() != nullptr);

        // create apply expression: mul(3.0, 7.0)
        TypeRef result_type = TypeRef::resolved(Reflect::require<double>());
        auto apply_expr = DApplyExpr::make2(alloc, result_type, fn_expr, arg1, arg2);
        REQUIRE(apply_expr.data() != nullptr);
    }

    TEST_CASE("DApplyExpr-extype", "[expression2][DApplyExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dapplyexpr_extype_test",
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

        obj<AGCObject> prim_gco = with_facet<AGCObject>::mkobj(&Primitives::s_mul_gco_gco_pm);
        obj<AExpression,DConstant> fn_expr = DConstant::make(alloc, prim_gco);

        obj<AGCObject> val1 = DFloat::box<AGCObject>(alloc, 3.0);
        obj<AGCObject> val2 = DFloat::box<AGCObject>(alloc, 7.0);
        obj<AExpression,DConstant> arg1 = DConstant::make(alloc, val1);
        obj<AExpression,DConstant> arg2 = DConstant::make(alloc, val2);

        TypeRef result_type = TypeRef::resolved(Reflect::require<double>());
        auto apply_expr = DApplyExpr::make2(alloc, result_type, fn_expr, arg1, arg2);

        REQUIRE(apply_expr.data()->extype() == xo::scm::exprtype::apply);
    }

    TEST_CASE("DApplyExpr-n_args", "[expression2][DApplyExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dapplyexpr_n_args_test",
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

        obj<AGCObject> prim_gco = with_facet<AGCObject>::mkobj(&Primitives::s_mul_gco_gco_pm);
        obj<AExpression,DConstant> fn_expr = DConstant::make(alloc, prim_gco);

        obj<AGCObject> val1 = DFloat::box<AGCObject>(alloc, 3.0);
        obj<AGCObject> val2 = DFloat::box<AGCObject>(alloc, 7.0);
        obj<AExpression,DConstant> arg1 = DConstant::make(alloc, val1);
        obj<AExpression,DConstant> arg2 = DConstant::make(alloc, val2);

        TypeRef result_type = TypeRef::resolved(Reflect::require<double>());
        auto apply_expr = DApplyExpr::make2(alloc, result_type, fn_expr, arg1, arg2);

        REQUIRE(apply_expr.data()->n_args() == 2);
    }

    TEST_CASE("DApplyExpr-fn", "[expression2][DApplyExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dapplyexpr_fn_test",
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

        obj<AGCObject> prim_gco = with_facet<AGCObject>::mkobj(&Primitives::s_mul_gco_gco_pm);
        obj<AExpression,DConstant> fn_expr = DConstant::make(alloc, prim_gco);

        obj<AGCObject> val1 = DFloat::box<AGCObject>(alloc, 3.0);
        obj<AGCObject> val2 = DFloat::box<AGCObject>(alloc, 7.0);
        obj<AExpression,DConstant> arg1 = DConstant::make(alloc, val1);
        obj<AExpression,DConstant> arg2 = DConstant::make(alloc, val2);

        TypeRef result_type = TypeRef::resolved(Reflect::require<double>());
        auto apply_expr = DApplyExpr::make2(alloc, result_type, fn_expr, arg1, arg2);

        // verify fn() returns an expression
        obj<AExpression> fn = apply_expr.data()->fn();
        REQUIRE(fn.data() != nullptr);
        REQUIRE(fn.extype() == xo::scm::exprtype::constant);
    }

    TEST_CASE("DApplyExpr-arg", "[expression2][DApplyExpr]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dapplyexpr_arg_test",
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

        obj<AGCObject> prim_gco = with_facet<AGCObject>::mkobj(&Primitives::s_mul_gco_gco_pm);
        obj<AExpression,DConstant> fn_expr = DConstant::make(alloc, prim_gco);

        obj<AGCObject> val1 = DFloat::box<AGCObject>(alloc, 3.0);
        obj<AGCObject> val2 = DFloat::box<AGCObject>(alloc, 7.0);
        obj<AExpression,DConstant> arg1 = DConstant::make(alloc, val1);
        obj<AExpression,DConstant> arg2 = DConstant::make(alloc, val2);

        TypeRef result_type = TypeRef::resolved(Reflect::require<double>());
        auto apply_expr = DApplyExpr::make2(alloc, result_type, fn_expr, arg1, arg2);

        // verify arg(0) and arg(1) return expressions
        obj<AExpression> a0 = apply_expr.data()->arg(0);
        obj<AExpression> a1 = apply_expr.data()->arg(1);

        REQUIRE(a0.data() != nullptr);
        REQUIRE(a1.data() != nullptr);
        REQUIRE(a0.extype() == xo::scm::exprtype::constant);
        REQUIRE(a1.extype() == xo::scm::exprtype::constant);
    }

    TEST_CASE("DApplyExpr-pretty", "[expression2][DApplyExpr][pp]")
    {
        scope log(XO_DEBUG(false));

        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dapplyexpr_pretty_test",
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

        obj<AGCObject> prim_gco = with_facet<AGCObject>::mkobj(&Primitives::s_mul_gco_gco_pm);
        obj<AExpression,DConstant> fn_expr = DConstant::make(alloc, prim_gco);

        obj<AGCObject> val1 = DFloat::box<AGCObject>(alloc, 3.0);
        obj<AGCObject> val2 = DFloat::box<AGCObject>(alloc, 7.0);
        obj<AExpression,DConstant> arg1 = DConstant::make(alloc, val1);
        obj<AExpression,DConstant> arg2 = DConstant::make(alloc, val2);

        TypeRef result_type = TypeRef::resolved(Reflect::require<double>());
        auto apply_expr = DApplyExpr::make2(alloc, result_type, fn_expr, arg1, arg2);

        std::stringstream ss;
        ppconfig ppc;
        ppstate_standalone pps(&ss, 0, &ppc);

        obj<APrintable,DApplyExpr> expr_pr(apply_expr.data());
        pps.pretty(expr_pr);

        std::string output = ss.str();

        log && log(output);

        CHECK(output.find("ApplyExpr") != std::string::npos);
    }
}

/* end DApplyExpr.test.cpp */
