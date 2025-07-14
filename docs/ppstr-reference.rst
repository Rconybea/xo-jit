.. _ppstr-reference:

.. toctree::
   :maxdepth: 2

Ppstr Reference
===============

.. ditaa::
    :--scale: 0.85

    +--------------------+
    |cBLU    ppstr       |
    +--------------------+
    |      ppdetail      |
    +--------------------+
    |      ppstate       |
    +--------------------+
    |      ppconfig      |
    +--------------------+

.. code-block:: cpp

    #include <xo/indentlog/ppstr.hpp>

Types
-----

.. doxygenclass:: xo::ppconcat

Functions
---------

.. doxygenfunction:: xo::toppstr

.. doxygenfunction:: xo::toppstr2
