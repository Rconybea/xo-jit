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
                 &FloatOps::divide,
                 &FloatOps::add,
                 &FloatOps::subtract,
                 &FloatOps::cmp_equal,
                 &FloatOps::cmp_notequal,
                 &FloatOps::cmp_less,
                 &FloatOps::cmp_lessequal,
                 &FloatOps::cmp_greater,
                 &FloatOps::cmp_greatequal);

            NumericDispatch::instance().register_impl<DFloat, DInteger>
                (&FloatIntegerOps::multiply,
                 &FloatIntegerOps::divide,
                 &FloatIntegerOps::add,
                 &FloatIntegerOps::subtract,
                 &FloatIntegerOps::cmp_equal,
                 &FloatIntegerOps::cmp_notequal,
                 &FloatIntegerOps::cmp_less,
                 &FloatIntegerOps::cmp_lessequal,
                 &FloatIntegerOps::cmp_greater,
                 &FloatIntegerOps::cmp_greatequal);

            NumericDispatch::instance().register_impl<DInteger, DFloat>
                (&IntegerFloatOps::multiply,
                 &IntegerFloatOps::divide,
                 &IntegerFloatOps::add,
                 &IntegerFloatOps::subtract,
                 &IntegerFloatOps::cmp_equal,
                 &IntegerFloatOps::cmp_notequal,
                 &IntegerFloatOps::cmp_less,
                 &IntegerFloatOps::cmp_lessequal,
                 &IntegerFloatOps::cmp_greater,
                 &IntegerFloatOps::cmp_greatequal);

            NumericDispatch::instance().register_impl<DInteger, DInteger>
                (&IntegerOps::multiply,
                 &IntegerOps::divide,
                 &IntegerOps::add,
                 &IntegerOps::subtract,
                 &IntegerOps::cmp_equal,
                 &IntegerOps::cmp_notequal,
                 &IntegerOps::cmp_less,
                 &IntegerOps::cmp_lessequal,
                 &IntegerOps::cmp_greater,
                 &IntegerOps::cmp_greatequal);

            log && log(xtag("ANumeric.tseq", typeseq::id<ANumeric>()));

            return true;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end numeric_register_facets.cpp */
