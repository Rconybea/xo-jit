/** @file ISyntaxStateMachine_Any.cpp
 *
 **/

#include "ssm/ISyntaxStateMachine_Any.hpp"
#include <iostream>

namespace xo {
namespace scm {

using xo::facet::DVariantPlaceholder;
using xo::facet::typeseq;
using xo::facet::valid_facet_implementation;

void
ISyntaxStateMachine_Any::_fatal()
{
    /* control here on uninitialized IAllocator_Any.
     * Initialized instance will have specific implementation type
     */
    std::cerr << "fatal"
              << ": attempt to call uninitialized"
              << " ISyntaxStateMachine_Any method"
              << std::endl;
    std::terminate();
}

typeseq
ISyntaxStateMachine_Any::s_typeseq = typeseq::id<DVariantPlaceholder>();

bool
ISyntaxStateMachine_Any::_valid
  = valid_facet_implementation<ASyntaxStateMachine, ISyntaxStateMachine_Any>();

// nonconst methods

auto
ISyntaxStateMachine_Any::on_symbol_token(Opaque, const Token &, ParserStateMachine *)  -> void
{
    _fatal();
}

auto
ISyntaxStateMachine_Any::on_def_token(Opaque, const Token &, ParserStateMachine *)  -> void
{
    _fatal();
}

auto
ISyntaxStateMachine_Any::on_if_token(Opaque, const Token &, ParserStateMachine *)  -> void
{
    _fatal();
}

auto
ISyntaxStateMachine_Any::on_colon_token(Opaque, const Token &, ParserStateMachine *)  -> void
{
    _fatal();
}

auto
ISyntaxStateMachine_Any::on_parsed_symbol(Opaque, std::string_view, ParserStateMachine *)  -> void
{
    _fatal();
}


} /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_Any.cpp */