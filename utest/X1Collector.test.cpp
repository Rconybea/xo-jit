/** @file X1Collector.test.cpp
 *
 *  Minimal collector test for DUniqueString
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include "StringTable.hpp"
#include "detail/IGCObject_DUniqueString.hpp"

#include <xo/object2/DFloat.hpp>
#include <xo/object2/DInteger.hpp>
#include <xo/object2/DList.hpp>
#include <xo/object2/DString.hpp>

#include <xo/gc/CollectorTypeRegistry.hpp>
#include <xo/gc/DX1Collector.hpp>

#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/gc/detail/ICollector_DX1Collector.hpp>

#include <xo/subsys/Subsystem.hpp>

#include <catch2/catch.hpp>

#include <cstring>

namespace ut {
    using xo::S_expression2_tag;
    using xo::scm::StringTable;
    using xo::scm::DUniqueString;
    using xo::scm::DFloat;
    using xo::scm::DInteger;
    using xo::scm::DList;
    using xo::scm::DString;
    using xo::mm::CollectorTypeRegistry;
    using xo::mm::AAllocator;
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::mm::DX1Collector;
    using xo::mm::X1CollectorConfig;
    using xo::mm::ArenaConfig;
    using xo::facet::with_facet;
    using xo::facet::typeseq;
    using xo::InitEvidence;
    using xo::InitSubsys;

    // Ensure subsystem initialized before tests
    static InitEvidence s_init = InitSubsys<S_expression2_tag>::require();

    TEST_CASE("x1_duniquestring", "[gc][x1][expression2]")
    {
        REQUIRE(s_init.evidence());

        // Create collector
        X1CollectorConfig cfg{ .name_ = "x1_duniquestring_test",
                               .arena_config_ = ArenaConfig{
                                   .size_ = 8192,
                                   .store_header_flag_ = true},
                               .object_types_z_ = 16384,
                               .gc_trigger_v_{{1024, 1024}},
                               .debug_flag_ = false,
        };

        DX1Collector gc(cfg);

        auto gc_o = with_facet<AAllocator>::mkobj(&gc);
        auto c_o = with_facet<ACollector>::mkobj(&gc);

        // Install types from CollectorTypeRegistry
        bool ok = CollectorTypeRegistry::instance().install_types(c_o);
        REQUIRE(ok);

        // Verify types from object2 are installed
        ok = c_o.is_type_installed(typeseq::id<DFloat>());
        REQUIRE(ok);
        ok = c_o.is_type_installed(typeseq::id<DInteger>());
        REQUIRE(ok);
        ok = c_o.is_type_installed(typeseq::id<DList>());
        REQUIRE(ok);
        ok = c_o.is_type_installed(typeseq::id<DString>());
        REQUIRE(ok);

        // Verify DUniqueString from expression2 is installed
        ok = c_o.is_type_installed(typeseq::id<DUniqueString>());
        REQUIRE(ok);

        // Create DUniqueString via StringTable
        StringTable table(1024);
        const DUniqueString * ustr = table.intern("hello");
        REQUIRE(ustr != nullptr);
        REQUIRE(ustr->size() == 5);

        // Wrap as obj<AGCObject>
        // Note: const_cast is safe here since DUniqueString is immutable by design,
        // and we're only wrapping for facet access, not modification
        auto ustr_o = with_facet<AGCObject>::mkobj(const_cast<DUniqueString*>(ustr));
        REQUIRE(ustr_o.iface() != nullptr);
        REQUIRE(ustr_o.data() != nullptr);

        // verify unique string contents
        REQUIRE(strcmp(ustr_o.data()->chars(), "hello") == 0);

        // Add as gc root
        c_o.add_gc_root(&ustr_o);

        // Verify string content accessible through wrapped object
        REQUIRE(ustr_o.data()->size() == 5);
        REQUIRE(std::strcmp(ustr_o.data()->chars(), "hello") == 0);
    }
}

/* end X1Collector.test.cpp */
