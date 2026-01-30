/* file DExpectSymbolSsm.hpp
 *
 * author: Roland Conybeare, Aug 2024
 */

#pragma once

#include "DSyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/indentlog/print/ppindentinfo.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /** @class DExpectSymbolSsm
         *  @brief state machine to expect + capture a symbol
         *
         *  For example:
         *  -  lhs in a define-expression
         **/
        class DExpectSymbolSsm : public DSyntaxStateMachine<DExpectSymbolSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectSymbolSsm>;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DExpectSymbolSsm();

            /** create instance using memory from @p parser_mm **/
            static DExpectSymbolSsm * make(DArena & parser_mm);

            /** start nested parser expecting a symbol,
             *  on top of parser state machine @p p_psm.
             *  On success will deliver symbol by invoking
             *    .on_symbol(sym, p_psm)
             *  to the state machine on top of the stack
             *  as of when this start() method invoked
             **/
            static void start(ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  with overall parser state in @p p_psm
             **/
            static void on_symbol_token(const Token & tk,
                                        ParserStateMachine * p_psm);

            const char * ssm_classname() const noexcept { return "DExpectSymbolSsm"; }

            /** @defgroup scm-expectsymbol-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error mesages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectsymbol-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectSymbolSsm.hpp */
