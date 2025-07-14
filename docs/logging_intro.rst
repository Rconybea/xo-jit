.. _logging-intro:

Introduction
============

Indentlog provides is a logging library that autoindents to reveal the calling program's call graph.

Features
--------

* **Indenting** Nesting level and is consistent with program structure.

* **Pretty-Printer** Includes configurable, general-purpose pretty-printer.

* **Efficient** Uses custom streambuf for efficiency.
  Avoids executing code to assemble input values that would be discarded below currently configured
  logging level

* **Configurable** configure from global configuration object `logconfig`.

* **Thread-aware** logging state is accessed from thread-local variables.
  Logging for different threads is assembled in thread-local state for improved concurrency.

* **Colorized** generate colorized output, depending on configuration.

Examples
--------

.. code-block:: cpp

    #include "xo/indentlog/scope.hpp"

    using namespace xo;

    void inner(int x) {
        scope log(XO_ENTER0(always), ":x ", x);
    }

    void outer(int y) {
        scope log(XO_ENTER0(always), ":y ", y);

        inner(2*y);
    }

    int
    main(int argc, char ** argv) {
        outer(123);
    }

output:

.. code-block:: text

    15:28:35.702611 +(0) outer :y 123                                               [ex1.cpp:12]
    15:28:35.702723   +(1) inner :x 246                                             [ex1.cpp:8]
    15:28:35.702737   -(1) inner
    15:28:35.702760 -(0) outer
