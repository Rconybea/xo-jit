# indentlog unit test

set(SELF_EXECUTABLE_NAME utest.indentlog)
set(SELF_SOURCE_FILES
    fixed.test.cpp quoted.test.cpp vector.test.cpp array.test.cpp timeutil.test.cpp tag.test.cpp
    filename.test.cpp code_location.test.cpp function.test.cpp pretty_vector.test.cpp
    indentlog_utest_main.cpp log_streambuf.test.cpp toppstr.test.cpp cond.test.cpp)

if (ENABLE_TESTING)
    xo_add_utest_executable(${SELF_EXECUTABLE_NAME} ${SELF_SOURCE_FILES})
    xo_self_dependency(${SELF_EXECUTABLE_NAME} indentlog)
    xo_external_target_dependency(${SELF_EXECUTABLE_NAME} Catch2 Catch2::Catch2)
endif()

# end CMakeLists.txt
