/** @file interpreter2_utest_main.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/facet/FacetRegistry.hpp>
#include <xo/subsys/Subsystem.hpp>

#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

int
main(int argc, char* argv[])
{
    using xo::facet::FacetRegistry;
    using xo::Subsystem;

    // initialize facet registry
    FacetRegistry::instance(1024);

    // initialize subsystems
    Subsystem::initialize_all();

    // Run Catch2's test session
    int result = Catch::Session().run(argc, argv);

    // cleanup here, if any

    return result;
}

/* end interpreter2_utest_main.cpp */
