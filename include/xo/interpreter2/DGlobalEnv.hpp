/** @file DGlobalEnv.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/expression2/DGlobalSymtab.hpp>
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {

        /** @brief runtime bindings for global variabels
         *
         *  Implementation here uses a DArenaHashMap to hold <key,value> pairs.
         *  The hash map has its own memory outside GC space.
         *  Keys are DUniqueStrings, also outside GC space.
         *  Values are regular gc-aware objects, generally will be in GC space.
         *
         *  We need collector to traverse all the values in a global env
         *  on each cycle.  Arrange that by having DGlobalEnv itself
         *  in GC space.
         *
         **/
        class DGlobalEnv {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            /** @defgroup scm-globalenv-ctors constructors **/
            ///@{

            DGlobalEnv(DGlobalSymtab * symtab, DArray * values);

            static DGlobalEnv * _make(obj<AAllocator> mm,
                                      DGlobalSymtab * symtab);


            ///@}
            /** @defgroup scm-globalenv-methods methods **/
            ///@{

            /** symbol-table size.  Is the number of distinct global symbols **/
            size_type size() const noexcept { return symtab_->size(); }

            /** lookup current value associated with binding @p ix **/
            obj<AGCObject> lookup_value(Binding ix) const noexcept;

            /** assign value associated with binding @p to @p x.
             *  If need to expand size of this env, use memory from @p mm
             **/
            void assign_value(obj<AAllocator> mm, Binding ix, obj<AGCObject> x);

            ///@}
            /** @defgroup scm-globalenv-gcobject-facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DGlobalEnv * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgroup scm-globalenv-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:

            /** symbol table assigns a unique index for each symbol **/
            DGlobalSymtab * symtab_;

            /** value for a symbol S will be in values_[symtab->lookup_binding(S)] **/
            DArray * values_ = nullptr;
        };
    }
}
