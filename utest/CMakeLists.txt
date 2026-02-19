# built unittest xo-numeric/utest

set(UTEST_EXE utest.numeric)
set(UTEST_SRCS
    numeric_utest_main.cpp
    Numeric.test.cpp
)

xo_add_utest_executable(${UTEST_EXE} ${UTEST_SRCS})
xo_self_dependency(${UTEST_EXE} xo_numeric)
#xo_dependency(${UTEST_EXE} randomgen)
xo_external_target_dependency(${UTEST_EXE} Catch2 Catch2::Catch2)
