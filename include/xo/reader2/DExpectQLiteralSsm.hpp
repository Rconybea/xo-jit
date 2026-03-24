/** @file DExpectQLiteralSsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"

namespace xo {
    namespace scm {
        /** @class DExpectQLiteralSsm
         *  @brief parser state-machine for a formal parameter list
         **/
        class DExpectQLiteralSsm : public DSyntaxStateMachine<DExpectQLiteralSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectQLiteralSsm>;
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            explicit DExpectQLiteralSsm(bool cxl_on_rightparen,
                                        bool cxl_on_rightbracket);

            /** create instance, using memory from @parser_mm **/
            static obj<ASyntaxStateMachine,DExpectQLiteralSsm> make(DArena & parser_mm,
                                                                    bool cxl_on_rightparen,
                                                                    bool cxl_on_rightbracket);
            static DExpectQLiteralSsm * _make(DArena & parser_mm,
                                              bool cxl_on_rightparen,
                                              bool cxl_on_rightbracket);

            /** start nested syntax for a quoted literal **/
            static void start(ParserStateMachine * p_psm,
                              bool cxl_on_rightparen = false,
                              bool cxl_on_rightbracket = false);

            /** @defgroup scm-expectformalarglistssm-methods general methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectQLiteralSsm"; }

            /** update state for f64 token @p tk, with overall parser state in @p p_psm.
             *  completes syntax + delegates to parent ssm via @ref on_quoted_literal
             **/
            void on_f64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for i64 token @p tk, with overall parser state in @p p_psm.
             *  completes syntax + delegates to parent ssm via @ref on_quoted_literal
             **/
            void on_i64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for string token @p tk, with overall parser state in @p p_psm.
             *  completes syntax + delegates to parent ssm via @ref on_quoted_literal
             **/
            void on_string_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** update state on incoming token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  Forward in-place to ExpectQListSsm.
             **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state on incoming rightparen token @p tk,
             *  with overall parser state in @p p_psm
             *
             *  Error unless @ref cxl_on_rightparen_
             **/
            void on_rightparen_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            /** update state on incoming leftbracket token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  Forward in-place to ExpectQArraySsm
             **/
            void on_leftbracket_token(const Token &tk,
                                      ParserStateMachine * p_psm);

            /** update state on incoming rightbracket token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  Error unless @ref cxl_on_rightbracket_
             **/
            void on_rightbracket_token(const Token & tk,
                                       ParserStateMachine * p_psm);

            /** update state on incoming leftbrace token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  Forward in-place to ExpectQDictSsm
             **/
            void on_leftbrace_token(const Token & tk,
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
            /** @defgroup scm-expectqliteralssm-gc-support gc support methods **/
            ///@{

            /** gc support: visit gc-aware child pointers **/
            void forward_children(obj<ACollector> gc) noexcept;

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
            /** if true rightparen pops + delegates to parent ssm **/
            bool cxl_on_rightparen_ = false;
            /** if true rightbracket pops + delegates to parent ssm **/
            bool cxl_on_rightbracket_ = false;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectQLiteralSsm.hpp */
