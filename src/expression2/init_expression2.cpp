/** @file init_expression2.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "init_expression2.hpp"
#include "SetupExpression2.hpp"
#include "expression2_register_types.hpp"

#include <xo/procedure2/init_procedure2.hpp>
#include <xo/alloc2/CollectorTypeRegistry.hpp>

namespace xo {
    using xo::scm::SetupExpression2;
    using xo::mm::CollectorTypeRegistry;

    void
    InitSubsys<S_expression2_tag>::init()
        {
            SetupExpression2::register_facets();

            CollectorTypeRegistry::instance().register_types(&SetupExpression2::register_types);
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
