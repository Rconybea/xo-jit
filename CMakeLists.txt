# xo-tokenizer2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo_tokenizer2 VERSION 1.0)
enable_language(CXX)

include(GNUInstallDirs)
include(cmake/xo-bootstrap-macros.cmake)

xo_cxx_toplevel_options3()

# ----------------------------------------------------------------
# c++ settings

# one-time project-specific c++ flags. usually empty
set(PROJECT_CXX_FLAGS "")
add_definitions(${PROJECT_CXX_FLAGS})

# ----------------------------------------------------------------
# output targets

add_subdirectory(src/tokenizer2)
add_subdirectory(example)
#add_subdirectory(utest)
xo_export_cmake_config(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME}Targets)

if (XO_ENABLE_EXAMPLES)
    install(TARGETS xo_tokenizer2_repl DESTINATION bin/xo/example/tokenizer2)
endif()

# ----------------------------------------------------------------
# docs targets depends on all the other library/utest targets
#
#add_subdirectory(docs)

# end CMakeLists.txt
