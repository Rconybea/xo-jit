.. _implementation:

.. toctree::
   :maxdepth: 2

Library
=======

Library dependency tower for *xo-alloc*:

.. ditaa::

    +------------------------------------------+
    |                xo_alloc                  |
    +------------------------------------------+
    |              xo_indentlog                |
    +------------------------------------------+

Install instructions :doc:`here<install>`

Components
==========

Abstraction tower for *xo-alloc* components:

.. ditaa::
    :--scale: 0.85

    +----------------+-------------+
    |     IAlloc     |   Object    |
    +----------------+-------------+

    +-------------+    +-------------+
    |      GC     |    | Forwarding1 |
    +-------------+    +-------------+
    |  ListAlloc  |
    +-------------+
    |  ArenaAlloc |
    +-------------+

* *IAlloc*
  Allocator interface.

* *Object*
    Root Object Interface for types participating in garbage collection

* *GC*
    Incremental compacting garbage collector.

* *ListAlloc*
    Auto-expanding allocator.  Contains a collection of ArenaAllocs

* *ArenaAlloc*
    Arena allocator (a.k.a bump allocator).

* *Object*
    Interface for types that participate in garbage collection

* *Forwarding1*
    Forwarding pointer. Supports the Object interface;
    used internally by GC during evacuation.

Key Points
----------

* Allocators can be reset, but do not support freeing of individual allocs.
* GC works with types that implement auxiliary GC-support methods.
  Such types must inherit Object.
* A region may uses multiple arenas, but because of allocation activity
  since the last GC.  If necessary, GC will allocate a new to-space with a
  single arena that's large enough to accomodate all objects that might survive
  from a from-space that has acquired multiple arenas.
  Intent is to scale up to find application's working set size, then stabilize

Components
==========

Allocators
----------

Inheritance
^^^^^^^^^^^

.. uml::
    :caption: allocators
    :scale: 99%
    :align: center

    class IAlloc {
     + alloc()
     + alloc_gc_copy()
     + checkpoint()
     + clear()
    }

    class ArenaAlloc {
     + free_ptr()
     - lo_ : byte*
     - checkpoint_ : byte*
     - limit_ : byte*
    }

    IAlloc <|-- ArenaAlloc

    class ListAlloc {
     + expand()
     + free_ptr()
     - start_z_
     - hd_
     - full_l_
    }

    IAlloc <|-- ListAlloc

    class GC {
     + add_gc_root()
     + request_gc()
     + gc_statistics()
     - gc_root_v_[] : Object**
     - nursery_[2] : ListAlloc*
     - tenured_[2] : ListAlloc*
    }

    IAlloc <|-- GC


Composition
^^^^^^^^^^^

.. uml::
    :caption: allocator composition
    :scale: 99%
    :align: center

    object gc<<GC>>
    gc : nursery[from] = n0
    gc : nursery[to] = n1
    gc : tenured[from] = t0
    gc : tenured[to] = t1

    object n0<<ListAlloc>>

    object n1<<ListAlloc>>

    object t0<<ListAlloc>>

    object t1<<ListAlloc>>

    gc o-- n0
    gc o-- n1
    gc o-- t0
    gc o-- t1


Each ListAlloc composes like this:

.. uml::
    :caption: ListAlloc composition
    :scale: 99%
    :align: center

    object x<<ListAlloc>>
    x : hd_ = a0
    x : full_l = {a1, a2}

    object a0<<ArenaAlloc>>
    a0 : lo_ = 0
    a0 : free_ = 12345
    a0 : hi_ = 1000000

    object a1<<ArenaAlloc>>

    object a2<<ArenaAlloc>>

     x o-- a0
     x o-- a1
     x o-- a2

Here *a1* and *a2* are full, while *a0* can still allocate memory.

Objects

.. uml::
    :caption: objects
    :scale: 99%
    :align: center

    class Object {
     + _is_forwarded()
     + _offset_destination()
     + _forward_to()
     + _destination()
     + _shallow_size()
     + _shallow_copy()
     + _forward_children()
    }

    class Forwarding1 {
     - dest_ : Object*
    }

    Object <|-- Forwarding1
