/* file expression2_utest_main.cpp */

#include <xo/subsys/Subsystem.hpp>

#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

int
main(int argc, char* argv[])
{
    using xo::Subsystem;

    // Your custom initialization code here
    Subsystem::initialize_all();

    // Run Catch2's test session
    int result = Catch::Session().run(argc, argv);

    // cleanup here, if any

    return result;
}

/* end expression2_utest_main.cpp */
