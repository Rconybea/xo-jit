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
    FACET SyntaxStateMachine
    REPR ToplevelSeqSsm
    INPUT idl/ISyntaxStateMachine_DToplevelSeqSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-toplevelseqssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ToplevelSeqSsm
    INPUT idl/IPrintable_DToplevelSeqSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-definessm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR DefineSsm
    INPUT idl/ISyntaxStateMachine_DDefineSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-definessm
    FACET_PKG xo_printable2
    FACET Printable
    REPR DefineSsm
    INPUT idl/IPrintable_DDefineSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-lambdassm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR LambdaSsm
    INPUT idl/ISyntaxStateMachine_DLambdaSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-lambdassm
    FACET_PKG xo_printable2
    FACET Printable
    REPR LambdaSsm
    INPUT idl/IPrintable_DLambdaSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-parenssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ParenSsm
    INPUT idl/ISyntaxStateMachine_DParenSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-parenssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ParenSsm
    INPUT idl/IPrintable_DParenSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectformalarglistssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectFormalArglistSsm
    INPUT idl/ISyntaxStateMachine_DExpectFormalArglistSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectformalarglistssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectFormalArglistSsm
    INPUT idl/IPrintable_DExpectFormalArglistSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectformalargssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectFormalArgSsm
    INPUT idl/ISyntaxStateMachine_DExpectFormalArgSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectformalargssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectFormalArgSsm
    INPUT idl/IPrintable_DExpectFormalArgSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-ifelsessm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR IfElseSsm
    INPUT idl/ISyntaxStateMachine_DIfElseSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-ifelsessm
    FACET_PKG xo_printable2
    FACET Printable
    REPR IfElseSsm
    INPUT idl/IPrintable_DIfElseSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-sequencessm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR SequenceSsm
    INPUT idl/ISyntaxStateMachine_DSequenceSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-sequencessm
    FACET_PKG xo_printable2
    FACET Printable
    REPR SequenceSsm
    INPUT idl/IPrintable_DSequenceSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-applyssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ApplySsm
    INPUT idl/ISyntaxStateMachine_DApplySsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-applyssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ApplySsm
    INPUT idl/IPrintable_DApplySsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectsymbolssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectSymbolSsm
    INPUT idl/ISyntaxStateMachine_DExpectSymbolSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectsymbolssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectSymbolSsm
    INPUT idl/IPrintable_DExpectSymbolSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expecttypessm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectTypeSsm
    INPUT idl/ISyntaxStateMachine_DExpectTypeSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expecttypessm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectTypeSsm
    INPUT idl/IPrintable_DExpectTypeSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-expectexprssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ExpectExprSsm
    INPUT idl/ISyntaxStateMachine_DExpectExprSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-expectexprssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ExpectExprSsm
    INPUT idl/IPrintable_DExpectExprSsm.json5
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-syntaxstatemachine-progressssm
    FACET_PKG xo_reader2
    FACET SyntaxStateMachine
    REPR ProgressSsm
    INPUT idl/ISyntaxStateMachine_DProgressSsm.json5
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-reader2-facetimpl-printable-progressssm
    FACET_PKG xo_printable2
    FACET Printable
    REPR ProgressSsm
    INPUT idl/IPrintable_DProgressSsm.json5
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
