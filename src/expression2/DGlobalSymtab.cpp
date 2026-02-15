/** @file DGlobalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DGlobalSymtab.hpp"
#include "DUniqueString.hpp"
#include <xo/expression2/Expression.hpp>
#include <xo/expression2/Variable.hpp>
#include <xo/object2/Array.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::map::DArenaHashMap;
    using xo::mm::AGCObject;

    namespace scm {

        DGlobalSymtab::DGlobalSymtab(repr_type * map,
                                     DArray * vars)
        : map_{map}, vars_{vars}
        {
        }

        DGlobalSymtab *
        DGlobalSymtab::make(obj<AAllocator> global_mm,
                            obj<AAllocator> mm,
                            const ArenaHashMapConfig & cfg)
        {
            repr_type * map = nullptr;
            {
                /** memory DGlobalSymtab::map_
                 *  (but not counting the mmap()'s that map will make for itself)
                 **/
                void * global_mem = global_mm.alloc_for<repr_type>();

                map = new (global_mem) repr_type(cfg);
            }            
            assert(map);

            void * symtab_mem = mm.alloc_for<DGlobalSymtab>();

            /* choosing same capacity for hash, vars */
            DArray * vars = DArray::empty(mm, map->capacity());
            assert(vars);

            DGlobalSymtab * symtab = new (symtab_mem) DGlobalSymtab(map, vars);
            assert(symtab);

            return symtab;
        }

        void
        DGlobalSymtab::visit_pools(const MemorySizeVisitor & visitor) const
        {
            if (map_)
                map_->visit_pools(visitor);
        }

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

                // NOTE: expansion here is moot at present (Feb 2026).
                //       Not implemented in ArenaHashMap

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

                assert(map_->size() < map_->capacity());

                (*map_)[sym] = binding.j_slot();

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

            auto ix = map_->find(sym);

            if (ix == map_->end()) 
                return Binding::null();

            return Binding::global(ix->second);
        }

        // ----- gcobject facet -----

        std::size_t
        DGlobalSymtab::shallow_size() const noexcept
        {
            return sizeof(DGlobalSymtab);
        }

        DGlobalSymtab *
        DGlobalSymtab::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            return mm.std_copy_for<DGlobalSymtab>(this);
        }

        std::size_t
        DGlobalSymtab::forward_children(obj<ACollector> gc) noexcept
        {
            // map_ doesn't contain any gc-owned data, can skip

            gc.forward_inplace(&vars_);

            return this->shallow_size();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.cpp */
