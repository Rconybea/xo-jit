# jit/CMakeLists.txt

set(SELF_LIB xo_jit)
set(SELF_SRCS
    LlvmContext.cpp
    IrPipeline.cpp
    MachPipeline.cpp
    intrinsics.cpp
    activation_record.cpp
)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
xo_dependency(${SELF_LIB} xo_expression)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM_DIR=${LLVM_DIR}")
message(STATUS "LLVM_DEFINITIONS=${LLVM_DEFINITIONS}")
message(STATUS "LLVM_INCLUDE_DIRS=[${LLVM_INCLUDE_DIRS}]")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

message(STATUS "LLVM_DEFINITIONS_LIST=[${LLVM_DEFINITIONS_LIST}]")

# LLVM library directory
execute_process(
    COMMAND llvm-config --libdir
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE LLVM_LIBRARY_DIR
)

message(STATUS "LLVM_LIBRARY_DIR=[${LLVM_LIBRARY_DIR}]")

# Find the libraries that correspond to the LLVM components
execute_process(
    COMMAND llvm-config --libs all
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE LLVM_LIBS
)

message(STATUS "LLVM_LIBS=[${LLVM_LIBS}]")

target_include_directories(${SELF_LIB} PUBLIC ${LLVM_INCLUDE_DIRS})
target_compile_definitions(${SELF_LIB} PUBLIC ${LLVM_DEFINITIONS_LIST})
target_link_directories(${SELF_LIB} PUBLIC ${LLVM_LIBRARY_DIR})
target_link_libraries(${SELF_LIB} PUBLIC ${LLVM_LIBS})

# end CMakeLists.txt
