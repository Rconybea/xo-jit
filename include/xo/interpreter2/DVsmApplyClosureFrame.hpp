/** @file DVsmApplyClosureFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include "DLocalEnv.hpp"
#include <xo/expression2/DApplyExpr.hpp>

namespace xo {
    namespace scm {

        /** Frame to preserve VSM registers:
         *   (stack_, parent_, cont_)
         *  while applying a closure.
         **/
        class DVsmApplyClosureFrame {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DVsmApplyClosureFrame(obj<AGCObject> stack,
                                  VsmInstr cont,
                                  DLocalEnv * env);

            /** create instance, using memory from @p mm **/
            static DVsmApplyClosureFrame * make(obj<AAllocator> mm,
                                                obj<AGCObject> parent,
                                                VsmInstr cont,
                                                DLocalEnv * env);

            obj<AGCObject> parent() const { return stack_; }
            VsmInstr cont() const { return cont_; }
            DLocalEnv * local_env() const { return local_env_; }

            /** gcobject facet **/
            std::size_t shallow_size() const noexcept;
            DVsmApplyClosureFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

        protected:
            /** saved VSM stack_ register **/
            obj<AGCObject> stack_;
            /** saved VSM cont_ register **/
            VsmInstr cont_;
            /** saved VSM local_env_ register **/
            DLocalEnv * local_env_ = nullptr;
        };


    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmApplyClosureFrame.hpp */
