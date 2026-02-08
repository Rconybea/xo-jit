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

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-symboltable-localsymtab
    FACET_PKG xo_expression2
    FACET SymbolTable
    REPR LocalSymtab
    INPUT idl/ISymbolTable_DLocalSymtab.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR symtab
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-localsymtab
    FACET_PKG xo_gc
    FACET GCObject
    REPR LocalSymtab
    INPUT idl/IGCObject_DLocalSymtab.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR symtab
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-localsymtab
    FACET_PKG xo_printable2
    FACET Printable
    REPR LocalSymtab
    INPUT idl/IPrintable_DLocalSymtab.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR symtab
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

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
    TARGET xo-expression2-facetimpl-gcobject-constant
    FACET_PKG xo_gc
    FACET GCObject
    REPR Constant
    INPUT idl/IGCObject_DConstant.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-constant
    FACET_PKG xo_printable2
    FACET Printable
    REPR Constant
    INPUT idl/IPrintable_DConstant.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

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
    TARGET xo-expression2-facetimpl-gcobject-variable
    FACET_PKG xo_gc
    FACET GCObject
    REPR Variable
    INPUT idl/IGCObject_DVariable.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-variable
    FACET_PKG xo_printable2
    FACET Printable
    REPR Variable
    INPUT idl/IPrintable_DVariable.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-varref
    FACET_PKG xo_expression2
    FACET Expression
    REPR VarRef
    INPUT idl/IExpression_DVarRef.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-varref
    FACET_PKG xo_gc
    FACET GCObject
    REPR VarRef
    INPUT idl/IGCObject_DVarRef.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-varref
    FACET_PKG xo_printable2
    FACET Printable
    REPR VarRef
    INPUT idl/IPrintable_DVarRef.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-defineexpr
    FACET_PKG xo_expression2
    FACET Expression
    REPR DefineExpr
    INPUT idl/IExpression_DDefineExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-defineexpr
    FACET_PKG xo_printable2
    FACET Printable
    REPR DefineExpr
    INPUT idl/IPrintable_DDefineExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-applyexpr
    FACET_PKG xo_expression2
    FACET Expression
    REPR ApplyExpr
    INPUT idl/IExpression_DApplyExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-applyexpr
    FACET_PKG xo_gc
    FACET GCObject
    REPR ApplyExpr
    INPUT idl/IGCObject_DApplyExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-applyexpr
    FACET_PKG xo_printable2
    FACET Printable
    REPR ApplyExpr
    INPUT idl/IPrintable_DApplyExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-lambdaexpr
    FACET_PKG xo_expression2
    FACET Expression
    REPR LambdaExpr
    INPUT idl/IExpression_DLambdaExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-lambdaexpr
    FACET_PKG xo_gc
    FACET GCObject
    REPR LambdaExpr
    INPUT idl/IGCObject_DLambdaExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-lambdaexpr
    FACET_PKG xo_printable2
    FACET Printable
    REPR LambdaExpr
    INPUT idl/IPrintable_DLambdaExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-ifelseexpr
    FACET_PKG xo_expression2
    FACET Expression
    REPR IfElseExpr
    INPUT idl/IExpression_DIfElseExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-ifelseexpr
    FACET_PKG xo_gc
    FACET GCObject
    REPR IfElseExpr
    INPUT idl/IGCObject_DIfElseExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-ifelseexpr
    FACET_PKG xo_printable2
    FACET Printable
    REPR IfElseExpr
    INPUT idl/IPrintable_DIfElseExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-expression-sequenceexpr
    FACET_PKG xo_expression2
    FACET Expression
    REPR SequenceExpr
    INPUT idl/IExpression_DSequenceExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-gcobject-sequenceexpr
    FACET_PKG xo_gc
    FACET GCObject
    REPR SequenceExpr
    INPUT idl/IGCObject_DSequenceExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# note: manual target; generated code committed to git
xo_add_genfacetimpl(
    TARGET xo-expression2-facetimpl-printable-sequenceexpr
    FACET_PKG xo_printable2
    FACET Printable
    REPR SequenceExpr
    INPUT idl/IPrintable_DSequenceExpr.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

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
    REPR UniqueString
    INPUT idl/IPrintable_DUniqueString.json5
    OUTPUT_HPP_DIR include/xo/expression2
    OUTPUT_IMPL_SUBDIR detail
    OUTPUT_CPP_DIR src/expression2
)

# ----------------------------------------------------------------

xo_add_genfacet_all(xo-expression2-genfacet-all)

# ----------------------------------------------------------------
# shared library

add_subdirectory(src/expression2)

# ----------------------------------------------------------------
# cmake helper (for external xo-expression2 users)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# end CMakeLists.txt
