/** @file DExpectFormalArglistSsm.hpp
 *
 *  @author Roland Conybeare, Aug 2024
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include <xo/object2/DArray.hpp>
#include <xo/arena/DArena.hpp>

#ifdef NOT_YET
#include "exprstate.hpp"
#include "formal_arg.hpp"
#include <vector>
#endif

namespace xo {
    namespace scm {
        /**
         *   ( name(1) : type(1) , ..., )
         *  ^ ^                 ^ ^    ^
         *  | |                 | |    |
         *  | |                 | |    argl_1b
         *  | argl_1a           | argl_1a
         *  argl_0              argl_1b
         *
         *  argl_0 --on_leftparen_token()--> argl_1a
         *  argl_1a --on_formal()--> argl_1b
         *  argl_1b -+-on_comma_token()--> argl_1a
         *           \-on_rightparen_token()--> (done)
         **/
        enum class formalarglstatetype {
            invalid = -1,

            argl_0,
            argl_1a,
            argl_1b,

            n_formalarglstatetype,
        };

        extern const char *
        formalarglstatetype_descr(formalarglstatetype x);

        inline std::ostream &
        operator<< (std::ostream & os, formalarglstatetype x) {
            os << formalarglstatetype_descr(x);
            return os;
        }

        /** @class expect_formal_arglist
         *  @brief parser state-machine for a formal parameter list
         **/
        class DExpectFormalArglistSsm {
        public:
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            DExpectFormalArglistSsm(DArray * argl);

            /** create instance, using memory from @parser_mm **/
            static obj<ASyntaxStateMachine,DExpectFormalArglistSsm> make(DArena & parser_mm);
            static DExpectFormalArglistSsm * _make(DArena & parser_mm);

            static void start(ParserStateMachine * p_psm);

            /** @defgroup scm-expectformalarglistssm-methods general methods **/
            ///@{

            /** update state on incoming token @p tk, with overall parser state in @p psm **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectformalarglistssm-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** mnemonic for expected input (for this ssm) in current state **/
            std::string_view get_expect_str() const;

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

#ifdef NOT_YET

            virtual void on_formal(const rp<Variable> & formal,
                                   parserstatemachine * p_psm) override;
            virtual void on_comma_token(const token_type & tk,
                                        parserstatemachine * p_psm) override;
            virtual void on_rightparen_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;
#endif

            ///@}
            /** @defgroup scm-expectformalarglistssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** parsing state-machine state **/
            formalarglstatetype fastate_ = formalarglstatetype::argl_0;
            /** number of formal parameters encountered.
             *  Invariant: n_args_ <= argl_->size()
             **/
            size_type n_args_ = 0;
            /** populate with (parmaeter-name, parameter-type) list
             *  as they're encountered.
             *
             *  Not using flexible array here since we don't know size
             **/
            DArray * argl_ = nullptr;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectFormalArglistSsm.hpp */
