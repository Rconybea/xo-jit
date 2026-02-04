/** @file VsmFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "VsmFrame.hpp"

namespace xo {
    using xo::facet::typeseq;

    namespace scm {

        VsmApplyFrame::VsmApplyFrame(VsmFrame * old_parent,
                                     VsmInstr old_cont,
                                     DArray * args)
        : VsmFrame(old_parent, old_cont),
          args_{args}
        {}            

        VsmApplyFrame *
        VsmApplyFrame::make(obj<AAllocator> mm,
                            VsmFrame * old_parent,
                            VsmInstr old_cont,
                            DArray * args)
        {
            VsmApplyFrame * result = nullptr;

            void * mem = mm.alloc(typeseq::id<VsmApplyFrame>(),
                                  sizeof(VsmApplyFrame));

            result = new (mem) VsmApplyFrame(old_parent,
                                             old_cont,
                                             args);

            assert(result);

            return result;
        }

        // ----- VsmEvalArgsFrame -----

        VsmEvalArgsFrame::VsmEvalArgsFrame(VsmApplyFrame * old_parent,
                                           VsmInstr old_cont)
        : VsmFrame(old_parent, old_cont)
        {}

        VsmEvalArgsFrame *
        VsmEvalArgsFrame::make(obj<AAllocator> mm,
                               VsmApplyFrame * apply_frame,
                               VsmInstr cont)
        {
            VsmEvalArgsFrame * result = nullptr;

            void * mem = mm.alloc(typeseq::id<VsmEvalArgsFrame>(),
                                  sizeof(VsmEvalArgsFrame));

            result = new (mem) VsmEvalArgsFrame(apply_frame, cont);

            assert(result);

            return result;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end VsmFrame.cpp */

