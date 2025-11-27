/** @file Blob.hpp
 *
 *  @author Roland Conybeare, Nov 2025
 **/

#pragma once

#include "Object.hpp"
#include "IAlloc.hpp"

namespace xo {
    /** Use to allocate opaque binary data,
     *  with object header.
     *
     *  Not sure if we want to bother implementing reflection for this...
     **/
    class Blob : public Object {
    public:
        Blob(std::size_t z) : z_{z} {};

        static gp<Blob> make(gc::IAlloc * mm, std::size_t z);

        std::size_t size() const { return z_; }
        std::byte * data() { return data_; }

        virtual TaggedPtr self_tp() const final override;
        virtual void display(std::ostream & os) const final override;

        virtual std::size_t _shallow_size() const final override;
        virtual Object * _shallow_copy(gc::IAlloc * gc) const final override;
        virtual std::size_t _forward_children(gc::IAlloc * gc) final override;

    private:
        std::size_t z_ = 0;
        /** flexible array, with @ref z_ bytes **/
        std::byte data_[];
    };
}

/* end Blob.hpp */
