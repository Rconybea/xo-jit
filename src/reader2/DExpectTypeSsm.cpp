/** @file DExpectTypeSsm.cpp
 *
 *  @author Roland Conybeare, Aug 2024
 **/

#include "DExpectTypeSsm.hpp"

namespace xo {
    namespace scm {
        DExpectTypeSsm::DExpectTypeSsm()
        {}

        DExpectTypeSsm *
        DExpectTypeSsm::make(DArena & mm)
        {
            void * mem = mm.alloc(typeseq::id<DArena>(),
                                  sizeof(DArena));

            return new (mem) DExpectTypeSsm();
        }

        void
        DExpectTypeSsm::start(DArena & mm,
        //obj<AAllocator> expr_mm,
                          PArserStateMachine * p_psm)
        {
            DExpectTypeSsm * expect_type_ssm = DExpectTypeSsm::make(mm);

            auto ssm
                = with_facet<ASyntaxStateMachine>::mkobj(expect_type_ssm);

            p_psm->push_ssm(ssm);
        }

        syntaxstatetype
        DExpectTypeSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::expect_type;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectTypeSsm.cpp */
