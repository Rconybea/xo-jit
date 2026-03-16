/** @file init_interpreter2.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_interpreter2.hpp"
#include "SetupInterpreter2.hpp"

#include <xo/reader2/init_reader2.hpp>
#include <xo/alloc2/CollectorTypeRegistry.hpp>

namespace xo {
    using xo::scm::SetupInterpreter2;
    using xo::scm::PrimitiveRegistry;
    using xo::mm::CollectorTypeRegistry;

    void
    InitSubsys<S_interpreter2_tag>::init()
    {
        SetupInterpreter2::register_facets();
        CollectorTypeRegistry::instance().register_types(&SetupInterpreter2::register_types);
        PrimitiveRegistry::instance().register_primitives(&SetupInterpreter2::register_primitives);
    }

    InitEvidence
    InitSubsys<S_interpreter2_tag>::require()
    {
        scope log(XO_DEBUG(true));

        InitEvidence retval;

        /* direct subsystem deps for xo-interpreter2/ */
        retval ^= InitSubsys<S_reader2_tag>::require();

        /* xo-interpreter2/'s own initialization code */
        retval ^= Subsystem::provide<S_interpreter2_tag>("interpreter2", &init);

        return retval;
    }
} /*namespace xo*/

/* end init_interpreter2.cpp */
