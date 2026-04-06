/** @file DExpectQDictSsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/object2/Dictionary.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /**
         *  When this syntax is active, parser is already in a quoted-literal context
         *
         *      { key(1) : value(1) ; ... ; }
         *     ^ ^      ^ ^        ^ ^   ^   ^
         *     | |      | qdict_1c | |   |   qdict_2(done)
         *     | |      qdict_1b   | |   qdict_1d
         *     | qdict_1a          | qdict_1a
         *     qdict_0             qdict_1d
         *
         *  qdict_0  --on_leftbrace_token()-->  qdict_1a [push ExpectSymbolSsm]
         *  qdict_1a --on_parsed_symbol()-->    qdict_1b [remember key]
         *  qdict_1a --onrightbace_token()-->   qdictZ(done)
         *  qdict_1b --on_colon_token()-->      qdict_1c [push ExpectQLiteralSsm]
         *  qdict_1c --on_quoted_literal()-->   qdict_1d [remember value]
         *  qdict_1d --on_semicolon_token()-->  qdict_1a [loop]
         *  qdict_1d --on_rightbrace_token()--> qdict_2(done)
         **/
        class QDictXst {
        public:
            enum class code {
                invalid = -1,

                qdict_0,
                qdict_1a,
                qdict_1b,
                qdict_1c,
                qdict_1d,
                qdict_2,

                N
            };

            explicit QDictXst(code x) : code_{x} {}

            /** @return string representation for enum @p x **/
            static const char * _descr(code x);

            code code() const noexcept { return code_; }

            enum code code_;
        };

        inline std::ostream &
        operator<< (std::ostream & os, QDictXst x) {
            os << QDictXst::_descr(x.code_);
            return os;
        }

        class DExpectQDictSsm : public DSyntaxStateMachine<DExpectQDictSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectQDictSsm>;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-expectqdictssm-ctors constructors **/
            ///@{

            DExpectQDictSsm();

            /** create instance using memory from @parser_mm **/
            static obj<ASyntaxStateMachine,DExpectQDictSsm> make(DArena & parser_mm);
            /** create instance using memory from @parser_mm **/
            static DExpectQDictSsm * _make(DArena & parser_mm);

            /** start nested syntax for a quoted dictionary **/
            static void start(ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqdictssm-methods general methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectQDictSsm"; }

            /** update state on incoming leftbrace token @p tk,
             *  with overall parser state in @p p_psm
             *
             *  in qdict_0 advance state to qdict_1a; otherwise error
             **/
            void on_leftbrace_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state on parsed symbol @p sym emitted by nested ssm,
             *  with overall parser state in @p p_psm
             *
             *  in qdict_1a capture key string + advance to qdict_1b; otherwise error
             **/
            void on_symbol_token(const Token & tk,
                                  ParserStateMachine * p_psm);

            /** update state on incoming colon token @p tk,
             *  with overall parser state in @p p_psm
             *
             *  in qdict_1b advance to qdict_1c + push ExpectQLiteralSsm; otherwise error
             **/
            void on_colon_token(const Token & tk,
                                ParserStateMachine * p_psm);

            /** update state in on incoming semicolon token @p tk,
             *  with overall parser state in @p p_psm.
             *
             *  in qdict_1d advance to qdict_1a (ready for another (key,value) pair); otherwise error
             **/
            void on_semicolon_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update state on incoming rightbrace token @p tk,
             *  with overall parser state in @p p_psm
             *
             *  in qdict_1a complete syntax + report literal to parent ssm; otherwise error
             **/
            void on_rightbrace_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqdictssm-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** mnemonic *for expected input (for this ssm) in current state **/
            std::string_view get_expect_str() const;

            /** update state on incoming token @p tk, with overall parser state in @p p_psm **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** update state on quoted literal @p lit, with overall parser state in @p p_psm
             *
             *  in qdict_1c capture (key,value) pair into dictionary + advance to qdict_1d; otherwise error
             **/
            void on_quoted_literal(obj<AGCObject> lit,
                                   ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqdictssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
            /** @defgroup scm-expectqdictssm-gc-support gc support methods **/
            ///@{

            /** gc support: visit gc-aware child pointers **/
            void visit_gco_children(obj<AGCObjectVisitor> gc) noexcept;

            ///@}

        private:
            /** @defgroup ssm-expectqdictssm-member-vars **/
            ///@{

            /** iddentifies qdict parsing state **/
            QDictXst state_;

            /** captured key in next (key,value) pair;
             *  expect to include in @ref dict_
             **/
            const DString * key_ = nullptr;

            /** literal dictionary (assembled incrementally) **/
            DDictionary * dict_ = nullptr;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectQDictSsm.hpp */
