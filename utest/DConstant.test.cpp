/** @file DConstant.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/expression2/detail/IPrintable_DConstant.hpp>

#include <xo/object2/DFloat.hpp>
#include <xo/object2/DInteger.hpp>
#include <xo/object2/number/IGCObject_DFloat.hpp>
#include <xo/object2/number/IGCObject_DInteger.hpp>

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
    using xo::scm::DConstant;
    using xo::scm::DFloat;
    using xo::scm::DInteger;
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
    using xo::facet::typeseq;
    using xo::reflect::Reflect;
    using xo::InitEvidence;
    using xo::InitSubsys;
    using xo::scope;

    // Ensure subsystem initialized before tests
    static InitEvidence s_init = InitSubsys<S_expression2_tag>::require();

    TEST_CASE("DConstant-init", "[expression2][DConstant]")
    {
        // Verify subsystem initialization succeeded
        REQUIRE(s_init.evidence());
    }

    TEST_CASE("DConstant-from-float", "[expression2][DConstant]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dconstant_float_test",
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

        // Box a float value
        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 3.14);
        REQUIRE(fval.data() != nullptr);

        // Create DConstant from the boxed float
        auto expr = DConstant::make(alloc, fval);
        REQUIRE(expr.data() != nullptr);

        // Verify expression type
        REQUIRE(expr.data()->extype() == xo::scm::exprtype::constant);

        // Verify valuetype is double (DFloat::value_type)
        REQUIRE(expr.data()->valuetype() == Reflect::require<double>());

        // Verify value is accessible
        REQUIRE(expr.data()->value().data() != nullptr);
    }

    TEST_CASE("DConstant-from-integer", "[expression2][DConstant]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dconstant_int_test",
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

        // Box an integer value
        obj<AGCObject> ival = DInteger::box<AGCObject>(alloc, 42);
        REQUIRE(ival.data() != nullptr);

        // Create DConstant from the boxed integer
        auto expr = DConstant::make(alloc, ival);
        REQUIRE(expr.data() != nullptr);

        // Verify expression type
        REQUIRE(expr.data()->extype() == xo::scm::exprtype::constant);

        // Verify valuetype is long (DInteger::value_type)
        REQUIRE(expr.data()->valuetype() == Reflect::require<long>());

        // Verify value is accessible
        REQUIRE(expr.data()->value().data() != nullptr);
    }

    TEST_CASE("DConstant-pretty-float", "[expression2][DConstant][pp]")
    {
        scope log(XO_DEBUG(true));

        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dconstant_pp_float_test",
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

        // Box a float value
        obj<AGCObject> fval = DFloat::box<AGCObject>(alloc, 2.718);
        auto expr = DConstant::make(alloc, fval);
        REQUIRE(expr.data() != nullptr);

        // Pretty print
        std::stringstream ss;
        ppconfig ppc;
        ppstate_standalone pps(&ss, 0, &ppc);

        obj<APrintable,DConstant> expr_pr(expr.data());
        pps.pretty(expr_pr);

        std::string output = ss.str();

        log && log(output);

        // Output should contain "DConstant" struct name
        CHECK(output.find("DConstant") != std::string::npos);
    }

    TEST_CASE("DConstant-pretty-integer", "[expression2][DConstant][pp]")
    {
        scope log(XO_DEBUG(false));

        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dconstant_pp_int_test",
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

        // Box an integer value
        obj<AGCObject> ival = DInteger::box<AGCObject>(alloc, 123);
        auto expr = DConstant::make(alloc, ival);
        REQUIRE(expr.data() != nullptr);

        // Pretty print
        std::stringstream ss;
        ppconfig ppc;
        ppstate_standalone pps(&ss, 0, &ppc);

        obj<APrintable,DConstant> expr_pr(expr.data());
        pps.pretty(expr_pr);

        std::string output = ss.str();

        log && log(output);

        // Output should contain "DConstant" struct name
        CHECK(output.find("DConstant") != std::string::npos);
    }
}

/* end DConstant.test.cpp */
