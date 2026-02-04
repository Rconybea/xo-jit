/** @file VsmFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include <xo/procedure2/Procedure.hpp>
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {
        class VsmFrame {
        public:
            VsmFrame(VsmFrame * parent, VsmInstr cont) : parent_{parent}, cont_{cont} {}

            VsmFrame * parent() const noexcept { return parent_; }
            VsmInstr cont() const noexcept { return cont_; }

        protected:
            /** saved VSM stack; restore when this frame consumed **/
            VsmFrame * parent_ = nullptr;
            /** saved continuation; restore when this frame consumed **/
            VsmInstr cont_;
        };

        class VsmApplyFrame : public VsmFrame {
        public:
            using AProcedure = xo::scm::AProcedure;
            using AAllocator = xo::mm::AAllocator;

        public:
            VsmApplyFrame(VsmFrame * old_parent,
                          VsmInstr old_cont,
                          DArray * args);

            static VsmApplyFrame * make(obj<AAllocator> mm,
                                        VsmFrame * old_parent,
                                        VsmInstr old_cont,
                                        DArray * args);

            obj<AProcedure> fn() const noexcept { return fn_; }
            DArray * args() const noexcept { return args_; }

        private:
            /** evaluated target procedure.
             *
             *  note: when initially created, this will be unpopulated;
             *        don't know correct value until we evaluate
             *        expression in head position
             **/
            obj<AProcedure> fn_;
            /** evaluated arguments (to target procedure) **/
            DArray * args_;
        };

        /** frame for executing an apply expression **/
        class VsmEvalArgsFrame : public VsmFrame {
        public:
            using AAllocator = xo::mm::AAllocator;

        public:
            /** see picture in VirtualSchematikaMachine._do_eval_apply_op()
             *
             * old_parent = [apply frame]
             * old_cont   = [xfer to called function]
             *
             **/
            VsmEvalArgsFrame(VsmApplyFrame * old_parent,
                             VsmInstr old_cont);

            static VsmEvalArgsFrame * make(obj<AAllocator> mm,
                                           VsmApplyFrame * apply_frame,
                                           VsmInstr old_cont);

        private:
            /** next argument to be evaluated. -1 means function head **/
            int32_t i_arg_ = -1;
        };

    }  /*namespace scm*/
} /*namespace xo*/

/* end VsmFrame.hpp */

