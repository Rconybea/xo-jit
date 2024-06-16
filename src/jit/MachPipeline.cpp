/* @file MachPipeline.cpp */

#include "MachPipeline.hpp"

namespace xo {
    using xo::ast::exprtype;
    using xo::ast::Expression;
    using xo::ast::ConstantInterface;
    using xo::ast::PrimitiveInterface;
    using xo::ast::Lambda;
    using xo::ast::Variable;
    using xo::ast::Apply;
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
         * + 'jit_copmile_layer'
         * + 'jit_our_dynamic_lib'
         */
        llvm::Expected<std::unique_ptr<MachPipeline>>
        MachPipeline::make_aux()
        {
            MachPipeline::init_once();

            static llvm::ExitOnError llvm_exit_on_err;

            std::unique_ptr<KaleidoscopeJIT> kal_jit = llvm_exit_on_err(KaleidoscopeJIT::Create());

            return std::unique_ptr<MachPipeline>(new MachPipeline(std::move(kal_jit)
                                            ));
        } /*make*/

        xo::ref::rp<MachPipeline>
        MachPipeline::make() {
            static llvm::ExitOnError llvm_exit_on_err;

            std::unique_ptr<MachPipeline> jit = llvm_exit_on_err(make_aux());

            return jit.release();
        } /*make*/

        MachPipeline::MachPipeline(std::unique_ptr<KaleidoscopeJIT> kal_jit)
            : kal_jit_{std::move(kal_jit)}
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
            llvm_module_->setDataLayout(kal_jit_->data_layout());

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

