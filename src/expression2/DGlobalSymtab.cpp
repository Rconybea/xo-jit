/** @file DGlobalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "GlobalSymtab.hpp"
#include "Typename.hpp"
#include "Binding.hpp"
#include "DUniqueString.hpp"
#include <xo/expression2/Expression.hpp>
#include <xo/expression2/Variable.hpp>
#include <xo/object2/Array.hpp>
#include <xo/alloc2/GCObject.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::map::DArenaHashMap;
    using xo::mm::ACollector;
    using xo::mm::AGCObject;

    namespace scm {

        DGlobalSymtab::DGlobalSymtab(dp<repr_type> var_map,
                                     DArray * vars,
                                     dp<repr_type> type_map,
                                     DArray * types)
            : var_map_{std::move(var_map)}, vars_{vars},
              type_map_{std::move(type_map)}, types_{types}
        {
        }

        DGlobalSymtab::DGlobalSymtab(DGlobalSymtab && other)
        : var_map_{std::move(other.var_map_)},
          vars_{std::move(other.vars_)},
          type_map_{std::move(other.type_map_)},
          types_{std::move(other.types_)}
        {
        }

        DGlobalSymtab *
        DGlobalSymtab::_make(obj<AAllocator> mm,
                             obj<AAllocator> aux_mm,
                             const ArenaHashMapConfig & var_cfg,
                             const ArenaHashMapConfig & type_cfg)
        {
            /* note: using aux_mm for DArenaHashMap superstructure.
             * {variable, type} storage allocated from mm.
             */

            auto var_map = dp<repr_type>::make(aux_mm, var_cfg);
            assert(var_map);

            /* choosing same capacity for hash, vars */
            DArray * vars = DArray::_empty(mm, var_map->capacity());
            assert(vars);

            auto type_map = dp<repr_type>::make(aux_mm, type_cfg);
            assert(type_map);

            DArray * types = DArray::_empty(mm, type_map->capacity());

            void * mem = mm.alloc_for<DGlobalSymtab>();

            auto symtab = new (mem) DGlobalSymtab(std::move(var_map),
                                                  vars,
                                                  std::move(type_map),
                                                  types);
            assert(symtab);

            return symtab;
        }

        obj<AGCObject,DGlobalSymtab>
        DGlobalSymtab::make(obj<AAllocator> mm,
                            obj<AAllocator> aux_mm,
                            const ArenaHashMapConfig & var_cfg,
                            const ArenaHashMapConfig & type_cfg)
        {
            return obj<AGCObject,DGlobalSymtab>(_make(mm, aux_mm, var_cfg, type_cfg));
        }

        void
        DGlobalSymtab::visit_pools(const MemorySizeVisitor & visitor) const
        {
            if (var_map_)
                var_map_->visit_pools(visitor);
            if (type_map_)
                type_map_->visit_pools(visitor);
        }

        DVariable *
        DGlobalSymtab::lookup_variable(const DUniqueString * sym) const noexcept
        {
            Binding existing = this->lookup_binding(sym);

            if (existing.is_null())
                return nullptr;

            auto var_gco = obj<AGCObject,DVariable>::from((*vars_)[existing.j_slot()]);

            //auto var = var_gco.to_facet<AExpression>();
            //assert(var.data());

            return var_gco.data();
        }

        void
        DGlobalSymtab::upsert_variable(obj<AAllocator> mm,
                                       DVariable * var)
        {
            scope log(XO_DEBUG(false), std::string_view(*var->name()));

            //auto gc = mm.try_to_facet<ACollector>();

            // It's possible there's already a global variable
            // with the same name.
            //
            // For example redefining a variable in an interactive session.
            // In this case use the established binding.
            //
            DVariable * existing = this->lookup_variable(var->name());

            if (existing) {
                log && log("variable with this symbol already exists");

                if (existing == var) {
                    // impossible, but.. noop, right?
                    return;
                }

                // adopt the existing binding
                var->assign_path(existing->path());

                // stash new definition (possibly has different type),
                // replacing previous one
                //
                log && log("STUB: need write barrier");
                vars_->assign_at(mm,
                                 existing->path().j_slot(),
                                 obj<AGCObject,DVariable>(var));
                //(*vars_)[existing->path().j_slot()] = obj<AGCObject,DVariable>(var);
            } else {
                log && log("variable is new");

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
                (*var_map_)[var->name()] = binding.j_slot();

                vars_->push_back(mm, obj<AGCObject,DVariable>(var));
            }
        }

        DTypename *
        DGlobalSymtab::lookup_typename(const DUniqueString * sym) const noexcept
        {
            auto ix = type_map_->find(sym);

            if (ix == type_map_->end())
                return nullptr;

            Binding::slot_type i_slot = ix->second;

            auto tname_gco = obj<AGCObject,DTypename>::from((*types_)[i_slot]);

            return tname_gco.data();
        }

        void
        DGlobalSymtab::upsert_typename(obj<AAllocator> mm,
                                       DTypename * tname)
        {
            scope log(XO_DEBUG(true),
                      std::string_view(*tname->name()));

            //auto gc = mm.try_to_facet<ACollector>();

            auto ix = type_map_->find(tname->name());

            if (ix == type_map_->end()) {
                log && log("typename is new");

                DArray::size_type n = types_->size();

                /** make sure types_ has room **/
                if (n == types_->capacity()) {
                    // DArray is out of room.
                    // Reallocate with more capacity
                    DArray * types_2x = DArray::copy(mm, types_, types_->capacity() * 2);

                    if (!types_2x) {
                        assert(false);

                        // in any case, we can't make progress
                        return;
                    }

                    log && log("STUB: need write barrier");
                    this->types_ = types_2x;
                }

                (*type_map_)[tname->name()] = n;

                log && log("STUB: need write barrier");
                types_->push_back(mm, obj<AGCObject,DTypename>(tname));
            } else {
                Binding::slot_type i_slot = ix->second;

                log && log("STUB: need write barrier");
                types_->assign_at(mm, i_slot,
                                  obj<AGCObject,DTypename>(tname));
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

            scope log(XO_DEBUG(false), std::string_view(*sym));

            auto ix = var_map_->find(sym);

            if (ix == var_map_->end())
                return Binding::null();

            return Binding::global(ix->second);
        }

        // ----- gcobject facet -----

        DGlobalSymtab *
        DGlobalSymtab::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DGlobalSymtab::visit_gco_children(VisitReason reason,
                                          obj<AGCObjectVisitor> gc) noexcept
        {
            // map_ doesn't contain any gc-owned data, can skip

#ifdef __APPLE__
            // clang not recognizing these as comptime eligible
            assert(var_map_.is_gc_eligible() == false);
            assert(type_map_.is_gc_eligible() == false);
#else
            static_assert(var_map_.is_gc_eligible() == false);
            static_assert(type_map_.is_gc_eligible() == false);
#endif

            gc.visit_child(reason, &vars_);
            gc.visit_child(reason, &types_);
        }

        // ----- printable facet -----

        bool
        DGlobalSymtab::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "DGlobalSymtab",
                        refrtag("nvar", vars_->size()),
                        refrtag("var_capacity", vars_->capacity()),
                        refrtag("ntype", types_->size()),
                        refrtag("type_capacity", types_->capacity()));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.cpp */
