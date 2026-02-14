/** @file DGlobalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DGlobalSymtab.hpp"
#include "DUniqueString.hpp"
#include <xo/expression2/Expression.hpp>
#include <xo/expression2/Variable.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

#ifdef NOT_YET
        DVariable *
        DGlobalSymtab::lookup_binding(Binding ix) noexcept
        {
            assert(ix.i_link() == -1);
            assert(ix.j_slot() >= 0);
            assert(vars_);
            assert(std::uint64_t(ix.j_slot()) < vars_->size());

            auto var_gco = obj<AGCObject,DVariable>::from((*vars_)[ix.j_slot()]);;
            auto var = var_gco.to_facet<AExpression>();

            assert(var.data());

            return var.data();
        }
#endif

        DVariable *
        DGlobalSymtab::lookup_variable(const DUniqueString * sym) const noexcept
        {
            Binding existing = this->lookup_binding(sym);

            if (existing.is_null())
                return nullptr;

            auto var_gco = obj<AGCObject,DVariable>::from((*vars_)[existing.j_slot()]);
            auto var = var_gco.to_facet<AExpression>();

            assert(var.data());

            return var.data();
        }

        DVariable *
        DGlobalSymtab::establish_variable(obj<AAllocator> mm,
                                          const DUniqueString * sym,
                                          TypeRef typeref)
        {
            DVariable * var = this->lookup_variable(sym);

            if (!var) {
                assert(vars_);

                DArray::size_type n = vars_->size();

                /** make sure vars_ has room **/
                if (n == vars_->capacity()) {
                    // reallocate with more capacity
                    DArray * vars_2x = DArray::copy(mm, vars_, vars_->capacity() * 2);

                    assert(vars_2x);

                    this->vars_ = vars_2x;
                }

                /** create new variable **/
                Binding binding = Binding::global(n);
                var = DVariable::make(mm, sym, typeref, binding);

                if (!var) {
                    // something terribly wrong
                    assert(false);
                    return var;
                }

                map_[sym] = binding.j_slot();

                bool ok = vars_->push_back(obj<AGCObject,DVariable>(var));

                if (!ok)
                    assert(false);
            }

            return var;
        }

        Binding
        DGlobalSymtab::lookup_binding(const DUniqueString * sym) const noexcept
        {
            assert(sym);

            scope log(XO_DEBUG(true), "stub");
            log && log(xtag("sym", std::string_view(*sym)));

            auto ix = map_.find(sym);

            if (ix == map_.end()) 
                return Binding::null();

            return Binding::global(ix->second);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.cpp */
