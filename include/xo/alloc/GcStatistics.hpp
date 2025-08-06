/* GcStatistics.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#pragma once

#include "generation.hpp"
#include "xo/indentlog/print/pretty.hpp"
#include <ostream>
#include <array>

namespace xo {
    namespace gc {
        /** @class ObjectStatistics
         *  @brief placeholder for type-driven allocation statistics
         *
         *  Passed to @ref Object::deep_move for example
         **/
        class ObjectStatistics {
        };

        /** @class PerGenerationStatistics
         *  @brief garbage collection statistics for particular GC generation
         **/
        class PerGenerationStatistics {
        public:
            /** update statistics after a GC cycle
             *  @param alloc_z.    new allocations (since preceding GC)
             *  @param before_z.   generation size (bytes allocated) before collection
             *  @param after_z.    generation size after collection
             *  @param promote_z.  bytes promoted to next generation
             **/
            void include_gc(std::size_t alloc_z, std::size_t before_z, std::size_t after_z,
                            std::size_t promote_z);
            /** update with current state (use at end of gc cycle) **/
            void update_snapshot(std::size_t after_z);

            /** @param os.  write stats on this output stream **/
            void display(std::ostream & os) const;

            /** number of bytes currently in use **/
            std::size_t used_z_ = 0;

            /** number of collection cycles completed **/
            std::size_t n_gc_ = 0;
            /** sum of new alloc bytes, sampled at start of each collection cycle **/
            std::size_t new_alloc_z_ = 0;
            /** sum of allocated bytes sampled at beginning of each collection cycle **/
            std::size_t scanned_z_ = 0;
            /** sum of bytes remaining after collection cycle **/
            std::size_t survive_z_ = 0;
            /** sum of bytes promoted to next generation **/
            std::size_t promote_z_ = 0;
        };

        inline std::ostream & operator<< (std::ostream & os, const PerGenerationStatistics & x) {
            x.display(os);
            return os;
        }

        /** @class GcStatistics
         *  @brief garbage collection statistics
         **/
        class GcStatistics {
        public:
            /** update statistics after a GC cycle
             *  @param upto.      nursery -> incremental collection; tenured -> full collection
             *  @param alloc_z.   new allocations (since preceding GC)
             *  @param before_z.  generation size (bytes allocated) before collection
             *  @param after_z.   generation size after collection
             *  @param promote_z. bytes promoted to next generation
             **/
            void include_gc(generation upto, std::size_t alloc_z,
                            std::size_t before_z, std::size_t after_z, std::size_t promote_z);
            /** update snapshot for current state.
             *  Use with tenured stats after incremental gc
             **/
            void update_snapshot(generation upto, std::size_t after_z);

            /** @param os. write stats on this output stream **/
            void display(std::ostream & os) const;

            /** statistics gathered across {incr, full} GCs respectively **/
            std::array<PerGenerationStatistics, static_cast<std::size_t>(generation::N)> gen_v_;
            /** total bytes allocated since inception **/
            std::size_t total_allocated_ = 0;
            /** snapshot of total bytes promoted asof beginning of last gc cycle **/
            std::size_t total_promoted_sab_ = 0;
            /** total bytes promoted from nursery->tenured since inception **/
            std::size_t total_promoted_ = 0;

            /** total number of mutations to already-allocated objects,
             *  whether or not GC needs to log them.
             **/
            std::size_t n_mutation_ = 0;
            /** total number of mutation eligible for logging (cumulative across GCs) **/
            std::size_t n_logged_mutation_ = 0;
            /** total number of cross-generation mutations
             *  (tenured->nursery when reported; cumulative across GCs) **/
            std::size_t n_xgen_mutation_ = 0;
            /** total number of cross-checkpoint mutations
             *  (N0 -> N1 when reported; cumulative across GCs)
             **/
            std::size_t n_xckp_mutation_ = 0;

            /** per-type statistics (placeholder) **/
            ObjectStatistics per_type_stats_;
        };

        inline std::ostream & operator<< (std::ostream & os, const GcStatistics & x) {
            x.display(os);
            return os;
        }

        /** @class GcStatisticsExt
         *  @brief extend GcStatistics for application convenience
         **/
        class GcStatisticsExt : public GcStatistics {
        public:
            explicit GcStatisticsExt(const GcStatistics & x) : GcStatistics{x} {}

            /** @param os. write stats on this output stream **/
            void display(std::ostream & os) const;

            /** current capacity of nursery generation **/
            std::size_t nursery_z_ = 0;
            /** current nursery survivor size **/
            std::size_t nursery_before_checkpoint_z_ = 0;
            /** current nursery new alloc size **/
            std::size_t nursery_after_checkpoint_z_ = 0;
            /** current capacity of tenured generation **/
            std::size_t tenured_z_ = 0;
        };

        inline std::ostream & operator<< (std::ostream & os, const GcStatisticsExt & x) {
            x.display(os);
            return os;
        }

    } /*namespace gc*/

    namespace print {
        template <>
        struct ppdetail<xo::gc::PerGenerationStatistics> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::PerGenerationStatistics &);
        };

        template<>
        struct ppdetail<xo::gc::GcStatistics> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::GcStatistics &);
        };

        template<>
        struct ppdetail<xo::gc::GcStatisticsExt> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::GcStatisticsExt &);
        };
    } /*namespace print*/
} /*namespace xo*/

/* end GcStatistics.hpp */
