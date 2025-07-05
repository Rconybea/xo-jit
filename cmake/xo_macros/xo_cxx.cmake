option(XO_ENABLE_EXAMPLES "enable building example programs" OFF)

macro(xo_cxx_config_message)
    message(STATUS "GUESSED_CMAKE_CMD=cmake -DXO_CMAKE_CONFIG_EXECUTABLE=${XO_CMAKE_CONFIG_EXECUTABLE} -DENABLE_TESTING=${ENABLE_TESTING} -DXO_ENABLE_EXAMPLES=${XO_ENABLE_EXAMPLES} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH} -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_DOCDIR=${CMAKE_INSTALL_DOCDIR} -B ${CMAKE_BINARY_DIR}")
    message(STATUS "XO_CMAKE_CONFIG_EXECUTABLE=${XO_CMAKE_CONFIG_EXECUTABLE}")
    message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
endmacro()

macro(xo_cxx_bootstrap_message)
    if (NOT XO_SUBMODULE_BUILD)
        xo_cxx_config_message()
    endif()
endmacro()

# deprecated -- prefer xo_cxx_toplevel_options2()
macro(xo_cxx_toplevel_options)
    message(WARNING "deprecated: prefer xo_cxx_toplevel_options3")

    message("xo_cxx_toplevel_options: PROJECT=${PROJECT_NAME}")

    enable_language(CXX)
    xo_toplevel_compile_options()
    xo_toplevel_testing_options()

    add_custom_target(all_executables_${PROJECT_NAME})
    set_property(
        TARGET all_executables_${PROJECT_NAME}
        PROPERTY targets "")

    add_custom_target(all_libraries_${PROJECT_NAME})
    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        PROPERTY targets "")
endmacro()

# deprecated -- prefer xo_cxx_toplevel_options3()
macro(xo_cxx_toplevel_options2)
    if (NOT DEFINED _xo_cxx_toplevel_done)
        message("xo_cxx_toplevel_options2: PROJECT=${PROJECT_NAME}")

        enable_language(CXX)
        xo_toplevel_compile_options()
        enable_testing()

        add_custom_target(all_executables_${PROJECT_NAME})
        set_property(
            TARGET all_executables_${PROJECT_NAME}
            PROPERTY targets "")

        add_custom_target(all_libraries_${PROJECT_NAME})
        set_property(
            TARGET all_libraries_${PROJECT_NAME}
            PROPERTY targets "")

        add_custom_target(all_utest_executables_${PROJECT_NAME})
        set_property(
            TARGET all_utest_executables_${PROJECT_NAME}
            PROPERTY targets "")

        add_custom_target(docs_${PROJECT_NAME})
        set_property(
            TARGET docs_${PROJECT_NAME}
            PROPERTY targets "")
    endif()
endmacro()

macro(xo_cxx_toplevel_options3)
    xo_cxx_toplevel_options2()
    xo_toplevel_config2()
endmacro()

