/** @file DDeftypeSsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/facet/obj.hpp>
#include <string_view>

namespace xo {
    namespace scm {
        /**
         * @pre
         *
         *   deftype foo :: f64 ;
         *  ^       ^   ^  ^   ^ ^
         *  |       |   |  |   | (done)
         *  |       |   |  |   def_4
         *  |       |   |  def_3
         *  |       |   def_2
         *  |       def_1
         *  def_0
         *
         *  def_0 --on_deftype_token()--> def_1      [expect symbol]
         *  def_1 --on_symbol_token()--> def_2       [expect ::]
         *  def_2 --on_doublecolon_token()--> def_3  [start ExpectTypeSsm]
         *  def_3 --on_parsed_type()--> def_4        [++ symbol table]
         *  def_4 --on_semicolon_token()--> (done)   [pop]
         *
         * @endpre
         **/
        class DeftypeXst {
        public:
            enum class code {
                invalid = -1,

                def_0,
                def_1,
                def_2,
                def_3,
                def_4,

                N,
            };

            explicit DeftypeXst(code x) : code_{x} {}

            /** @return  string representation for enum @p x **/
            static const char * _descr(code x);

            code code() const noexcept { return code_; }

            enum code code_;
        };

        std::ostream &
        operator<<(std::ostream & os, DeftypeXst x);

        /** @class DDeftypeSsm
         *  @brief state machine for parsing a deftype expression
         **/
        class DDeftypeSsm : public DSyntaxStateMachine<DDeftypeSsm> {
        public:
            using Super = DSyntaxStateMachine<DDeftypeSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-deftypessm-ctors constructors **/
            ///@{

            /** constructor; using @p def_expr for initial expression scaffold **/
            explicit DDeftypeSsm();

            /** Create instance using memory from @p parser_mm **/
            static DDeftypeSsm * _make(DArena & parser_mm);

            /** create fop referring to new DDeftypeSsm **/
            static obj<ASyntaxStateMachine,DDeftypeSsm> make(DArena & parser_mm);

            /** start nested parser for a define-expression,
             *  on top of parser state machine @p p_psm
             *  Use @p parser_mm to allocate syntax state machines,
             *  and @p expr_mm to allocate expressions
             **/
            static void start(DArena & parser_mm,
                              //obj<AAllocator> expr_mm,
                              ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-definessm-access-methods **/
            ///@{

            /** identify this nested state machine **/
            static const char * ssm_classname() { return "DDeftypeSsm"; }

            /** internal state **/
            DeftypeXst deftypestate() const noexcept { return deftype_xst_; }

            ///@}
            /** @defgroup scm-definessm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error messages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** update state for this syntax on incoming @c deftype token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_deftype_token(const Token & tk,
                                  ParserStateMachine * p_psm);

            /** update state for this syntax on incoming double-colon token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_doublecolon_token(const Token & tk,
                                      ParserStateMachine * p_psm);

            /** update state for this syntax after parsing a symbol @p sym;
             *  overall parser state in @p p_psm
             **/
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);

            /** update state for this syntax after type @p type emitted by nested
             *  state machine, with overall parser state in @p p_psm
             **/
            void on_parsed_type(obj<AType> type,
                                ParserStateMachine * p_psm);

#ifdef NOT_YET
            /** update state for this syntax after parsing a type-description @p td,
             *  overall parser state in @p p_psm
             **/
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);
#endif

            /** update state for this syntax after parsing semicolon token @p tk,
             *  overall parser state in @p p_psm.
             *  if state def_4 completes deftype statement.
             **/
            void on_semicolon_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-define-printable-facet printable facet methods **/
            ///@{

            /** pretty-printer support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** @defgroup scm-deftypessm-member-vars **/
            ///@{

            /** identify deftype ssm state **/
            DeftypeXst deftype_xst_;

            /** lhs symbol for type definition **/
            const DUniqueString * lhs_symbol_ = nullptr;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineSsm.hpp */
