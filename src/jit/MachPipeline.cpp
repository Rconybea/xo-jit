/* @file MachPipeline.cpp */

#include "MachPipeline.hpp"

namespace xo {
    using xo::ast::exprtype;
    using xo::ast::Expression;
    using xo::ast::ConstantInterface;
    using xo::ast::FunctionInterface;
    using xo::ast::PrimitiveInterface;
    using xo::ast::Lambda;
    using xo::ast::Variable;
    using xo::ast::Apply;
    using xo::ast::IfExpr;
    using xo::reflect::Reflect;
    using xo::reflect::TypeDescr;
    using std::cerr;
    using std::endl;

    namespace jit {
        void
        MachPipeline::init_once() {
            static bool s_init_once = false;

            if (!s_init_once) {
                s_init_once = true;

                llvm::InitializeNativeTarget();
                llvm::InitializeNativeTargetAsmPrinter();
                llvm::InitializeNativeTargetAsmParser();
            }
        } /*init_once*/

        /* tracking KaleidoscopeJIT::Create() here..
         *
         * Verified:
         * + 'execution session' as per Kaleidoscope JIT,
         *   can instantiate from python
         * + 'jit object layer'
         *   (realtime dynamic library object linking layer)
         * + 'jit_compile_layer'
         * + 'jit_our_dynamic_lib'
         */
        llvm::Expected<std::unique_ptr<MachPipeline>>
        MachPipeline::make_aux()
        {
            MachPipeline::init_once();

            static llvm::ExitOnError llvm_exit_on_err;

            std::unique_ptr<Jit> jit = llvm_exit_on_err(Jit::Create());

            return std::unique_ptr<MachPipeline>(new MachPipeline(std::move(jit)
                                            ));
        } /*make*/

        xo::ref::rp<MachPipeline>
        MachPipeline::make() {
            static llvm::ExitOnError llvm_exit_on_err;

            std::unique_ptr<MachPipeline> jit = llvm_exit_on_err(make_aux());

            return jit.release();
        } /*make*/

        MachPipeline::MachPipeline(std::unique_ptr<Jit> jit)
            : jit_{std::move(jit)}
        {
            this->recreate_llvm_ir_pipeline();
        }

        void
        MachPipeline::recreate_llvm_ir_pipeline()
        {
            //llvm_cx_ = std::make_unique<llvm::LLVMContext>();
            llvm_cx_ = LlvmContext::make();
            llvm_ir_builder_ = std::make_unique<llvm::IRBuilder<>>(llvm_cx_->llvm_cx_ref());

            llvm_module_ = std::make_unique<llvm::Module>("xojit", llvm_cx_->llvm_cx_ref());
            llvm_module_->setDataLayout(this->jit_->data_layout());

            if (!llvm_cx_.get()) {
                throw std::runtime_error("MachPipeline::ctor: expected non-empty llvm context");
            }
            if (!llvm_ir_builder_.get()) {
                throw std::runtime_error("MachPipeline::ctor: expected non-empty llvm IR builder");
            }
            if (!llvm_module_.get()) {
                throw std::runtime_error("MachPipeline::ctor: expected non-empty llvm module");
            }

            ir_pipeline_ = new IrPipeline(llvm_cx_);
        } /*recreate_llvm_ir_pipeline*/

        /** identifies target host/architecture for machine code.
         *  e.g. "x86_64-unknown-linux-gnu"
         **/
        const std::string &
        MachPipeline::target_triple() const {
            // although this getter is defined,  seems to be empty in practice
            //return llvm_module_->getTargetTriple();

            return this->jit_->target_triple();
        }

        std::vector<std::string>
        MachPipeline::get_function_name_v() {
            std::vector<std::string> retval;
            for (const auto & fn_name : *llvm_module_)
                retval.push_back(fn_name.getName().str());

            return retval;
        } /*get_function_names*/

        void
        MachPipeline::dump_execution_session() {
            this->jit_->dump_execution_session();
        }

