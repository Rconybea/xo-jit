# ----------------------------------------------------------------
# for example:
#   $ PREFIX=/usr/local   # for example
#   $ cmake -DCMAKE_MODULE_PATH=prefix -DCMAKE_INSTALL_PREFIX=$PREFIX -B .build
#
# will get
#   CMAKE_MODULE_PATH
# from xo-cmake-config --cmake-module-path
#
# and expect .cmake macros in
#   CMAKE_MODULE_PATH/xo_macros/xo_cxx.cmake
# ----------------------------------------------------------------

find_program(XO_CMAKE_CONFIG_EXECUTABLE NAMES xo-cmake-config REQUIRED)

if ("${XO_CMAKE_CONFIG_EXECUTABLE}" STREQUAL "XO_CMAKE_CONFIG_EXECUTABLE-NOT_FOUND")
    message(FATAL "could not find xo-cmake-config executable")
endif()

message(STATUS "XO_CMAKE_CONFIG_EXECUTABLE=${XO_CMAKE_CONFIG_EXECUTABLE}")

if (NOT XO_SUBMODULE_BUILD)
    if (("${CMAKE_MODULE_PATH}" STREQUAL "") OR ("${CMAKE_MODULE_PATH}" STREQUAL prefix))
        # default to typical install location for xo-project-macros
        execute_process(COMMAND ${XO_CMAKE_CONFIG_EXECUTABLE} --cmake-module-path OUTPUT_VARIABLE CMAKE_MODULE_PATH)
        message(STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
    endif()
endif()

# needs to have been installed somewhere on CMAKE_MODULE_PATH,
# (e.g. from xo-cmake with the same value for CMAKE_INSTALL_PREFIX)
#
include(xo_macros/xo_cxx)

xo_cxx_bootstrap_message()
