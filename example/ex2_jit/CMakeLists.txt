# xo-jit/example/ex2_jit/CMakeLists.txt

set(SELF_EXE xo_jit_ex2)
set(SELF_SRCS ex2_jit.cpp)

if (XO_ENABLE_EXAMPLES)
    xo_add_executable(${SELF_EXE} ${SELF_SRCS})
    xo_self_dependency(${SELF_EXE} xo_jit)
    #xo_dependency(${SELF_EXE} xo_expression)
endif()

# end CMakeLists.txt
