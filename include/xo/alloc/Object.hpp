/* Object.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "xo/reflect/TaggedPtr.hpp"
#include "IAlloc.hpp"
#include "xo/allocutil/IObject.hpp"
#include "xo/allocutil/gc_ptr.hpp"
#include <concepts>
#include <cstdint>

namespace xo {
    namespace gc {
        class GC;
        class ObjectStatistics;
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
    class Object : public IObject {
    public:
        using TaggedPtr = xo::reflect::TaggedPtr;

    public:
        static gp<Object> from(gp<IObject> x) {
            return dynamic_cast<Object*>(x.ptr());
        }

        virtual ~Object() = default;

        /** memory allocator for objects.  Likely this will be a GC instance,
         *  but simple arena also supported.
         *
         *  Load-bearing for .assign_member()
         **/
        static gc::IAlloc * mm;

        /** assign value @p rhs to member @p *lhs of @p parent.
         *  if assignment creates a cross-generational or cross-checkpoint pointer,
         *  add mutation log entry
         **/
        template <typename T>
        static void assign_member(gp<IObject> parent, gp<T> * lhs, gp<IObject> rhs);

        /** use from GC aux functions **/
        static gc::GC * _gc() { return reinterpret_cast<gc::GC*>(mm); }

        /** during GC
         *  1. copy destination object @p *addr to (new) to-space.
         *  2. overwrite existing object @p *addr with a forwarding pointer to
         *     copy made in step 1.
         *  3. return the location of the copy make in step 1.
         *
         *  @p src.        source object to be forwarded
         *  @p gc.         allocator (poassibly garbage collector)
         */
        static IObject * _forward(IObject * src, gc::IAlloc * gc);

        template <typename T>
        static void _forward_inplace(T ** src_addr, gc::IAlloc * gc) {
            IObject * fwd = _forward(*src_addr, gc);

            *src_addr = reinterpret_cast<T *>(fwd);
        }

        template <typename T>
        static void _forward_inplace(gp<T> & src, gc::IAlloc * gc) {
            _forward_inplace<T>(src.ptr_address(), gc);
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
        static IObject * _deep_move(IObject * src, gc::GC * gc, gc::ObjectStatistics * stats);

        /** copy @p src to to-space. Overwrite original with forwarding pointer to new location.
         *  return the new location
         **/
        static IObject * _shallow_move(IObject * src, gc::IAlloc * gc);

        // Reflection support

        /** tagged pointer with runtime type information
         **/
        virtual TaggedPtr self_tp() const = 0;

        /** print on stream @p os **/
        virtual void display(std::ostream & os) const = 0;

        // Inherited from IObject..

        //virtual bool _is_forwarded() const override { return false; }
        //virtual IObject * _offset_destination(IObject * src) const override { return src; };
        virtual void _forward_to(IObject * dest) override;
        //virtual IObject * _destination() override { return nullptr; }

        virtual std::size_t _shallow_size() const override = 0;
        virtual IObject * _shallow_copy(gc::IAlloc * gc) const override = 0;
        virtual std::size_t _forward_children(gc::IAlloc * gc) override = 0;
    };

    template <typename T>
    void
    Object::assign_member(gp<IObject> parent, gp<T> * lhs, gp<IObject> rhs)
    {
        Object::mm->assign_member(reinterpret_cast<IObject *>(parent.ptr()),
                                  reinterpret_cast<IObject **>(lhs->ptr_address()),
                                  reinterpret_cast<IObject *>(rhs.ptr()));
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
        explicit Cpof(gc::IAlloc * mm, const Object * src) : mm_{mm}, src_{src} {}

        gc::IAlloc * mm_ = nullptr;
        const void * src_ = nullptr;
    };
} /*namespace xo*/

void * operator new (std::size_t z, const xo::Cpof & copy);

/* end Object.hpp */
