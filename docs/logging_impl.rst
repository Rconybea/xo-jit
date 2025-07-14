.. _logging_impl:

Logging Components
==================

Install instructions :doc:`here<install>`

Abstraction tower for *xo-indentlog* logging components:

.. ditaa::
    :--scale: 0.85

    +------------------------------------------+
    |               basic_scope                |
    +------------------------------------------+
    |                state_impl                |
    +-----------------------+------------------+
    |                       |     log_config   |
    |      log_streambuf    +------------------+
    |                       |     log_level    |
    +-----------------------+------------------+

**basic_scope**
    Primary entry point for scope-indented logging.
    Lightweight with RAII semantics.

**state_impl**
    Bookkeeping for internal state, managed by ``basic_scope``.

**log_streambuf**
    Custom streambuf implementation, buffering in memory.

**log_config**
    Logging configuration, to control formatting behavior.

**log_level**
    Categorize log messages by importance for filtering.

.. toctree::
   :maxdepth: 2

   xo::scope <scope-reference>


Log_streambuf
-------------

Indentlog relies on a custom ``std::streambuf`` implementation, ``xo::log_streambuf``.
log_streambuf captures content in an internal vector, expanding it on demand.
To support pretty-printing, log_streambuf tracks current position relative to left margin

.. uml::
    :caption: log_streambuf
    :scale: 99%
    :align: center

    class streambuf {
    }

    class log_streambuf {
    - solpos_: size_t
    - buf_v_: vector
    }

    streambuf <|-- log_streambuf
