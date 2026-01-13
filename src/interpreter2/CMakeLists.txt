# interpreter2/CMakeLists.txt

set(SELF_LIB xo_interpreter2)
set(SELF_SRCS
    VirtualSchematikaMachine.cpp
    #IExpression_Any.cpp
    #interpreter2_register_facets.cpp
    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_interpreter2Config.cmake.in
xo_dependency(${SELF_LIB} xo_expression2)
xo_dependency(${SELF_LIB} xo_gc)
#xo_dependency(${SELF_LIB} reflect)
#xo_dependency(${SELF_LIB} xo_printable2)
#xo_dependency(${SELF_LIB} xo_flatstring)
#xo_dependency(${SELF_LIB} indentlog)
