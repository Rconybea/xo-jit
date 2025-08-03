# build unittest alloc/utest

set(SELF_EXE utest.alloc)
set(SELF_SRCS
    alloc_utest_main.cpp
    ArenaAlloc.test.cpp
    GC.test.cpp)

xo_add_utest_executable(${SELF_EXE} ${SELF_SRCS})
xo_self_dependency(${SELF_EXE} xo_alloc)
xo_external_target_dependency(${SELF_EXE} Catch2 Catch2::Catch2)
