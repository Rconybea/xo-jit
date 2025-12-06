/* @file time.hpp */

#pragma once

#include <iostream>
#include <iomanip>
#ifdef NOT_YET
# include <format>
#endif
#include <chrono>
#include <compare>
#include <cstdint>
#include <time.h>

namespace xo {
    namespace time {
        using nanos = std::chrono::nanoseconds;
        using microseconds = std::chrono::microseconds;
        using milliseconds = std::chrono::milliseconds;
        using seconds = std::chrono::seconds;
        using hours = std::chrono::hours;
        using days = std::chrono::days;

        using utc_nanos = std::chrono::time_point<std::chrono::system_clock,
                                                  std::chrono::nanoseconds>;
        using utc_micros = std::chrono::time_point<std::chrono::system_clock,
                                                   std::chrono::microseconds>;


        struct timeutil {
            static utc_nanos now() {
                return utc_nanos(std::chrono::system_clock::now());
            }

            static utc_nanos epoch() {
                return utc_nanos(std::chrono::system_clock::from_time_t(0));
            } /*epoch*/

            static utc_nanos ymd_hms(uint32_t ymd, uint32_t hms) {
                /* e.g. ymd=20220610 -> n_yr=2022, n_mon=06, n_dy=10 */

                uint32_t n_yr = ymd / 10000;
                uint32_t n_mon = (ymd % 10000) / 100;
                uint32_t n_dy = ymd % 100;

                uint32_t n_hr = hms / 10000;
                uint32_t n_min = (hms % 10000) / 100;
                uint32_t n_sec = hms % 100;

                struct tm t;

                t.tm_year = n_yr - 1900; /* 0 means 1900 */
                t.tm_mon = n_mon - 1;    /* 0 means january */
                t.tm_mday = n_dy;

                t.tm_hour = n_hr;    /* 24 hour clock */
                t.tm_min = n_min;
                t.tm_sec = n_sec;

                /* time since epoch */
                time_t epoch_time = timegm(&t);

                return std::chrono::system_clock::from_time_t(epoch_time);
            } /*ymd_hms*/

            /* midnight UTC on date ymd.
             *   e.g. ymd_midnight(20220707) -> midnight UTC on 7jul22
             */
            static utc_nanos ymd_midnight(uint32_t ymd) {
                return ymd_hms(ymd, 0);
            } /*ymd_midnight*/

            static utc_nanos ymd_hms_usec(uint32_t ymd, uint32_t hms, uint32_t usec) {
                utc_nanos s = ymd_hms(ymd, hms);

                return s + microseconds(usec);
            } /*ymd_hms_usec*/

            /* .first:  UTC midnight on same calendar day as t0
             * .second: elapsed time from .first to t0 (i.e. UTC time-of-day for t0)
             */
            static std::pair<utc_nanos, nanos> utc_split_vs_midnight(utc_nanos t0) {
                //using xo::timeutil::microseconds;
                //using xo::timeutil::utc_nanos;

                /* use yyyymmdd.hh:mm:ss.nnnnnn */

                time_t t0_time_t = (std::chrono::system_clock::to_time_t
                                    (std::chrono::time_point_cast<xo::time::microseconds>(t0)));

                /* convert to std::tm,
                 * only provides 1-second precision
                 */
                std::tm t0_tm;
                ::gmtime_r(&t0_time_t, &t0_tm);

                /* midnight on the same calendar day as t0_tm */
                std::tm midnight_tm = t0_tm;
                {
                    midnight_tm.tm_hour = 0;
                    midnight_tm.tm_min = 0;
                    midnight_tm.tm_sec = 0;
                }

                /* convert to UTC epoch seconds */
                time_t midnight_time_t = ::timegm(&midnight_tm);

                utc_nanos t0_midnight =
                    (std::chrono::time_point_cast<xo::time::microseconds>(
                        std::chrono::system_clock::from_time_t(midnight_time_t)));

                nanos t0_tdy = t0 - t0_midnight;

                return std::pair<utc_nanos, nanos>(t0_midnight, t0_tdy);
            } /*utc_split_vs_midnight*/

            /* .first:  LOCAL midnight on same calendar day as t0 (but in UTC coords)
             * .second: elapsed time from .first to t0 (i.e. LOCAL time-of-day for t0)
             */
            static std::pair<utc_nanos, nanos> local_split_vs_midnight(utc_nanos t0) {
                using xo::time::microseconds;
                using xo::time::utc_nanos;

                /* use yyyymmdd.hh:mm:ss.nnnnnn */

                time_t t0_time_t = (std::chrono::system_clock::to_time_t
                                    (std::chrono::time_point_cast<xo::time::microseconds>(t0)));

                /* convert to std::tm,
                 * only provides 1-second precision
                 */
                std::tm t0_tm;
                ::localtime_r(&t0_time_t, &t0_tm);

                /* midnight on the same calendar day as t0_tm */
                std::tm midnight_tm = t0_tm;
                {
                    midnight_tm.tm_hour = 0;
                    midnight_tm.tm_min = 0;
                    midnight_tm.tm_sec = 0;
                }

                /* convert local midnight to UTC epoch seconds */
                time_t midnight_time_t = ::timelocal(&midnight_tm);

                utc_nanos t0_midnight =
                    (std::chrono::time_point_cast<xo::time::microseconds>(
                        std::chrono::system_clock::from_time_t(midnight_time_t)));

                nanos t0_tdy = t0 - t0_midnight;

                return std::pair<utc_nanos, nanos>(t0_midnight, t0_tdy);
            } /*local_split_vs_midnight*/

