/** @file ParserResult.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserResult.hpp"

namespace xo {
    namespace scm {
        ParserResult::ParserResult(parser_result_type type,
                                   obj<AExpression> expr,
                                   std::string_view error_src_fn,
                                   const DString * error_description)
                : result_type_{type},
                  result_expr_{expr},
                  error_src_fn_{error_src_fn},
                  error_description_{error_description}
        {}

        ParserResult
        ParserResult::error(std::string_view ssm_name,
                            const DString * errmsg)
        {
            return ParserResult(parser_result_type::error,
                                obj<AExpression>(),
                                ssm_name,
                                errmsg);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserResult.cpp */
