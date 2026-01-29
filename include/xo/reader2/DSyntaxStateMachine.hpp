/** @file DSyntaxStateMachine.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {

        /** @class DSyntaxStateMachine
         *  @brief static interface for implementing ASyntaxStateMachine
         *
         *  Using CRTP to collect methods common to ASyntaxStateMachine
         *  implementations.
         *
         *  Deliberately unusable through base class pointer.
         *  For runtime polymorphism use something like:
         *  @code
         *    Derived * d = ...;
         *    auto obj = with_facet<ASyntaxStateMachine>::mkobj(d);
         *    // or
         *    obj<ASyntaxStateMachine,Derived>(d)
         *  @endcode
         **/
        template<typename Derived>
        class DSyntaxStateMachine {
            friend Derived;

            /** arglist is DArray of obj<AGCObejct,DVariable> **/
            void on_parsed_formal_arglist(this auto&& self, DArray * arglist, ParserStateMachine * p_psm) {
                p_psm->illegal_parsed_formal_arglist(

            }
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DSyntaxStateMachine.hpp */
