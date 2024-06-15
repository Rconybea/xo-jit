# xo-jit/example/ex1/CMakeLists.txt

set(SELF_EXE xo_kaleidoscope4)
set(SELF_SRCS ex_kaleidoscope4.cpp)

if (XO_ENABLE_EXAMPLES)
    xo_add_executable(${SELF_EXE} ${SELF_SRCS})
    xo_self_dependency(${SELF_EXE} xo_jit)
    #xo_dependency(${SELF_EXE} xo_expression)
endif()

# end CMakeLists.txt
