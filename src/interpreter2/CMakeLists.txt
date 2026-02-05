# interpreter2/CMakeLists.txt

set(SELF_LIB xo_interpreter2)
set(SELF_SRCS
    init_interpreter2.cpp
    interpreter2_register_facets.cpp
    interpreter2_register_types.cpp

    VirtualSchematikaMachine.cpp

    DVsmEvalArgsFrame.cpp
    IGCObject_DVsmEvalArgsFrame.cpp
    IPrintable_DVsmEvalArgsFrame.cpp

    DVsmApplyFrame.cpp
    IGCObject_DVsmApplyFrame.cpp
    IPrintable_DVsmApplyFrame.cpp

    IProcedure_DClosure.cpp

    VsmInstr.cpp

    DClosure.cpp
    DLocalEnv.cpp

    #IExpression_Any.cpp
    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_interpreter2Config.cmake.in
xo_dependency(${SELF_LIB} xo_reader2)
xo_dependency(${SELF_LIB} xo_gc)
#xo_dependency(${SELF_LIB} reflect)
#xo_dependency(${SELF_LIB} xo_printable2)
#xo_dependency(${SELF_LIB} xo_flatstring)
#xo_dependency(${SELF_LIB} indentlog)
