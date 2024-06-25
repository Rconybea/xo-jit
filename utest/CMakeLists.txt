# xo-jit/utest/CMakeLists.txt

set(SELF_EXE utest.jit)
set(SELF_SRCS
    jit_utest_main.cpp
    MachPipeline.test.cpp
)

if (ENABLE_TESTING)
    xo_add_utest_executable(${SELF_EXE} ${SELF_SRCS})
    xo_self_dependency(${SELF_EXE} xo_jit)
    xo_dependency(${SELF_EXE} xo_ratio)
    xo_dependency(${SELF_EXE} xo_reflectutil)
    xo_external_target_dependency(${SELF_EXE} Catch2 Catch2::Catch2)
endif()

# end CMakeLists.txt
