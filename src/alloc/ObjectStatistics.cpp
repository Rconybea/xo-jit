/* file ObjectStatistics.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "ObjectStatistics.hpp"
#include "xo/reflect/TypeDescr.hpp"
#include "xo/indentlog/print/pretty_vector.hpp"

namespace xo {
    namespace gc {
        void
        PerObjectTypeStatistics::display(std::ostream & os) const
        {
            os << "<PerObjectTypeStatistics";
            if (td_)
                os << rtag("td", td_->short_name());
            else
                os << rtag("td", "nullptr");
            os << rtag("scanned_n", scanned_n_)
               << rtag("scanned_z", scanned_z_)
               << rtag("survive_n", survive_n_)
               << rtag("survive_z", survive_z_)
               << ">";
        }

        void
        ObjectStatistics::display(std::ostream & os) const
        {
            os << "<ObjectStatistics";

            std::size_t i = 0;
            for (const auto & x : per_type_stats_v_) {
                os << " :[" << i << "] " << x;
            }

            os << ">";
        }
    } /*namespace gc*/

    namespace print {
        bool
        ppdetail<xo::gc::PerObjectTypeStatistics>::print_pretty(const ppindentinfo & ppii,
                                                                const xo::gc::PerObjectTypeStatistics & x)
        {
            static constexpr std::string_view c_nullptr_str = "nullptr";

            if (x.td_) {
                return ppii.pps()->pretty_struct(ppii,
                                                 "PerObjectTypeStatistics",
                                                 refrtag("td", x.td_ ? x.td_->short_name() : c_nullptr_str),
                                                 refrtag("scanned_n", x.scanned_n_),
                                                 refrtag("scanned_z", x.scanned_z_),
                                                 refrtag("survive_n", x.survive_n_),
                                                 refrtag("survive_z", x.survive_z_));
            } else {
                /* print nothing -- empty struct */
                return true;
            }
        }

        bool
        ppdetail<xo::gc::ObjectStatistics>::print_pretty(const ppindentinfo & ppii,
                                                         const xo::gc::ObjectStatistics & x)
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "ObjectTypeStatistics",
                                             refrtag("per_type_stats_v", x.per_type_stats_v_));
        }
    } /*namespace gc*/
} /*namespace xo*/

/* end ObjectStatistics.cpp */
