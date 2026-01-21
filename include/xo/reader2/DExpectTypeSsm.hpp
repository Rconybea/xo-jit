/** @file expect_type_xs.hpp
 *
 * @author Roland Conybeare, Aug 2024
 **/

#pragma once

#include "SyntaxStateMachine.hpp"

namespace xo {
    namespace scm {
        /** @class DExpectTypeSsm
         *  @brief syntax state-machine for accepting a Schemtika typename-expression
         *
         *  Jan 2026
         *  Placeholder implementation at present.
         *  Only types are a handful of baked-in values
         *
         *  @pre
         *
         *  @endpre
         **/
        class DExpectTypeSsm {
        public:
            DExpectTypeSsm();

            static DExpectTypeSsm * make(DArena & parser_mm);

            static void start(DArena & parser_mm,
                              //obj<AAllocator> expr_mm,
                              ParserStateMachine * p_psm);

            virtual const char * get_expect_str() const override;

            virtual void on_symbol_token(const token_type & tk,
                                         parserstatemachine * p_psm) override;

            /** @defgroup scm-expecttype-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error messages.
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machien for this syntax on incoming symbol-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            ///@}

        private:
            static std::unique_ptr<expect_type_xs> make();
        };
    } /*namespace scm*/
} /*namespace xo*/


/* end expect_type_xs.hpp */
