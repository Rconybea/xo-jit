/** @file ex1.cpp **/

#include "xo/jit/MachPipeline.hpp"
#include "xo/expression/Constant.hpp"
#include "xo/expression/Primitive.hpp"
#include "xo/expression/Apply.hpp"
#include "xo/expression/Lambda.hpp"
#include "xo/expression/Variable.hpp"
#include <iostream>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

//double foo(double x) { return x; }

int
main() {
    using xo::scope;
    using xo::jit::MachPipeline;
    using xo::ast::make_constant;
    using xo::ast::make_primitive;
    using xo::ast::llvmintrinsic;
    using xo::ast::make_apply;
    using xo::ast::make_var;
    using xo::ast::make_lambda;
    using xo::reflect::Reflect;
    using xo::xtag;
    using std::cerr;
    using std::endl;

    //using xo::ast::make_constant;

    static llvm::ExitOnError llvm_exit_on_err;

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    //auto jit = llvm_exit_on_err(Jit::make_aux());
    auto jit = MachPipeline::make();

    //static_assert(std::is_function_v<decltype(&foo)>);

    scope log(XO_DEBUG(true));

    {
        auto expr = make_constant(7.0);

        log && log(xtag("expr", expr));

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
        auto expr = make_primitive("sqrt", &sqrt,
                                   false /*!explicit_symbol_def*/,
                                   llvmintrinsic::fp_sqrt);

        log && log(xtag("expr", expr));

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

        auto fn = make_primitive("sqrt", &sqrt,
                                 false /*!explicit_symbol_def*/,
                                 llvmintrinsic::fp_sqrt);
        auto arg = make_constant(2.0);

        auto call = make_apply(fn, {arg});

        log && log(xtag("expr", call));

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

        auto sin = make_primitive("sin",
                                  ::sin,
                                  false /*!explicit_symbol_def*/,
                                  llvmintrinsic::fp_sin);
        auto cos = make_primitive("cos",
                                  ::cos,
                                  false /*!explicit_symbol_def*/,
                                  llvmintrinsic::fp_cos);

        auto x_var = make_var("x", Reflect::require<double>());
        auto call1 = make_apply(cos, {x_var}); /* (cos x) */
        auto call2 = make_apply(sin, {call1}); /* (sin (cos x)) */

        /* (define (lm_1 x) (sin (cos x))) */
        auto lambda = make_lambda("lm_1",
                                  {x_var},
                                  call2);

        log && log(xtag("expr", lambda));

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

        /* is this in module? */
        cerr << "ex1: jit execution session" << endl;
        jit->dump_execution_session();
    }
}

/** end ex1.cpp **/
