# interpreter2/CMakeLists.txt

set(SELF_LIB xo_interpreter2)
set(SELF_SRCS
    init_interpreter2.cpp
    interpreter2_register_facets.cpp
    interpreter2_register_types.cpp

    VirtualSchematikaMachine.cpp

    DVsmDefContFrame.cpp
    IGCObject_DVsmDefContFrame.cpp
    IPrintable_DVsmDefContFrame.cpp

    DVsmEvalArgsFrame.cpp
    IGCObject_DVsmEvalArgsFrame.cpp
    IPrintable_DVsmEvalArgsFrame.cpp

    DVsmApplyFrame.cpp
    IGCObject_DVsmApplyFrame.cpp
    IPrintable_DVsmApplyFrame.cpp

    DVsmApplyClosureFrame.cpp
    IGCObject_DVsmApplyClosureFrame.cpp
    IPrintable_DVsmApplyClosureFrame.cpp

    DVsmIfElseContFrame.cpp
    IGCObject_DVsmIfElseContFrame.cpp
    IPrintable_DVsmIfElseContFrame.cpp

    DVsmSeqContFrame.cpp
    IGCObject_DVsmSeqContFrame.cpp
    IPrintable_DVsmSeqContFrame.cpp

    DClosure.cpp
    IGCObject_DClosure.cpp
    IPrintable_DClosure.cpp

    DGlobalEnv.cpp
    IGCObject_DGlobalEnv.cpp
    IPrintable_DGlobalEnv.cpp

    DLocalEnv.cpp
    IGCObject_DLocalEnv.cpp
    IPrintable_DLocalEnv.cpp

    DVsmRcx.cpp
    IRuntimeContext_DVsmRcx.cpp

    VsmInstr.cpp
    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_interpreter2Config.cmake.in
xo_dependency(${SELF_LIB} xo_reader2)
xo_dependency(${SELF_LIB} xo_gc)
#xo_dependency(${SELF_LIB} reflect)
#xo_dependency(${SELF_LIB} xo_printable2)
#xo_dependency(${SELF_LIB} xo_flatstring)
#xo_dependency(${SELF_LIB} indentlog)
