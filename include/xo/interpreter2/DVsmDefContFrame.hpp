/** @file DVsmDefContFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include <xo/expression2/DefineExpr.hpp>
#include <xo/gc/GCObject.hpp>

namespace xo {
    namespace scm {
        /** @brief saved VSM state during evaluation of a SequenceExpr
         **/
        class DVsmDefContFrame {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-vsmdefcontframe-ctors constructors **/
            ///@{

            DVsmDefContFrame(obj<AGCObject> parent,
                             VsmInstr cont,
                             DDefineExpr * def_expr);

            /** create instance using memory from allocator @p mm **/
            static DVsmDefContFrame * make(obj<AAllocator> mm,
                                           obj<AGCObject> parent_stack,
                                           VsmInstr cont,
                                           DDefineExpr * def_expr);

            ///@}
            /** @defgroup scm-vsmdefcontframe-access-methods access methods **/
            ///@{

            obj<AGCObject> parent() const noexcept { return parent_; }
            VsmInstr cont() const noexcept { return cont_; }
            DDefineExpr * def_expr() const noexcept { return def_expr_; }

            ///@}
            /** @defgroup scm-vsmdefcontframe-general-methods general methods **/
            ///@{

            ///@}
            /** @defgroup scm-vsmdefcontframe-gcobject-facet gcobject facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DVsmDefContFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgrouop scm-vsmseqcontframe-printable-facet printable facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const noexcept;

            ///@}

        private:
            /** @defgroup scm-vsmdefcontframe-members member variables **/
            ///@{

            /** saved VSM stack; restore when this frame consumed **/
            obj<AGCObject> parent_;
            /** saved continuation; restore when this frame consumed **/
            VsmInstr cont_;
            /** saved expr. evaluate elements of this sequence in order **/
            DDefineExpr * def_expr_ = nullptr;

            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmDefContFrame.hpp */
