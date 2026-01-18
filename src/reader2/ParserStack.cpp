/** @file ParserStack.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserStack.hpp"
#include "SyntaxStateMachine.hpp"

namespace xo {
    using xo::facet::typeseq;

    namespace scm {

        ParserStack *
        ParserStack::push(obj<AAllocator> mm,
                          obj<ASyntaxStateMachine> ssm)

        {
            void * mem = mm.alloc(typeseq::id<ParserStack>(),
                                  sizeof(ParserStack));

            return new (mem) ParserStack(ssm, parent_);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStack.cpp */
