/** @file DVarRef.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVarRef.hpp"

namespace xo {
    using xo::mm::AGCObject;
    using xo::reflect::TypeDescr;

    namespace scm {

        DVarRef::DVarRef(DVariable * vardef,
                         Binding path)
                : vardef_{vardef},
                  path_{path}
        {}

        DVarRef *
        DVarRef::make(obj<AAllocator> mm,
                      DVariable * vardef,
                      int32_t link)
        {
            assert(vardef);

            void * mem = mm.alloc_for<DVarRef>();

            return new (mem) DVarRef(vardef,
                                     Binding::relative(link,
                                                       vardef->path()));
        }

        const DUniqueString *
        DVarRef::name() const {
            return vardef_->name();
        }

        TypeRef
        DVarRef::typeref() const noexcept {
            assert(vardef_);

            return vardef_->typeref();
        }

        TypeDescr
        DVarRef::valuetype() const noexcept
        {
            return this->typeref().td();
        }

        void
        DVarRef::assign_valuetype(TypeDescr td) noexcept
        {
            assert(vardef_);
            vardef_->assign_valuetype(td);
        }

        // gcobject facet

        std::size_t
        DVarRef::shallow_size() const noexcept
        {
            return sizeof(DVarRef);
        }

        DVarRef *
        DVarRef::shallow_move(obj<ACollector> gc) noexcept
        {
            return gc.std_copy_for(this);
        }

        std::size_t
        DVarRef::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&vardef_);
            //auto iface = xo::facet::impl_for<AGCObject,DVariable>();
            //gc.forward_inplace(&iface, (void **)vardef_.data_);

            // TODO: concept to indicate that no gc pointers in Binding

            return shallow_size();
        }

        // printable facet

        bool
        DVarRef::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "DVarRef",
                        refrtag("name", std::string_view(*(this->name()))),
                        refrtag("path", this->path_));
        }

    }
} /*namespace xo*/

/* end DVarRef.cpp */
