/** @file DSyntaxStateMachine.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {

        /** @class DSyntaxStateMachine
         *  @brief static helper interface for ASyntaxStateMachine implementations
         *
         *  Using CRTP.
         *
         *  Deliberately unusable through base class pointer.
         *  For runtime polymorphism use something like:
         *  @code
         *    Derived * d = ...;
         *    auto obj = with_facet<ASyntaxStateMachine>::mkobj(d);
         *    // or
         *    obj<ASyntaxStateMachine,Derived>(d)
         *  @endcode
         **/
        template<typename Derived>
        class DSyntaxStateMachine {
        public:
            using TypeDescr = xo::reflect::TypeDescr;

            /** Default implementation for required SyntaxStateMachine facet method
             **/
            void on_token(this auto&& self,
                          const Token & tk,
                          ParserStateMachine * p_psm)
            {
                p_psm->illegal_input_on_token(self.ssm_classname(),
                                              tk,
                                              self.get_expect_str());
            }

            void on_parsed_symbol(this auto&& self,
                                  std::string_view sym,
                                  ParserStateMachine * p_psm)
            {
                p_psm->illegal_input_on_symbol(self.ssm_classname(),
                                               sym,
                                               self.get_expect_str());
            }

            /** Default implementation for required SyntaxStateMachine facet method
             **/
            void on_parsed_typedescr(this auto&& self,
                                     TypeDescr td,
                                     ParserStateMachine * p_psm)
            {
                p_psm->illegal_input_on_typedescr(self.ssm_classname(),
                                                  td,
                                                  self.get_expect_str());
            }

            /** Default implementation for required SyntaxStateMachine facet method
             **/
            void on_parsed_formal(this auto&& self,
                                  const DUniqueString * param_name,
                                  TypeDescr param_type,
                                  ParserStateMachine * p_psm)
            {
                p_psm->illegal_parsed_formal(self.ssm_classname(),
                                             param_name,
                                             param_type,
                                             self.get_expect_str());
            }

            /** Default implementation for required SyntaxStateMachine facet method
             *
             *  arglist is DArray of obj<AGCObejct,DVariable>
             **/
            void on_parsed_formal_arglist(this auto&& self,
                                          DArray * arglist,
                                          ParserStateMachine * p_psm)
            {
                p_psm->illegal_parsed_formal_arglist(self.ssm_classname(),
                                                     arglist,
                                                     self.get_expect_str());
            }

            /** Default implementation for required SyntaxStateMachine facet method
             **/
            void on_parsed_expression(this auto&& self,
                                      obj<AExpression> expr,
                                      ParserStateMachine * p_psm)
            {
                p_psm->illegal_parsed_expression(self.ssm_classname(),
                                                 expr,
                                                 self.get_expect_str());

            }

            /** Default implementation for required SyntaxStateMachine facet method
             **/
            void on_parsed_expression_with_semicolon(this auto&& self,
                                                     obj<AExpression> expr,
                                                     ParserStateMachine * p_psm)
            {
                // We don't need a separate entry point,
                // since the semicolon isn't relevant to problem with syntax
                //

                p_psm->illegal_parsed_expression(self.ssm_classname(),
                                                 expr,
                                                 self.get_expect_str());

            }
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DSyntaxStateMachine.hpp */
