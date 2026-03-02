/** @file DExpectQLiteralSsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
//#include <xo/object2/DArray.hpp>
//#include <xo/arena/DArena.hpp>

namespace xo {
    namespace scm {
#ifdef NOT_USING
        /**
         *  Already in quoted-literal context
         *
         *   #q{            }
         *      ^
         *      qliteral_0
         *
         *  qliteral_0 --on_leftparen_token()--> push QuotedListSsm
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
#endif

        /** @class expect_formal_arglist
         *  @brief parser state-machine for a formal parameter list
         **/
        class DExpectQLiteralSsm : public DSyntaxStateMachine<DExpectQLiteralSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectQLiteralSsm>;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            DExpectQLiteralSsm();

            /** create instance, using memory from @parser_mm **/
            static obj<ASyntaxStateMachine,DExpectQLiteralSsm> make(DArena & parser_mm);
            static DExpectQLiteralSsm * _make(DArena & parser_mm);

            /** start nested syntax for a quoted literal **/
            static void start(ParserStateMachine * p_psm);

            /** @defgroup scm-expectformalarglistssm-methods general methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectQLiteralSsm"; }

            /** update state for f64 token @p tk, with overall parser state in @p p_psm.
             *  delegates to parent ssm via @ref on_quoted_literal
             **/
            void on_f64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state on incoming token @p tk, with overall parser state in @p p_psm **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state on incoming token @p tk, with overall parser state in @p p_psm **/
            void on_comma_token(const Token & tk,
                                ParserStateMachine * p_psm);

            /** update state on incoming rightparen token @p tk, with overall parser state in @p p_psm **/
            void on_rightparen_token(const Token & tk,
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

            ///@}
            /** @defgroup scm-expectformalarglistssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** @defgroup scm-expectformalarglistssm-impl-methods **/
            ///@{

            /** update local state to include parsed formal (param_name, param_type).
             *  If stack memory needed, get from @p parser_alloc.
             *  Lifetime until formal arglist completely parsed
             **/
            void _accept_formal(obj<AAllocator> expr_alloc,
                                DArena & parser_alloc,
                                const DUniqueString * param_name,
                                TypeDescr param_type);

            ///@}

        private:
#ifdef NOT_USING
            /** parsing state-machine state **/
            formalarglstatetype fastate_ = formalarglstatetype::argl_0;
            /** populate with (parameter-name, parameter-type) list
             *  as they're encountered.
             *
             *  Not using flexible array here since we don't know size at construction time
             **/
            DArray * argl_ = nullptr;
#endif
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectQLiteralSsm.hpp */
