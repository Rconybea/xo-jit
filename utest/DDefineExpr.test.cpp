/** @file DDefineExpr.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include <xo/expression2/DDefineExpr.hpp>
#include <xo/expression2/detail/IExpression_DDefineExpr.hpp>
#include <xo/expression2/detail/IPrintable_DDefineExpr.hpp>
#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/expression2/StringTable.hpp>

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
#include <cstring>

namespace ut {
    using xo::S_expression2_tag;
    using xo::scm::DDefineExpr;
    using xo::scm::DConstant;
    using xo::scm::DFloat;
    using xo::scm::DVariable;
    using xo::scm::DUniqueString;
    using xo::scm::StringTable;
    using xo::scm::AExpression;
    using xo::mm::CollectorTypeRegistry;
    using xo::mm::AAllocator;
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::mm::DX1Collector;
    using xo::mm::X1CollectorConfig;
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

    TEST_CASE("DDefineExpr-init", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());
    }

    TEST_CASE("DDefineExpr-make", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_make_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("x");
        REQUIRE(name != nullptr);

        // Create rhs expression: constant 42.0
        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 42.0);
        auto rhs_expr = DConstant::make(alloc, fval);
        REQUIRE(rhs_expr.data() != nullptr);

        // Create define expression: def x = 42.0
        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);
    }

    TEST_CASE("DDefineExpr-lhs", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_lhs_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("myvar");

        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 3.14);
        auto rhs_expr = DConstant::make(alloc, fval);

        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);

        DVariable * lhs = def->lhs();
        REQUIRE(lhs != nullptr);
        REQUIRE(lhs->name() == name);
    }

    TEST_CASE("DDefineExpr-rhs", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_rhs_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("y");

        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 2.718);
        auto rhs_expr = DConstant::make(alloc, fval);

        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);

        obj<AExpression> rhs = def->rhs();
        REQUIRE(rhs.data() != nullptr);
        // Verify rhs expression type is constant
        REQUIRE(rhs.extype() == xo::scm::exprtype::constant);
    }

    TEST_CASE("DDefineExpr-name", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_name_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("foo");

        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 1.0);
        auto rhs_expr = DConstant::make(alloc, fval);

        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);
        REQUIRE(def->name() == name);
        REQUIRE(std::strcmp(def->name()->chars(), "foo") == 0);
    }

    TEST_CASE("DDefineExpr-extype", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_extype_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("z");

        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 0.0);
        auto rhs_expr = DConstant::make(alloc, fval);

        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);
        REQUIRE(def->extype() == xo::scm::exprtype::define);
    }

    TEST_CASE("DDefineExpr-valuetype", "[expression2][DDefineExpr]")
    {
        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_valuetype_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("w");

        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 99.9);
        auto rhs_expr = DConstant::make(alloc, fval);

        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);
        REQUIRE(def->valuetype() == Reflect::require<double>());
    }

    TEST_CASE("DDefineExpr-pretty", "[expression2][DDefineExpr][pp]")
    {
        scope log(XO_DEBUG(true));

        REQUIRE(s_init.evidence());

        X1CollectorConfig cfg{
            .name_ = "ddefineexpr_pretty_test",
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

        StringTable table(1024);
        const DUniqueString * name = table.intern("bar");

        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 123.456);
        auto rhs_expr = DConstant::make(alloc, fval);

        DDefineExpr * def = DDefineExpr::make(alloc, name, rhs_expr);
        REQUIRE(def != nullptr);

        std::stringstream ss;
        ppconfig ppc;
        ppstate_standalone pps(&ss, 0, &ppc);

        obj<APrintable,DDefineExpr> def_pr(def);
        pps.pretty(def_pr);

        std::string output = ss.str();

        log && log(output);

        CHECK(output.find("DDefineExpr") != std::string::npos);
    }
}

/* end DDefineExpr.test.cpp */
