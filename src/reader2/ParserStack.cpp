/** @file ParserStack.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserStack.hpp"
#include "SyntaxStateMachine.hpp"

namespace xo {
    using xo::facet::typeseq;

    namespace scm {
        ParserStack::ParserStack(obj<ASyntaxStateMachine> ssm,
                                 ParserStack * parent)
            : ssm_{ssm}, parent_{parent}
        {}

        ParserStack *
        ParserStack::push(ParserStack * stack,
                          obj<AAllocator> mm,
                          obj<ASyntaxStateMachine> ssm)

        {
            void * mem = mm.alloc(typeseq::id<ParserStack>(),
                                  sizeof(ParserStack));

            return new (mem) ParserStack(ssm, stack);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStack.cpp */
