.. _glossary:

Glossary
--------

.. glossary::
    GC
     | garbage collector

    mlog
     | mutation log.
     | Remembers cross-generation and cross-checkpoint pointers

    nursery
     | in garbage collector, memory region dedicated to young objects.
     | These are objects that have survived less than 2 incremental collection cycles.

    tenured
     | in garbage collector, memory region dedicated to older objects.
     | These are defined as objects that have survived 2 or more incremental collection cycles.

    xgen
     | cross-generation tenured->nursery pointer; requires special GC bookkeeping

    xckp
     | cross-checkpoint pointer; requires special GC bookkeeping

.. toctree::
