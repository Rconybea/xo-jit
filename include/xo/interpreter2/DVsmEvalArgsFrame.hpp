/** @file DVsmEvalArgsFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmApplyFrame.hpp"
#include <xo/expression2/DApplyExpr.hpp>

namespace xo {
    namespace scm {

        /** frame for executing an apply expression **/
        class DVsmEvalArgsFrame {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** see picture in VirtualSchematikaMachine._do_eval_apply_op()
             *
             * old_parent = [apply frame]
             * old_cont   = [xfer to called function]
             *
             **/
            DVsmEvalArgsFrame(DVsmApplyFrame * parent,
                              VsmInstr cont,
                              DApplyExpr * apply_expr);

            static DVsmEvalArgsFrame * make(obj<AAllocator> mm,
                                            DVsmApplyFrame * apply_frame,
                                            VsmInstr old_cont,
                                            DApplyExpr * apply_expr);

            DVsmApplyFrame * parent() const noexcept { return parent_; }
            VsmInstr cont() const noexcept { return cont_; }
            const DApplyExpr * apply_expr() const noexcept { return apply_expr_;  }
            int32_t i_arg() const noexcept { return i_arg_; }

            int32_t increment_arg() { return ++i_arg_; }

            std::size_t shallow_size() const noexcept;
            DVsmEvalArgsFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            bool pretty(const ppindentinfo & ppii) const;

        protected:
            /** parent stack frame **/
            DVsmApplyFrame * parent_ = nullptr;
            /** continuation after eval args completed (always VsmInstr::c_apply) **/
            VsmInstr cont_;

            /** expression being evaluated **/
            DApplyExpr * apply_expr_ = nullptr;

            /** next argument to be evaluated. -1 means function head **/
            int32_t i_arg_ = -1;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmEvalArgsFrame.hpp */
