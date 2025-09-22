# xo-flatstring/CMakeLists.txt

cmake_minimum_required(VERSION 3.25)

project(xo_flatstring VERSION 1.0)

include(GNUInstallDirs)
include(cmake/xo-bootstrap-macros.cmake)

xo_cxx_toplevel_options2()

# ----------------------------------------------------------------
# cmake -DCMAKE_BUILD_TYPE=coverage
xo_toplevel_coverage_config2()

# ----------------------------------------------------------------
# cmake -DCMAKE_BUILD_TYPE=debug
xo_toplevel_debug_config2()

# ----------------------------------------------------------------
# c++ settings

# one-time project-specific c++ flags.  usually empty
set(PROJECT_CXX_FLAGS "")
#set(PROJECT_CXX_FLAGS "-fconcepts-diagnostics-depth=2")
add_definitions(${PROJECT_CXX_FLAGS})

# ----------------------------------------------------------------

add_subdirectory(example)
add_subdirectory(utest)

# ----------------------------------------------------------------

set(SELF_LIB xo_flatstring)
xo_add_headeronly_library(${SELF_LIB})
xo_install_library4(${SELF_LIB} ${PROJECT_NAME}Targets)
# provide find_package() support for projects using this library
xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# ----------------------------------------------------------------

if (XO_ENABLE_EXAMPLES)
    install(TARGETS flatstring_ex1)
endif()

# ----------------------------------------------------------------
# docs targets depend on all the other library/utest targets
#
add_subdirectory(docs)

# ----------------------------------------------------------------
# dependencies

#xo_headeronly_dependency(${SELF_LIB} randomgen)
# etc..

# end CMakeLists.txt
