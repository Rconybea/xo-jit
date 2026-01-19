/** @file init_reader2.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_reader2.hpp"
#include "reader2_register_facets.hpp"
#include "reader2_register_types.hpp"

#include <xo/expression2/init_expression2.hpp>
#include <xo/gc/CollectorTypeRegistry.hpp>

namespace xo {
    using xo::scm::reader2_register_facets;
    using xo::scm::reader2_register_types;
    using xo::mm::CollectorTypeRegistry;

    void
    InitSubsys<S_reader2_tag>::init()
    {
        reader2_register_facets();

        CollectorTypeRegistry::instance().register_types(&reader2_register_types);
    }

    InitEvidence
    InitSubsys<S_reader2_tag>::require()
    {
        InitEvidence retval;

        /* direct subsystem deps for xo-reader2/ */
        retval ^= InitSubsys<S_expression2_tag>::require();

        /* xo-reader2/'s own initialization code */
        retval ^= Subsystem::provide<S_reader2_tag>("reader2", &init);

        return retval;
    }
} /*namespace xo*/

/* end init_reader2.cpp */
