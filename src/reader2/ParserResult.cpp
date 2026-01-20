/** @file ParserResult.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ParserResult.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;

    namespace scm {

        const char *
        parser_result_type_descr(parser_result_type x)
        {
            switch (x) {
            case parser_result_type::none: return "none";
            case parser_result_type::expression: return "expression";
            case parser_result_type::error: return "error";
            case parser_result_type::N: break;
            }

            return "parser_result_type?";
        }

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

        void
        ParserResult::print(std::ostream & os) const
        {
            os << "<ParserResult" ;
            os << xtag("type", result_type_);
            os << xtag("expr", result_expr_);
            os << xtag("src_fn", error_src_fn_);
            if (error_description_)
                os << xtag("error", error_description_);
            os << ">";
        }

        bool
        ParserResult::pretty(const ppindentinfo & ppii) const
        {
            switch (result_type_) {
            case parser_result_type::none:
                return ppii.pps()->pretty_struct
                           (ppii,
                            "ParserResult",
                            refrtag("type", result_type_));
            case parser_result_type::expression:
                {
                    auto expr = FacetRegistry::instance().variant<APrintable,AExpression>(result_expr_);

                    return ppii.pps()->pretty_struct
                           (ppii,
                            "ParserResult",
                            refrtag("type", result_type_),
                            refrtag("expr", expr));
                }
                break;
            case parser_result_type::error:
                return ppii.pps()->pretty_struct
                           (ppii,
                            "ParserResult",
                            refrtag("type", result_type_),
                            refrtag("src_fn", error_src_fn_),
                            refrtag("error", error_description_));
            case parser_result_type::N:
                assert(false);
                break;
            }

            return false;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserResult.cpp */