            /* split utc_nanos into
             *   std::tm
             *    .tm_year
             *    .tm_mon   (1-12)
             *    .tm_mday  (1-31)
             *    .tm_hour  (0-23)
             *    .tm_min   (0-59)
             *    .tm_sec   (0-59)
             *    .tm_wday  (0=sunday .. 6=saturday)
             *    .tm_yday  (0=1jan .. 365)
             *    .tm_isdst (daylight savings time flag)
             *   usec       (0-999999)
             */
            static std::pair<std::tm, uint32_t> utc_split_tm(utc_nanos t0) {
                using xo::time::microseconds;
                using xo::time::utc_nanos;

                /* use yyyymmdd.hh:mm:ss.nnnnnn */

                time_t t0_time_t
                    = (std::chrono::system_clock::to_time_t
                       (std::chrono::time_point_cast<microseconds>(t0)));
                //time_t t0_time_t = (std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<xo::time::microseconds>(t0)));

                /* convert to std::tm, in UTC coords,
                 * only provides 1-second precision
                 */
                std::tm t0_tm;
                ::gmtime_r(&t0_time_t, &t0_tm);

                /* midnight on the same calendar day as t0_tm */
                std::tm midnight_tm = t0_tm;

                midnight_tm.tm_isdst = 0;
                midnight_tm.tm_hour = 0;
                midnight_tm.tm_min = 0;
                midnight_tm.tm_sec = 0;

                /* convert back to epoch seconds */
                time_t midnight_time_t = ::timegm(&midnight_tm);

                utc_nanos t0_midnight =
                    (std::chrono::time_point_cast<xo::time::microseconds>(
                        std::chrono::system_clock::from_time_t(midnight_time_t)));

                uint32_t usec =
                    (std::chrono::duration_cast<microseconds>(
                        std::chrono::hh_mm_ss(t0 - t0_midnight).subseconds()))
                    .count();

                return std::make_pair(t0_tm, usec);
            } /*utc_split_tm*/

            static void print_hms_msec(nanos dt, std::ostream & os) {
                /* use hhmmss.nnn */
                using std::int32_t;

                auto hms = std::chrono::hh_mm_ss(dt);
                int32_t h = hms.hours().count();
                int32_t m = hms.minutes().count();
                int32_t s = hms.seconds().count();
                int32_t msec = std::chrono::duration_cast<milliseconds>(hms.subseconds()).count();

                char buf[32];
                snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d", h, m, s, msec);

                os << buf;
            } /*print_hms_msec*/

            static void print_utc_hms_msec(utc_nanos t0, std::ostream & os) {
                print_hms_msec(utc_split_vs_midnight(t0).second, os);
            } /*print_utc_hms_usec*/

            static void print_hms_usec(nanos dt, std::ostream & os) {
                /* use hhmmss.uuuuuu */
                using std::int32_t;

                auto hms = std::chrono::hh_mm_ss(dt);
                int32_t h = hms.hours().count();
                int32_t m = hms.minutes().count();
                int32_t s = hms.seconds().count();
                int32_t usec = std::chrono::duration_cast<microseconds>(hms.subseconds()).count();

                char buf[32];
                snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%06d", h, m, s, usec);

                os << buf;
            } /*print_hms_usec*/

            /* print t0 like:
             *   yyyymmdd:hh:mm:ss.uuuuuu
             * e.g.
             *   19700101:00:00:00.000000       // epoch
             *   20230921:16:29:35.123456       // 21sep2023 4:29:35 pm + 12345 us
             */
            static void print_utc_ymd_hms_usec(utc_nanos t0, std::ostream & os) {
                using xo::time::microseconds;
                using xo::time::utc_nanos;

                /* use yyyymmdd.hh:mm:ss.nnnnnn */

                //std::tm t0_tm;
                //uint32_t t0_usec;

                /* (structured binding ftw!) */
                auto [t0_tm, t0_usec] = utc_split_tm(t0);

                /* no std::format in clang11 afaict */
                char usec_buf[15];
                snprintf(usec_buf, sizeof(usec_buf), "%06d", t0_usec);


                /* control string              | example
                 * ----------------------------+--------------------------
                 * %c - locale-specific string | Fri Jun 10 16:29:05 2022
                 * %Y - year                   | 2022
                 * %m - month                  | 06
                 * %d - day of month           | 10
                 * %H - hour                   | 16
                 * %M - minute                 | 29
                 * %S - second                 | 05
                 * %Z - timezone               | UTC
                 */
                os << std::put_time(&t0_tm, "%Y%m%d:%H:%M:%S.") << usec_buf;
            } /*print_utc_ymd_hms_usec*/

            /* print datetime in format compatible with ISO 8601.
             * copying the format javascript uses, e.g:
             *   2012-04-23T18:25:43.511Z
             */
            static void print_iso8601(utc_nanos t0, std::ostream & os) {
                auto [t0_tm, t0_usec] = utc_split_tm(t0);

                char msec_buf[8];
                snprintf(msec_buf, sizeof(msec_buf), "%03d", t0_usec / 1000);

                os << std::put_time(&t0_tm, "%Y-%m-%dT%H:%M:%S.") << msec_buf << "Z";
            } /*print_iso8601*/
        }; /*timeutil*/

    } /*namespace time*/
} /*namespace xo*/

/* end timeutil.hpp */
