/** @file init_interpreter2.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_interpreter2.hpp"

#ifdef NOT_YET
#include "interpreter2_register_facets.hpp"
#include "interpreter2_register_types.hpp"
#endif

#include <xo/reader2/init_reader2.hpp>
#ifdef NOT_YET
#include <xo/gc/CollectorTypeRegistry.hpp>
#endif

namespace xo {
#ifdef NOT_YET
    using xo::scm::interpreter2_register_facets;
    using xo::scm::interpreter2_register_types;
    using xo::mm::CollectorTypeRegistry;
#endif

    void
    InitSubsys<S_interpreter2_tag>::init()
    {
#ifdef NOT_YET
        interpreter2_register_facets();

        CollectorTypeRegistry::instance().register_types(&interpreter2_register_types);
#endif
    }

    InitEvidence
    InitSubsys<S_interpreter2_tag>::require()
    {
        InitEvidence retval;

        /* direct subsystem deps for xo-interpreter2/ */
        retval ^= InitSubsys<S_reader2_tag>::require();

        /* xo-interpreter2/'s own initialization code */
        retval ^= Subsystem::provide<S_interpreter2_tag>("interpreter2", &init);

        return retval;
    }
} /*namespace xo*/

/* end init_interpreter2.cpp */
