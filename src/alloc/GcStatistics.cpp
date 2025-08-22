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

            //++n_gc_;
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
               << xrtag("used", used_z_)
               << xrtag("n_gc", n_gc_)
               << xrtag("new_alloc_z", new_alloc_z_)
               << xrtag("scanned_z", scanned_z_)
               << xrtag("survive_z", survive_z_)
               << xrtag("promote_z", promote_z_)
               << ">";
        }

        void
        GcStatistics::begin_gc(generation upto,
                               std::size_t new_alloc)
        {
            ++(this->gen_v_[static_cast<std::size_t>(upto)].n_gc_);
            this->total_allocated_ += new_alloc;
            this->total_promoted_sab_ = total_promoted_;
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
               << xrtag("gen_v", gen_v_)
               << xrtag("total_allocated", total_allocated_)
               << xrtag("total_promoted_sab", total_promoted_sab_)
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
               << xrtag("gen_v", gen_v_)
               << xrtag("total_allocated", total_allocated_)
               << xrtag("total_promoted_sab", total_promoted_)
               << xrtag("nursery_z", nursery_z_)
               << xrtag("nursery_before_ckp_z", nursery_before_checkpoint_z_)
               << xrtag("nursery_after_ckp_z", nursery_after_checkpoint_z_)
               << xrtag("tenured_z", tenured_z_)
               << xrtag("n_mutation", n_mutation_)
               << xrtag("n_logged_mutation", n_logged_mutation_)
               << xrtag("n_xgen_mutation", n_xgen_mutation_)
               << xrtag("n_xckp_mutation", n_xckp_mutation_)
                // << xtag("per_type_stats", per_type_stats_)
               << ">";
        }

        void
        GcStatisticsHistoryItem::display(std::ostream & os) const
        {
            os << "<GcStatisticsHistoryItem"
               << xrtag("upto", upto_)
               << xrtag("survive_z", survive_z_)
               << xrtag("promote_z", promote_z_)
               << xrtag("persist_z", persist_z_)
               << xrtag("effort_z", effort_z_)
               << xrtag("garbage0_z", garbage0_z_)
               << xrtag("garbage1_z", garbage1_z_)
               << xrtag("garbageN_z", garbageN_z_)
               << xrtag("dt", dt_)
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

        bool
        ppdetail<xo::gc::GcStatisticsHistoryItem>::print_pretty(const ppindentinfo & ppii,
                                                                const xo::gc::GcStatisticsHistoryItem & x)
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "GcStatisticsHistoryItem",
                                             refrtag("upto", gen2str(x.upto_)),
                                             refrtag("survive_z", x.survive_z_),
                                             refrtag("promote_z", x.promote_z_),
                                             refrtag("persist_z", x.persist_z_),
                                             refrtag("effort_z", x.effort_z_),
                                             refrtag("garbage0_z", x.garbage0_z_),
                                             refrtag("garbage1_z", x.garbage1_z_),
                                             refrtag("garbageN_z", x.garbageN_z_),
                                             refrtag("dt", x.dt_));
        }
    } /*namespace print*/
} /*namespace xo*/

/* end GcStatistics.cpp */
