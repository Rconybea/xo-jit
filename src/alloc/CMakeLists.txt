# alloc/CMakeLists.txt

set(SELF_LIB xo_alloc)
set(SELF_SRCS
    IAlloc.cpp
    ArenaAlloc.cpp
    ListAlloc.cpp
    GC.cpp
    GcStatistics.cpp
    ObjectStatistics.cpp
    Object.cpp
    Forwarding1.cpp
    generation.cpp
)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})
# xo-unit used for time measurement
xo_dependency(${SELF_LIB} xo_unit)
xo_dependency(${SELF_LIB} indentlog)
xo_dependency(${SELF_LIB} reflect)
xo_dependency(${SELF_LIB} callback)

#end CMakeLists.txt
