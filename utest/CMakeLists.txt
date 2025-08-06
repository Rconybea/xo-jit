# build unittest alloc/utest
#
# NOTE: more GC tests in xo-object/utest

set(UTEST_EXE utest.alloc)
set(UTEST_SRCS
    alloc_utest_main.cpp
    IAlloc.test.cpp
    ArenaAlloc.test.cpp
    GC.test.cpp)

xo_add_utest_executable(${UTEST_EXE} ${UTEST_SRCS})
xo_self_dependency(${UTEST_EXE} xo_alloc)
xo_external_target_dependency(${UTEST_EXE} Catch2 Catch2::Catch2)
