/** @file DLocalSymtab.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
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

            struct Slot {
                // obj<Expression,DVariable> var_;
                Binding binding_;
            };

        public:
//            explicit DLocalSymtab(obj<AGCObject> value) noexcept;

            /** @defgroup xo-expression2-symboltable-facet symboltable facet**/
            ///@{

            /** true for global symbol table **/
            bool is_global_symtab() const noexcept { return false; }

            /** lookup binding for variable @p sym **/
            Binding lookup_binding(const DUniqueString * sym) const noexcept;

            ///@}

        private:

        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.hpp */
