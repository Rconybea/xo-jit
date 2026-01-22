# xo-reader2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_reader2 VERSION 1.0)
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
    TARGET xo-reader2-facet-syntaxstatemachine
    FACET SyntaxStateMachine
    INPUT idl/SyntaxStateMachine.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
    )

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-exprseqstate
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExprSeqState
    INPUT idl/ISyntaxStateMachine_DExprSeqState.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-exprseqstate
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExprSeqState
    INPUT idl/IPrintable_DExprSeqState.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-definessm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR DefineSsm
    INPUT idl/ISyntaxStateMachine_DDefineSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-definessm
    FACET_PKG xo_printable2
    FACET Printable
    REPR DefineSsm
    INPUT idl/IPrintable_DDefineSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectsymbolssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectSymbolSsm
    INPUT idl/ISyntaxStateMachine_DExpectSymbolSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectsymbolssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectSymbolSsm
    INPUT idl/IPrintable_DExpectSymbolSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expecttypessm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectTypeSsm
    INPUT idl/ISyntaxStateMachine_DExpectTypeSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expecttypessm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectTypeSsm
    INPUT idl/IPrintable_DExpectTypeSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectexprssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectExprSsm
    INPUT idl/ISyntaxStateMachine_DExpectExprSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectexprssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectExprSsm
    INPUT idl/IPrintable_DExpectExprSsm.json5
    OUTPUT_HPP_DIR include/xo/reader2
    OUTPUT_IMPL_SUBDIR ssm
    OUTPUT_CPP_DIR src/reader2
)

# ----------------------------------------------------------------

xo_add_genfacet_all(xo-reader2-genfacet-all)

# ----------------------------------------------------------------
# shared library

add_subdirectory(src/reader2)

# ----------------------------------------------------------------
# cmake helper (for external xo-reader2 users)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# end CMakeLists.txt
