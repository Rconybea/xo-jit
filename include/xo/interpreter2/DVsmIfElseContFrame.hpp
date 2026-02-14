/** @file DVsmIfElseContFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include <xo/expression2/IfElseExpr.hpp>
#include <xo/gc/GCObject.hpp>

namespace xo {
    namespace scm {
        /** @brief saved VSM state during evaluation of a SequenceExpr
         **/
        class DVsmIfElseContFrame {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-vsmevalsequenceframe-ctors constructors **/
            ///@{

            DVsmIfElseContFrame(obj<AGCObject> parent,
                                VsmInstr cont,
                                DIfElseExpr * ifelse_expr);

            /** create instance using memory from allocator @p mm **/
            static DVsmIfElseContFrame * make(obj<AAllocator> mm,
                                              obj<AGCObject> parent,
                                              VsmInstr cont,
                                              DIfElseExpr * ifelse_expr);

            ///@}
            /** @defgroup scm-vsmevalsequenceframe-access-methods access methods **/
            ///@{

            obj<AGCObject> parent() const noexcept { return parent_; }
            VsmInstr cont() const noexcept { return cont_; }
            DIfElseExpr * ifelse_expr() const noexcept { return ifelse_expr_; }

            ///@}
            /** @defgroup scm-vsmevalsequenceframe-general-methods general methods **/
            ///@{

            ///@}
            /** @defgroup scm-vsmevalsequenceframe-gcobject-facet gcobject facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DVsmIfElseContFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgrouop scm-vsmseqcontframe-printable-facet printable facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const noexcept;

            ///@}
            
        private:
            /** @defgroup scm-vsmevalsequenceframe-members member variables **/
            ///@{

            /** saved VSM stack; restore when this frame consumed **/
            obj<AGCObject> parent_;
            /** saved continuation; restore when this frame consumed **/
            VsmInstr cont_;
            /** saved expr. evaluate elements of this sequence in order **/
            DIfElseExpr * ifelse_expr_ = nullptr;

            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmIfElseContFrame.hpp */
