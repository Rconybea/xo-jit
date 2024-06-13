/** @file ex1.cpp **/

#include "xo/jit/Jit.hpp"
#include "xo/expression/Constant.hpp"
#include "xo/expression/Primitive.hpp"
#include <iostream>

int
main() {
    using xo::jit::Jit;
    using xo::ast::make_constant;
    using xo::ast::make_primitive;
    using xo::xtag;
    using std::cerr;
    using std::endl;

    //using xo::ast::make_constant;

    auto jit = Jit::make();

    {
        auto expr = make_constant(7.0);

        auto llvm_ircode = jit->codegen(expr);

        if (llvm_ircode) {
            /* note: llvm:errs() is 'raw stderr stream' */
            cerr << "ex1 llvm_ircode:" << endl;
            llvm_ircode->print(llvm::errs());
            cerr << endl;
        } else {
            cerr << "ex1: code generation failed"
                 << xtag("expr", expr)
                 << endl;
        }
    }

    {
        auto expr = make_primitive("sqrt", ::sqrt);

        auto llvm_ircode = jit->codegen(expr);

        if (llvm_ircode) {
            /* note: llvm:errs() is 'raw stderr stream' */
            cerr << "ex1 llvm_ircode:" << endl;
            llvm_ircode->print(llvm::errs());
            cerr << endl;
        } else {
            cerr << "ex1: code generation failed"
                 << xtag("expr", expr)
                 << endl;
        }
    }
}

/** end ex1.cpp **/
