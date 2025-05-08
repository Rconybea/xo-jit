# jit/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_jit VERSION 0.1)

include(GNUInstallDirs)
include(cmake/xo-bootstrap-macros.cmake)

xo_cxx_toplevel_options3()

# ----------------------------------------------------------------
# c++ settings

set(PROJECT_CXX_FLAGS "")
#set(PROJECT_CXX_FLAGS "-fconcepts-diagnostics-depth=2")  # gcc-only!
add_definitions(${PROJECT_CXX_FLAGS})

# ----------------------------------------------------------------

# must complete definition of jit lib before configuring examples
add_subdirectory(src/jit)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# ----------------------------------------------------------------

add_subdirectory(example)
add_subdirectory(utest)

# reminder: must come last: docs targets depend on all the other library/utest targets
add_subdirectory(docs)

# end CMakeLists.txt
