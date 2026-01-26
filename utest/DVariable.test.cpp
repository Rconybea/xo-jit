/** @file DVariable.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include <xo/expression2/DVariable.hpp>
#include <xo/expression2/detail/IExpression_DVariable.hpp>
#include <xo/expression2/detail/IPrintable_DVariable.hpp>
#include <xo/expression2/StringTable.hpp>

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
    using xo::scm::DVariable;
    using xo::scm::DUniqueString;
    using xo::scm::StringTable;
    using xo::scm::TypeRef;
    using xo::scm::Binding;
    using xo::scm::AExpression;
    using xo::mm::CollectorTypeRegistry;
    using xo::mm::AAllocator;
    using xo::mm::ACollector;
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

    TEST_CASE("DVariable-init", "[expression2][DVariable]")
    {
        REQUIRE(s_init.evidence());
    }

    TEST_CASE("DVariable-make", "[expression2][DVariable]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dvariable_make_test",
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

        TypeRef typeref = TypeRef::resolved(Reflect::require<double>());

        DVariable * var = DVariable::make(alloc, name, typeref);
        REQUIRE(var != nullptr);
    }

    TEST_CASE("DVariable-extype", "[expression2][DVariable]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dvariable_extype_test",
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
        TypeRef typeref = TypeRef::resolved(Reflect::require<long>());

        DVariable * var = DVariable::make(alloc, name, typeref);
        REQUIRE(var != nullptr);
        REQUIRE(var->extype() == xo::scm::exprtype::variable);
    }

    TEST_CASE("DVariable-valuetype", "[expression2][DVariable]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dvariable_valuetype_test",
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
        TypeRef typeref = TypeRef::resolved(Reflect::require<double>());

        DVariable * var = DVariable::make(alloc, name, typeref);
        REQUIRE(var != nullptr);
        REQUIRE(var->valuetype() == Reflect::require<double>());
    }

    TEST_CASE("DVariable-name", "[expression2][DVariable]")
    {
        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dvariable_name_test",
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
        TypeRef typeref = TypeRef::resolved(Reflect::require<double>());

        DVariable * var = DVariable::make(alloc, name, typeref);
        REQUIRE(var != nullptr);
        REQUIRE(var->name() == name);
        REQUIRE(std::strcmp(var->name()->chars(), "myvar") == 0);
    }

    TEST_CASE("DVariable-pretty", "[expression2][DVariable][pp]")
    {
        scope log(XO_DEBUG(false));

        REQUIRE(s_init.evidence());

        CollectorConfig cfg{
            .name_ = "dvariable_pretty_test",
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
        TypeRef typeref = TypeRef::resolved(Reflect::require<double>());

        DVariable * var = DVariable::make(alloc, name, typeref);
        REQUIRE(var != nullptr);

        std::stringstream ss;
        ppconfig ppc;
        ppstate_standalone pps(&ss, 0, &ppc);

        obj<APrintable,DVariable> var_pr(var);
        pps.pretty(var_pr);

        std::string output = ss.str();

        log && log(output);

        CHECK(output.find("DVariable") != std::string::npos);
    }
}

/* end DVariable.test.cpp */
