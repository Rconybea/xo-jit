# built unittest xo-expression2/utest

set(UTEST_EXE utest.expression2)
set(UTEST_SRCS
    expression2_utest_main.cpp
    StringTable.test.cpp
    X1Collector.test.cpp
)

xo_add_utest_executable(${UTEST_EXE} ${UTEST_SRCS})
xo_self_dependency(${UTEST_EXE} xo_expression2)
xo_external_target_dependency(${UTEST_EXE} Catch2 Catch2::Catch2)
