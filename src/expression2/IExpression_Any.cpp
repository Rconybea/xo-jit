/** @file IExpression_Any.cpp
 *
 **/

#include "detail/IExpression_Any.hpp"
#include <iostream>

namespace xo {
namespace scm {

using xo::facet::DVariantPlaceholder;
using xo::facet::typeseq;
using xo::facet::valid_facet_implementation;

void
IExpression_Any::_fatal()
{
    /* control here on uninitialized IAllocator_Any.
     * Initialized instance will have specific implementation type
     */
    std::cerr << "fatal"
              << ": attempt to call uninitialized"
              << " IExpression_Any method"
              << std::endl;
    std::terminate();
}

typeseq
IExpression_Any::s_typeseq = typeseq::id<DVariantPlaceholder>();

bool
IExpression_Any::_valid
  = valid_facet_implementation<AExpression, IExpression_Any>();

// nonconst methods

auto
IExpression_Any::assign_valuetype(Opaque, TypeDescr)  noexcept -> void
{
    _fatal();
}


} /*namespace scm*/
} /*namespace xo*/

/* end IExpression_Any.cpp */