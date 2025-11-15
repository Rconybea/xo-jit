/* Object.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "xo/reflect/TaggedPtr.hpp"
#include "IAlloc.hpp"
#include <concepts>
#include <cstdint>

namespace xo {
    namespace gc {
        class GC;
        class ObjectStatistics;
    };

    template <typename T>
    class gc_ptr;

    template <typename T>
    using gp = gc_ptr<T>;

    /** wrapper for a pointer to garbage-collector-eligible T.
     *  Application code will usually use the alias template gp<T>
     **/
    template <typename T>
    class gc_ptr {
    public:
        using element_type = T;

    public:
        gc_ptr() = default;
        gc_ptr(T * p) : ptr_{p} {}
        gc_ptr(const gc_ptr & x) : ptr_{x.ptr_} {}

        /** create from gc_ptr to some related type @tparam S **/
        template <typename S>
        gc_ptr(const gc_ptr<S> & x) : ptr_{x.ptr()} {}

        static bool is_eq(gc_ptr x1, gc_ptr x2) {
            std::uintptr_t u1 = reinterpret_cast<std::uintptr_t>(x1.ptr());
            std::uintptr_t u2 = reinterpret_cast<std::uintptr_t>(x2.ptr());

            // multiple inheritance shenanigans.
            // (allow interface pointers separated by one pointer)

            if (u1 >= u2)
                return (u1 <= u2 + sizeof(std::uintptr_t));
            else
                return (u2 <= u1 + sizeof(std::uintptr_t));
        }

        T * ptr() const { return ptr_; }
        T ** ptr_address() { return &ptr_; }

        bool is_null() const { return ptr_ == nullptr; }
        void make_null() { ptr_ = nullptr; }

        void assign_ptr(T * x) { ptr_ = x; }

        gc_ptr & operator=(const gc_ptr & x) { ptr_ = x.ptr(); return *this; }
        T * operator->() const { return ptr_; }

    private:
        T * ptr_ = nullptr;
    };

    /** Root class for all xo GC-collectable objects.
     *
     *  Design note:
     *
     *  relying on inheritance means we insist that GC traits
     *  for a type appear directly in that type's vtable, and at specific locations.
     *  This implies one level of indirection when GC traverses an instance.
     *
     *  Would be feasible to relax the must-inherit-from-Object constraint
     *  by having GC use its own wrapper, at cost of an extra layer of indirection
     **/
    class Object {
    public:
        using TaggedPtr = xo::reflect::TaggedPtr;

    public:
        virtual ~Object() = default;

        /** memory allocator for objects.  Likely this will be a GC instance,
         *  but simple arena also supported.
         **/
        static gc::IAlloc * mm;

        /** assign value @p rhs to member @p *lhs of @p parent.
         *  if assignment creates a cross-generational or cross-checkpoint pointer,
         *  add mutation log entry
         **/
        template <typename T>
        static void assign_member(gp<Object> parent, gp<T> * lhs, gp<Object> rhs);

        /** use from GC aux functions **/
        static gc::GC * _gc() { return reinterpret_cast<gc::GC*>(mm); }

        /** during GC
         *  1. copy destination object @p *addr to (new) to-space.
         *  2. overwrite existing object @p *addr with a forwarding pointer to
         *     copy made in step 1.
         *  3. return the location of the copy make in step 1.
         *
         *  @p src.        source object to be forwarded
         *  @p gc.         garbage collector
         */
        static Object * _forward(Object * src, gc::GC * gc);

        template <typename T>
        static void _forward_inplace(T ** src_addr) {
            Object * fwd = _forward(*src_addr, _gc());

            *src_addr = reinterpret_cast<T *>(fwd);
        }

        template <typename T>
        static void _forward_inplace(gp<T> & src) {
            _forward_inplace<T>(src.ptr_address());
        }

        /** primary workhorse for garbage collection.
         *
         *  we assign each object one of three colors: black|gray|white.
         *
         *  color | location   | children   | action                  |
         *  ------+------------+------------+-------------------------+
         *  black | from-space | any        | move to to-space        |
         *  gray  |   to-space | any        | move remaining children |
         *  white |   to-space | white/gray | done                    |
         *
         *  initially all reachable objects are black.
         *  GC is complete when all reachable objects are white.
         *  GC needs a variable amount of temporary storage to keep track of all gray objects
         *
         *  Evacuate reachable object graph rooted at @p src to to-space.
         *  On return all objects reachable from @p src are white
         *
         *  @param src     address of object to evacuate
         *  @param gc      garbage collector
         *  @param stats   per-object-type GC statistics
         **/
        static Object * _deep_move(Object * src, gc::GC * gc, gc::ObjectStatistics * stats);

        /** copy @p src to to-space, and replace original with forwarding pointer to new location.
         *  return the new location
         **/
        static Object * _shallow_move(Object * src, gc::GC * gc);

        // Reflection support

        /** tagged pointer with runtime type information
         **/
        virtual TaggedPtr self_tp() const = 0;

        /** print on stream @p os **/
        virtual void display(std::ostream & os) const = 0;

        // GC support

        /** true iff this object represents a forwarding pointer.
         *  Forwarding pointers are exclusively created by the garbage collector;
         *  forwarding pointers (and only forwarding pointers) return true here.
         **/
        virtual bool _is_forwarded() const { return false; }

        /** offset for uncommon situation where pointer address is offset from object
         *  base address
         **/
        virtual Object * _offset_destination(Object * src) const { return src; };

        /** replace this object with a forwarding pointer referring to @p dest.
         **/
        virtual void _forward_to(Object * dest);

        /** if this object represents a forwarding pointer, return its new location.
         *  forwarding pointers belong to the garbage collector implementation.
         *  (if you have to ask -- no, your class is not a forwarding pointer)
         *  all other objects return nullptr here.
         **/
        virtual Object * _destination() { return nullptr; }

        /** return amount of storage (including padding) consumed by this object,
         *  excluding immediate Object-pointer children
         **/
        virtual std::size_t _shallow_size() const = 0;

         // TODO: _shallow_move() also overwrite *this with gc-only forwarding object point to C

        /** if subject is allocated by GC:
         *  - create copy C in to-space
         *  - destination C will be nursery|tenured depending on location of this.
         *  else
         *  - return this to disengage from GC
         *
         *  Require: @ref mm is an instance of @ref gc::GC
         **/
        virtual Object * _shallow_copy() const = 0;

        /** update child pointers that refer to forwarding pointers,
         *  replacing them with the correct destination.
         *  See @ref Object::deep_move
         *
         *  this   gray object, located in to-space.
         *  fwd1   forwarding objects.
         *         Located in from-space. Invalid at end of GC cycle.
         *  p1,p2  source pointers.
         *  D1,D2  already-forwarded objects. located in to-space.
         *
         *  before:
         *    this       fwd1
         *    +----+     +-+
         *    | p1 ----->|x|-------> D1
         *    |    |     +-+
         *    |    |
         *    | p2 ----------------> D2
         *    +----+
         *
         *  after:
         *    this
         *    +----+
         *    | p1 ----------------> D1
         *    |    |
         *    |    |
         *    | p2 ----------------> D2
         *    +----+
         *
         *  this is now white
         *
         *  @return shallow size of *this.  Must exactly match the amount of memory in to-space
         *  allocated by @ref _shallow_move
         *
         **/
        virtual std::size_t _forward_children() = 0;
    };

    template <typename T>
    void
    Object::assign_member(gp<Object> parent, gp<T> * lhs, gp<Object> rhs)
    {
        Object::mm->assign_member(parent.ptr(),
                                  reinterpret_cast<Object **>(lhs->ptr_address()),
                                  rhs.ptr());
    }

    std::ostream &
    operator<< (std::ostream & os, gp<Object> x);

    /** @class Cpof
     *  @brief argument to operator new used for garbage collector evacuation phase
     *
     *  Tag overloaded operator new to activate allocation policy based on location
     *  in memory of source object.
     **/
    class Cpof {
    public:
        explicit Cpof(const Object * src) : src_{src} {}

        const void * src_ = nullptr;
    };
} /*namespace xo*/

void * operator new (std::size_t z, const xo::Cpof & copy);

/* end Object.hpp */
