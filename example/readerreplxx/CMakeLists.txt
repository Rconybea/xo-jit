# xo-reader2/example/readerreplxx/CMakeLists.txt

set(SELF_EXE xo_reader2_readereplxx)
set(SELF_SRCS readerreplxx.cpp)

if (XO_ENABLE_EXAMPLES)
    xo_add_executable(${SELF_EXE} ${SELF_SRCS})
    xo_self_dependency(${SELF_EXE} xo_reader2)
    xo_external_target_dependency(${SELF_EXE} replxx replxx::replxx)

    # replxx requires this
    find_package(Threads REQUIRED)
    target_link_libraries(${SELF_EXE} PUBLIC Threads::Threads)
endif()
