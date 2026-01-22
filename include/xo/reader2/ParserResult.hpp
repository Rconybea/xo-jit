/** @file ParserResult.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/expression2/Expression.hpp>
#include <xo/object2/DString.hpp>
#include <xo/indentlog/print/pretty.hpp>
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

        /** @return string representation for enum @p x **/
        const char * parser_result_type_descr(parser_result_type x);

        inline std::ostream & operator<<(std::ostream & os, parser_result_type x) {
            os << parser_result_type_descr(x);
            return os;
        }

        class ParserResult {
        public:
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            ParserResult() = default;
            ParserResult(parser_result_type type,
                         obj<AExpression> expr,
                         std::string_view error_src_fn,
                         const DString * error_description);

            /** create ParserResult for parsing success;
             *  parsing yields expression @p expr
             **/
            static ParserResult expression(std::string_view ssm,
                                           obj<AExpression> expr);

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

            /** ordinary not-pretty printer **/
            void print(std::ostream & os) const;
            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

        private:
            /** none|expression|error_description
             *
             *  @text
             *   result_type | error_src_function | error_description
             *  -------------+--------------------+-------------------
             *          none |            nullptr |             empty
             *    expression |            nullptr |             empty
             *         error |           non-null |         non-empty
             *  @endtext
             **/
            parser_result_type result_type_ = parser_result_type::none;
            /** non-null iff @ref result_type_ is expression **/
            obj<AExpression> result_expr_;
            /** non-null iff @ref result_type_ is error.
             *  In which case gives parsing function detecting this error
             **/
            std::string_view error_src_fn_;
            /** non-null iff @ref result_type_ is error
             *  Human-targeted error description.
             **/
            const DString * error_description_ = nullptr;
        };

        inline std::ostream & operator<<(std::ostream & os, const ParserResult & x) {
            x.print(os);
            return os;
        }

    } /*namespace scm*/

    namespace print {
        /** pretty printer in <xo/indentlog/print/pretty.hpp> relies on this specialization
         *  to handle ParserResult instances
         **/
        template <>
        struct ppdetail<xo::scm::ParserResult> {
            static inline bool print_pretty(const ppindentinfo & ppii, const xo::scm::ParserResult & x) {
                return x.pretty(ppii);
            }
        };
    }
} /*namespace xo*/

/* end ParserResult.hpp */
