# numeric/CMakeLists.txt

set(SELF_LIB xo_numeric)
set(SELF_SRCS
    init_numeric.cpp
    numeric_register_facets.cpp
    NumericPrimitives.cpp
    NumericDispatch.cpp
    INumeric_Any.cpp
    FloatIntegerOps.cpp
    FloatOps.cpp
    IntegerOps.cpp
)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
xo_dependency(${SELF_LIB} xo_procedure2)
xo_dependency(${SELF_LIB} subsys)

xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

# ----------------------------------------------------------------
# docs targets depend on other library/utest/exec targets above,
# --> must come after them.
#
#add_subdirectory(docs)