        llvm::Value *
        MachPipeline::codegen_constant(ref::brw<ConstantInterface> expr)
        {
            TypeDescr td = expr->value_td();

            if (Reflect::is_native<double>(td)) {
                return llvm::ConstantFP::get(llvm_cx_->llvm_cx_ref(),
                                             llvm::APFloat(*(expr->value_tp().recover_native<double>())));
            } else if (Reflect::is_native<float>(td)) {
                return llvm::ConstantFP::get(llvm_cx_->llvm_cx_ref(),
                                             llvm::APFloat(*(expr->value_tp().recover_native<float>())));
            } else if (Reflect::is_native<int>(td)) {
                return llvm::ConstantInt::get(llvm_cx_->llvm_cx_ref(),
                                              llvm::APSInt(*(expr->value_tp().recover_native<int>())));
            } else if (Reflect::is_native<unsigned int>(td)) {
                return llvm::ConstantInt::get(llvm_cx_->llvm_cx_ref(),
                                              llvm::APSInt(*(expr->value_tp().recover_native<unsigned int>())));
            }

            return nullptr;
        } /*codegen_constant*/

        namespace {
            llvm::Type *
            td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx, TypeDescr td) {
                auto & llvm_cx_ref = llvm_cx->llvm_cx_ref();

                if (Reflect::is_native<bool>(td)) {
                    return llvm::Type::getInt1Ty(llvm_cx_ref);
                } else if (Reflect::is_native<char>(td)) {
                    return llvm::Type::getInt8Ty(llvm_cx_ref);
                } else if (Reflect::is_native<short>(td)) {
                    return llvm::Type::getInt16Ty(llvm_cx_ref);
                } else if (Reflect::is_native<int>(td)) {
                    return llvm::Type::getInt32Ty(llvm_cx_ref);
                } else if (Reflect::is_native<long>(td)) {
                    return llvm::Type::getInt64Ty(llvm_cx_ref);
                } else if (Reflect::is_native<float>(td)) {
                    return llvm::Type::getFloatTy(llvm_cx_ref);
                } else if (Reflect::is_native<double>(td)) {
                    return llvm::Type::getDoubleTy(llvm_cx_ref);
                } else {
                    cerr << "td_to_llvm_type: no llvm type available for T"
                         << xtag("T", td->short_name())
                         << endl;
                    return nullptr;
                }
            }
        }

        llvm::Function *
        MachPipeline::codegen_primitive(ref::brw<PrimitiveInterface> expr)
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            /** note: documentation (such as it is) for llvm::Function here:
             *
             *  https://llvm.org/doxygenL/classllvm_1_1Function.html
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
            // just make prototype for function :: double^n -> double

            TypeDescr fn_td = expr->valuetype();
            int n_fn_arg = fn_td->n_fn_arg();

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("fn_td", fn_td->short_name()),
                      xtag("n_fn_arg", n_fn_arg));

            std::vector<llvm::Type *> llvm_argtype_v;
            llvm_argtype_v.reserve(n_fn_arg);

            /** check function args are all known **/
            for (int i = 0; i < n_fn_arg; ++i) {
                TypeDescr arg_td = fn_td->fn_arg(i);

                log && log(xtag("i_arg", i),
                           xtag("arg_td", arg_td->short_name()));

                llvm::Type * llvm_argtype = td_to_llvm_type(llvm_cx_.borrow(), arg_td);

                if (!llvm_argtype)
                    return nullptr;

                llvm_argtype_v.push_back(llvm_argtype);
            }

            //std::vector<llvm::Type *> double_v(n_fn_arg, llvm::Type::getDoubleTy(*llvm_cx_));

            TypeDescr retval_td = fn_td->fn_retval();

            log && log(xtag("retval_td", retval_td->short_name()));

            llvm::Type * llvm_retval = td_to_llvm_type(llvm_cx_.borrow(), retval_td);

            if (!llvm_retval)
                return nullptr;

            auto * llvm_fn_type = llvm::FunctionType::get(llvm_retval,
                                                          llvm_argtype_v,
                                                          false /*!varargs*/);

