/** @file ParserStack.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserStack.hpp"
#include "SyntaxStateMachine.hpp"

namespace xo {
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

    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStack.cpp */
