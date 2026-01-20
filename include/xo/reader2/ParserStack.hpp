/** @file ParserStack.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include <xo/arena/DArena.hpp>
#include <xo/facet/obj.hpp>
#include <xo/indentlog/print/pretty.hpp>

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
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            ParserStack(DArena::Checkpoint ckp,
                        obj<ASyntaxStateMachine> ssm,
                        ParserStack * parent);

            /** create new top of stack for syntax @p ssm, using memory from @p mm.
             *  previous stack given by @p parent
             **/
            static ParserStack * push(ParserStack * stack,
                                      DArena & mm,
                                      obj<ASyntaxStateMachine> ssm);

            /** unwind effect of last call to @ref push **/
            static ParserStack * pop(ParserStack * stack,
                                     DArena & mm);

            DArena::Checkpoint ckp() const noexcept { return ckp_; }
            obj<ASyntaxStateMachine> top() const noexcept { return ssm_; }
            ParserStack * parent() const noexcept { return parent_; }

            /** regular printing **/
            void print(std::ostream & os) const;
            /** pretty-printer support **/
            bool pretty(const ppindentinfo & ppii) const;

        private:
            /** stack pointer: top of stack just before this instance created **/
            DArena::Checkpoint ckp_;
            /** top of parsing stack: always non-null **/
            obj<ASyntaxStateMachine> ssm_;
            /** remainder of parsing stack excluding top **/
            ParserStack * parent_ = nullptr;
        };

        inline std::ostream & operator<< (std::ostream & os, const ParserStack * x) {
            if (x) {
                x->print(os);
            } else {
                os << "nullptr";
            }
            return os;
        }

    } /*namespace scm*/

    namespace print {
        /** pretty printer in <xo/indentlog/print/pretty.hpp> relies on this specialization
         *  to handle ParserResult instances
         **/
        template <>
        struct ppdetail<xo::scm::ParserStack*> {
            static inline bool print_pretty(const ppindentinfo & ppii, const xo::scm::ParserStack * p) {
                if (p)
                    return p->pretty(ppii);
                else
                    return ppii.pps()->print_upto("nullptr");
            }
        };
    }

} /*namespace xo*/

/* end ParserStack.hpp */
