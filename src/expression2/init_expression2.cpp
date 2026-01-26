/** @file init_expression2.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include "expression2_register_facets.hpp"
#include "expression2_register_types.hpp"

#include <xo/procedure2/init_procedure2.hpp>
#include <xo/gc/CollectorTypeRegistry.hpp>

namespace xo {
    using xo::scm::expression2_register_facets;
    using xo::scm::expression2_register_types;
    using xo::mm::CollectorTypeRegistry;

    void
    InitSubsys<S_expression2_tag>::init()
        {
            expression2_register_facets();

            CollectorTypeRegistry::instance().register_types(&expression2_register_types);
        }

    InitEvidence
    InitSubsys<S_expression2_tag>::require()
        {
            InitEvidence retval;

            /* direct subsystem deps for xo-object2/ */
            retval ^= InitSubsys<S_procedure2_tag>::require();

            /* xo-expression2/'s own initialization code */
            retval ^= Subsystem::provide<S_expression2_tag>("expression2", &init);

            return retval;
        }
} /*namespace xo*/

/* end init_expression2.cpp */
