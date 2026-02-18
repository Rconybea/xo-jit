# xo-interpreter2/src/repl/CMakeLists.txt

set(SELF_EXE skrepl)
set(SELF_SRCS skreplxx.cpp)

xo_add_executable(${SELF_EXE} ${SELF_SRCS})
xo_self_dependency(${SELF_EXE} xo_interpreter2)
xo_external_target_dependency(${SELF_EXE} replxx replxx::replxx)

# replxx requires this
find_package(Threads REQUIRED)
target_link_libraries(${SELF_EXE} PUBLIC Threads::Threads)