macro(xo_toplevel_testing_options)
    enable_testing()
    add_code_coverage()
    add_code_coverage_all_targets(EXCLUDE /nix/store* utest/*)

    add_custom_target(all_utest_executables_${PROJECT_NAME})
    set_property(
        TARGET all_utest_executables_${PROJECT_NAME}
        PROPERTY targets "")
endmacro()

# default build (cmake -DCMAKE_BUILD_TYPE= path/to/source)
#
macro(xo_toplevel_default_config2)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        # clear out hardwired default.
        # we want to override project-level defaults,
        # but need to prevent interference from hardwired defaults
        # (the problem with non-empty hardwired defaults is that we can't tell if they've
        # been set on the command line)
        #
        set(CMAKE_CXX_FLAGS_DEFAULT "")

        # CMAKE_CXX_FLAGS_DEBUG is built-in to cmake and has non-empty default.
        #  -> we cannot tell whether it was set on the command line
        #  -> use PROJECT_CXX_FLAGS_DEBUG instead
        #
        # built-in default value is -g; can hardwire different project policy here
        #
        if (NOT DEFINED PROJECT_CXX_FLAGS_DEFAULT)
            set(PROJECT_CXX_FLAGS_DEFAULT ${PROJECT_CXX_FLAGS} -fno-strict-aliasing -O
                CACHE STRING "default c++ compiler flags")
        endif()

        message(STATUS "PROJECT_CXX_FLAGS_DEFAULT: default c++ flags are [${PROJECT_CXX_FLAGS_DEFAULT}]")

        # note no $<CONFIG:xxx> selector here
        add_compile_options("${PROJECT_CXX_FLAGS_DEFAULT}")
    endif()
endmacro()

# release build (cmake -DCMAKE_BUILD_TYPE=release path/to/source)
#
macro(xo_toplevel_release_config2)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "release")
        # clear out hardwired default
        # we want to override project-level defaults,
        # but need to prevent interference from hardwired defaults
        # (the problem with non-empty hardwired defaults is that we can't tell if they've
        # been set on the command line)
        #
        set(CMAKE_CXX_FLAGS_RELEASE "")

        # CMAKE_CXX_FLAGS_RELEASE is built-in to cmake and has non-empty default
        # -> we cannot tell whether it was set on the command line
        # -> use PROJECT_CXX_FLAGS_RELEASE instead
        #
        # built-in default value is -march=native -O3 -DNDEBUG
        #
        if (NOT DEFINED PROJECT_CXX_FLAGS_RELEASE)
            set(PROJECT_CXX_FLAGS_RELEASE ${PROJECT_CXX_FLAGS} -fno-strict-aliasing -march=native -O3 -DNDEBUG
                CACHE STRING "release c++ compiler flags")
        endif()

        message(STATUS "PROJECT_CXX_FLAGS_RELEASE: release c++ flags are [${PROJECT_CXX_FLAGS_RELEASE}]")

        add_compile_options("$<$<CONFIG:RELEASE>:${PROJECT_CXX_FLAGS_RELEASE}>")
    endif()
endmacro()

# debug build (cmake -DCMAKE_BUILD_TYPE=debug path/to/source)
#
macro(xo_toplevel_debug_config2)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
        # clear out hardwired default.
        # we want to override project-level defaults,
        # but need to prevent interference from hardwired defaults
        # (the problem with non-empty hardwired defaults is that we can't tell if they've
        # been set on the command line)
        #
        set(CMAKE_CXX_FLAGS_DEBUG "")

        # CMAKE_CXX_FLAGS_DEBUG is built-in to cmake and has non-empty default.
        #  -> we cannot tell whether it was set on the command line
        #  -> use PROJECT_CXX_FLAGS_DEBUG instead
        #
        # built-in default value is -g; can hardwire different project policy here
        #
        if (NOT DEFINED PROJECT_CXX_FLAGS_DEBUG)
            set(PROJECT_CXX_FLAGS_DEBUG ${PROJECT_CXX_FLAGS} -fno-strict-aliasing -ggdb -Og
                CACHE STRING "debug c++ compiler flags")
        endif()

        message(STATUS "PROJECT_CXX_FLAGS_DEBUG: debug c++ flags are [${PROJECT_CXX_FLAGS_DEBUG}]")

        add_compile_options("$<$<CONFIG:DEBUG>:${PROJECT_CXX_FLAGS_DEBUG}>")
    endif()
endmacro()


# asan (address sanitizer) build (cmake -DCMAKE_BUILD_TYPE=asan path/to/source)
#
macro(xo_toplevel_asan_config2)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "asan")
        # following the same pattern used for xo_toplevel_debug_config2(),
        # although not needed here (if PROJECT_CXX_FLAGS_ASAN has no builtin default value)

        set(CMAKE_CXX_FLAGS_ASAN "")

        if (NOT DEFINED PROJECT_CXX_FLAGS_ASAN)
            set(PROJECT_CXX_FLAGS_ASAN ${PROJECT_CXX_FLAGS} -fno-strict-aliasing -Og -fsanitize=address
                CACHE STRING "asan c++ compiler flags")
        endif()

        message(STATUS "PROJECT_CXX_FLAGS_ASAN: asan c++ flags are [${PROJECT_CXX_FLAGS_ASAN}]")

        add_compile_options("$<$<CONFIG:ASAN>:${PROJECT_CXX_FLAGS_ASAN}>")

        if (NOT DEFINED PROJECT_LINK_FLAGS_ASAN)
            set(PROJECT_LINK_FLAGS_ASAN ${PROJECT_LINK_FLAGS} -fsanitize=address
                CACHE STRING "asan link flags")
        endif()
        message(STATUS "PROJECT_LINK_FLAGS_ASAN: asan link flags are [${PROJECT_LINK_FLAGS_ASAN}]")

        add_link_options("$<$<CONFIG:ASAN>:${PROJECT_LINK_FLAGS_ASAN}>")
    endif()
endmacro()

# support for
#   cmake -DCMAKE_BUILD_TYPE=
#   cmake -DCMAKE_BUILD_TYPE=release
#   cmake -DCMAKE_BUILD_TYPE=debug
#   cmake -DCMAKE_BUILD_TYPE=asan
#   cmake -DCMAKE_BUILD_TYPE=coverage
#
macro(xo_toplevel_config2)
    xo_toplevel_default_config2()
    xo_toplevel_release_config2()
    xo_toplevel_debug_config2()
    xo_toplevel_asan_config2()
    xo_toplevel_coverage_config2()
endmacro()

# coverage build:
# 0.
#     (cmake -DCMAKE_BUILD_TYPE=coverage path/to/source)
# 1. invoke instrumented executables for which you want coverage:
#     (cmake --build path/to/build -- test)
# 2. post-process low-level coverage data
#     (path/to/build/gen-ccov)
# 3. point browser to generated html data
#     file:///path/to/build/ccov/html/index.html
#
macro(xo_toplevel_coverage_config2)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "coverage")
        #find_program(LCOV_EXECUTABLE NAMES lcov)
        #find_program(GENHTML_EXECUTABLE NAMES genhtml)
        # see bin/xo-cmake-lcov-harness in this repo
        execute_process(COMMAND ${XO_CMAKE_CONFIG_EXECUTABLE} --lcov-harness-exe OUTPUT_VARIABLE XO_CMAKE_LCOV_HARNESS_EXECUTABLE)
        execute_process(COMMAND ${XO_CMAKE_CONFIG_EXECUTABLE} --gen-ccov-template OUTPUT_VARIABLE XO_CMAKE_GEN_CCOV_TEMPLATE)

        if (NOT DEFINED PROJECT_CXX_FLAGS_COVERAGE)
            # note: for clang would use -fprofile-instr-generate -fcoverage-mapping here instead and also at link time
            set(PROJECT_CXX_FLAGS_COVERAGE -ggdb -Og -fprofile-arcs -ftest-coverage
                CACHE STRING "coverage c++ compiler flags")
        endif()
        message(STATUS "PROJECT_CXX_FLAGS_COVERAGE: coverage c++ flags are [${PROJECT_CXX_FLAGS_COVERAGE}]")

        add_compile_options("$<$<CONFIG:COVERAGE>:${PROJECT_CXX_FLAGS_COVERAGE}>")
        # when -DCMAKE_BUILD_TYPE=coverage, must also link executables with gcov
        link_libraries("$<$<CONFIG:COVERAGE>:gcov>")

        if("${XO_CMAKE_GEN_CCOV_TEMPLATE}" STREQUAL "")
            message(ERROR "xo_toplevel_testing_config2: XO_CMAKE_GEN_CCOV_TEMPLATE not set")
            message(ERROR "see xo_toplevel_testing_options2()")
        else()
            message(DEBUG "XO_CMAKE_GEN_CCOV_TEMPLATE=${XO_CMAKE_GEN_CCOV_TEMPLATE}")
            configure_file(${XO_CMAKE_GEN_CCOV_TEMPLATE} ${PROJECT_BINARY_DIR}/gen-ccov @ONLY)
            file(CHMOD ${PROJECT_BINARY_DIR}/gen-ccov PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
        endif()
    endif()
endmacro()

# target to build+install coverage report.
#
macro(xo_utest_coverage_config2)
    if (XO_SUBMODULE_BUILD)
        # in submodule build, will generate aggregate coverage report
        # for all xo libraries.
    else()
        set(CCOV_OUTPUT_DIR ${PROJECT_BINARY_DIR}/ccov/html)
        set(CCOV_INDEX_FILE ${CCOV_OUTPUT_DIR}/index.html)
        # see xo_toplevel_coverage_report()
        set(CCOV_REPORT_EXE ${PROJECT_BINARY_DIR}/gen-ccov)
        # CMAKE_INSTALL_DOCDIR
        #  =default=> DATAROOTDIR/doc/PROJECT_NAME
        #  =default=> CMAKE_INSTALL_PREFIX/share/doc/xo_flatstring
        set(CCOV_INSTALL_DOCDIR ${CMAKE_INSTALL_DOCDIR}/ccov)

        # collect utest deps (like xo_doxygen_collect_deps())
        get_target_property(_all_utests all_utest_executables_${PROJECT_NAME} targets)
        message(DEBUG "_all_utests=${_all_utests}")

        # 'test' target should always be out-of-date
        #
        # DEPENDS: reminder - can't put 'test' here,  requires 'all' target
        #
        add_custom_command(
            OUTPUT ${CCOV_INDEX_FILE}
            DEPENDS ${_all_utests}
            COMMAND ${CCOV_REPORT_EXE}
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            COMMENT "Generating coverage report -> [${CCOV_OUTPUT_DIR}]")

        add_custom_target(
            ccov
            DEPENDS ${CCOV_INDEX_FILE} ${SELF_EXE})

        # OPTIONAL: quietly skip this step if ccov report not generated
        install(
            DIRECTORY ${CCOV_OUTPUT_DIR}
            FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            DESTINATION ${CCOV_INSTALL_DOCDIR}
            COMPONENT Documentation
            OPTIONAL)
    endif()
endmacro()

# caller should set ALL_LIBRARY_TARGETS ALL_UTEST_TARGETS before calling
# xo_doxygen_standard_deps().
#
# xo_add_headeronly_library4(), xo_add_shared_library4(), xo_pybind11_library()
# all add their target to ALL_LIBRARY_TARGETS.
#
macro(xo_doxygen_collect_deps)
    get_target_property(_all_exes all_executables_${PROJECT_NAME} targets)
    get_target_property(_all_libs all_libraries_${PROJECT_NAME} targets)
    get_target_property(_all_utests all_utest_executables_${PROJECT_NAME} targets)

    message(DEBUG "_all_exes=${_all_exes}")
    message(DEBUG "_all_libs=${_all_libs}")
    message(DEBUG "_all_utests=${_all_utests}")

    set(DOX_DEPS ${_all_exes} ${_all_libs} ${_all_utests})

    message(STATUS "DOX_DEPS=${DOX_DEPS}")
endmacro()

# for example
#   xo_umbrella_doxygen_deps(xo_flatstring xo-pyjit ...)
# use
#   cmake --build path/to/build --target help
# to review available targets
#
macro(xo_umbrella_doxygen_deps)
    # using a target here for symmetry with satellite builds
    add_custom_target(umbrella_libs)

    foreach(arg IN ITEMS ${ARGN})
        message(arg=${arg})

        set_property(
            TARGET umbrella_libs
            APPEND
            PROPERTY targets all_libraries_${arg})
    endforeach()

    get_target_property(_all_libs umbrella_libs targets)

    set(DOX_DEPS ${_all_libs})

    message(STATUS "DOX_DEPS=${DOX_DEPS}")
endmacro()


# caller must set DOX_DEPS before invoking xo_toplevel_doxygen_config()
#
macro(xo_docdir_doxygen_config)
    if (XO_SUBMODULE_BUILD)
        # in submodule build, rely on toplevel docs/CMakeLists.txt file instead
    else()
        # look for doxygen executable
        find_program(DOXYGEN_EXECUTABLE NAMES doxygen REQUIRED)
        message(STATUS "DOXYGEN_EXECUTABLE=${DOXYGEN_EXECUTABLE}")

        message(STATUS "XO_CMAKE_CONFIG_EXECUTABLE=${XO_CMAKE_CONFIG_EXECUTABLE}")

        execute_process(COMMAND ${XO_CMAKE_CONFIG_EXECUTABLE} --doxygen-template OUTPUT_VARIABLE DOXYGEN_CONFIG_TEMPLATE)
        message(STATUS "DOXYGEN_CONFIG_TEMPLATE=${DOXYGEN_CONFIG_TEMPLATE}")
        message(STATUS "DOX_EXCLUDE=${DOX_EXCLUDE}")
        message(STATUS "DOX_EXCLUDE_PATTERNS=${DOX_EXCLUDE_PATTERNS}")

        set(DOX_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        set(DOX_INPUT_DIR ${PROJECT_SOURCE_DIR})
        set(DOX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/dox)

        set(DOX_INDEX_FILE ${DOX_OUTPUT_DIR}/html/index.html)

        # note: expansion variables in Doxyfile.in:
        #   @PROJECT_NAME@ @DOX_INPUT_DIR@ @DOX_OUTPUT_DIR@ @DOX_EXCLUDE@ @DOX_EXCLUDE_PATTERNS@
        #
        configure_file(
            ${DOXYGEN_CONFIG_TEMPLATE} ${DOX_CONFIG_FILE}
            FILE_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
            @ONLY)

        file(MAKE_DIRECTORY ${DOX_OUTPUT_DIR})
        add_custom_command(
            OUTPUT ${DOX_INDEX_FILE}
            DEPENDS "${DOX_DEPS}" ${DOX_CONFIG_FILE}
            COMMAND "${DOXYGEN_EXECUTABLE}" ${DOX_CONFIG_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            MAIN_DEPENDENCY ${DOX_CONFIG_FILE}
            COMMENT "Generating docs (doxygen)")

        # To build this target
        #   $ cmake --build .build -j -- doxygen
        # or
        #   $ cd .build
        #   $ make doxygen
        #
        add_custom_target(
            doxygen_${PROJECT_NAME}
            DEPENDS ${DOX_INDEX_FILE} ${DOX_DEPS}
        )

    endif()
endmacro()

# config for an umbrella project that composes standalone subprojects
macro(xo_umbrella_doxygen_config)
    # look for doxygen executable
    find_program(DOXYGEN_EXECUTABLE NAMES doxygen REQUIRED)
    message(STATUS "DOXYGEN_EXECUTABLE=${DOXYGEN_EXECUTABLE}")

    message(STATUS "XO_CMAKE_CONFIG_EXECUTABLE=${XO_CMAKE_CONFIG_EXECUTABLE}")

    execute_process(COMMAND ${XO_CMAKE_CONFIG_EXECUTABLE} --doxygen-template OUTPUT_VARIABLE DOXYGEN_CONFIG_TEMPLATE)
    message(STATUS "DOXYGEN_CONFIG_TEMPLATE=${DOXYGEN_CONFIG_TEMPLATE}")
    message(STATUS "DOX_EXCLUDE=${DOX_EXCLUDE}")
    message(STATUS "DOX_EXCLUDE_PATTERNS=${DOX_EXCLUDE_PATTERNS}")

    set(DOX_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    set(DOX_INPUT_DIR ${PROJECT_SOURCE_DIR})
    set(DOX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/dox)

    set(DOX_INDEX_FILE ${DOX_OUTPUT_DIR}/html/index.html)

        # note: expansion variables in Doxyfile.in:
        #   @PROJECT_NAME@ @DOX_INPUT_DIR@ @DOX_OUTPUT_DIR@ @DOX_EXCLUDE@ @DOX_EXCLUDE_PATTERNS@
        #
    configure_file(
        ${DOXYGEN_CONFIG_TEMPLATE} ${DOX_CONFIG_FILE}
        FILE_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        @ONLY)

    file(MAKE_DIRECTORY ${DOX_OUTPUT_DIR})
    add_custom_command(
        OUTPUT ${DOX_INDEX_FILE}
        DEPENDS "${DOX_DEPS}"
        COMMAND "${DOXYGEN_EXECUTABLE}" ${DOX_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        MAIN_DEPENDENCY ${DOX_CONFIG_FILE}
        COMMENT "Generating docs (doxygen)")

    # To build this target
    #   $ cmake --build .build -j -- doxygen
    # or
    #   $ cd .build
    #   $ make doxygen
    #
    add_custom_target(
        doxygen_${PROJECT_NAME}
        DEPENDS ${DOX_INDEX_FILE} ${DOX_DEPS}
    )
endmacro()


macro(xo_docdir_sphinx_config rst_files)
    list(APPEND SPHINX_RST_FILES ${rst_files})
    foreach(arg IN ITEMS ${ARGN})
        list(APPEND SPHINX_RST_FILES ${arg})
    endforeach()

    message(STATUS "SPHINX_RST_FILES=${SPHINX_RST_FILES}")

    if (XO_SUBMODULE_BUILD)
        # in submodule build, rely on toplevel docs/CMakeLists.txt file instead
    else()
        # build docs starting from here only in standalone build.
        # otherwise use top-level doxygen setup.

        # look for sphinx-build executable
        find_program(SPHINX_EXECUTABLE NAMES sphinx-build REQUIRED)
        message(STATUS "SPHINX_EXECUTABLE=${SPHINX_EXECUTABLE}")

        set(SPHINX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html)
        set(SPHINX_INDEX_FILE ${SPHINX_OUTPUT_DIR}/index.html)

        # root of sphinx doc tree
        set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
        set(SPHINX_DEPS doxygen_${PROJECT_NAME} conf.py ${SPHINX_RST_FILES} ${SPHINX_RST_FILES_GLOB} ${DOX_DEPS})
        #set(SPHINX_DEPS conf.py ${SPHINX_RST_FILES} ${SPHINX_RST_FILES_GLOB} ${DOX_DEPS})

        add_custom_command(
            OUTPUT ${SPHINX_INDEX_FILE}
            DEPENDS ${SPHINX_DEPS}
            COMMAND ${SPHINX_EXECUTABLE}
            -b html -Dbreathe_projects.xodoxxml=${CMAKE_CURRENT_BINARY_DIR}/dox/xml
            ${SPHINX_SOURCE} ${SPHINX_OUTPUT_DIR}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating docs (sphinx) -> [${SPHINX_OUTPUT_DIR}]")

        # make sphinx --> generate sphinx documentation
        #
        add_custom_target(
            sphinx_${PROJECT_NAME}
            DEPENDS ${SPHINX_INDEX_FILE})

        # - html docs generated in build/docs/sphinx
        # - copy the doc tree to share/doc/xo_unit/html
        #
        #  DESTINATION: CMAKE_INSTALL_DOCDIR
        #                => DATAROOTDIR/doc/PROJECT_NAME
        #                => CMAKE_INSTALL_PREFIX/share/doc/xo_flatstring
        #  OPTIONAL:    install directory tree if it exists,
        #               but don't complain if it's missing
        install(
            DIRECTORY ${SPHINX_OUTPUT_DIR}
            FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            COMPONENT Documentation
            OPTIONAL)

        # make docs --> generate sphinx documentation
        add_custom_target(
            docs
            DEPENDS sphinx_${PROJECT_NAME})
    endif()
endmacro()

# config for an umbrella project that composes standalone subprojects
#
macro(xo_umbrella_sphinx_config rst_files)
    list(APPEND SPHINX_RST_FILES ${rst_files})
    foreach(arg IN ITEMS ${ARGN})
        list(APPEND SPHINX_RST_FILES ${arg})
    endforeach()

    # look for sphinx-build executable
    find_program(SPHINX_EXECUTABLE NAMES sphinx-build REQUIRED)
    message(STATUS "SPHINX_EXECUTABLE=${SPHINX_EXECUTABLE}")

    set(SPHINX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html)
    set(SPHINX_INDEX_FILE ${SPHINX_OUTPUT_DIR}/index.html)

    # root of sphinx doc tree
    set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
    set(SPHINX_DEPS doxygen_${PROJECT_NAME} conf.py ${SPHINX_RST_FILES} ${SPHINX_RST_FILES_GLOB} ${DOX_DEPS})

    add_custom_command(
        OUTPUT ${SPHINX_INDEX_FILE}
        DEPENDS ${SPHINX_DEPS}
        COMMAND ${SPHINX_EXECUTABLE}
        -b html -Dbreathe_projects.xodoxxml=${CMAKE_CURRENT_BINARY_DIR}/dox/xml
        ${SPHINX_SOURCE} ${SPHINX_OUTPUT_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating docs (sphinx) -> [${SPHINX_OUTPUT_DIR}]")

    # make sphinx --> generate sphinx documentation
    #
    add_custom_target(
        sphinx_${PROJECT_NAME}
        DEPENDS ${SPHINX_INDEX_FILE})

    # - html docs generated in build/docs/sphinx
    # - copy the doc tree to share/doc/xo_unit/html
    #
    #  DESTINATION: CMAKE_INSTALL_DOCDIR
    #                => DATAROOTDIR/doc/PROJECT_NAME
    #                => CMAKE_INSTALL_PREFIX/share/doc/xo_flatstring
    #  OPTIONAL:    install directory tree if it exists,
    #               but don't complain if it's missing
    install(
        DIRECTORY ${SPHINX_OUTPUT_DIR}
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Documentation
        OPTIONAL)

    # make docs --> generate sphinx documentation
    add_custom_target(
        docs
        DEPENDS sphinx_${PROJECT_NAME})

endmacro()

macro(xo_toplevel_compile_options)
    define_property(
        TARGET
        PROPERTY xo_deps
        BRIEF_DOCS "transitive completion of xo-dependencies for a target.  Used with XO_SUBMODULE_BUILD"
    )
    define_property(
        TARGET
        PROPERTY xo_srcdir
        BRIEF_DOCS "snapshot of PROJECT_SOURCE_DIR asof when this target introduced"
    )
    define_property(
        TARGET
        PROPERTY xo_bindir
        BRIEF_DOCS "snapshot of PROJECT_BINARY_DIR asof when this target introduced"
    )

    if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 20)
    endif()
    if(NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
        set(CMAKE_CXX_STANDARD_REQUIRED True)
    endif()

    if(NOT DEFINED PROJECT_INCLUDE_STEM_DIR)
        set(PROJECT_INCLUDE_STEM_DIR xo)
    endif()

    # ----------------------------------------------------------------
    # variable
    #   XO_ADDRESS_SANITIZE
    # determines whether to enable address sanitizer for the XO project
    # (see toplevel CMakeLists.txt)
    # ----------------------------------------------------------------
    if(XO_ADDRESS_SANITIZE)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()

    set(XO_STANDARD_COMPILE_OPTIONS -Werror -Wall -Wextra)
    # XO_ADDRESS_SANITIZE_COMPILE_OPTIONS: use when XO_ADDRESS_SANITIZE=ON
    #
    # address sanitizer build complains about _FORTIFY_SOURCE redefines
    #    In file included from <built-in>:460:
    #    <command line>:1:9: error: '_FORTIFY_SOURCE' macro redefined [-Werror,-Wmacro-redefined]
    #    #define _FORTIFY_SOURCE 2
    #
    set(XO_ADDRESS_SANITIZE_COMPILE_OPTIONS -Werror -Wall -Wextra -Wno-macro-redefined)

    if(XO_ADDRESS_SANITIZE)
        set(XO_COMPILE_OPTIONS ${XO_ADDRESS_SANITIZE_COMPILE_OPTIONS})
    else()
        set(XO_COMPILE_OPTIONS ${XO_STANDARD_COMPILE_OPTIONS})
    endif()

    # writes ${PROJECT_BINARY_DIR}/compile_commands.json;
    # (symlink from toplevel git dir to tell LSP how to build)
    #
    # note: trying to protect this with if(NOT DEFINED ..) is /not/ effective
    #
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

    if(NOT CMAKE_INSTALL_RPATH)
        set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib CACHE STRING
            "runpath in installed libraries/executables")
    endif()
endmacro()

# xo_strip_xo_prefix(xo_foo tmp) --> tmp=foo
# xo_strip_xo_prefix(xo-foo tmp) --> tmp=foo
# xo_strip_xo_prefix(foo tmp)    --> tmp=foo
#
macro(xo_strip_xo_prefix str outputvar)
    string(REGEX REPLACE "^${PROJECT_INCLUDE_STEM_DIR}_" "" _tmp ${str})
    string(REGEX REPLACE "^${PROJECT_INCLUDE_STEM_DIR}-" "" ${outputvar} ${_tmp})
endmacro()

# e.g.
# - xo_target = xo_pyutil
#
macro(xo_include_headeronly_options target)
    xo_strip_xo_prefix(${target} _nxo_target)

    # ----------------------------------------------------------------
    # PROJECT_SOURCE_DIR:
    #   so we can for example write
    #     #include "ordinaltree/foo.hpp"
    #   from anywhere in the project
    # PROJECT_BINARY_DIR:
    #   since generated version file will be in build directory,
    #   need that build directory to also appear in
    #   compiler's include path
    #
    # NOTE: using INTERFACE here is mandatory.  Otherwise get error:
    #        target_include_directories may only set INTERFACE properties on INTERFACE targets
    #
    target_include_directories(
      ${target} INTERFACE
      $<INSTALL_INTERFACE:include>
      $<INSTALL_INTERFACE:include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>                      # e.g. for #include "indentlog/scope.hpp"
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}>    # e.g. for #include "TypeDescr.hpp" in reflect/src when ${target}=reflect
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}>    # e.g. for generated .hpp files
    )

    # ----------------------------------------------------------------
    # make standard directories for std:: includes explicit
    # so that
    # (1) they appear in compile_commands.json.
    # (2) clangd (run from emacs lsp-mode) can find them
    #
    if(CMAKE_EXPORT_COMPILE_COMMANDS)
      set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
    endif()
endmacro()

macro(xo_include_headeronly_options2 target)
    xo_include_headeronly_options(${target})
endmacro()

# ----------------------------------------------------------------
# use this to introduce a shared library.
# - has symlink-enabled .hpp install
#
macro(xo_add_shared_library4 target projectTargets targetversion soversion sources)
    add_library(${target} SHARED ${sources})

    set_property(
        TARGET ${target}
        PROPERTY xo_deps "${target}")
    set_property(
        TARGET ${target}
        PROPERTY xo_srcdir ${PROJECT_SOURCE_DIR})
    set_property(
        TARGET ${target}
        PROPERTY xo_bindir ${PROJECT_BINARY_DIR})

    foreach(arg IN ITEMS ${ARGN})
        #message("target=${target}; arg=${arg}")

        # to use PUBLIC here would need to split:
        #   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${arg}}>
        #   $<INSTALL_INTERFACE:${arg}>
        # but shouldn't need that,  since we arrange to install includes via
        # xo_include_options2() below
        #
        target_sources(${target} PRIVATE ${arg})
    endforeach()

    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

    set_target_properties(
        ${target}
        PROPERTIES
        VERSION ${targetversion}
        SOVERSION ${soversion})

    xo_compile_options(${target})
    xo_include_options2(${target})
    xo_install_library4(${target} ${projectTargets})
endmacro()

# ----------------------------------------------------------------
# OBSOLETE.  prefer xo_add_shared_library4()
#
macro(xo_add_shared_library3 target projectTargets targetversion soversion sources)
    message(WARNING "${target}: obsolete call to xo_add_shared_library3(); prefer xo_add_shared_library4()")

    add_library(${target} SHARED ${sources})

    foreach(arg IN ITEMS ${ARGN})
        #message("target=${target}; arg=${arg}")

        # to use PUBLIC here would need to split:
        #   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${arg}}>
        #   $<INSTALL_INTERFACE:${arg}>
        # but shouldn't need that,  since we arrange to install includes via
        # xo_include_options2() below
        #
        target_sources(${target} PRIVATE ${arg})
    endforeach()

    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

    set_target_properties(
        ${target}
        PROPERTIES
        VERSION ${targetversion}
        SOVERSION ${soversion})
    xo_compile_options(${target})
    xo_include_options2(${target})
    xo_install_library3(${target} ${projectTargets})
endmacro()

# ----------------------------------------------------------------
# OBSOLETE.  prefer xo_add_shared_library3()
#
macro(xo_add_shared_library target targetversion soversion sources)
    message(WARNING "${target}: obsolete call to xo_add_shared_library(); prefer xo_add_shared_library4()")

    add_library(${target} SHARED ${sources})

    foreach(arg IN ITEMS ${ARGN})
        #message("target=${target}; arg=${arg}")

        # to use PUBLIC here would need to split:
        #   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${arg}}>
        #   $<INSTALL_INTERFACE:${arg}>
        # but shouldn't need that,  since we arrange to install includes via
        # xo_include_options2() below
        #
        target_sources(${target} PRIVATE ${arg})
    endforeach()

    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

    set_target_properties(
        ${target}
        PROPERTIES
        VERSION ${targetversion}
        SOVERSION ${soversion})
    xo_compile_options(${target})
    xo_include_options2(${target})
    xo_install_library2(${target})
endmacro()

# ----------------------------------------------------------------
# use this for a header-only library
#
# e.g.
# - target=xo_pyutil      cmake target name for this library
#
macro(xo_add_headeronly_library4 target projectTargets)
    add_library(${target} INTERFACE)

    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

    set_property(
        TARGET ${target}
        PROPERTY xo_deps "${target}")
    set_property(
        TARGET ${target}
        PROPERTY xo_srcdir ${PROJECT_SOURCE_DIR})
    set_property(
        TARGET ${target}
        PROPERTY xo_bindir ${PROJECT_BINARY_DIR})

    xo_include_headeronly_options(${target})

    xo_install_library4(${target} ${projectTargets})
endmacro()

macro(xo_add_headeronly_library target)
    add_library(${target} INTERFACE)

    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

    set_property(
        TARGET ${target}
        PROPERTY xo_deps "${target}")
    set_property(
        TARGET ${target}
        PROPERTY xo_srcdir ${PROJECT_SOURCE_DIR})
    set_property(
        TARGET ${target}
        PROPERTY xo_bindir ${PROJECT_BINARY_DIR})

    xo_include_headeronly_options(${target})
endmacro()

# ----------------------------------------------------------------
# use this for an executable
#
macro(xo_add_executable target sources)
    # this only adds the first argument..
    add_executable(${target} ${sources})
    xo_include_options2(${target})

    # this adds all the remaining arguments
    foreach(arg IN ITEMS ${ARGN})
        target_sources(${target} PRIVATE ${arg})
    endforeach()

    # remember executable for certain downstream targets,
    # such as xo_doxygen_collect_deps()
    #
    set_property(
        TARGET all_executables_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})
endmacro()

# ----------------------------------------------------------------
# use this for a unit test executable
#
macro(xo_add_utest_executable target sources)
    # this only adds the first argument..
    add_executable(${target} ${sources})
    xo_include_options2(${target})

    add_test(NAME ${target} COMMAND ${target})

    # this adds all the remaining arguments
    foreach(arg IN ITEMS ${ARGN})
        target_sources(${target} PRIVATE ${arg})
    endforeach()

    set_property(
        TARGET all_utest_executables_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

endmacro()

# ----------------------------------------------------------------
#
# in submodule build each xo codebases cmake files are incorporated
# directly (via add_subdirectory()) into a single umbrella build.
#
# In such case we don't use find_package for xo dependencies
#
macro(xo_establish_submodule_build)
    if(NOT DEFINED XO_SUBMODULE_BUILD)
        set(XO_SUBMODULE_BUILD False)
    endif()
endmacro()

# ----------------------------------------------------------------
# use this in subdirs that compile c++ code.
# do not use for header-only subsystems;  see xo_include_headeronly_options2()
#
macro(xo_include_options2 target)
    xo_establish_submodule_build()

    xo_strip_xo_prefix(${target} _nxo_target)

    #message("xo_include_options2: XO_SUBMODULE_BUILD=${XO_SUBMODULE_BUILD}")

    # ----------------------------------------------------------------
    # PROJECT_SOURCE_DIR:
    #   so we can for example write
    #     #include "ordinaltree/foo.hpp"
    #   from anywhere in the project
    # PROJECT_BINARY_DIR:
    #   since generated version file will be in build directory,
    #   need that build directory to also appear in
    #   compiler's include path
    #
    target_include_directories(
      ${target} PUBLIC
      $<INSTALL_INTERFACE:include>
      $<INSTALL_INTERFACE:include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>                      # e.g. for #include "indentlog/scope.hpp"
      #$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${target}>           # e.g. for #include "Refcounted.hpp" in refcnt/src [DEPRECATED]
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}>    # e.g. for #include "TypeDescr.hpp" in reflect/src when ${target}=reflect
      #$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>                     # e.g. for generated .hpp files
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}>    # e.g. for generated .hpp files
    )

    # ----------------------------------------------------------------
    # make standard directories for std:: includes explicit
    # so that
    # (1) they appear in compile_commands.json.
    # (2) clangd (run from emacs lsp-mode) can find them
    #
    if(CMAKE_EXPORT_COMPILE_COMMANDS)
      set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
    endif()
endmacro()

# ----------------------------------------------------------------
#
# Require:
#     needs to be preceded by call to xo_toplevel_compile_options()
#
macro(xo_compile_options target)
    target_compile_options(${target} PRIVATE ${XO_COMPILE_OPTIONS})
endmacro()

# ----------------------------------------------------------------
#
# establish default value for XO_SYMLINK_INSTALL.
#
# may want to use this for a nested build,
# where we want to run cmake for nested codebase using externalproject_add().
#
# in this case:
# 1. will need to build+install nested project foo to temporary location in build tree,
#    so that build artifacts (such as fooConfig.cmake) are available to depending
#    projects
# 2. bona fide install with (for example) copied .hpp files interferes with
#    cross-codebase development.
#    2a. want changes to original .hpp files to trigger rebuild of depending codebases.
#        This won't happen if depending project relies on a copy
#    2b. want IDE that observes compiler commands (i.e. LSP) to visit .hpp files
#        in their original codebase,  since that's the correct place to make any edits.
#
# see
# - xo_install_include_tree()
#
macro(xo_establish_symlink_install)
    if(NOT DEFINED XO_SYMLINK_INSTALL)
        set(XO_SYMLINK_INSTALL False)

        #message(XO_SYMLINK_INSTALL=${XO_SYMLINK_INSTALL})
    endif()
endmacro()

# ----------------------------------------------------------------
# use this to install typical include file subtree
#
macro(xo_install_include_tree)
    message(WARNING "deprecated xo_install_include_tree(); prefer xo_install_include_tree3()")

    xo_establish_symlink_install()

    if(XO_SYMLINK_INSTALL)
        message(FATAL_ERROR "symlink install not implemented for ${PROJECT_SOURCE_DIR}/include -- use xo_install_include_tree3()")
    else()
        install(
            DIRECTORY ${PROJECT_SOURCE_DIR}/include/
            FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
    endif()
endmacro()

# create symlink from ${symlink_path} -> ${dest_path},
# from
#   make install
#
macro(xo_install_make_symlink dest_path symlink_dir symlink_name)
    install(CODE "message(\"make_directory: ${symlink_dir}\")")
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${symlink_dir})")

    install(CODE "message(\"symlink: ${symlink_dir}/${symlink_name} -> ${dest_path}/${symlink_name}\")")
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${dest_path}/${symlink_name} ${symlink_dir}/${symlink_name})")
endmacro()

# e.g. path = include/xo/foo to install xo-foo/include/xo/foo
#
macro(xo_install_include_tree3 subdir_path)
    xo_establish_symlink_install()

    # ugh.  cmake doesn't allow input path argument to cmake_path()
    # to be a macro variable.
    set(_xo_install_include_tree3_subdir_path ${subdir_path})
    set(_xo_install_include_tree3_dirname "")
    set(_xo_install_include_tree3_basename "")
    cmake_path(GET _xo_install_include_tree3_subdir_path PARENT_PATH _xo_install_include_tree3_dirname)
    cmake_path(GET _xo_install_include_tree3_subdir_path FILENAME _xo_install_include_tree3_basename)

    if(XO_SYMLINK_INSTALL)

        xo_install_make_symlink(
            ${PROJECT_SOURCE_DIR}/${_xo_install_include_tree3_dirname}
            ${CMAKE_INSTALL_PREFIX}/${_xo_install_include_tree3_dirname}
            ${_xo_install_include_tree3_basename})
    else()
        install(
            DIRECTORY ${PROJECT_SOURCE_DIR}/${subdir_path}
            FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            DESTINATION ${CMAKE_INSTALL_PREFIX}/${_xo_install_include_tree3_dirname})
    endif()
endmacro()

# ----------------------------------------------------------------
# use this for a subdir that builds a library
# and supports find_package()
#
# note: used deliberately in xo_pybind11_library() below
#
macro(xo_install_library2 target)
    install(
        TARGETS ${target}
        EXPORT ${target}Targets
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
    )
endmacro()

macro(xo_install_library3 target projectTargets)
    install(
        TARGETS ${target}
        EXPORT ${projectTargets}
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
    )

    xo_install_include_tree()
endmacro()

macro(xo_install_library4 target projectTargets)
    xo_strip_xo_prefix(${target} _nxo_target)

    install(
        TARGETS ${target}
        EXPORT ${projectTargets}
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
    )

    xo_install_include_tree3(include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target})

    #xo_install_include_tree() -- use xo_install_include_tree3() separately
endmacro()

macro(xo_install_library4_noincludes target projectTargets)
    install(
        TARGETS ${target}
        EXPORT ${projectTargets}
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
    )
endmacro()

# ----------------------------------------------------------------

# for projectname=foo,  require:
#   cmake/fooConfig.cmake.in
#
# prepares
#   ${PREFIX}/lib/cmake/foo/fooConfig.cmake
#   ${PREFIX}/lib/cmake/foo/fooConfigVersion.cmake
#   ${PREFIX}/lib/cmake/foo/fooTargets.cmake
#
macro(xo_export_cmake_config projectname projectversion projecttargets)
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/${projectname}ConfigVersion.cmake"
        VERSION ${projectversion}
        COMPATIBILITY AnyNewerVersion
    )
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${projectname}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${projectname}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${projectname}
    )
    install(
        EXPORT ${projecttargets}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/${projectname})
    install(
        FILES
        "${PROJECT_BINARY_DIR}/${projectname}ConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/${projectname}Config.cmake"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/${projectname}
    )
endmacro()

# ----------------------------------------------------------------
# helper macro for xo_dependency_helper() see below
#
macro(xo_dependency_helper1 target visibility dep_include_subdir)
    target_include_directories(${target} ${visibility}
        $<BUILD_INTERFACE:${XO_UMBRELLA_SOURCE_DIR}/${dep_include_subdir}>)
    target_include_directories(${target} ${visibility}
        $<BUILD_INTERFACE:${XO_UMBRELLA_BINARY_DIR}/${dep_include_subdir}>)

    # note: header directories owned by dep get added to target.
    # however,  header directories _used_ by dep don't get automatically added to target,
    # (for example if referred to from a dep-owned .hpp file)
    #
endmacro()

# ----------------------------------------------------------------
# dependency helper when depending on an xo codebase
#
# Two flavors:
# - depended-on codebase separately built+installed.
#   In this case behaves like any other best-practice cmake dep:
#   use find_package() to rely on install .cmake config files
#   in PREFIX/lib/cmake
# - depended-on codebase in same umbrella source tree as codebase
#   invoking this helper.
#   1. XO_UMBRELLA_SOURCE_DIR gives top of umbrella source tree
#   2. depended-on codebase foo in one of
#      XO_UMBRELLA_SOURCE_DIR/repo/{foo, xo-foo}
#
# target:     cmake target for which to supply a dependency
# visibility: INTERFACE|PUBLIC
#             - INTERFACE must be used when supplying a dependency to a header-only target
# dep:        cmake target on which to depend      (e.g. xo_pyutil)
# nxo_dep:    cmake target without any xo_ prefix. (e.g. pyutil)
#
macro(xo_dependency_helper target visibility dep)
    xo_strip_xo_prefix(${dep} _nxo_dep)

    xo_establish_submodule_build()

    if(XO_SUBMODULE_BUILD)
        if(EXISTS ${XO_UMBRELLA_SOURCE_DIR}/${XO_UMBRELLA_REPO_SUBDIR}/xo-${_nxo_dep})
            xo_dependency_helper1(${target} ${visibility} ${XO_UMBRELLA_REPO_SUBDIR}/xo-${_nxo_dep}/include)
        endif()

        if(EXISTS ${XO_UMBRELLA_SOURCE_DIR}/${XO_UMBRELLA_REPO_SUBDIR}/${_nxo_dep})
            xo_dependency_helper1(${target} ${visibility} ${XO_UMBRELLA_REPO_SUBDIR}/${_nxo_dep}/include)
        endif()

    else()
       message(STATUS "[${target}] find_package(${dep}) (xo_dependency_helper)")
        find_package(${dep} CONFIG REQUIRED)
    endif()

    if (XO_SUBMODULE_BUILD)
        #get_target_property(_tmp ${target} LINK_LIBRARIES)
        #message("xo_dependency_helper: ${target} -> ${dep}: ${target}.LINK_LIBRARIES=${_tmp}")
        #get_target_property(_tmp ${dep} LINK_LIBRARIES)
        #message("xo_dependency_helper: ${target} -> ${dep}: ${dep}.LINK_LIBRARIES=${_tmp}")

        #get_target_property(_tmp ${target} INTERFACE_LINK_LIBRARIES)
        #message("xo_dependency_helper: ${target} -> ${dep}: ${target}.INTERFACE_LINK_LIBRARIES=${_tmp}")
        #get_target_property(_tmp ${dep} INTERFACE_LINK_LIBRARIES)
        #message("xo_dependency_helper: ${target} -> ${dep}: ${dep}.INTERFACE_LINK_LIBRARIES=${_tmp}")

        # add INCLUDE_DIRECTORIES from ${dep} to ${target}.
        #
        # 1. we only need this for a submodule build
        #    1a. cmake automatically adds ${dep}'s directly-set include directories
        #        (i.e. attached via target_include_directories(${dep} ..) etc.
        #    1b. furthermore, cmake adds transitive deps when we use generated
        #        cmake support
        #    1c. cmake doesn't seem automatically to arrange transitive include directories
        #        when we have a chain of target_link_libraries() calls.
        #        (NOTE: it does incorporate include paths from direct dependencies)
        # 2. because of 1a,  need to exclude ${dep}'s own dirs here (to avoid too-long include path)
        #
        get_target_property(_depsrcdir ${dep} xo_srcdir)
        get_target_property(_depbindir ${dep} xo_bindir)
        get_target_property(_tmp ${dep} INTERFACE_INCLUDE_DIRECTORIES)
        if(_tmp)
            #message("xo_dependency_helper: ${target}: + ${dep}.INCLUDE_DIRECTORIES: ${_tmp}")
            foreach(dir ${_tmp})
                # want to add these to compile line for $target}.
                # this will happen automatically for ${dep}'s own directories;
                # those will have been added by
                #   xo_include_options2() / xo_include_headeronly_options2()
                # however we also need directories for ${dep}'s transitive dependencies
                #
                if(${dir} MATCHES "BUILD_INTERFACE")
                    #message("xo_dependency_helper: ${target} -> ${dep}: consider dir=${dir}")
                    if(${dir} MATCHES ${_depsrcdir})
                        #message(" skip ${dir}")
                    elseif(${dir} MATCHES ${_depbindir})
                        #message(" skip ${dir}")
                    else()
                        #message(" KEEP ${dir}")
                        target_include_directories(${target} ${visibility} ${dir})
                    endif()
                endif()
            endforeach()
            get_target_property(_tmp ${target} INTERFACE_INCLUDE_DIRECTORIES)
            list(REMOVE_DUPLICATES _tmp)
            set_property(
                TARGET ${target}
                PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${_tmp})
            #message("xo_dependency_helper: ${target}.INCLUDE_DIRECTORIES: ${_tmp}")
        endif()
    endif()

    # want ${target}.xo_deps to contain union of previous value,  and ${dep}.xo_deps
    set_property(
        TARGET ${target}
        APPEND
        PROPERTY xo_deps ${dep})
    get_target_property(_tmp ${target} xo_deps)
    list(REMOVE_DUPLICATES _tmp)
    set_property(
        TARGET ${target}
        PROPERTY xo_deps ${_tmp})

    #list(REMOVE_DUPLICATES _xo_dependency_tmp1)
endmacro()

# ----------------------------------------------------------------
#
# dependency on an xo library (including header-only libraries)
# e.g. indentlog
#
# An xo package foo works with cmake
#   find_package(foo)
# by providing plugin .cmake files in
#   ${PREFIX}/lib/cmake/foo/fooConfig.cmake
#   ${PREFIX}/lib/cmake/foo/fooConfigVersion.cmake
#   ${PREFIX}/lib/cmake/foo/fooTargets.cmake
#
# dep: name of required dependency,  e.g. indentlog
#
macro(xo_dependency target dep)
    xo_establish_submodule_build()

    #message("xo_dependency: XO_SUBMODULE_BUILD=${XO_SUBMODULE_BUILD}")
    #message("xo_dependency: XO_UMBRELLA_SOURCE_DIR=${XO_UMBRELLA_SOURCE_DIR}")
    #message("xo_dependency: PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")

    xo_dependency_helper(${target} PUBLIC ${dep})

    #message("----------------------------------------------------------------")
    ##message("xo_dependency: ${target}.xo_deps.pre=${_xo_dependency_tmp0}")
    ##message("xo_dependency: ${dep}.xo_deps=${_xo_dependency_tmp2}")
    #get_target_property(_tmp ${target} xo_deps)
    #message("xo_dependency: ${target}.xo_deps=${_tmp}")
    ##get_target_property(_tmp ${target} INCLUDE_DIRECTORIES)
    ##message("xo_dependency: ${target}.INCLUDE_DIRECTORIES=${_tmp} before target_link_libraries with ${dep}")

    target_link_libraries(${target} PUBLIC ${dep})
    #target_link_libraries(${target} ${dep})

    #get_target_property(_tmp ${target} INCLUDE_DIRECTORIES)
    #message("xo_dependency: ${target}.INCLUDE_DIRECTORIES=${_tmp} after target_link_libraries with ${dep}")
    #get_target_property(_tmp ${dep} INCLUDE_DIRECTORIES)
    #message("xo_dependency: ${dep}.INCLUDE_DIRECTORIES=${_tmp}")
    #get_target_property(_tmp ${dep} INTERFACE_INCLUDE_DIRECTORIES)
    #message("xo_dependency: ${dep}.INTERFACE_INCLUDE_DIRECTORIES=${_tmp}")
    #message("----------------------------------------------------------------")
endmacro()

# dependency of a header-only library on another header-only library
#
macro(xo_headeronly_dependency target dep)
    xo_establish_submodule_build()

    xo_dependency_helper(${target} INTERFACE ${dep})

    # Conflict here between PUBLIC and INTERFACE
    #
    # PUBLIC ensures that include directories required by ${dep} will also be included in compilation of ${target};
    # i.e. will appear in property ${target}.INCLUDE_DIRECTORIES
    #
    # INTERFACE mandatory when depending on a header-only library (created with add_library(foo INTERFACE)).
    # otherwise get error:
    #   INTERFACE library can only be used with the INTERFACE keyword of
    #   target_link_libraries
    # Unfortunately target_link_libraries() does not copy dependent's INTERFACE_INCLUDE_DIRECTORIES property
    # (at least asof cmake 3.25.3, cmake 3.29.2).
    # Dependent's INCLUDE_DIRECTORIES property will be empty,  since it's header-only.
    #
    # Workaround by copying property explicity,  which we do below.
    #
    # See xo-unit/examples/ex1 for an executable that needs this workaround to build
    #
    target_link_libraries(${target} INTERFACE ${dep})

    get_target_property(xo_dependency_headeronly__tmp ${dep} INTERFACE_INCLUDE_DIRECTORIES)
    set_property(
        TARGET ${target}
        APPEND PROPERTY INCLUDE_DIRECTORIES ${xo_dependency_headeronly__tmp})
endmacro()

# dependency on external (non-xo) namespaced target
# e.g.
#   add_library(foo ..) or add_executable(foo ...)
# then
#   xo_external_target_dependency(foo Catch2 Catch2::Catch2)
# equivalent to
#   find_package(Catch2 CONFIG REQUIRED)
#   target_link_libraries(foo PUBLIC Catch2::Catch2)
#
# NOTE: this won't work for builtin module targets. For Threads we need just
#   find_package(${pkg} Threads)
#
macro(xo_external_target_dependency target pkg pkgtarget)
    message("-- [${target}] find_package(${pkg}) (xo_external_target_dependency)")
    find_package(${pkg} CONFIG REQUIRED)
    target_link_libraries(${target} PUBLIC ${pkgtarget})
    #target_link_libraries(${target} ${pkgtarget})
endmacro()

# dependency on external (non-xo) target
#
macro(xo_external_dependency target pkg)
    xo_external_target_dependency(${target} ${pkg} ${target})
endmacro()

# dependency on target provided from this codebase.
#
# 1. don't need find_package() in this case,  since details of dep targets
#    must be known to cmake for it to build them.
# 2. in any case, can't use find_package() when cmake runs,
#    because supporting .cmake files haven't been generated yet
#
macro(xo_self_dependency target dep)
    target_link_libraries(${target} PUBLIC ${dep})
endmacro()

# dependency on target provided from this codebase.
#
# Similar comments as for xo_self_dependency()
# 1. don't need find_package() in this case,  since details of dep targets
#    must be known to cmake for it to build them.
# 2. in any case, can't use find_package() when cmake runs,
#    because supporting .cmake files haven't been generated yet
# 3. need to use INTERFACE instead of PUBLIC for a header-only dep
#
macro(xo_self_headeronly_dependency target dep)
    target_link_libraries(${target} INTERFACE ${dep})
endmacro()

# ----------------------------------------------------------------
# need this when linking pybind11-generated libraries
#
macro(xo_pybind11_link_flags)
    # see:
    # 1. FAQ Build Issues Q2
    # 2. CMAKE_SHARED_LINKER_FLAGS in src/CMakeLists.txt
    # 3. pybind11 cmake support, somewhere like
    #   [path/to/pybind11-2.9.2/
    #     lib/python3.9-pybind11-2.9.2/
    #     lib/python3.9/site-packages/
    #     pybind11/share/cmake/
    #     pybind11/pybind11Common.cmake]
    #
    set_property(
      TARGET pybind11::python_link_helper
      APPEND
      PROPERTY
      INTERFACE_LINK_OPTIONS "$<$<PLATFORM_ID:Darwin>:LINKER:-flat_namespace>")

    # looks like pybind11_add_module() tries to link transitive deps
    # of libs mentioned in xo_dependency() -- perhaps for link-time optimization?
    #
    # For example when linking libpyreflect, get link line with -lrefcnt
    # (presumably since cmake knows that libreflect.so depends on librefcnt.so);
    # this triggers error,  since link doesn't know where to find librefcnt.so
    #
    # To workaround,  add ${CMAKE_INSTALL_PREFIX}/lib to the link line.
    #
    # WARNING: expect this not to work in a hermetic nix build!
    #
    set_property(TARGET pybind11::python_link_helper
        APPEND
        PROPERTY
        INTERFACE_LINK_OPTIONS "-L${CMAKE_INSTALL_PREFIX}/lib")
endmacro()

# ----------------------------------------------------------------
# use this for a subdir that builds a python library using pybind11
#
# expecting the following
# 1. a directory pyfoo/ -> library pyfoo
# 2. pyfoo/pyfoo.hpp.in -> pyfoo/pyfoo.hpp
#
macro(xo_pybind11_library target projectTargets source_files)
    xo_strip_xo_prefix(${target} _nxo_target)

    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target})

    configure_file(
        ${_nxo_target}.hpp.in
        ${PROJECT_BINARY_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}/${_nxo_target}.hpp)
    # was ${PROJECT_SOURCE_DIR}/include/xo/${target}/${target}.hpp)

    xo_establish_symlink_install()

    if(XO_SYMLINK_INSTALL)
        xo_install_make_symlink(
            ${PROJECT_BINARY_DIR}/include/xo
            ${CMAKE_INSTALL_PREFIX}/include/xo
            ${_nxo_target})
    else()
        install(
            FILES ${PROJECT_BINARY_DIR}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target}/${_nxo_target}.hpp
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_INCLUDE_STEM_DIR}/${_nxo_target})
    endif()

    message(STATUS "[${target}] find_package(Python) (xo_pybind11_library)")
    find_package(Python COMPONENTS Interpreter Development REQUIRED)
    message(STATUS "[${target}] find_package(pybind11) (xo_pybind11_library)")
    find_package(pybind11)

    # this only works if one source file, right?
    #
    # 6oct2023
    # Having trouble at link time with this.
    # Getting broken link for nix,  because link line lists short library nicknames
    # (e.g. -lfoo) for transitive deps.  nix link needs to be given the directory
    # in which libfoo.so resides,  so we need to ensure full path
    #
    # - source files:
    #     -fPIC -fvisibility=hidden -flto -fno-fat-lto-objects
    # - library:
    #     -fPIC -flto
    #     (transitive closure of library deps for lto?)
    #
    pybind11_add_module(${target} MODULE ${source_files})

    set_property(
        TARGET all_libraries_${PROJECT_NAME}
        APPEND
        PROPERTY targets ${target})

    set_property(
        TARGET ${target}
        PROPERTY xo_deps "${target}")
    set_property(
        TARGET ${target}
        PROPERTY xo_srcdir ${PROJECT_SOURCE_DIR})
    set_property(
        TARGET ${target}
        PROPERTY xo_bindir ${PROJECT_BINARY_DIR})

    xo_pybind11_link_flags()
    xo_include_options2(${target})
    # don't want to symlink include tree,  because lives in build dir.
    # see install for generated .hpp above
    xo_install_library4_noincludes(${target} ${projectTargets})
endmacro()

# ----------------------------------------------------------------
# use this for a dependency of a pybind11 library,
# e.g. that was introduced by xo_pybind11_library()
#
# Working around the following problem (cmake 3.25.3, pybind11 2.10.4)
# if:
# 1. we have pybind11 library pyfoo,  depending on c++ native library foo.
# 2. foo depends on other libraries foodep1, foodep2;
#    assume also that foodep2 is header-only
#
# if we write:
#   # CMakeLists.txt
#   pybind11_add_module(pyfoo MODULE pyfoo.cpp)
#   find_package(foo CONFIG_REQUIRED)
#   target_link_libraries(pyfoo PUBLIC foo)
#
# get compile instructions like:
#   g++ -o pyfoo.cpython-311-x86_64-linux-gnu.so path/to/pyfoo.cpp.o path/to/libfoo.so.x.y -lfoodep1 -lfoodep2
#
# 1. This is broken for foodep2,  since in this case no libfoodep2.so exists
# 2. Also broken for nix build,  because directory containing libfoodep1.so doesn't appear on the compile line.
#    (It's likely possible to extract this from the .cmake package in lib/cmake/foo/fooTargets.cmake,
#     but I don't know how to do that yet)
#
# workaround here is to suppress these secondary dependencies.
# This assumes:
# 1. secondary dependencies are all in shared libraries (not needed on link line)
# 2. (maybe?) primary dependency libfoo.so is sufficient to satisfy g++
#    -- conceivably true if libfoo.so has RUNPATH etc.
#
macro(xo_pybind11_dependency target dep)
    xo_establish_submodule_build()

    xo_dependency_helper(${target} PUBLIC ${dep})
    # clobber secondary dependencies, as discussed above
    if(XO_SUBMODULE_BUILD)
        # ok to keep dep libraries on link line in submodule build
        #message("xo_pybind11_dependency: ${target}: don't clobber ${dep}.INTERFACE_LINK_LIBRARIES")

        # looks like also broken in submodule build?
        #message("-- [${target}] remove ${dep}.INTERFACE_LINK_LIBRARIES to avoid problems with transitive deps (xo_pybind11_dependency)")
        #set_property(TARGET ${dep} PROPERTY INTERFACE_LINK_LIBRARIES "")
    else()
        message("-- [${target}] remove ${dep}.INTERFACE_LINK_LIBRARIES to avoid problems with transitive deps (xo_pybind11_dependency)")
        set_property(TARGET ${dep} PROPERTY INTERFACE_LINK_LIBRARIES "")

        # also have to clobber libraries for
    endif()
    # now that secondary deps are gone,  attach to target pybind11 library
    # skip xo_dependency() here,  that would repeat the find_package() expansion
    target_link_libraries(${target} PUBLIC ${dep})
endmacro()

# use when one xo pybind library imports another.
# for example
#   pyprintjson -> pyreflect
#
macro(xo_pybind11_header_dependency target dep)
    xo_dependency_helper(${target} PUBLIC ${dep})
endmacro()
