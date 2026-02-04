/** @file DVsmEvalArgsFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "DVsmApplyFrame.hpp"

namespace xo {
    namespace scm {

        /** frame for executing an apply expression **/
        class DVsmEvalArgsFrame : public VsmFrame {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** see picture in VirtualSchematikaMachine._do_eval_apply_op()
             *
             * old_parent = [apply frame]
             * old_cont   = [xfer to called function]
             *
             **/
            DVsmEvalArgsFrame(obj<AGCObject> old_parent,
                              VsmInstr old_cont);

            static DVsmEvalArgsFrame * make(obj<AAllocator> mm,
                                            obj<AGCObject> apply_frame,
                                            VsmInstr old_cont);

            std::size_t shallow_size() const noexcept;
            DVsmEvalArgsFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            bool pretty(const ppindentinfo & ppii) const;

        protected:
            /** next argument to be evaluated. -1 means function head **/
            int32_t i_arg_ = -1;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmEvalArgsFrame.hpp */
