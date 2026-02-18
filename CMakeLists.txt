# xo-numeric/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_numeric VERSION 1.0)
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

# note: manual target; generated code committed to git
xo_add_genfacet(
    TARGET xo-numeric-facet-numeric
    FACET Numeric
    INPUT idl/Numeric.json5
    )

# ----------------------------------------------------------------

xo_add_genfacet_all(xo-numeric-genfacet-all)

# ----------------------------------------------------------------

#add_subdirectory(src/numeric)   # NOTE: will need cmake export here
#add_subdirectory(utest)

# ----------------------------------------------------------------
# cmake export


