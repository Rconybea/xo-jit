/** @file DVsmApplyFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmFrame.hpp"

namespace xo {
    namespace scm {
        class DVsmApplyFrame : public VsmFrame {
        public:
            using AProcedure = xo::scm::AProcedure;
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DVsmApplyFrame(obj<AGCObject> old_parent,
                           VsmInstr old_cont,
                           DArray * args);

            static DVsmApplyFrame * make(obj<AAllocator> mm,
                                         obj<AGCObject> old_parent,
                                         VsmInstr old_cont,
                                         DArray * args);

            obj<AProcedure> fn() const noexcept { return fn_; }
            DArray * args() const noexcept { return args_; }

            void assign_fn(obj<AProcedure> x) { this->fn_ = x; }

            std::size_t shallow_size() const noexcept;
            DVsmApplyFrame * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

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
    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmApplyFrame.hpp */
