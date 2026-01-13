# xo-expression2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_expression2 VERSION 1.0)
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

# note: manual target; generated code committed to git
xo_add_genfacet(
    TARGET xo-expression2-facet-expression
    FACET Expression
    INPUT idl/Expression.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
    )

# ----------------------------------------------------------------
# header-only library

add_subdirectory(src/expression2)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# ----------------------------------------------------------------
# input dependencies
#
# NOTE: dependency set here must be kept consistent with
#       xo-expression2/cmake/xo_expression2Config.cmake.in

#xo_headeronly_dependency(${SELF_LIB} xo_flatstring)

# end CMakeLists.txt
