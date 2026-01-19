/** @file ParserStack.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include <xo/alloc2/Allocator.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {

        /** @brief A stack of expression state machines
         *
         *  Each state machine is dedicated to a particular syntax instance.
         *  The innermost machine is in xsm; machines for surrounding expressions
         *  are in progressively removed frames reached via parent links.
         **/
        class ParserStack {
        public:
            using AAllocator = xo::mm::AAllocator;

        public:
            ParserStack(obj<ASyntaxStateMachine> ssm, ParserStack * parent);

            /** create new top of stack for syntax @p ssm, using memory from @p mm.
             *  previous stack given by @p parent
             **/
            static ParserStack * push(ParserStack * stack,
                                      obj<AAllocator> mm,
                                      obj<ASyntaxStateMachine> ssm);

            obj<ASyntaxStateMachine> top() const noexcept { return ssm_; }
            ParserStack * parent() const noexcept { return parent_; }

        private:
            /** top of parsing stack: always non-null **/
            obj<ASyntaxStateMachine> ssm_;
            /** remainder of parsing stack excluding top **/
            ParserStack * parent_ = nullptr;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStack.hpp */
