# reader2/CMakeLists.txt

set(SELF_LIB xo_reader2)
set(SELF_SRCS
    init_reader2.cpp
    SetupReader2.cpp

    SchematikaReader.cpp
    ReaderConfig.cpp

    SchematikaParser.cpp
    ParserStateMachine.cpp
    ParserStack.cpp
    ParserResult.cpp

    DGlobalEnv.cpp
    facet/IGCObject_DGlobalEnv.cpp
    facet/IPrintable_DGlobalEnv.cpp

    syntaxstatetype.cpp
    ISyntaxStateMachine_Any.cpp

    DToplevelSeqSsm.cpp
    ISyntaxStateMachine_DToplevelSeqSsm.cpp
    IPrintable_DToplevelSeqSsm.cpp

    DDefineSsm.cpp
    ISyntaxStateMachine_DDefineSsm.cpp
    IPrintable_DDefineSsm.cpp

    DDeftypeSsm.cpp
    facet/ISyntaxStateMachine_DDeftypeSsm.cpp
    facet/IPrintable_DDeftypeSsm.cpp

    DIfElseSsm.cpp
    facet/ISyntaxStateMachine_DIfElseSsm.cpp
    facet/IPrintable_DIfElseSsm.cpp

    DSequenceSsm.cpp
    ISyntaxStateMachine_DSequenceSsm.cpp
    IPrintable_DSequenceSsm.cpp

    DLambdaSsm.cpp
    facet/ISyntaxStateMachine_DLambdaSsm.cpp
    facet/IPrintable_DLambdaSsm.cpp

    DApplySsm.cpp
    facet/ISyntaxStateMachine_DApplySsm.cpp
    facet/IPrintable_DApplySsm.cpp

    DParenSsm.cpp
    ISyntaxStateMachine_DParenSsm.cpp
    IPrintable_DParenSsm.cpp

    DExpectFormalArglistSsm.cpp
    ISyntaxStateMachine_DExpectFormalArglistSsm.cpp
    IPrintable_DExpectFormalArglistSsm.cpp

    DExpectFormalArgSsm.cpp
    facet/ISyntaxStateMachine_DExpectFormalArgSsm.cpp
    facet/IPrintable_DExpectFormalArgSsm.cpp

    DExpectSymbolSsm.cpp
    ISyntaxStateMachine_DExpectSymbolSsm.cpp
    IPrintable_DExpectSymbolSsm.cpp

    DExpectTypeSsm.cpp
    ISyntaxStateMachine_DExpectTypeSsm.cpp
    IPrintable_DExpectTypeSsm.cpp

    DExpectListTypeSsm.cpp
    facet/ISyntaxStateMachine_DExpectListTypeSsm.cpp
    facet/IPrintable_DExpectListTypeSsm.cpp

    DExpectExprSsm.cpp
    ISyntaxStateMachine_DExpectExprSsm.cpp
    IPrintable_DExpectExprSsm.cpp

    DExpectQLiteralSsm.cpp
    ISyntaxStateMachine_DExpectQLiteralSsm.cpp
    IPrintable_DExpectQLiteralSsm.cpp

    DExpectQListSsm.cpp
    ISyntaxStateMachine_DExpectQListSsm.cpp
    IPrintable_DExpectQListSsm.cpp

    DExpectQArraySsm.cpp
    ISyntaxStateMachine_DExpectQArraySsm.cpp
    IPrintable_DExpectQArraySsm.cpp

    DExpectQDictSsm.cpp
    facet/ISyntaxStateMachine_DExpectQDictSsm.cpp
    facet/IPrintable_DExpectQDictSsm.cpp

    DProgressSsm.cpp
    ISyntaxStateMachine_DProgressSsm.cpp
    IPrintable_DProgressSsm.cpp

    DQuoteSsm.cpp
    ISyntaxStateMachine_DQuoteSsm.cpp
    IPrintable_DQuoteSsm.cpp

    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_expression2Config.cmake.in
xo_dependency(${SELF_LIB} xo_numeric)
xo_dependency(${SELF_LIB} xo_procedure2)
xo_dependency(${SELF_LIB} xo_gc)
xo_dependency(${SELF_LIB} xo_type)
xo_dependency(${SELF_LIB} xo_tokenizer2)
xo_dependency(${SELF_LIB} xo_expression2)
#xo_dependency(${SELF_LIB} reflect)
#xo_dependency(${SELF_LIB} xo_flatstring)
xo_dependency(${SELF_LIB} subsys)
#xo_dependency(${SELF_LIB} indentlog)
