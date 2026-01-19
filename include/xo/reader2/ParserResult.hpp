/** @file ParserResult.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/expression2/Expression.hpp>
#include <xo/object2/DString.hpp>
#include <string_view>

namespace xo {
    namespace scm {
        enum class parser_result_type {
            /** no result yet (no input or incomplete expression) **/
            none,
            /** emit expression **/
            expression,
            /** emit parsing error **/
            error,
            N
        };

        class ParserResult {
        public:
            ParserResult() = default;
            ParserResult(parser_result_type type,
                         obj<AExpression> expr,
                         std::string_view error_src_fn,
                         const DString * error_description);

            /** create ParserResult for a parsing error.
             *  Reporting detailed message @p errmsg
             *  from syntax state machine @p ssm
             **/
            static ParserResult error(std::string_view ssm,
                                      const DString * errmsg);

            parser_result_type result_type() const { return result_type_; }
            obj<AExpression> result_expr() const { return result_expr_; }
            const DString * error_description() const { return error_description_; }

            bool is_incomplete() const { return result_type_ == parser_result_type::none; }
            bool is_expression() const { return result_type_ == parser_result_type::expression; }
            bool      is_error() const { return result_type_ == parser_result_type::error; }

        private:
            parser_result_type result_type_ = parser_result_type::none;
            obj<AExpression> result_expr_;
            std::string_view error_src_fn_;
            const DString * error_description_ = nullptr;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserResult.hpp */
