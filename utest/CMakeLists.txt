# xo-alloc/utest/CMakeLists.txt
#
# NOTE: more GC tests in xo-object/utest

set(UTEST_EXE utest.alloc)
set(UTEST_SRCS
    alloc_utest_main.cpp
    IAlloc.test.cpp
    ArenaAlloc.test.cpp
    ArenaAllocT.test.cpp
    ListAlloc.test.cpp
    GC.test.cpp
    GcStatistics.test.cpp
    ObjectStatistics.test.cpp
    Forwarding1.test.cpp
    CircularBuffer.test.cpp
    generation.test.cpp
)

if (ENABLE_TESTING)
    xo_add_utest_executable(${UTEST_EXE} ${UTEST_SRCS})
    xo_self_dependency(${UTEST_EXE} xo_alloc)
    xo_dependency(${UTEST_EXE} reflect)
    xo_external_target_dependency(${UTEST_EXE} Catch2 Catch2::Catch2)
endif()

# end CmakeLists.txt
