# xo-interpreter2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_interpreter2 VERSION 1.0)
enable_language(CXX)

include(GNUInstallDirs)
include(cmake/xo-bootstrap-macros.cmake)

xo_cxx_toplevel_options3()

# ----------------------------------------------------------------
# c++ settings

# one-time project-specific c++ flags. usually empty
set(PROJECT_CXX_FLAGS "")
add_definitions(${PROJECT_CXX_FLAGS})

# ----------------------------------------------------------------
# output targets

#add_subdirectory(utest)

# ----------------------------------------------------------------
# header-only library

set(SELF_LIB xo_interpreter2)
xo_add_headeronly_library(${SELF_LIB})
xo_install_library4(${SELF_LIB} ${PROJECT_NAME}Targets)
xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# ----------------------------------------------------------------
# input dependencies
#
# NOTE: dependency set here must be kept consistent with
#       xo-interpreter2/cmake/xo_interpreter2Config.cmake.in

#xo_headeronly_dependency(${SELF_LIB} xo_flatstring)

# end CMakeLists.txt
