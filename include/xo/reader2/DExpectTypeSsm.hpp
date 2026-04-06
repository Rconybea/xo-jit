/** @file expect_type_xs.hpp
 *
 * @author Roland Conybeare, Aug 2024
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/facet/obj.hpp>
#include <xo/indentlog/print/ppindentinfo.hpp>

namespace xo {
    namespace scm {
        /** @class DExpectTypeSsm
         *  @brief syntax state-machine for accepting a Schemtika typename-expression
         *
         *  @pre
         *    any                 universal polymorphic type
         *    unit                empty type (like c/c++ void)
         *    f64                 64-bit float
         *    i32                 32-bit signed integer
         *    list<T>             parameterized list (polymorphic if T is any)
         *    array<T>            parameterized array (polymorphic if T is any)
         *    function<T (U,..)>  function (U x ...) -> T
         *  @endpre
         **/
        class DExpectTypeSsm : public DSyntaxStateMachine<DExpectTypeSsm> {
        public:
            using Super = DSyntaxStateMachine<DExpectTypeSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-expecttype-ctors constructors **/
            ///@{

            explicit DExpectTypeSsm(bool corrected);

            /** create instance **/
            static DExpectTypeSsm * _make(DArena & parser_mm, bool corrected);

            /** create fop referring to new DExpectTypeSsm **/
            static obj<ASyntaxStateMachine,DExpectTypeSsm> make(DArena & parser_mm, bool corrected);

            ///@}
            /** @defgroup scm-expecttype-general-methods general methods **/
            ///@{

            static void start(bool corrected, ParserStateMachine * p_psm);

            static const char * ssm_classname() { return "DExpectTypeSsm"; }

            ///@}
            /** @defgroup scm-expecttype-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error messages.
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** operate state machien for this syntax on incoming symbol-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expecttype-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
            /** @defgroup scm-expecttypessm-gc-support gc support methods **/
            ///@{

            /** gc support: visit gc-aware child pointers **/
            void visit_gco_children(obj<AGCObjectVisitor> gc) noexcept;

            ///@}

        private:
            /** temporary shim.
             *  if true, construct obj<AType>
             *  if false, construct TypeDescr
             **/
            bool corrected_ = false;
        };
    } /*namespace scm*/
} /*namespace xo*/


/* end DExpectTypeSsm.hpp */
