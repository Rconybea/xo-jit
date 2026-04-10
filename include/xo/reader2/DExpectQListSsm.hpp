/** @file DExpectQListSsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/object2/ListOps.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /**
         *  Already in quoted-literal context
         *
         *       ( quote-expr ... )
         *      ^ ^              ^ ^
         *      | qlist_1a       | qlist_2(done)
         *      qlist_0          qlist_1a
         *
         *  qlist_0 --on_leftparen_token()--> qlist_1a [push ExpectQLiteralSsm]
         *  qlist_1a --on_quoted_literal()--> qlist_1a [append literal]
         *  qlist_1a --on_rightparen_token()--> qlist_2(done) [report quoted list]
         *  qlist_2 end state
         **/
        class QListXst {
        public:
            enum class code {
                invalid = -1,

                qlist_0,
                qlist_1a,
                qlist_2,

                N
            };

            explicit QListXst(code x) : code_{x} {}

            /** @return string representation for enum @p x **/
            static const char * _descr(code x);

            code code() const noexcept { return code_; }

            enum code code_;
        };

        inline std::ostream &
        operator<< (std::ostream & os, QListXst x) {
            os << QListXst::_descr(x.code_);
            return os;
        }

        /** @class DExpectQListSsm
         *  @brief parser state-machine for a literal list
         **/
        class DExpectQListSsm : public DSyntaxStateMachine<DExpectQListSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectQListSsm>;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using VisitReason = xo::mm::VisitReason;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            /** @defgroup scm-expectqlistssm-ctors constructors **/
            ///@{

            DExpectQListSsm();

            /** create instance, using memory from @parser_mm **/
            static obj<ASyntaxStateMachine,DExpectQListSsm> make(DArena & parser_mm);
            static DExpectQListSsm * _make(DArena & parser_mm);

            /** start nested syntax for a quoted literal **/
            static void start(ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectformalarglistssm-methods general methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectQListSsm"; }

            /** update state on incoming token @p tk, with overall parser state in @p p_psm **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            void on_rightparen_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqlistssm-ssm-facet syntaxstatemachine facet methods **/
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
             *  Appends @p lit to target list
             **/
            void on_quoted_literal(obj<AGCObject> lit,
                                   ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectqlistssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
            /** @defgroup scm-expectqlistssm-gc-support gc support methods **/
            ///@{

            /** gc support: visit gc-aware child pointers **/
            void visit_gco_children(VisitReason reason, obj<AGCObjectVisitor> gc) noexcept;

            ///@}

        private:
            /** @defgroup scm-expectqlistssm-member-vars **/
            ///@{

            /** identifies qlist parsing state **/
            QListXst state_;

            /** first node in literal list **/
            DList * start_ = nullptr;
            /** last node in literal list **/
            DList * end_ = nullptr;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectQListSsm.hpp */