            return kal_jit_->target_triple();
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
            kal_jit_->dump_execution_session();
        }

        llvm::Value *
        MachPipeline::codegen_constant(ref::brw<ConstantInterface> expr)
        {
            TypeDescr td = expr->value_td();

            if (td->is_native<double>()) {
                return llvm::ConstantFP::get(llvm_cx_->llvm_cx_ref(),
                                             llvm::APFloat(*(expr->value_tp().recover_native<double>())));
            } else if (td->is_native<float>()) {
                return llvm::ConstantFP::get(llvm_cx_->llvm_cx_ref(),
                                             llvm::APFloat(*(expr->value_tp().recover_native<float>())));
            }

            return nullptr;
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
            // just make prototype for function :: double -> double

            TypeDescr fn_td = expr->value_td();
            int n_fn_arg = fn_td->n_fn_arg();

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("fn_td", fn_td->short_name()),
                      xtag("n_fn_arg", n_fn_arg));

            std::vector<llvm::Type *> llvm_argtype_v;
            llvm_argtype_v.reserve(n_fn_arg);

            /** check function args are all doubles **/
            for (int i = 0; i < n_fn_arg; ++i) {
                TypeDescr arg_td = fn_td->fn_arg(i);

                log && log(xtag("i_arg", i), xtag("arg_td", arg_td->short_name()));

                if (arg_td->is_native<double>()) {
                    llvm_argtype_v.push_back(llvm::Type::getDoubleTy(llvm_cx_->llvm_cx_ref()));

                    // TODO: extend with other native types here...
                } else {
                    cerr << "MachPipeline::codegen_primitive: error: primitive f with arg i of type T where double expected"
                         << xtag("f", expr->name())
                         << xtag("i", i)
                         << xtag("T", arg_td->short_name())
                         << endl;
                    return nullptr;
                }
            }

            //std::vector<llvm::Type *> double_v(n_fn_arg, llvm::Type::getDoubleTy(*llvm_cx_));

            TypeDescr retval_td = fn_td->fn_retval();

            log && log(xtag("retval_td", retval_td->short_name()));

            llvm::Type * llvm_retval = nullptr;

            if (retval_td->is_native<double>()) {
                llvm_retval = llvm::Type::getDoubleTy(llvm_cx_->llvm_cx_ref());
            } else {
                cerr << "MachPipeline::codegen_primitive: error: primitive f returning T where double expected"
                     << xtag("f", expr->name())
                     << xtag("T", retval_td->short_name())
                     << endl;
                return nullptr;
            }

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
            if (apply->fn()->extype() == exprtype::primitive) {
                auto pm = PrimitiveInterface::from(apply->fn());
                auto * fn = this->codegen_primitive(pm);

#ifdef NOT_USING_DEBUG
                cerr << "MachPipeline::codegen_apply: fn:" << endl;
                fn->print(llvm::errs());
                cerr << endl;
#endif

                if (fn->arg_size() != apply->argv().size()) {
                    cerr << "MachPipeline::codegen_apply: error: callee f expecting n1 args where n2 supplied"
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
                    cerr << "MachPipeline::codegen_apply: arg:" << endl;
                    arg->print(llvm::errs());
                    cerr << endl;
#endif

                    args.push_back(arg);
                }

                return llvm_ir_builder_->CreateCall(fn, args, "calltmp");
            } else {
                cerr << "MachPipeline::codegen_apply: error: only allowing call to known primitives at present" << endl;
                return nullptr;
            }
        } /*codegen_apply*/

        llvm::Function *
        MachPipeline::codegen_lambda(ref::brw<Lambda> lambda)
        {
            /* reminder! this is the *expression*, not the *closure* */

            global_env_[lambda->name()] = lambda.get();

            /* do we already know a function with this name? */
            auto * fn = llvm_module_->getFunction(lambda->name());

            if (fn) {
                /** function with this name already defined?? **/
                return nullptr;
            }

            /* establish prototype for this function */

            // PLACEHOLDER
            // just handle double arguments + return type for now

            std::vector<llvm::Type *> double_v(1, llvm::Type::getDoubleTy(llvm_cx_->llvm_cx_ref()));

            auto * llvm_fn_type = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm_cx_->llvm_cx_ref()),
                                                          double_v,
                                                          false /*!varargs*/);

            /* create (initially empty) function */
            fn = llvm::Function::Create(llvm_fn_type,
                                        llvm::Function::ExternalLinkage,
                                        lambda->name(),
                                        llvm_module_.get());
            /* also capture argument names */
            int i = 0;
            for (auto & arg : fn->args())
                arg.setName(lambda->argv().at(i));

            /* generate function body */

            auto block = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(), "entry", fn);

            llvm_ir_builder_->SetInsertPoint(block);

            /* formal parameters need to appear in named_value_map_ */
            nested_env_.clear();
            for (auto & arg : fn->args())
                nested_env_[std::string(arg.getName())] = &arg;

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
            }

            return ix->second;
        } /*codegen_variable*/

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
        MachPipeline::machgen_current_module()
        {
            static llvm::ExitOnError llvm_exit_on_err;

            auto tracker = kal_jit_->getMainJITDylib().createResourceTracker();

            /* invalidates llvm_cx_->llvm_cx_ref();  will discard and re-create
             *
             * Note that @ref ir_pipeline_ holds reference,  which is invalidated here
             */
            auto ts_module = llvm::orc::ThreadSafeModule(std::move(llvm_module_),
                                                         std::move(llvm_cx_->llvm_cx()));

            /* note does not discard llvm_cx_->llvm_cx(),  it's already been moved */
            this->llvm_cx_ = nullptr;

            llvm_exit_on_err(kal_jit_->addModule(std::move(ts_module), tracker));

            this->recreate_llvm_ir_pipeline();
        }

        llvm::orc::ExecutorAddr
        MachPipeline::lookup_symbol(const std::string & sym)
        {
            static llvm::ExitOnError llvm_exit_on_err;

            /* llvm_sym: ExecutorSymbolDef */
            auto llvm_sym = llvm_exit_on_err(kal_jit_->lookup(sym));

            /* llvm_addr: ExecutorAddr */
            auto llvm_addr = llvm_sym.getAddress();

            return llvm_addr;
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
