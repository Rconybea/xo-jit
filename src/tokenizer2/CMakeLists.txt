# tokenizer2/CMakeLists.txt

set(SELF_LIB xo_tokenizer2)
set(SELF_SRCS
    Tokenizer.cpp
    TokenizerError.cpp
    TkInputState.cpp
    scan_result.cpp
    Token.cpp
    tokentype.cpp)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
xo_dependency(${SELF_LIB} indentlog)

# end CMakeLists.txt
