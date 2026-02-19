/** @file numeric_register_facets.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "numeric_register_facets.hpp"
#include "NumericDispatch.hpp"
#include "Numeric.hpp"

#include "FloatIntegerOps.hpp"
#include "FloatOps.hpp"
#include "float/INumeric_DFloat.hpp"

#include "IntegerOps.hpp"
#include "integer/INumeric_DInteger.hpp"

#include <xo/object2/DFloat.hpp>
#include <xo/object2/Integer.hpp>

#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;

    namespace scm {

        bool
        numeric_register_facets()
        {
            scope log(XO_DEBUG(true));

            FacetRegistry::register_impl<ANumeric, DInteger>();

            NumericDispatch::instance().register_impl<DFloat, DFloat>
                (&FloatOps::multiply,
                 &FloatOps::divide);

            NumericDispatch::instance().register_impl<DFloat, DInteger>
                (&FloatIntegerOps::multiply,
                 &FloatIntegerOps::divide);

            NumericDispatch::instance().register_impl<DInteger, DFloat>
                (&IntegerFloatOps::multiply,
                 &IntegerFloatOps::divide);

            NumericDispatch::instance().register_impl<DInteger, DInteger>
                (&IntegerOps::multiply,
                 &IntegerOps::divide);

            log && log(xtag("ANumeric.tseq", typeseq::id<ANumeric>()));

            return true;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end numeric_register_facets.cpp */
