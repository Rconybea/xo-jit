.. _introduction:

.. toctree
   :maxdepth: 2

Introduction
============

The ``xo-alloc`` library provides a in incremental, generational collector for c++ code.

Features:

* *incremental* - can reasonably expect short pause times.
* *generational* - focuses effort on collecting young objects,
  on the basis that they're more likely to be garbage.
* *compacting* - each garbage collection cycle evacuates survivors to contiguous memory,
  so effect is to defragment.
* *collects cycles* - collection algorithm naturally collects cyclic references

Tradeoffs:

* Application is responsible for spilling register values and protecting hardware stack,
  since garbage collector cannot indepndently distinguish collectable object pointers from
  non-pointer values.

* GC will not spontaneously run without permission.  Instead will set a pending bit, with GC
  occurring only when application releases it (e.g. when stack+registers are known to be empty of values
  subject to GC).

* GC implementation is single-threaded.  It cannot run in parallel with the mutator (i.e. application code)
  In return this allows GC to be only lightly coupled with application.

* GC divides each generation into separate from- and to- spaces.  A collection cycle copies surviving
  objects out of from-space. Once complete, the entire from-space is treated as empty, and available to
  become to-space on a future cycle.  This means that at any time only half of allocated memory is available
  to the application; the rest is waiting to receive survivors from the next GC cycle.

Design
------

Garbage Collector
^^^^^^^^^^^^^^^^^

The garbage collector supports two generations, labelled *nursery* and *tenured*.
Nursery objects that survive two collection cycles are promoted to tenured space.
Nursery and tenured objects are kept in separate memory areas, instead of being interspersed.

Collection cycles come in two flavors:

1. *incremental* collections - these collect only the nursery space.

2. *full* collections - these collect both nursery and tenured spaces.
   Full collection may incur noticeable GC pauses.

Application Interaction
^^^^^^^^^^^^^^^^^^^^^^^

Application code that interacts with GC has several responsibilities.

1. application must explicitly invoke GC, when convenient. Since in general any GC-eligible object
   may get moved by the collector: once a collection cycle completes,
   it's up to the application to re-load pointers from memory addresses
   (GC roots) that have been shared with the collector.

2. application must identify a set of GC roots.  GC preserves everything reachable from any GC root

3. The collector needs to know how to traverse GC-managed objects.
   We teach it this by requiring that such objects inherit the ``xo::Object`` interface,
   and implement auxiliary function detailed below.

4. GC also needs to know when a mutation alters a pointer from one GC-managed object to another.
   In particular, GC needs to track pointers from tenured space into nursery space,
   and update them when an incremental collection moves nursery objects.
   We do this by requiring application code use a GC-provided assignment primitive
   on GC-eligible pointers.


Example GC Use
--------------

.. code-block:: cpp
   :linenos:

    #include "xo/object/List.hpp"     // polymorphic List with GC support
    #include "xo/object/String.hpp"   // string type with GC support
    #include "xo/alloc/GC.hpp"

    int main() {
        using xo::gc::Config;
        using xo::obj::String;
        using xo::obj::List;
        using xo::gp;

        Config config = { .initial_nursery_z_ = 50*1000,
                          .initial_tenured_z_ = 10*1000*1000,
                          .debug_flag_ = false };

        up<GC> gc = GC::make(config);

        Object::mm = gc; // use GC for allocation of Object (+ derived classes)

        gc->disable_gc(); // gc forbidden

        // tiny example data structure
        gp<String> s1 = String::copy("hello");
        gp<String> s2 = String::copy(", ");
        gp<String> s3 = String::copy("world!");
        gp<List> list = List::cons(s1, List::cons(s2, List::cons(s3, List::nil)));

        // tell GC what to preserve
        gc->add_gc_root(reinterpret_cast<Object **>(list.ptr_address());

        gc->enable_gc();  // triggers immediate gc

        // s1, s2, s3 invalid.
        // list at new address

        std::cout << "list.size=" << list->size << std::endl;
    }

GC-Eligible Types
-----------------

Or, how to inherit ``xo::Object`` and provide GC support

A type Foo that inherits ``xo::Object`` needs to provide overrides for Object methods ``_shallow_size()``,
``_shallow_copy()`` and ``_forward_children()``:

Typical Pattern
^^^^^^^^^^^^^^^

GC support methods look something like this:

* class definition

.. code-block:: cpp
    :linenos:

     #include "xo/alloc/Object.hpp"

     namespace xo {
         class Foo : public xo::Object {
         public:
             ...
             virtual std::size_t _shallow_size() const override;
             virtual Object * _shallow_copy() const override;
             virtual std::size_t _forward_children() override;
         };
     }

* use overloaded ``operator new``

A GC-eligible class will allocate instances using the ``MMPtr`` overload.
This allocates memory in GC-owned space

.. code-block::cpp
    :linenos:

     gp<Foo> Foo::make(...) {
         ...
         return new MMPtr(mm) Foo(...);
     }

* ``_shallow_size()`` returns the amount of memory used by the subject:

.. code-block:: cpp
    :linenos:

     std::size_t Foo::_shallow_size() const { return sizeof(Foo); }

* ``_shallow_copy()`` is invoked during GC to create a copy of the subject

  It should use the ``xo::Cpof`` argument to ``operator new``.

.. code-block:: cpp
    :linenos:

     Object *
     Foo::_shallow_copy() const;

* ``_forward_children()`` is invoked during GC to vist child ``xo::Object`` pointers
  to make sure they survive

.. code-block:: cpp
    :linenos:

     std::size_t
     Foo::_forward_children();

Atomic Types Without Object Pointers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Plain-old-data classes without embedded pointers

.. code-block:: cpp
    :linenos:

     Object *
     Foo::_shallow_copy() const {
         return new (Cpof(this)) Foo(*this);
     }

.. code-block:: cpp
    :linenos:

     std::size_t
     Foo::_forward_children() { return Foo::_shallow_size(); }

For example see ``xo::obj::String`` in ``xo-object``

Non-GC Objects
^^^^^^^^^^^^^^

A class *Foo* that inherits ``xo::Object`` can opt-out of garbage collection by
omitting the ``MMptr(mm)`` overload.

In that case `Foo::_shallow_size()`, `Foo::_shallow_copy()` and `Foo::_forward_children()`
will not be called:

.. code-block:: cpp
    :linenos:

     std::size_t Foo::_shallow_size() const { return sizeof(Foo); }
     Object *    Foo::_shallow_copy() const { assert(false); return nullptr; }
     std::size_t Foo::_forward_children()   { assert(false); return 0; }

For example see ``xo::obj::Boolean`` in ``xo-object``

Structs Containing Object Pointers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A class with object pointers needs to tell GC how to traverse them

.. code-block:: cpp
    :linenos:

     #include "xo/alloc/Object.hpp"

     namespace xo {
         class Foo : public xo::Object {
         public:
             ...
             virtual std::size_t _shallow_size() const override;
             virtual Object * _shallow_copy() const override;
             virtual std::size_t _forward_children() override;

         private:
             gp<Object> bar_;
             gp<Object> quux_;
         };
     }

* ``_forward_children()`` is invoked during GC to fixup child pointers
  that refer to forwarding objects:

.. code-block:: cpp
    :linenos:

     std::size_t
     Foo::_forward_children()
     {
         Object::_forward_inplace(bar_);
         Object::_forward_inplace(quux_);

         return Foo::_shallow_size();
     }

For example see ``xo::obj::List`` in ``xo-object``
