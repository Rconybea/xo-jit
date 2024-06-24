/** @file ex2_jit.cpp **/

#include "xo/jit/MachPipeline.hpp"
#include "xo/jit/activation_record.hpp"
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
    using xo::jit::activation_record;
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

    /* try spelling everything out */

    {
        auto sqrt = make_primitive("sqrt",
                                   ::sqrt,
                                   false /*!explicit_symbol_def*/,
                                   llvmintrinsic::fp_sqrt);

        {
            auto llvm_ircode = jit->codegen_toplevel(sqrt);

            if (llvm_ircode) {
                /* note: llvm:errs() is 'raw stderr stream' */
                cerr << "ex1 llvm_ircode:" << endl;
                llvm_ircode->print(llvm::errs());
                cerr << endl;
            } else {
                cerr << "ex1: code generation failed"
                     << xtag("expr", sqrt)
                     << endl;
            }
        }

#define CHOICE 2

#if CHOICE == 0
#define FUNCTION_SYMBOL "callit"
        /* def callit(f :: double->double, x :: double) { f(x); } */

        auto f_var = make_var("f", Reflect::require<double (*)(double)>());
        auto x_var = make_var("x", Reflect::require<double>());
        auto call1 = make_apply(f_var, {x_var}); /* (f x) */
        //auto call2 = make_apply(f_var, {call1}); /* (f (f x)) */

        auto lambda = make_lambda("callit",
                                  {f_var, x_var},
                                  call1);
#elif CHOICE == 1
#define FUNCTION_SYMBOL "root4"
        /* def root4(x : double) { sqrt(sqrt(x)) } */

        auto x_var = make_var("x", Reflect::require<double>());
        auto call1 = make_apply(sqrt, {x_var});
        auto call2 = make_apply(sqrt, {call1});

        auto lambda = make_lambda("root4",
                                  {x_var},
                                  call2);
#elif CHOICE == 2
#define FUNCTION_SYMBOL "twice"
        auto root = make_primitive("sqrt",
                                   ::sqrt,
                                   false /*!explicit_symbol_def*/,
                                   llvmintrinsic::fp_sqrt);

        /* def twice(f :: int->int, x :: int) { f(f(x)) } */
        auto f_var = make_var("f", Reflect::require<int (*)(int)>());
        auto x_var = make_var("x", Reflect::require<int>());
        auto call1 = make_apply(f_var, {x_var}); /* (f x) */
        auto call2 = make_apply(f_var, {call1}); /* (f (f x)) */

        /* (define (twice f ::int->int x ::int) (f (f x))) */
        auto lambda = make_lambda("twice",
                                  {f_var, x_var},
                                  call2);
#endif

        log && log(xtag("lambda", lambda));

        auto llvm_ircode = jit->codegen_toplevel(lambda);

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

        jit->machgen_current_module();

        /* is this in module? */
        cerr << "ex2: jit execution session" << endl;
        jit->dump_execution_session();

        auto fn = jit->lookup_symbol(FUNCTION_SYMBOL);

        if (!fn) {
            cerr << "ex2: lookup: symbol not found"
                 << xtag("symbol", FUNCTION_SYMBOL)
                 << endl;
        } else {
            cerr << "ex2: lookup: symbol found"
                 << xtag("fn", fn.get().getValue())
                 << xtag("symbol", FUNCTION_SYMBOL)
                 << endl;
        }
    }
}

/** end ex2_jit.cpp **/
