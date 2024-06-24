# xo-jit/example/ex3_fptr/CMakeLists.txt

set(SELF_EXE xo_fptr_ex3)
set(SELF_SRCS ex3_fptr.cpp)

if (XO_ENABLE_EXAMPLES)
    xo_add_executable(${SELF_EXE} ${SELF_SRCS})
    xo_self_dependency(${SELF_EXE} xo_jit)
    #xo_dependency(${SELF_EXE} xo_expression)
endif()

# end CMakeLists.txt
