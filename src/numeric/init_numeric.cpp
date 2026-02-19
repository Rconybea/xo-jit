/** @file init_numeric.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "init_numeric.hpp"
#include <xo/procedure2/init_procedure2.hpp>
#include "Subsystem.hpp"
#include "numeric_register_facets.hpp"

namespace xo {
    using xo::scm::numeric_register_facets;

    void
    InitSubsys<S_numeric_tag>::init()
    {
        numeric_register_facets();
    }

    InitEvidence
    InitSubsys<S_numeric_tag>::require()
    {
        InitEvidence retval;

        /* direct subsystem deps for xo-numeric/ */
        retval ^= InitSubsys<S_procedure2_tag>::require();

        /* xo-numeric/'s own initialization code */
        retval ^= Subsystem::provide<S_numeric_tag>("numeric", &init);

        return retval;
    }

} /*namespace xo*/

/* end init_numeric.cpp */
