/** @file ParserStack.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserStack.hpp"
#include "SyntaxStateMachine.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::typeseq;

    namespace scm {
        ParserStack::ParserStack(DArena::Checkpoint ckp,
                                 obj<ASyntaxStateMachine> ssm,
                                 ParserStack * parent)
            : ckp_{ckp}, ssm_{ssm}, parent_{parent}
        {}

        ParserStack *
        ParserStack::push(ParserStack * stack,
                          DArena & mm,
                          obj<ASyntaxStateMachine> ssm)

        {
            DArena::Checkpoint ckp = mm.checkpoint();

            void * mem = mm.alloc(typeseq::id<ParserStack>(),
                                  sizeof(ParserStack));

            return new (mem) ParserStack(ckp, ssm, stack);
        }

        ParserStack *
        ParserStack::pop(ParserStack * stack,
                         DArena & mm)
        {
            assert(stack);

            mm.restore(stack->ckp());

            return stack->parent();
        }

        void
        ParserStack::print(std::ostream & os) const
        {
            os << "<ParserStack>";
            os << xtag("ssm", "*placeholder*");
            os << xtag("parent", "*placeholder*");
            os << ">";
        }

        bool
        ParserStack::pretty(const ppindentinfo & ppii) const
        {
            auto * pps = ppii.pps();

            /* always use multiple lines */

            if (ppii.upto())
                return false;

            pps->write("<ParserStack");

            const ParserStack * frame = this;
            std::size_t i_frame = 0;

            while (frame) {
                char buf[80];
                snprintf(buf, sizeof(buf), "[%lu]", i_frame);

                auto ssm = (FacetRegistry::instance().variant
                            <APrintable, ASyntaxStateMachine> (frame->top()));
                assert(ssm.data());

                pps->newline_pretty_tag(ppii.ci1(), buf, ssm);

                ++i_frame;
                frame = frame->parent_;
            }

            pps->write(">");

            return false;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStack.cpp */
