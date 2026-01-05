/** @file ppindentinfo.hpp
 *
 *  @author Roland Conybeare, Jul 2025
 **/

#pragma once

#include <cstdint>

namespace xo {
    namespace print {
        struct ppstate;

        /** @class ppindentinfo
         *  @brief pretty-printing state passed down the stack while traversing object graph
         **/
        struct ppindentinfo {
            ppindentinfo(ppstate * pps, std::uint32_t ci0, std::uint32_t indent_width, bool upto)
                : pps_{pps}, ci0_{ci0}, ci1_{ci0 + indent_width}, upto_{upto} {}

            ppstate * pps() const { return pps_; }
            std::uint32_t ci0_unused() const { return ci0_; }
            std::uint32_t ci1() const { return ci1_; }
            bool upto() const { return upto_; }

        private:
            /** pretty-printing state. flyweight. See [xo/indentlog/print/pretty.hpp] **/
            ppstate * pps_ = nullptr;
            /** current indent **/
            std::uint32_t ci0_ = 0;
            /** ci0 +1 indent level **/
            std::uint32_t ci1_ = 0;
            /**
             *  true  -> print on remainder of current line, unless past right margin
             *  false -> pretty across across multiple lines
             **/
            bool upto_;
        };

    }
}

/* end ppindentinfo.hpp */
