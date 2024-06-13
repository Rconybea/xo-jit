/** @file ex1.cpp **/

#include "xo/jit/Jit.hpp"
#include "xo/expression/Constant.hpp"
#include "xo/expression/Primitive.hpp"
#include "xo/expression/Apply.hpp"
#include <iostream>

int
main() {
    using xo::jit::Jit;
    using xo::ast::make_constant;
    using xo::ast::make_primitive;
    using xo::ast::make_apply;
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

    {
        /* (sqrt 2) */

        auto fn = make_primitive("sqrt", ::sqrt);
        auto arg = make_constant(2.0);

        auto call = make_apply(fn, arg);

        auto llvm_ircode = jit->codegen(call);

        if (llvm_ircode) {
            /* note: llvm:errs() is 'raw stderr stream' */
            cerr << "ex1 llvm_ircode:" << endl;
            llvm_ircode->print(llvm::errs());
            cerr << endl;
        } else {
            cerr << "ex1: code generation failed"
                 << xtag("expr", call)
                 << endl;
        }
    }
}

/** end ex1.cpp **/
