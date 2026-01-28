/** @file DLocalSymtab.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
#include "DVariable.hpp"
#include "DUniqueString.hpp"
//#include "exprtype.hpp"
//#include <xo/reflect/TaggedPtr.hpp>
//#include <xo/gc/GCObject.hpp>

namespace xo {
    namespace scm {
        /** @class DLocalSymtab
         *  @brief symbol table for a local stack frame
         **/
        struct DLocalSymtab {
        public:
//            using TaggedPtr = xo::reflect::TaggedPtr;
//            using TypeDescr = xo::reflect::TypeDescr;
//            using AGCObject = xo::mm::AGCObject;
//            using typeseq = xo::reflect::typeseq;

            using ppindentinfo = xo::print::ppindentinfo;
            using AAllocator = xo::mm::AAllocator;
            /* note: uint16_t would be fine too */
            using size_type = std::uint32_t;

            struct Slot {
                Slot() = default;
                explicit Slot(const DVariable * var) : var_{var} {}

                /** variable representing a formal argument.
                 *  binding will be correct only within the same layer
                 *  as top-level lambda body
                 *  (i.e. up to the doorstep of each and every nested lambda)
                 **/
                const DVariable * var_ = nullptr;
            };

        public:
            /** @defgroup scm-lambdaexpr-constructors **/
            ///@{

            /** empty instance with capacity for n slots.
             *  Caller must ensure that slots_[0..n) are actually addressable
             **/
            DLocalSymtab(size_type n);

            /** scaffold empty symtab instance,
             *  with capacity for @p n slots, using memory from allocator @p mm
             **/
            static DLocalSymtab * _make_empty(obj<AAllocator> mm, size_type n);

            ///@}
            /** @defgroup scm-lambdaexpr-methods **/
            ///@{

            size_type capacity() const noexcept { return capacity_; }
            size_type size() const noexcept { return size_; }

            const DVariable * lookup_var(Binding ix) const noexcept {
                assert(ix.i_link() == 0);
                assert(ix.j_slot() < static_cast<int32_t>(size_));

                return slots_[ix.j_slot()].var_;
            }

            /** increase slot size (provided beleow capacity) to append
             *  binding for one local variable.  Local variable will be allocated
             *  from @p mm, named @p name, with type described by @p typeref.
             **/
            Binding append_var(obj<AAllocator> mm,
                               const DUniqueString * name,
                               TypeRef typeref);

            ///@}
            /** @defgroup xo-localsymtab-symboltable-facet symboltable facet**/
            ///@{

            /** true for global symbol table **/
            bool is_global_symtab() const noexcept { return false; }

            /** lookup binding for variable @p sym **/
            Binding lookup_binding(const DUniqueString * sym) const noexcept;

            ///@}
            /** @defgroup xo-localsymtab-printable-facet printable facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** actual range of slots_[] array.  Can use inices in [0,..,n) **/
            size_type capacity_ = 0;
            /** number of slots in use **/
            size_type size_ = 0;
            /** memory for names and bindings **/
            Slot slots_[];
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.hpp */
