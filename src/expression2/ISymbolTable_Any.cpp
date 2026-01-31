/** @file ISymbolTable_Any.cpp
 *
 **/

#include "symtab/ISymbolTable_Any.hpp"
#include <iostream>

namespace xo {
namespace scm {

using xo::facet::DVariantPlaceholder;
using xo::facet::typeseq;
using xo::facet::valid_facet_implementation;

void
ISymbolTable_Any::_fatal()
{
    /* control here on uninitialized IAllocator_Any.
     * Initialized instance will have specific implementation type
     */
    std::cerr << "fatal"
              << ": attempt to call uninitialized"
              << " ISymbolTable_Any method"
              << std::endl;
    std::terminate();
}

typeseq
ISymbolTable_Any::s_typeseq = typeseq::id<DVariantPlaceholder>();

bool
ISymbolTable_Any::_valid
  = valid_facet_implementation<ASymbolTable, ISymbolTable_Any>();

// nonconst methods


} /*namespace scm*/
} /*namespace xo*/

/* end ISymbolTable_Any.cpp */
