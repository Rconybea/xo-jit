.. _install:

.. toctree::
   :maxdepth: 2

Source
======

Source code lives on github `here`_

.. _here: https://github.com/rconybea/indentlog

To clone from git:

.. code-block:: bash

    git clone https://github.com/rconybea/indentlog xo-indentlog

Tested with gcc 13.3

Install
=======

Since ``xo-indentlog`` is header-only, can incorporate into another project by simply copying the
include directories to somewhere convenient

Copy includes
-------------

.. code-block:: bash

    # for example
    cd myproject
    mkdir -p ext/xo-indentlog
    rsync -a -v path/to/xo-indentlog/include/ ext/xo-indentlog/

Include as git submodule
------------------------

.. code-block::bash

    cd myproject
    git submodule add -b main https://github.com/rconybea/indentlog ext/xo-indentlog
    git submodule update --init ext/xo-indentlog

This assumes you organize directly-incorporated dependencies under directory ``myproject/ext``.
You would then add ``myproject/ext/xo-indentlog/include`` to your compiler's include path,
and from c++ do something like

.. code-block:: c++

    #include <xo/indentlog/scope.hpp>
    #include <xo/indentlog/print/pretty.hpp>

in c++ source files that rely on xo-indentlog and pretty printing

Install along with XO
---------------------

Install along with the rest of *XO* from `xo-umbrella2 source`_

.. _xo-umbrella2 source: https://github.com/rconybea/xo-umbrella2

(instructions in xo-umbrella2/README.md)

Building from source
--------------------

Although the xo-indentlog library is header-only, unit tests and examples have additional dependencies.
Example instructions for build strating from stock ubuntu (i.e. github CI) are in `ubuntu-main.yml`_

.. _ubuntu-main.yml: https://github.com/Rconybea/indentlog/blob/main/.github/workflows/ubuntu-main.yml

Unit-test and example dependencies:

* `catch2`_ header-only unit-test framework
* `xo-cmake`_ cmake macros (shared with other XO projects)

.. _catch2: https://github.com/catchorg/Catch2
.. _xo-cmake: https://github.com/rconybea/xo-cmake

To build documentation, will also need:

* `doxygen`
* `graphviz`
* `sphinx`
* `breathe`
* `sphinx_rtd_theme`
* `sphinxcontrib.ditaa`

Preamble:

.. code-block:: bash

    mkdir -p ~/proj/xo
    cd ~/proj/xo

    git clone https://github/com/rconybea/xo-cmake

    PREFIX=/usr/local

    # want PREFIX/bin in PATH to use xo-cmake helpers
    PATH=$PREFIX/bin:$PATH

Install `xo-cmake`:

.. code-block:: bash

    cmake -B xo-cmake/.build -S xo-cmake
    cmake --install xo-cmake/.build
    xo-build --clone --configure --build --install xo-indentlog

    # or with optional components
    xo-build --clone --configure --build --install --with-examples --with-utests xo-indentlog

Note that you can instead have ``xo-build`` tell you what it /would/ do:

.. code-block:: bash

    xo-build --configure -dry-run xo-indentlog
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_MODULE_PATH=/usr/local/share/cmake -S xo-indentlog -B xo-indentlog/.build -DENABLE_TESTING=1 -DXO_ENABLE_EXAMPLES=1 -DCMAKE_BUILD_TYPE=Debug


Directories under ``PREFIX`` will then contain:

.. code-block::

    PREFIX
    +- bin
    |  +- xo-build
    |  +- xo-cmake-config
    |  \- xo-cmake-lcov-harness
    +- include
    |  \- xo
    |     \- indentlog/
    +- lib
    |  \- cmake
    |     +- indentlog/
    +- share
       +- cmake
       |  \- xo_macros
       |     +- code-coverage.cmake
       |     +- xo-project-macros.cmake
       |     \- xo_cxx.cmake
       +- etc
       |  \- xo
       |     \- subsystem-list
       \- xo-macros
          +- Doxyfile.in
          +- gen-ccov.in
          \- xo-bootstrap-macros.cmake

CMake Support
-------------

To use built-in cmake support to use installed ``xo-indentlog`` from another project:

Make sure ``PREFIX/lib/cmake`` is searched by cmake (for example include it in ``CMAKE_PREFIX_PATH``)

Add to your ``CMakeLists.txt``:

.. code-block:: cmake

    FindPackage(indentlog CONFIG REQUIRED)
    target_link_libraries(mytarget INTERFACE indentlog)

Supported compilers
-------------------

* developed with gcc 13.2.0;  github CI using gcc 11.4.0 (asof April 2024)
