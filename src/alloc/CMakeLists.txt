# alloc/CMakeLists.txt

set(SELF_LIB xo_alloc)
set(SELF_SRCS
    IAlloc.cpp
    ArenaAlloc.cpp
    ListAlloc.cpp
    GC.cpp
    GcStatistics.cpp
    Object.cpp
    Forwarding1.cpp
)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
xo_dependency(${SELF_LIB} reflect)

#end CMakeLists.txt
