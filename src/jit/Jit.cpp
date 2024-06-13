/* @file Jit.cpp */

#include "Jit.hpp"

namespace xo {
    using xo::ast::exprtype;
    using xo::ast::Expression;
    using xo::ast::ConstantInterface;
    using xo::ast::PrimitiveInterface;
    using xo::reflect::TypeDescr;

    namespace jit {
        Jit::Jit()
            : llvm_cx_{std::make_unique<llvm::LLVMContext>()},
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
        Jit::codegen(ref::brw<Expression> expr)
        {
            switch(expr->extype()) {
            case exprtype::constant:
                return this->codegen_constant(ConstantInterface::from(expr));
            case exprtype::primitive:
                return this->codegen_primitive(PrimitiveInterface::from(expr));
            case exprtype::apply:
                break;
            case exprtype::invalid:
            case exprtype::n_expr:
                return nullptr;
                break;
            }

            return nullptr;
        } /*codegen*/

    } /*namespace jit*/
} /*namespace xo*/

/* end Jit.cpp */
