/** @file Numeric.test.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "init_numeric.hpp"
#include "NumericDispatch.hpp"
#include <xo/facet/FacetRegistry.hpp>
#include <xo/facet/TypeRegistry.hpp>
#include <catch2/catch.hpp>

namespace xo {
    using xo::scm::NumericDispatch;
    //using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::mm::ArenaConfig;;
    using xo::mm::MemorySizeInfo;
    using xo::facet::FacetRegistry;
    using xo::facet::TypeRegistry;
    //using xo::facet::with_facet;
    //using xo::facet::obj;

    namespace ut {

        namespace {
            struct Fixture {
                explicit Fixture(const std::string & testname,
                                 std::size_t aux_arena_size = 16 * 1024)
                    : aux_arena_(
                        ArenaConfig().with_name(testname).with_size(aux_arena_size))
                    {}


                bool log_memory_layout(scope * p_log) {
                    auto visitor = [p_log](const MemorySizeInfo & info) {
                        *p_log && (*p_log)(xtag("resource", info.resource_name_),
                                           xtag("used", info.used_),
                                           xtag("alloc", info.allocated_),
                                           xtag("commit", info.committed_),
                                           xtag("resv", info.reserved_));
                    };

                    aux_arena_.visit_pools(visitor);
                    TypeRegistry::instance().visit_pools(visitor);
                    FacetRegistry::instance().visit_pools(visitor);
                    NumericDispatch::instance().visit_pools(visitor);

                    return true;
                }

                DArena aux_arena_;
            };
        } /*namespace*/

        static InitEvidence s_init = (InitSubsys<S_numeric_tag>::require());

        TEST_CASE("Numeric-init" "[numeric]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            Fixture fixture(testname);
            {
                // real purpose: ensure s_init sutvives static linking
                REQUIRE(s_init.evidence());
            }

            log && fixture.log_memory_layout(&log);
        }

    } /*namespace ut*/
} /*namespace xo*/

/* end Numeric.test.cpp */
