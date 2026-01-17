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

add_subdirectory(utest)

# note: manual target; generated code committed to git
xo_add_genfacet(
    TARGET xo-expression2-facet-symboltable
    FACET SymbolTable
    INPUT idl/SymbolTable.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
    )

# note: manual target; generated code committed to git
xo_add_genfacet(
    TARGET xo-expression2-facet-expression
    FACET Expression
    INPUT idl/Expression.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
    )

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-constant
    FACET_PKG xo_expression2
    FACET Expression
    REPR Constant
    INPUT idl/IExpression_DConstant.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-variable
    FACET_PKG xo_expression2
    FACET Expression
    REPR Variable
    INPUT idl/IExpression_DVariable.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-uniquestring
    FACET_PKG xo_gc
    FACET GCObject
    REPR UniqueString
    INPUT idl/IGCObject_DUniqueString.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-uniquestring
    FACET_PKG xo_printable2
    FACET Printable
    REPR DUniqueString
    INPUT idl/IPrintable_DUniqueString.json5
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
