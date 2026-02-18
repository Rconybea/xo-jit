/** @file NumericDispatch.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

//#include <xo/facet/FacetRegistry.hpp>  // probably will need this at some point?
#include <xo/arena/DArenaHashMap.hpp>
#include <utility>

namespace xo {
    namespace scm {

        /** type-erased arithmetic dispatch.
         **/
        struct NumericOps {
            obj<ANumeric>
        };

        class NumericDispatch {
        public:
            using KeyType = std::pair<typeseq, typeseq>;
            using MapType = xo::map::DArenaHashMap<KeyType, MappedType, KeyHash>

            /** combine two typeseq's to get hash value **/
            struct KeyHash {
                std::size_t operator()(const key_type & k) const noexcept {
                    // combine the two seqno values
                    std::size_t h1 = std::hash<int32_t>{}(k.first.seqno());
                    std::size_t h2 = std::hash<int32_t>{}(k.second.seqno());
                    return h1 ^ (h2 << 1);
                }
            };


        };

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispqatch.hpp */


