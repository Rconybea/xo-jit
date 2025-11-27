/** @file Blob.cpp
 *
 *  @author Roland Conybeare, Nov 2025
 **/

#include "Blob.hpp"
#include "xo/reflect/Reflect.hpp"
#include "xo/alloc/IAlloc.hpp"

namespace xo {
    using xo::reflect::Reflect;
    using xo::reflect::TaggedPtr;

    gp<Blob>
    Blob::make(gc::IAlloc * mm, std::size_t z) {
        std::byte * mem = mm->alloc(sizeof(Blob) + z);

        return new (mem) Blob(z);
    }

    TaggedPtr
    Blob::self_tp() const
    {
        return Reflect::make_tp(const_cast<Blob*>(this));
    }

    void
    Blob::display(std::ostream & os) const
    {
        os << "<blob" << xtag("z", z_) << ">";
    }

    std::size_t
    Blob::_shallow_size() const {
        return sizeof(Blob) + z_;
    }

    Object *
    Blob::_shallow_copy(gc::IAlloc * mm) const {
        Cpof cpof(mm, this);
        std::byte * cp_mem = mm->alloc_gc_copy(sizeof(Blob) + z_, this);

        gp<Blob> copy = new (cp_mem) Blob(z_);

        ::memcpy(copy->data(), data_, z_);

        return copy.get();
    }

    std::size_t
    Blob::_forward_children(gc::IAlloc *)
    {
        return this->_shallow_size();
    }
}

/* end Blob.cpp */
