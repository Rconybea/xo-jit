.. _install:

.. toctree::
   :maxdepth: 2

Source
======

Source code lives on github `here`_

.. _here: https://github.com/rconybea/xo-alloc

To clone from git:

.. code-block:: bash

    git clone https://github.com/rconybea/xo-alloc

Tested with gcc 13.3

Install
=======

One-step Install
----------------

Install along with the rest of *XO* from `xo-umbrella2 source`_

.. _xo-umbrella2 source: https://github.com/rconybea/xo-umbrella2

Minimal Install
---------------

To build+install just required dependencies:
``xo-alloc`` uses several supporting libraries from the *XO* project:

- `xo-indentlog source`_ (structured logging)
- `xo-cmake source`_ (shared cmake macros)

.. _xo-indentlog source: https://github.com/rconybea/indentlog
.. _xo-cmake source: https://github.com/rconybea/xo-cmake

Building from source
--------------------

Install scripts for XO libraries depend on helper scripts installed from `xo-cmake`.

Preamble:

.. code-block:: bash

    mkdir -p ~/proj/xo
    cd ~/proj/xo

    git clone https://github.com/rconybea/xo-cmake

    PREFIX=/usr/local  # ..or desired installation prefix

    # want PREFIX/bin in PATH to use xo-cmake helpers
    PATH=$PREFIX/bin:$PATH

Install `xo-cmake`:

.. code-block:: bash

    cmake -B xo-cmake/.build -S xo-cmake
    cmake --install xo-cmake/.build

Install remaining dependencie(s) in topological order:

.. code-block:: bash

    xo-build --clone --configure --build --install xo-indentlog
    xo-build --clone --configure --build --install xo-alloc

Directories under ``PREFIX`` will then contain:

.. code-block::

    PREFIX
    +- bin
    |  +- xo-build
    |  +- xo-cmake-config
    |  \- xo-cmake-lcov-harness
    +- include
    |  \- xo
    |     +- alloc/
    |     \- indentlog/
    +- lib
    |  +- cmake
    |  |  +- xo_alloc/
    |  |  \- indentlog/
    |  +- lib*.so
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

To use built-in cmake support, when using ``xo-alloc`` from another project:

Make sure ``PREFIX/lib/cmake`` is searched by cmake (for example include it in ``CMAKE_PREFIX_PATH``)

Add to your ``CMakeLists.txt``:

.. code-block:: cmake

    FindPackage(xo_alloc CONFIG REQUIRED)
    target_link_libraries(mytarget INTERFACE xo_alloc)
