# xo-reader2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

#set(CMAKE_CXX_STANDARD 23)

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

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-gcobject-schematikaparser
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DSchematikaParser.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacet(
    TARGET xo-reader2-facet-syntaxstatemachine
    FACET SyntaxStateMachine
    INPUT idl/SyntaxStateMachine.json5
    )

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-toplevelseqssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DToplevelSeqSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-toplevelseqssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DToplevelSeqSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-definessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DDefineSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-definessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DDefineSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-deftypessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DDeftypeSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-deftypessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DDeftypeSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-lambdassm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DLambdaSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-lambdassm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DLambdaSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-parenssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DParenSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-parenssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DParenSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectformalarglistssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectFormalArglistSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectformalarglistssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectFormalArglistSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectformalargssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectFormalArgSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectformalargssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectFormalArgSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-ifelsessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DIfElseSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-ifelsessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DIfElseSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-sequencessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DSequenceSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-sequencessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DSequenceSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-applyssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DApplySsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-applyssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DApplySsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectsymbolssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectSymbolSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectsymbolssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectSymbolSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expecttypessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectTypeSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expecttypessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectTypeSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectlisttypessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectListTypeSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectlisttypessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectListTypeSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectexprssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectExprSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectexprssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectExprSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-progressssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DProgressSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-progressssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DProgressSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-quotessm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DQuoteSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-quotessm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DQuoteSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectqliteralssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectQLiteralSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectqliteralssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectQLiteralSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectqlistssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectQListSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectqlistssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectQListSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectqdictssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectQDictSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectqdictssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectQDictSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectqarrayssm
    FACET_PKG xo_reader2
    INPUT idl/ISyntaxStateMachine_DExpectQArraySsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectqarrayssm
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DExpectQArraySsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-gcobject-globalenv
    FACET_PKG xo_alloc2
    INPUT idl/IGCObject_DGlobalEnv.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-globalenv
    FACET_PKG xo_printable2
    INPUT idl/IPrintable_DGlobalEnv.json5
)

# ----------------------------------------------------------------

xo_add_genfacet_all(xo-reader2-genfacet-all)

# ----------------------------------------------------------------
# shared library

add_subdirectory(src/reader2)

# ----------------------------------------------------------------
# example programs

add_subdirectory(example)

# ----------------------------------------------------------------
# cmake helper (for external xo-reader2 users)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# end CMakeLists.txt
