/* GcStatistics.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "GcStatistics.hpp"
#include "xo/indentlog/print/pretty_vector.hpp"

namespace xo {
    namespace print {
        bool
        ppdetail<xo::gc::PerGenerationStatistics>::print_pretty(const ppindentinfo & ppii,
                                                                const xo::gc::PerGenerationStatistics & x)
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "PerGenerationStatistics",
                                             refrtag("used_z", x.used_z_),
                                             refrtag("n_gc", x.n_gc_),
                                             refrtag("new_alloc_z", x.new_alloc_z_),
                                             refrtag("scanned_z", x.scanned_z_),
                                             refrtag("survive_z", x.survive_z_),
                                             refrtag("promote_z", x.promote_z_)
                );
        }

        bool
        ppdetail<xo::gc::GcStatistics>::print_pretty(const ppindentinfo & ppii,
                                                     const xo::gc::GcStatistics & x)
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "GcStatistics",
                                             refrtag("gen_v", x.gen_v_),
                                             refrtag("total_allocated", x.total_allocated_),
                                             refrtag("total_promoted_sab", x.total_promoted_sab_),
                                             refrtag("total_promoted", x.total_promoted_),
                                             refrtag("n_mutation", x.n_mutation_),
                                             refrtag("n_logged_mutation", x.n_logged_mutation_),
                                             refrtag("n_xgen_mutation", x.n_xgen_mutation_),
                                             refrtag("n_xckp_mutation", x.n_xckp_mutation_)
                );
        }


        bool
        ppdetail<xo::gc::GcStatisticsExt>::print_pretty(const ppindentinfo & ppii,
                                                        const xo::gc::GcStatisticsExt & x)
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "GcStatisticsExt",
                                             refrtag("gen_v", x.gen_v_),
                                             refrtag("total_allocated", x.total_allocated_),
                                             refrtag("total_promoted_sab", x.total_promoted_sab_),
                                             refrtag("total_promoted", x.total_promoted_),
                                             refrtag("n_mutation", x.n_mutation_),
                                             refrtag("n_logged_mutation", x.n_logged_mutation_),
                                             refrtag("n_xgen_mutation", x.n_xgen_mutation_),
                                             refrtag("n_xckp_mutation", x.n_xckp_mutation_),
                                             refrtag("nursery_z", x.nursery_z_),
                                             refrtag("nursery_before_checkpoint_z", x.nursery_before_checkpoint_z_),
                                             refrtag("nursery_after_checkpoint_z", x.nursery_after_checkpoint_z_),
                                             refrtag("tenured_z", x.tenured_z_));
        }


    } /*namespace print*/
} /*namespace xo*/

/* end GcStatistics.cpp */
