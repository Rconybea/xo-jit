/** @file DVsmSeqContFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include <xo/expression2/SequenceExpr.hpp>
#include <xo/gc/GCObject.hpp>

namespace xo {
    namespace scm {
        /** @brief saved VSM state during evaluation of a SequenceExpr
         **/
        class DVsmSeqContFrame {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-vsmevalsequenceframe-ctors constructors **/
            ///@{

            DVsmSeqContFrame(obj<AGCObject> parent,
                                  VsmInstr cont,
                                  DSequenceExpr * seq_expr,
                                  uint32_t i_seq);

            /** create instance using memory from allocator @p mm **/
            static DVsmSeqContFrame * make(obj<AAllocator> mm,
                                                obj<AGCObject> parent,
                                                VsmInstr cont,
                                                DSequenceExpr * seq_expr,
                                                uint32_t i_seq);

            ///@}
            /** @defgroup scm-vsmevalsequenceframe-access-methods access methods **/
            ///@{

            obj<AGCObject> parent() const noexcept { return parent_; }
            VsmInstr cont() const noexcept { return cont_; }
            DSequenceExpr * seq_expr() const noexcept { return seq_expr_; }
            uint32_t i_seq() const noexcept { return i_seq_; }

            ///@}
            /** @defgroup scm-vsmevalsequenceframe-general-methods general methods **/
            ///@{

            uint32_t incr_i_seq() noexcept { return ++(this->i_seq_); }
            
            ///@}
            /** @defgroup scm-vsmevalsequenceframe-gcobject-facet gcobject facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DVsmSeqContFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
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
            DSequenceExpr * seq_expr_ = nullptr;
            /** current sequence element being evaluated **/
            uint32_t i_seq_ = 0;

            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmSeqContFrame.hpp */
