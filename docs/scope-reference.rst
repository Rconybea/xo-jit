.. _scope-reference:

.. toctree::
   :maxdepth: 2

Scope Reference
===============

.. ditaa::
    :--scale: 0.85

    +------------------------------------------+
    |cBLU           basic_scope                |
    +------------------------------------------+
    |                state_impl                |
    +-----------------------+------------------+
    |                       |     log_config   |
    |      log_streambuf    +------------------+
    |                       |     log_level    |
    +-----------------------+------------------+

.. code-block:: cpp

    #include <xo/indentlog/scope.hpp>

Convenience Macros
------------------

.. doxygendefine:: XO_SCOPE

Classes
-------

.. doxygenclass:: xo::scope_setup

Application code isn't expected to interact with this class directly

.. doxygengroup:: scope_setup-instance-vars

Basic Scope
~~~~~~~~~~~

.. doxygenclass:: xo::basic_scope
