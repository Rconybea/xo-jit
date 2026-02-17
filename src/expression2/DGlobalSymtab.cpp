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

        DGlobalSymtab::DGlobalSymtab(dp<repr_type> map,
                                     DArray * vars)
        : map_{std::move(map)}, vars_{vars}
        {
        }

        dp<DGlobalSymtab>
        DGlobalSymtab::make(obj<AAllocator> mm,
                            obj<AAllocator> aux_mm,
                            const ArenaHashMapConfig & cfg)
        {
            auto map = dp<repr_type>::make(aux_mm, cfg);
            assert(map);

            /* choosing same capacity for hash, vars */
            DArray * vars = DArray::empty(mm, map->capacity());
            assert(vars);

            auto symtab = dp<DGlobalSymtab>::make(mm, std::move(map), vars);
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

        void
        DGlobalSymtab::upsert_variable(obj<AAllocator> mm,
                                       DVariable * var)
        {
            // It's possible there's already a global variable
            // with the same name.
            //
            // For example redefining a variable in an interactive session.
            // In this case use the established binding.
            //
            DVariable * existing = this->lookup_variable(var->name());

            if (existing) {
                if (existing == var) {
                    // impossible, but.. noop, right?
                    return;
                }

                // adopt the existing binding
                var->assign_path(existing->path());

                // stash new definition (possibly has different type),
                // replacing previous one
                //
                (*vars_)[existing->path().j_slot()] = obj<AGCObject,DVariable>(var);
            } else {
                DArray::size_type n = vars_->size();

                // NOTE: expansion of var_ array here is moot at present (Feb 2026),
                //       since the feature isn't yet implemented in ArenaHashMap

                /** make sure vars_ has room **/
                if (n == vars_->capacity()) {
                    // DArray is out of room.  Reallocate with more capacity
                    DArray * vars_2x = DArray::copy(mm, vars_, vars_->capacity() * 2);

                    if (!vars_2x) {
                        assert(false);

                        // in any case, we can't make progress
                        return;
                    }

                    this->vars_ = vars_2x;
                }

                /** now we know binding for var **/
                Binding binding = Binding::global(n);

                var->assign_path(binding);

                // need slot# in .map_ for this unique symbol
                (*map_)[var->name()] = binding.j_slot();

                vars_->push_back(obj<AGCObject,DVariable>(var));
            }
        }

#ifdef NOT_USING // don't know if we need this path
        DVariable *
        DGlobalSymtab::establish_variable(obj<AAllocator> mm,
                                          const DUniqueString * sym,
                                          TypeRef typeref)
        {
            DVariable * var = this->lookup_variable(sym);

            if (!var) {
                assert(vars_);

                xxx;
            }

            return var;
        }
#endif

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
            /** can't use std_copy_for because of non-copyable dp<repr_type>
             *
             * TODO: rename to shallow_move() throughout, and have std_copy_for()
             *       -> std_move_for()
             *
             **/

            void * copy_mem = mm.alloc_copy_for(this);

            if (copy_mem) {
                DGlobalSymtab * self = const_cast<DGlobalSymtab*>(this);

                return new (copy_mem) DGlobalSymtab(std::move(self->map_), vars_);
            }

            return nullptr;
        }

        std::size_t
        DGlobalSymtab::forward_children(obj<ACollector> gc) noexcept
        {
            // map_ doesn't contain any gc-owned data, can skip

            gc.forward_inplace(&vars_);

            return this->shallow_size();
        }

        // ----- printable facet -----

        bool
        DGlobalSymtab::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "DGlobalSymtab",
                        refrtag("nsym", vars_->size()),
                        refrtag("capacity", vars_->capacity()));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.cpp */
