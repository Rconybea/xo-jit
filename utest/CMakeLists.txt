# build unittest xo-reader2/utest

set(UTEST_EXE utest.reader2)
set(UTEST_SRCS
    reader2_utest_main.cpp
    SchematikaParser.test.cpp
)

xo_add_utest_executable(${UTEST_EXE} ${UTEST_SRCS})
xo_self_dependency(${UTEST_EXE} xo_reader2)
xo_external_target_dependency(${UTEST_EXE} Catch2 Catch2::Catch2)
