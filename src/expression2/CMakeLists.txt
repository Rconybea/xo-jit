# expression2/CMakeLists.txt

set(SELF_LIB xo_expression2)
set(SELF_SRCS
    init_expression2.cpp
    DConstant.cpp
    TypeRef.cpp
    IExpression_Any.cpp
    IExpression_DConstant.cpp
    StringTable.cpp
    DUniqueString.cpp
    IGCObject_DUniqueString.cpp
    expression2_register_facets.cpp
    expression2_register_types.cpp
    )

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# note: deps here must also appear in cmake/xo_expression2Config.cmake.in
xo_dependency(${SELF_LIB} xo_gc)
xo_dependency(${SELF_LIB} reflect)
xo_dependency(${SELF_LIB} xo_object2)
xo_dependency(${SELF_LIB} xo_printable2)
xo_dependency(${SELF_LIB} xo_flatstring)
xo_dependency(${SELF_LIB} subsys)
xo_dependency(${SELF_LIB} indentlog)
