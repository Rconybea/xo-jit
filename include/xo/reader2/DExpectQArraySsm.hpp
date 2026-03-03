/** @file DExpectQArraySsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/object2/ListOps.hpp>
//#include <xo/arena/DArena.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /**
         *  Already in quoted-literal context
         *
         *       [ quote-expr ... ]
         *      ^ ^              ^
         *      | qarray_1a      qarray_2(done)
         *      qarray_0
         *
         *  qarray_0 --on_leftbrace_token()--> qarray_1a [push ExpectQLiteralSsm]
         *  qarray_1a --on_quoted_literal()--> qarray_1a [append literal]
         *  qarray_1a --on_rightbrace_token()--> qarray_2(done) [report quoted array]
         **/
        class QArrayXst {
        public:
            enum class code {
                invalid = -1,

                qarray_0,
                qarray_1a,
                qarray_2,

                N
            };

            explicit QArrayXst(code x) : code_{x} {}

            /** @return string representation for enum @p x **/
            static const char * _descr(code x);

            code code() const noexcept { return code_; }

            enum code code_;
        };

        inline std::ostream &
        operator<< (std::ostream & os, QArrayXst x) {
            os << QArrayXst::_descr(x.code_);
            return os;
        }

        /** @class DExpectQArraySsm
         *  @brief parser state-machine for a literal array
         **/
        class DExpectQArraySsm : public DSyntaxStateMachine<DExpectQArraySsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectQArraySsm>;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            /** @defgroup scm-expectqarrayssm-ctors constructors **/
            ///@{

            DExpectQArraySsm();

            /** create instance, using memory from @parser_mm **/
            static obj<ASyntaxStateMachine,DExpectQArraySsm> make(DArena & parser_mm);
            static DExpectQArraySsm * _make(DArena & parser_mm);

            /** start nested syntax for a quoted literal **/
            static void start(ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectformalargarrayssm-methods general methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectQArraySsm"; }

            /** update state on incoming token @p tk, with overall parser state in @p p_psm **/
            void on_leftbracket_token(const Token & tk,
                                      ParserStateMachine * p_psm);

            void on_rightbracket_token(const Token & tk,
                                       ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqarrayssm-ssm-facet syntaxstatemachine facet methods **/
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

            /** update state for nested qliteral @p lit, with overall parser state in @p p_psm.
             *  Appends @p lit to target array
             **/
            void on_quoted_literal(obj<AGCObject> lit,
                                   ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqarrayssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** @defgroup scm-expectqarrayssm-member-vars **/
            ///@{

            /** identifies qarray parsing state **/
            QArrayXst state_;

            /** target array **/
            DArray * array_ = nullptr;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectQArraySsm.hpp */
