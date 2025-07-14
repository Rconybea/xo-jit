.. _pretty_impl:

Pretty Printer
==============

Abstraction tower for *xo-indentlog* pretty-printer:

.. ditaa::
    :--scale: 0.85

    +--------------------+
    |      toppstr       |
    +--------------------+
    |      ppdetail      |
    +--------------------+
    |      ppstate       |
    +--------------------+
    |      ppconfig      |
    +--------------------+

**toppstr**
    Pretty print arguments to string.

**ppdetail**
    Template for Type-specific pretty-printing.

**ppstate**
    Temporary state object. Created once for each top-level pretty-print call

**ppconfig**
    Pretty-printer configuration.

.. toctree::
   :maxdepth: 1

   ppstr functions <ppstr-reference>
   ppconfig class <ppconfig-reference>
