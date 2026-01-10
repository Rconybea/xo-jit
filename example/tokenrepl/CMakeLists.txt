# xo-tokenizer2/example/tokenrepl/CMakeLists.txt

set(SELF_EXE xo_tokenizer2_repl)
set(SELF_SRCS tokenrepl.cpp)

if (XO_ENABLE_EXAMPLES)
    xo_add_executable(${SELF_EXE} ${SELF_SRCS})
    xo_self_dependency(${SELF_EXE} xo_tokenizer2)
    xo_external_target_dependency(${SELF_EXE} replxx replxx::replxx)

    find_package(Threads REQUIRED)   # replxx needs this
    target_link_libraries(${SELF_EXE} PUBLIC Threads::Threads)
endif()

# end CMakeLists.txt