            fn = llvm::Function::Create(llvm_fn_type,
                                        llvm::Function::ExternalLinkage,
                                        expr->name(),
                                        llvm_module_.get());

#ifdef NOT_USING
            // set names for arguments (for diagnostics?).  Monkey-see-kaleidoscope-monkey-do here
            {
                int i_arg = 0;
                for (auto & arg : fn->args()) {
                    std::stringstream ss;
                    ss << "x_" << i_arg;

                    arg.setName(ss.str());
                    ++i_arg;
                }
            }
#endif

            log && log("returning llvm function");

            return fn;
        } /*codegen_primitive*/

        llvm::Value *
        MachPipeline::codegen_apply(ref::brw<Apply> apply)
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
            if (apply->fn()->extype() == exprtype::primitive
                || apply->fn()->extype() == exprtype::lambda)
            {
                llvm::Function * llvm_fn = nullptr;
                FunctionInterface * fn = nullptr;
                {
                    // TODO: codgen_function()

                    auto pm = PrimitiveInterface::from(apply->fn());
                    if (pm) {
                        fn = pm.get();
                        llvm_fn = this->codegen_primitive(pm);
                    }

                    auto lm = Lambda::from(apply->fn());
                    if (lm) {
                        fn = lm.get();
                        llvm_fn = this->codegen_lambda(lm);
                    }
                }

                if (!llvm_fn) {
                    return nullptr;
                }

#ifdef NOT_USING_DEBUG
                cerr << "MachPipeline::codegen_apply: fn:" << endl;
                fn->print(llvm::errs());
                cerr << endl;
#endif

                if (llvm_fn->arg_size() != apply->argv().size()) {
                    cerr << "MachPipeline::codegen_apply: error: callee f expecting n1 args where n2 supplied"
                              << xtag("f", fn->name())
                              << xtag("n1", fn->n_arg())
                              << xtag("n2", apply->argv().size())
                              << endl;

                    return nullptr;
                }

                /** also check argument types **/
                for (size_t i = 0, n = fn->n_arg(); i < n; ++i) {
                    if (apply->argv()[i]->valuetype() != fn->fn_arg(i)) {
                        cerr << "MachPipeline::codegen_apply: error: callee f for arg i seeeing U instead of expected T"
                             << xtag("f", fn->name())
                             << xtag("i", i)
                             << xtag("U", apply->argv()[i]->valuetype()->short_name())
                             << xtag("T", fn->fn_arg(i)->short_name())
                             << endl;

                        return nullptr;
                    }
                }

                std::vector<llvm::Value *> args;
                args.reserve(apply->argv().size());

                for (const auto & arg_expr : apply->argv()) {
                    auto * arg = this->codegen(arg_expr);

#ifdef NOT_USING_DEBUG
                    cerr << "MachPipeline::codegen_apply: arg:" << endl;
                    arg->print(llvm::errs());
                    cerr << endl;
#endif

                    args.push_back(arg);
                }

                return llvm_ir_builder_->CreateCall(llvm_fn, args, "calltmp");
            } else {
                cerr << "MachPipeline::codegen_apply: error: only allowing call to known primitives at present" << endl;
                return nullptr;
            }
        } /*codegen_apply*/

        llvm::Function *
        MachPipeline::codegen_lambda(ref::brw<Lambda> lambda)
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("lambda-name", lambda->name()));

            /* reminder! this is the *expression*, not the *closure* */

            global_env_[lambda->name()] = lambda.get();

            /* do we already know a function with this name? */
            auto * fn = llvm_module_->getFunction(lambda->name());

            if (fn) {
                /** function with this name already defined?? **/
                cerr << "MachPipeline::codegen_lambda: function f already defined"
                     << xtag("f", lambda->name())
                     << endl;

                return nullptr;
            }

            /* establish prototype for this function */

            // PLACEHOLDER
            // just handle double arguments + return type for now

            llvm::Type * llvm_retval = td_to_llvm_type(llvm_cx_.borrow(), lambda->fn_retval());

            std::vector<llvm::Type *> arg_type_v(lambda->n_arg());

            for (size_t i = 0, n = lambda->n_arg(); i < n; ++i) {
                arg_type_v[i] = td_to_llvm_type(llvm_cx_.borrow(), lambda->fn_arg(i));
            }

            auto * llvm_fn_type = llvm::FunctionType::get(llvm_retval,
                                                          arg_type_v,
                                                          false /*!varargs*/);

            /* create (initially empty) function */
            fn = llvm::Function::Create(llvm_fn_type,
                                        llvm::Function::ExternalLinkage,
                                        lambda->name(),
                                        llvm_module_.get());
            /* also capture argument names */
            {
                int i = 0;
                for (auto & arg : fn->args()) {
                    log && log("llvm format param names", xtag("i", i), xtag("param", lambda->argv().at(i)));

                    arg.setName(lambda->argv().at(i)->name());
                    ++i;
                }
            }

            /* generate function body */

            auto block = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(), "entry", fn);

            llvm_ir_builder_->SetInsertPoint(block);

            /* formal parameters need to appear in named_value_map_ */
            nested_env_.clear();
            {
                int i = 0;
                for (auto & arg : fn->args()) {
                    log && log("nested environment", xtag("i", i), xtag("param", std::string(arg.getName())));

                    nested_env_[std::string(arg.getName())] = &arg;
                    ++i;
                }
            }

            llvm::Value * retval = this->codegen(lambda->body());

            if (retval) {
                /* completes the function.. */
                llvm_ir_builder_->CreateRet(retval);

                /* validate!  always validate! */
                llvm::verifyFunction(*fn);

                /* optimize!  improves IR */
                ir_pipeline_->run_pipeline(*fn); // llvm_fpmgr_->run(*fn, *llvm_famgr_);

                return fn;
            }

            /* oops,  something went wrong */
            fn->eraseFromParent();

            return nullptr;
        } /*codegen_lambda*/

        llvm::Value *
        MachPipeline::codegen_variable(ref::brw<Variable> var)
        {
            auto ix = nested_env_.find(var->name());

            if (ix == nested_env_.end()) {
                cerr << "MachPipeline::codegen_variable: no binding for variable x"
                     << xtag("x", var->name())
                     << endl;
                return nullptr;
            }

            return ix->second;
        } /*codegen_variable*/

        llvm::Value *
        MachPipeline::codegen_ifexpr(ref::brw<IfExpr> expr)
        {
            llvm::Value * test_ir = this->codegen(expr->test());

            /** need test result in a variable
             **/
            llvm::Value * test_with_cmp_ir
                = llvm_ir_builder_->CreateFCmpONE(test_ir,
                                                  llvm::ConstantFP::get(llvm_cx_->llvm_cx_ref(),
                                                                        llvm::APFloat(0.0)),
                                                  "iftest");

            llvm::Function * parent_fn = llvm_ir_builder_->GetInsertBlock()->getParent();

            /* when_true_bb, when_false_bb, merge_bb:
             * initially-empty basic-blocks for {when_true, when_false, merged} codegen
             */

            /* when_true branch inserted at (current) end of function */
            llvm::BasicBlock * when_true_bb
                = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(),
                                           "when_true",
                                           parent_fn);
            llvm::BasicBlock * when_false_bb
                = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(),
                                           "when_false");

            llvm::BasicBlock * merge_bb
                = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(),
                                           "merge");

            /* IR to direct control flow to one of {when_true_bb, when_false_bb},
             * depending on result of test_with_cmp_ir
             */
            llvm_ir_builder_->CreateCondBr(test_with_cmp_ir,
                                           when_true_bb,
                                           when_false_bb);

            /* populate when_true_bb */
            llvm_ir_builder_->SetInsertPoint(when_true_bb);

            llvm::Value * when_true_ir = this->codegen(expr->when_true());

            if (!when_true_ir)
                return nullptr;

            /* at end of when-true sequence, jump to merge suffix */
            llvm_ir_builder_->CreateBr(merge_bb);
            /* note: codegen for expr->when_true() may have altered builder's "current block" */
            when_true_bb = llvm_ir_builder_->GetInsertBlock();

            /* populate when_false_bb */
            parent_fn->insert(parent_fn->end(), when_false_bb);
            llvm_ir_builder_->SetInsertPoint(when_false_bb);

            llvm::Value * when_false_ir = this->codegen(expr->when_false());
            if (!when_false_ir)
                return nullptr;

            /* at end of when-false sequence, jump to merge suffix */
            llvm_ir_builder_->CreateBr(merge_bb);
            /* note: codegen for expr->when_false() may have altered builder's "current block" */
            when_false_bb = llvm_ir_builder_->GetInsertBlock();

            /* merged suffix sequence */
            parent_fn->insert(parent_fn->end(), merge_bb);
            llvm_ir_builder_->SetInsertPoint(merge_bb);

            /** TODO: switch to getInt1Ty here **/
            llvm::PHINode * phi_node
                = llvm_ir_builder_->CreatePHI(llvm::Type::getDoubleTy(llvm_cx_->llvm_cx_ref()),
                                              2 /*#of branches being merged (?)*/,
                                              "iftmp");
            phi_node->addIncoming(when_true_ir, when_true_bb);
            phi_node->addIncoming(when_false_ir, when_false_bb);

            return phi_node;
        }

        llvm::Value *
        MachPipeline::codegen(ref::brw<Expression> expr)
        {
            switch(expr->extype()) {
            case exprtype::constant:
                return this->codegen_constant(ConstantInterface::from(expr));
            case exprtype::primitive:
                return this->codegen_primitive(PrimitiveInterface::from(expr));
            case exprtype::apply:
                return this->codegen_apply(Apply::from(expr));
            case exprtype::lambda:
                return this->codegen_lambda(Lambda::from(expr));
            case exprtype::variable:
                return this->codegen_variable(Variable::from(expr));
            case exprtype::ifexpr:
                return this->codegen_ifexpr(IfExpr::from(expr));
            case exprtype::invalid:
            case exprtype::n_expr:
                return nullptr;
                break;
            }

            cerr << "MachPipeline::codegen: error: no handler for expression of type T"
                 << xtag("T", expr->extype())
                 << endl;

            return nullptr;
        } /*codegen*/

        void
        MachPipeline::dump_current_module()
        {
            /* dump module contents to console */

            llvm_module_->dump();
        }

        void
        MachPipeline::machgen_current_module()
        {
            static llvm::ExitOnError llvm_exit_on_err;

            auto tracker = this->jit_->dest_dynamic_lib_ref().createResourceTracker();

            /* invalidates llvm_cx_->llvm_cx_ref();  will discard and re-create
             *
             * Note that @ref ir_pipeline_ holds reference,  which is invalidated here
             */
            auto ts_module = llvm::orc::ThreadSafeModule(std::move(llvm_module_),
                                                         std::move(llvm_cx_->llvm_cx()));

            /* note does not discard llvm_cx_->llvm_cx(),  it's already been moved */
            this->llvm_cx_ = nullptr;

            llvm_exit_on_err(this->jit_->add_llvm_module(std::move(ts_module), tracker));

            this->recreate_llvm_ir_pipeline();
        }
        std::string
        MachPipeline::mangle(const std::string & sym) const
        {
            auto p = this->jit_->mangle(sym);

            if (p)
                return (*p).str();

            throw std::runtime_error(tostr("MachPipeline::mangle"
                                           ": mangle(sym) returned empty pointer",
                                           xtag("sym", sym)));
        } /*mangle*/

        llvm::Expected<llvm::orc::ExecutorAddr>
        MachPipeline::lookup_symbol(const std::string & sym)
        {
            /* llvm_sym: ExecutorSymbolDef */
            auto llvm_sym_expected = this->jit_->lookup(sym);

            if (llvm_sym_expected) {
                auto llvm_addr = llvm_sym_expected.get().getAddress();

                return llvm_addr;
            } else {
                return llvm_sym_expected.takeError();
            }
        } /*lookup_symbol*/

        void
        MachPipeline::display(std::ostream & os) const {
            os << "<MachPipeline>";
        }

        std::string
        MachPipeline::display_string() const {
            return tostr(*this);
        }
    } /*namespace jit*/
} /*namespace xo*/

/* end MachPipeline.cpp */
