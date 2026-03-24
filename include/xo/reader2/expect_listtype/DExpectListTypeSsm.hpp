/** @file DExpectListTypeSsm.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"

namespace xo {
    namespace scm {

        /**
         *       list < type-expr >
         *      ^    ^ ^         ^ ^
         *      |    | |         | (done)
         *      |    | |         type_3 --on_rightangle_token() --> (done) [pop + report listtype]
         *      |    | type_2 --on_parsed_type()--> type_3
         *      |    type_1 --on_leftangle_token() --> type_2 [push ExpectTypeSsm]
         *      type_0 --on_symbol_token()--> type_1
         *
         **/
        class ListTypeXst {
        public:
            enum class code {
                invalid = -1,

                type_0,
                type_1,
                type_2,
                type_3,

                N,
            };

            explicit ListTypeXst(code x) : code_{x} {}

            /** @return string representation for enum @p x **/
            static const char * _descr(code x);

            code code() const noexcept { return code_; }

            enum code code_;
        };

        inline std::ostream &
        operator<<(std::ostream & os, ListTypeXst x) {
            os << ListTypeXst::_descr(x.code_);
            return os;
        }

        /** @class DExpectListTypeSsm
         *  @brief parser state-machine for a list type
         *
         *  Examples:
         *  @pre
         *    list<i32>
         *    list<list<bool>>
         *  @endpre
         **/
        class DExpectListTypeSsm : public DSyntaxStateMachine<DExpectListTypeSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectListTypeSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using ACollector = xo::mm::ACollector;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** default ctor **/
            explicit DExpectListTypeSsm();

            /** create instance. Get memory from @p parser_mm **/
            static DExpectListTypeSsm * _make(DArena & parser_mm);

            /** create instance as fop. Get memory from @p parser_mm **/
            static obj<ASyntaxStateMachine, DExpectListTypeSsm> make(DArena & parser_mm);

            /** start nested state machine to parse a list type **/
            static void start(ParserStateMachine * p_psm);

            /** @defgroup scm-expectlisttypessm-expression-methods general methods **/
            ///@{

            static const char * ssm_classname() { return "DExpectListTypeSsm"; }

            syntaxstatetype ssm_type() const noexcept;

            std::string_view get_expect_str() const noexcept;

            /** update ssm for symbol token @p tk, with overall parsing state in @p p_psm.
             *  in state type_0: advance to type_1.
             *  otherwise error
             **/
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** update ssm for leftangle (<) token @p tk, with overall parser state in @p p_psm.
             *  in state type_1: advance to type_2, pushing nested ExpectTypeSsm.
             *  otherwise error.
             **/
            void on_leftangle_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update ssm for rightangle (>) token @p tk, with overall parser state in @p p_psm.
             *  in state type_3: aseemble list type, report completed syntax to parent ssm
             **/
            void on_rightangle_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectlisttypessm-expression-facet expression facet methods **/
            ///@{

            /** update ssm for token @p tk, with overall state in @p p_psm **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** update ssm for type @p type produced by nested ssm, with overall parser
             *  state in @p p_psm.
             *  in state type_2: @p type is element type for target list type.
             *  otherwise error.
             **/
            void on_parsed_type(obj<AType> type, ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectlisttypessm-printable-facet printable facet methods **/
            ///@{

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}
            /** @defgroup scm-expectlisttypessm-gc-support gc support methods **/
            ///@{

            /** gc support: visit gc-aware child pointers **/
            void forward_children(obj<ACollector> gc) noexcept;

            ///@}
        private:
            /** @defgroup scm-expectlisttypessm-instance-vars instance variables **/
            ///@{

            /** local parsing state **/
            ListTypeXst state_{ListTypeXst::code::type_0};

            /** element type for target list type **/
            obj<AType> elt_type_;

            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectListTypeSsm.hpp */
