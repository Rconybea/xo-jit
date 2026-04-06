/** @file DLocalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "LocalSymtab.hpp"
#include "Variable.hpp"
#include "Typename.hpp"
#include "DUniqueString.hpp"
#include <xo/object2/Array.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::print::APrintable;
    //using xo::facet::typeseq;
    using xo::print::ppstate;

    namespace scm {

        DLocalSymtab::DLocalSymtab(DLocalSymtab * p,
                                   DArray * vars, DArray * types)
            : parent_{p}, vars_{vars}, types_{types}
        {
        }

        DLocalSymtab *
        DLocalSymtab::_make_empty(obj<AAllocator> mm,
                                  DLocalSymtab * p,
                                  size_type nv,
                                  size_type nt)
        {
            void * mem = mm.alloc_for<DLocalSymtab>();

            DArray * vars = DArray::empty(mm, nv);
            DArray * types = DArray::empty(mm, nt);

            return new (mem) DLocalSymtab(p, vars, types);
        }

        DVariable *
        DLocalSymtab::lookup_var(Binding ix) noexcept
        {
            assert(ix.i_link() == 0);
            assert(ix.j_slot() < static_cast<int32_t>(vars_->size()));

            auto var = obj<AGCObject,DVariable>::from((*vars_)[ix.j_slot()]);

            assert(var);

            return var.data();
        }

        Binding
        DLocalSymtab::append_var(obj<AAllocator> mm,
                                 const DUniqueString * name,
                                 TypeRef typeref)
        {
            assert(name);

            if (vars_->size() >= vars_->capacity() || !name) {
                assert(false);

                return Binding::null();
            } else {
                //size_type i_slot = (this->size_)++;
                Binding binding = Binding::local(vars_->size());

                DVariable * var = DVariable::make(mm, name, typeref, binding);
                vars_->push_back(obj<AGCObject,DVariable>(var));

                return binding;
            }
        }

        void
        DLocalSymtab::append_type(obj<AAllocator> mm,
                                  const DUniqueString * name,
                                  obj<AType> type)
        {
            assert(name);

            if (types_->size() >= types_->capacity() || !name) {
                assert(false);
            } else {
                obj<AGCObject> tname = DTypename::make(mm, name, type);

                types_->push_back(tname);
            }
        }

        Binding
        DLocalSymtab::lookup_binding(const DUniqueString * sym) const noexcept
        {
            assert(sym);

            if (sym) {
                for (size_type i = 0, n = vars_->size(); i < n; ++i) {
                    auto var_i = obj<AGCObject,DVariable>::from((*vars_)[i]);

                    assert(var_i);

                    if (*sym == *(var_i->name()))
                        return var_i->path();
                }
            }

            return Binding();
        }

        // ----- gcobject facet -----

        DLocalSymtab *
        DLocalSymtab::shallow_move(obj<ACollector> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DLocalSymtab::visit_gco_children(obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(&parent_);
            gc.visit_child(&vars_);
            gc.visit_child(&types_);
        }

        // ----- printable facet -----

        bool
        DLocalSymtab::pretty(const ppindentinfo & ppii) const
        {
            ppstate * pps = ppii.pps();

            (void)pps;

            if (ppii.upto()) {
                /* perhaps print on one line */
                if (!pps->print_upto("<LocalSymtab"))
                    return false;

                if (!pps->print_upto(xrefrtag("nvars", vars_->size())))
                    return false;

                for (size_type i = 0, n = vars_->size(); i <n; ++i) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "[%u]", i);

                    obj<APrintable> arg_pr = (*vars_)[i].to_facet<APrintable>();

                    if (!pps->print_upto(xrefrtag(buf, arg_pr)))
                        return false;
                }

                if (!pps->print_upto(xrefrtag("ntypes", types_->size())))
                    return false;

                for (size_type i = 0, n = types_->size(); i < n; ++i) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "[%u]", i);

                    obj<APrintable> type_pr = (*types_)[i].to_facet<APrintable>();

                    if (!pps->print_upto(xrefrtag(buf, type_pr)))
                        return false;
                }

                pps->write(">");
                return true;
            } else {
                /* with line breaks */

                pps->write("<LocalSymtab");
                pps->newline_pretty_tag(ppii.ci1(), "nvars", vars_->size());

                for (size_type i = 0, n = vars_->size(); i < n; ++i) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "[%u]", i);

                    obj<APrintable> arg_pr = (*vars_)[i].to_facet<APrintable>();

                    pps->newline_pretty_tag(ppii.ci1(), buf, arg_pr);
                }

                pps->newline_pretty_tag(ppii.ci1(), "ntypes", types_->size());

                for (size_type i = 0, n = types_->size(); i < n; ++i) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "[%u]", i);

                    obj<APrintable> type_pr = (*types_)[i].to_facet<APrintable>();

                    pps->newline_pretty_tag(ppii.ci1(), buf, type_pr);
                }

                pps->write(">");

                return false;
            }
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.cpp */
