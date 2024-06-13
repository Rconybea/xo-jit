/* @file Jit.cpp */

#include "Jit.hpp"

namespace xo {
    using xo::ast::exprtype;
    using xo::ast::Expression;
    using xo::ast::ConstantInterface;
    using xo::ast::PrimitiveInterface;
    using xo::ast::Apply;
    using xo::reflect::TypeDescr;
    using std::cerr;
    using std::endl;

    namespace jit {
        Jit::Jit()
            : llvm_cx_{std::make_unique<llvm::LLVMContext>()},
              llvm_ir_builder_{std::make_unique<llvm::IRBuilder<>>(*llvm_cx_)},
              llvm_module_{std::make_unique<llvm::Module>("xojit", *llvm_cx_)}
        {}

        llvm::Value *
        Jit::codegen_constant(ref::brw<ConstantInterface> expr)
        {
            TypeDescr td = expr->value_td();

            if (td->is_native<double>()) {
                return llvm::ConstantFP::get(*llvm_cx_,
                                             llvm::APFloat(*(expr->value_tp().recover_native<double>())));
            } else if (td->is_native<float>()) {
                return llvm::ConstantFP::get(*llvm_cx_,
                                             llvm::APFloat(*(expr->value_tp().recover_native<float>())));
            }

            return nullptr;
        }

        llvm::Function *
        Jit::codegen_primitive(ref::brw<PrimitiveInterface> expr)
        {
            /** note: documentation (such as it is) for llvm::Function here:
             *
             *  https://llvm.org/doxygen/classllvm_1_1Function.html
             **/

            auto * fn = llvm_module_->getFunction(expr->name());

            if (fn) {
                /** function with this name already known to llvm module;
                 *  use that definition
                 *
                 *  TODO: verify that signatures match!
                 **/
                return fn;
            }

            /** establish prototype for this function **/

            // PLACEHOLDER
            // just make prototype for function :: double -> double

            std::vector<llvm::Type *> double_v(1, llvm::Type::getDoubleTy(*llvm_cx_));

            auto * llvm_fn_type = llvm::FunctionType::get(llvm::Type::getDoubleTy(*llvm_cx_),
                                                          double_v,
                                                          false /*!varargs*/);

            fn = llvm::Function::Create(llvm_fn_type,
                                        llvm::Function::ExternalLinkage,
                                        expr->name(),
                                        llvm_module_.get());

            // set names for arguments (for diagonostics?).  Money-see-kaleidoscope-monkey-do here
#ifdef NOT_USING
            for (auto & arg : fn->args())
                arg.setName(formalnameofthisarg);
#endif

            return fn;
        } /*codegen_primitive*/

        llvm::Value *
        Jit::codegen_apply(ref::brw<Apply> apply)
        {
            using std::cerr;
            using std::endl;

            /* editorial:
             *
             * to handle (computed functions) properly,
             * we will need a runtime representation for a 'primitive function pointer'
             *
             * For now, finesse by only handling PrimitiveInterface in function-callee position
             */
            if (apply->fn()->extype() == exprtype::primitive) {
                auto pm = PrimitiveInterface::from(apply->fn());
                auto * fn = this->codegen_primitive(pm);

#ifdef NOT_USING_DEBUG
                cerr << "Jit::codegen_apply: fn:" << endl;
                fn->print(llvm::errs());
                cerr << endl;
#endif

                if (fn->arg_size() != apply->argv().size()) {
                    cerr << "Jit::codegen_apply: error: callee f expecting n1 args where n2 supplied"
                              << xtag("f", pm->name())
                              << xtag("n1", pm->n_arg())
                              << xtag("n2", apply->argv().size())
                              << endl;
                    return nullptr;
                }

                std::vector<llvm::Value *> args;
                args.reserve(apply->argv().size());

                for (const auto & arg_expr : apply->argv()) {
                    auto * arg = this->codegen(arg_expr);

#ifdef NOT_USING_DEBUG
                    cerr << "Jit::codegen_apply: arg:" << endl;
                    arg->print(llvm::errs());
                    cerr << endl;
#endif

                    args.push_back(arg);
                }

                return llvm_ir_builder_->CreateCall(fn, args, "calltmp");
            } else {
                cerr << "Jit::codegen_apply: error: only allowing call to known primitives at present" << endl;
                return nullptr;
            }
        } /*codegen_apply*/

        llvm::Value *
        Jit::codegen(ref::brw<Expression> expr)
        {
            switch(expr->extype()) {
            case exprtype::constant:
                return this->codegen_constant(ConstantInterface::from(expr));
            case exprtype::primitive:
                return this->codegen_primitive(PrimitiveInterface::from(expr));
            case exprtype::apply:
                return this->codegen_apply(Apply::from(expr));
                break;
            case exprtype::invalid:
            case exprtype::n_expr:
                return nullptr;
                break;
            }

            cerr << "Jit::codegen: error: no handler for expression of type T"
                 << xtag("T", expr->extype())
                 << endl;

            return nullptr;
        } /*codegen*/

    } /*namespace jit*/
} /*namespace xo*/

/* end Jit.cpp */
