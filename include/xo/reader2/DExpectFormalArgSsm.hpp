/** @file DExpectFormalSsm.hpp
 *
 *  @author Roland Conybeare, Aug 2024
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
//#include <xo/arena/DArena.hpp>
//#include "exprstate.hpp"

namespace xo {
    namespace scm {
        /**
         *   name : type
         *  ^    ^ ^
         *  |    | formal_2
         *  |    formal_1
         *  formal_0
         *
         *  formal_0 --on_symbol()--> formal_1
         *  formal_1 --on_colon_token()--> formal_2
         *  formal_2 --on_typedescr()--> (done)
         **/
        enum class formalstatetype {
            invalid = -1,

            formal_0,
            formal_1,
            formal_2,

            n_formalstatetype,
        };

        extern const char *
        formalstatetype_descr(formalstatetype x);

        inline std::ostream &
        operator<< (std::ostream & os, formalstatetype x) {
            os << formalstatetype_descr(x);
            return os;
        }

        /** @class expect_formal_xs
         *  @brief parser state-machine for a typed formal parameter
         **/
        class DExpectFormalArgSsm {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DExpectFormalArgSsm();

            /** create empty instance using memory from @p mm **/
            static obj<ASyntaxStateMachine,DExpectFormalArgSsm> make(DArena & mm);

            /** create empty instance using memory from @p mm **/
            static DExpectFormalArgSsm * _make(DArena & mm);

            /** puah instance of this ssm onto @p p_psm **/
            static void start(ParserStateMachine * p_psm);

            /** @defgroup scm-expectformalargssm-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** mnemonic for expected input (for this ssm) in current state **/
            std::string_view get_expect_str() const noexcept;

            /** update state on incoming token @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** update state on parsed symbol @p sym emitted by nested ssm,
             *  with overall parser state in @p p_psm
             **/
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);

            /** update state on parsed typedescr @p td emitted by nested ssm,
             *  with overall parser state in @p p_psm
             **/
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);

            /** update state on parsed expression emitted by nested ssm
             *  with overall parser state in @p p_psm
             **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            /** update state on parsed expression, along with following semicolon,
             *  emitted by nested ssm with overall parser state in @p p_psm
             **/
            void on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectformalargssm-printable-facet printable facet methods **/
            ///@{

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

#ifdef NOT_YET

            virtual void on_symbol(const std::string & symbol_name,
                                   parserstatemachine * p_psm) override;

            virtual void on_colon_token(const token_type & tk,
                                        parserstatemachine * p_psm) override;

            // virtual void on_comma_token(...) override;

#ifdef PROBABLY_NOT
            virtual void on_rightparen_token(const token_type & tk,
                                             exprstatestack * p_stack,
                                             rp<Expression> * p_emit_expr) override;
#endif

            virtual void on_typedescr(TypeDescr td,
                                      parserstatemachine * p_psm) override;

        private:
            static std::unique_ptr<expect_formal_xs> make();
#endif

        private:
            /** parsing state-machine state **/
            formalstatetype fstate_ = formalstatetype::formal_0;

            /** formal parameter name **/
            const DUniqueString * name_ = nullptr;

#ifdef NOT_YET
            /** formal parameter type (if specified) **/
            TypeDescr td_ = nullptr;
#endif
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectFormalArgSsm.hpp */
