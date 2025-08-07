/* GcStatistics.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "GcStatistics.hpp"
#include "xo/indentlog/print/pretty_vector.hpp"

namespace xo {
    namespace gc {
        void
        PerGenerationStatistics::include_gc(std::size_t alloc_z,
                                            std::size_t before_z,
                                            std::size_t after_z,
                                            std::size_t promote_z)
        {
            this->update_snapshot(after_z);

            new_alloc_z_ += alloc_z;
            scanned_z_   += before_z;
            survive_z_   += after_z;
            promote_z_   += promote_z;
        }

        void
        PerGenerationStatistics::update_snapshot(std::size_t after_z)
        {
            used_z_ = after_z;
        }

        void
        PerGenerationStatistics::display(std::ostream & os) const
        {
            os << "<PerGenerationStatistics"
               << rtag("used", used_z_)
               << rtag("n_gc", n_gc_)
               << rtag("new_alloc_z", new_alloc_z_)
               << rtag("scanned_z", scanned_z_)
               << rtag("survive_z", survive_z_)
               << rtag("promote_z", promote_z_)
               << ">";
        }

        void
        GcStatistics::include_gc(generation upto,
                                 std::size_t alloc_z,
                                 std::size_t before_z,
                                 std::size_t after_z,
                                 std::size_t promote_z)
        {
            gen_v_[static_cast<std::size_t>(upto)].include_gc(alloc_z, before_z, after_z, promote_z);
        }

        void
        GcStatistics::update_snapshot(generation upto,
                                      std::size_t after_z)
        {
            gen_v_[static_cast<std::size_t>(upto)].update_snapshot(after_z);
        }

        void
        GcStatistics::display(std::ostream & os) const
        {
            os << "<GcStatistics"
               << rtag("gen_v", gen_v_)
               << rtag("total_allocated", total_allocated_)
               << rtag("total_promoted_sab", total_promoted_sab_)
                // total_promoted
                // n_mtuation
                // n_logged_mutation
                // n_xgen_mutation
                // n_xckp_mutation
                // << xtag("per_type_stats", per_type_stats_)
               << ">";
        }

        void
        GcStatisticsExt::display(std::ostream & os) const
        {
            os << "<GcStatisticsExt"
               << rtag("gen_v", gen_v_)
               << rtag("total_allocated", total_allocated_)
               << rtag("total_promoted_sab", total_promoted_)
               << rtag("nursery_z", nursery_z_)
               << rtag("nursery_before_ckp_z", nursery_before_checkpoint_z_)
               << rtag("nursery_after_ckp_z", nursery_after_checkpoint_z_)
               << rtag("tenured_z", tenured_z_)
               << rtag("n_mutation", n_mutation_)
               << rtag("n_logged_mutation", n_logged_mutation_)
               << rtag("n_xgen_mutation", n_xgen_mutation_)
               << rtag("n_xkcp_mutation", n_xckp_mutation_)
                // << xtag("per_type_stats", per_type_stats_)
               << ">";
        }
    } /*namespace gc*/

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
