.. _install:

.. toctree
   :maxdepth: 2

Source
======

Source code lives on github `here`_

.. _here: https://github.com/rconybea/xo-flatstring

To clone from git:

.. code-block:: bash

    git clone https://github.com/rconybea/xo-flatstring

Implementation relies on c++20 features (expanded use of constexpr; class-instances as template arguments).
Tested with gcc 13.2

Install
=======

Since xo-flatstring is header-only, can incorporate into another project just by copying the include directories
to somewhere convenient.

Copy includes
-------------

.. code-block:: bash

    # For example..
    cd myproject
    mkdir -p ext/xo-flatstring
    rsync -a -v path/to/xo-flatstring/include/ ext/xo-flatstring/

Include as git submodule
------------------------

.. code-block:: bash

    cd myproject
    git submodule add -b main https://github.com/rconybea/xo-flatstring ext/xo-flatstring
    git submodule update --init ext/xo-flatstring

This assumes you organize directly-incorporated dependencies under directory ``myproject/ext``.
You would then add ``myproject/ext/xo-flatstring/include`` to your compiler's include path,
and from c++ do something like

.. code-block:: c++

    #include <xo/flatstring/flatstring.hpp>

in c++ source files that rely on xo-flatstring

Building from source
--------------------

Although the xo-flatstring library is header-only, unit tests have some dependencies.
Example instructions (github CI) for build starting from stock ubuntu are in `ubuntu-main.yml`_

.. _ubuntu-main.yml: https://github.com/Rconybea/xo-flatstring/blob/main/.github/workflows/ubuntu-main.yml

Unit test dependencies:

* `catch2`_ header-only unit-test framework
* `xo-cmake`_ cmake macros
* `xo-indentlog`_ logging with call-structure indenting

.. _catch2: https://github.com/catchorg/Catch2
.. _xo-cmake: https://github.com/rconybea/xo-cmake
.. _xo-indentlog: https://github.com/rconybea/indentlog

To build documentation, will also need:

* `doxygen`
* `graphviz`
* `sphinx`
* `breathe`
* `sphinx_rtd_theme`

Supported compilers
-------------------

* developed with gcc 13.2.0;  github CI using gcc 11.4.0 (asof April 2024)
