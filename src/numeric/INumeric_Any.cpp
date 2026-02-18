/** @file INumeric_Any.cpp
 *
 **/

#include "detail/INumeric_Any.hpp"
#include <iostream>

namespace xo {
namespace scm {

using xo::facet::DVariantPlaceholder;
using xo::facet::typeseq;
using xo::facet::valid_facet_implementation;

void
INumeric_Any::_fatal()
{
    /* control here on uninitialized IAllocator_Any.
     * Initialized instance will have specific implementation type
     */
    std::cerr << "fatal"
              << ": attempt to call uninitialized"
              << " INumeric_Any method"
              << std::endl;
    std::terminate();
}

typeseq
INumeric_Any::s_typeseq = typeseq::id<DVariantPlaceholder>();

bool
INumeric_Any::_valid
  = valid_facet_implementation<ANumeric, INumeric_Any>();

// nonconst methods


} /*namespace scm*/
} /*namespace xo*/

/* end INumeric_Any.cpp */
