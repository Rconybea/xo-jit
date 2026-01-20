/** @file DGlobalSymtab.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"

namespace xo {
    namespace scm {
        class DUniqueString;

        /** @class DGlobalSymtab
         *  @brief symbol table for toplevel environment
         **/
        struct DGlobalSymtab {
        public:


        public:
            /** @defgroup xo-expression2-symboltable-facet symboltable facet**/
            ///@{

            /** true for global symbol table **/
            bool is_global_symtab() const noexcept { return true; }

            /** lookup binding for variable @p sym **/
            Binding lookup_binding(const DUniqueString * sym) const noexcept;

            ///@}

        private:
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.hpp */
