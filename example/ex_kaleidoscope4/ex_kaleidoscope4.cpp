/** ex_kaleidoscop4.cpp **/

#include "xo/jit/KaleidoscopeJit.hpp"
#include <iostream>

int
main() {
    using std::cerr;
    using std::endl;

    auto jit = xo::jit::KaleidoscopeJIT::Create();

    cerr << "created kaleidoscope jit successfully" << endl;
}

/** end ex_kaleidoscope4.cpp **/
