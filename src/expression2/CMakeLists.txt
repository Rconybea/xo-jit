# expression2/CMakeLists.txt

set(SELF_LIB xo_expression2)
set(SELF_SRCS
    init_expression2.cpp
    expression2_register_facets.cpp
    expression2_register_types.cpp

    DConstant.cpp
    DVariable.cpp
    DVarRef.cpp
    DApplyExpr.cpp

    TypeRef.cpp
    Binding.cpp

    IExpression_Any.cpp
    ISymbolTable_Any.cpp

    IExpression_DConstant.cpp
    IGCObject_DConstant.cpp
    IPrintable_DConstant.cpp

    IExpression_DVariable.cpp
    IGCObject_DVariable.cpp
    IPrintable_DVariable.cpp

    IExpression_DVarRef.cpp
    IGCObject_DVarRef.cpp
    IPrintable_DVarRef.cpp

    DDefineExpr.cpp
    IExpression_DDefineExpr.cpp
    IGCObject_DDefineExpr.cpp
    IPrintable_DDefineExpr.cpp

    IExpression_DApplyExpr.cpp
    IGCObject_DApplyExpr.cpp
    IPrintable_DApplyExpr.cpp

    DLambdaExpr.cpp
    IExpression_DLambdaExpr.cpp
    IGCObject_DLambdaExpr.cpp
    IPrintable_DLambdaExpr.cpp

    DIfElseExpr.cpp
    IExpression_DIfElseExpr.cpp
    IGCObject_DIfElseExpr.cpp
    IPrintable_DIfElseExpr.cpp

    DSequenceExpr.cpp
    IExpression_DSequenceExpr.cpp
    IGCObject_DSequenceExpr.cpp
    IPrintable_DSequenceExpr.cpp

    DLocalSymtab.cpp
    ISymbolTable_DLocalSymtab.cpp
    IGCObject_DLocalSymtab.cpp
    IPrintable_DLocalSymtab.cpp

    DGlobalSymtab.cpp
    ISymbolTable_DGlobalSymtab.cpp
    IGCObject_DGlobalSymtab.cpp
    IPrintable_DGlobalSymtab.cpp

    StringTable.cpp

    DUniqueString.cpp
    IGCObject_DUniqueString.cpp
    IPrintable_DUniqueString.cpp

    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_expression2Config.cmake.in
xo_dependency(${SELF_LIB} xo_gc)
xo_dependency(${SELF_LIB} reflect)
xo_dependency(${SELF_LIB} xo_procedure2)
xo_dependency(${SELF_LIB} xo_printable2)
xo_dependency(${SELF_LIB} xo_flatstring)
xo_dependency(${SELF_LIB} subsys)
xo_dependency(${SELF_LIB} indentlog)
