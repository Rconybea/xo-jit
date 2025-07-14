/* file vector.hpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

namespace std {
    template<typename T>
    inline std::ostream &
    operator<<(std::ostream & os,
               const std::vector<T> & v)
    {
        os << "[";
        for(size_t i=0, z=v.size(); i<z; ++i) {
            if(i > 0)
                os << " ";
            os << v[i];
        }
        os << "]";
        return os;
    } /*operator<<*/
} /*namespace std*/

/* end vector.hpp */
