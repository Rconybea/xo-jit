# reader2/CMakeLists.txt

set(SELF_LIB xo_reader2)
set(SELF_SRCS
    #init_reader2.cpp

    ParserStateMachine.cpp
    ParserStack.cpp
    ParserResult.cpp

    ISyntaxStateMachine_Any.cpp

    DExprSeqState.cpp
    ISyntaxStateMachine_DExprSeqState.cpp

    reader2_register_facets.cpp
    #reader2_register_types.cpp
    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_expression2Config.cmake.in
xo_dependency(${SELF_LIB} xo_gc)
xo_dependency(${SELF_LIB} xo_tokenizer2)
xo_dependency(${SELF_LIB} xo_expression2)
#xo_dependency(${SELF_LIB} reflect)
#xo_dependency(${SELF_LIB} xo_object2)
#xo_dependency(${SELF_LIB} xo_printable2)
#xo_dependency(${SELF_LIB} xo_flatstring)
#xo_dependency(${SELF_LIB} subsys)
#xo_dependency(${SELF_LIB} indentlog)
