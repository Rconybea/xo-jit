/** @file ex1.cpp **/

#include "xo/jit/Jit.hpp"
#include "xo/expression/Constant.hpp"
#include "xo/expression/Primitive.hpp"
#include "xo/expression/Apply.hpp"
#include "xo/expression/Lambda.hpp"
#include "xo/expression/Variable.hpp"
#include <iostream>

int
main() {
    using xo::jit::Jit;
    using xo::ast::make_constant;
    using xo::ast::make_primitive;
    using xo::ast::make_apply;
    using xo::ast::make_var;
    using xo::ast::make_lambda;
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

    {
        /* (lambda (x) (sin (cos x))) */

        auto sin = make_primitive("sin", ::sin);
        auto cos = make_primitive("cos", ::cos);

        auto x_var = make_var("x");
        auto call1 = make_apply(cos, x_var); /* (cos x) */
        auto call2 = make_apply(sin, call1); /* (sin (cos x)) */

        /* (define (lm_1 x) (sin (cos x))) */
        auto lambda = make_lambda("lm_1",
                                  {"x"},
                                  call2);

        auto llvm_ircode = jit->codegen(lambda);

        if (llvm_ircode) {
            /* note: llvm:errs() is 'raw stderr stream' */
            cerr << "ex1 llvm_ircode:" << endl;
            llvm_ircode->print(llvm::errs());
            cerr << endl;
        } else {
            cerr << "ex1: code generation failed"
                 << xtag("expr", lambda)
                 << endl;
        }
    }
}

/** end ex1.cpp **/
