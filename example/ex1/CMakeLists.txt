# xo-flatstring/example/ex1/CMakeLists.txt

set(SELF_EXE flatstring_ex1)
set(SELF_SRCS ex1.cpp)

if (XO_ENABLE_EXAMPLES)
    add_executable(${SELF_EXE} ${SELF_SRCS})
    xo_include_options2(${SELF_EXE})
    xo_self_headeronly_dependency(${SELF_EXE} xo_flatstring)
endif()

# end CMakeLists.txt
