/* @file MachPipeline.cpp */

#include "MachPipeline.hpp"
#include "activation_record.hpp"
#include "type2llvm.hpp"
#include <string>

namespace xo {
    using xo::ast::exprtype;
    using xo::ast::Expression;
    using xo::ast::ConstantInterface;
    //using xo::ast::FunctionInterface;
    using xo::ast::PrimitiveInterface;
    using xo::ast::Lambda;
    using xo::ast::Variable;
    using xo::ast::Apply;
    using xo::ast::IfExpr;
    using xo::ast::GlobalEnv;
    using xo::ast::llvmintrinsic;
    using xo::reflect::Reflect;
    using xo::reflect::StructMember;
    using xo::reflect::TypeDescr;
    using xo::scope;
    using llvm::orc::ExecutionSession;
    using llvm::DataLayout;
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
            : jit_{std::move(jit)},
              global_env_{GlobalEnv::make()}
        {
            this->recreate_llvm_ir_pipeline();
        }

        void
        MachPipeline::recreate_llvm_ir_pipeline()
        {
            //llvm_cx_ = std::make_unique<llvm::LLVMContext>();
            llvm_cx_ = LlvmContext::make();
            llvm_toplevel_ir_builder_ = std::make_unique<llvm::IRBuilder<>>(llvm_cx_->llvm_cx_ref());

            llvm_module_ = std::make_unique<llvm::Module>("xojit", llvm_cx_->llvm_cx_ref());
            llvm_module_->setDataLayout(this->jit_->data_layout());

            if (!llvm_cx_.get()) {
                throw std::runtime_error("MachPipeline::ctor: expected non-empty llvm context");
            }
            if (!llvm_toplevel_ir_builder_.get()) {
                throw std::runtime_error("MachPipeline::ctor: expected non-empty llvm IR builder");
            }
            if (!llvm_module_.get()) {
                throw std::runtime_error("MachPipeline::ctor: expected non-empty llvm module");
            }

            ir_pipeline_ = new IrPipeline(llvm_cx_);
        } /*recreate_llvm_ir_pipeline*/

        const DataLayout &
        MachPipeline::data_layout() const {
            return this->jit_->data_layout();
        }

        const ExecutionSession *
        MachPipeline::xsession() const {
            return this->jit_->xsession();
        }

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

        llvm::Type *
        MachPipeline::codegen_type(TypeDescr td) {
            return type2llvm::td_to_llvm_type(llvm_cx_.borrow(), td);
        }

        llvm::Function *
        MachPipeline::codegen_primitive(ref::brw<PrimitiveInterface> expr)
        {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG(c_debug_flag));

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

            TypeDescr fn_td = expr->valuetype();

            llvm::FunctionType * llvm_fn_type
                = type2llvm::function_td_to_llvm_type(llvm_cx_.borrow(), fn_td);

            if (!llvm_fn_type)
                return nullptr;

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

            if (expr->explicit_symbol_def()) {
                static llvm::ExitOnError llvm_exit_on_err;

                auto name = expr->name();
                auto fn_addr = expr->function_address();

                log && log(xtag("sym", name),
                           xtag("mangled_sym", this->jit_->mangle(name)));

                llvm_exit_on_err(this->jit_->intern_symbol(name, fn_addr));

#ifdef NOT_USING
                if (!llvm_result) {
                    cerr << "MachPipeline::codegen_primitive"
                         << ": intern_symbol failed"
                         << xtag("name", expr->name())
                         << xtag("addr", expr->function_address())
                         << endl;

                    return nullptr;
                }
#endif
            } else {
                log && log("not requiring absolute address", xtag("sym", expr->name()));
            }

#ifdef OBSOLETE
            log && log("returning llvm function");
#endif

            return fn;
        } /*codegen_primitive*/

        llvm::Function *
        MachPipeline::codegen_primitive_wrapper(ref::brw<PrimitiveInterface> expr,
                                                llvm::IRBuilder<> & ir_builder)
        {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("primitive-name", expr->name()));

            constexpr const char * c_prefix = "w.";

            /* unique name for wrapper.  Note we don't allow period in schematica identifiers
             * (though we could if we replace . with .. when lowering)
             */
            std::string wrap_name = std::string(c_prefix) + expr->name();

            /* original primitive */
            auto * native_lvfn = codegen_primitive(expr);

            /* wrapped primitive */
            auto * wrap_lvfn = llvm_module_->getFunction(wrap_name);

            if (wrap_lvfn) {
                /* wrapper already defined */
                return wrap_lvfn;
            }

            TypeDescr fn_td = expr->valuetype();

            llvm::FunctionType * native_lvtype
                = type2llvm::function_td_to_llvm_type(llvm_cx_.borrow(), fn_td);

            if (!native_lvtype)
                return nullptr;

            llvm::FunctionType * wrapper_lvtype
                = type2llvm::function_td_to_llvm_type(llvm_cx_.borrow(),
                                                      fn_td,
                                                      true /*wrapper_flag (for closure)*/);

            wrap_lvfn = llvm::Function::Create(wrapper_lvtype,
                                               llvm::Function::ExternalLinkage,
                                               wrap_name,
                                               llvm_module_.get());

            /* at least we know the name of the 1st argument :) */
            auto ix = wrap_lvfn->args().begin();
            ix->setName(".env");

            auto block = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(),
                                                  "entry", wrap_lvfn);

            ir_builder.SetInsertPoint(block);

            std::vector<llvm::Value *> args;

            /* call to native_lvfn,
             * forwarding all args of wrap_lvfn, except the first
             */
            {
                args.reserve(expr->n_arg());

                int i_wrap_arg = 0;
                for (auto & arg : wrap_lvfn->args()) {
                    if (i_wrap_arg > 0)
                        args.push_back(&arg);

                    ++i_wrap_arg;
                }
            }

            /* {caller,callee} must agree on calling convention,
             * so for primitives we need to assume c.
             */
            llvm::CallInst * call = ir_builder.CreateCall(native_lvtype,
                                                          native_lvfn,
                                                          args,
                                                          "w.calltmp");
            if (call) {
                call->setTailCall(true);

                /* does this work if call returns void? Is this needed with tail call? */
                ir_builder.CreateRet(call);

                llvm::verifyFunction(*wrap_lvfn);

                if (log) {
                    std::string buf;
                    llvm::raw_string_ostream ss(buf);
                    wrap_lvfn->print(ss);

                    log(xtag("IR-before-opt", buf));
                }

                /* optimize! */
                ir_pipeline_->run_pipeline(*wrap_lvfn);

                if (log) {
                    std::string buf;
                    llvm::raw_string_ostream ss(buf);
                    wrap_lvfn->print(ss);

                    log(xtag("IR-after-opt", buf));
                }
            } else {
                wrap_lvfn->eraseFromParent();
                wrap_lvfn = nullptr;
            }

            return wrap_lvfn;
        } /*codegen_primitive_wrapper*/

        llvm::Value *
        MachPipeline::codegen_apply(ref::brw<Apply> apply,
                                    llvm::IRBuilder<> & ir_builder)
        {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("apply", apply));

            // see here:
            //   https://stackoverflow.com/questions/54905211/how-to-implement-function-pointer-by-using-llvm-c-api

            using std::cerr;
            using std::endl;

            /* IR for value in function position.
             * Although it will generate a function (or pointer-to-function),
             * it need not have inherited type llvm::Function.
             */
            llvm::Value * llvm_fnval = nullptr;
            llvmintrinsic intrinsic = llvmintrinsic::invalid;
            /* function type in apply node's function position */
            TypeDescr ast_fn_td = apply->fn()->valuetype();
            {
                /* special treatement for primitive in apply position:
                 * allows substituting LLVM intrinsic
                 */
                if (apply->fn()->extype() == exprtype::primitive) {
                    auto pm = PrimitiveInterface::from(apply->fn());

                    if (pm) {
                        llvm_fnval = this->codegen_primitive(pm);
                        /* hint, when available. use faster alternative to IRBuilder::CreateCall below */
                        intrinsic = pm->intrinsic();
                    }
                } else {
                    llvm_fnval = this->codegen(apply->fn(), ir_builder);

                    /* we don't need any special checking here.
                     * already know (from xo-level checking) that pointer has the right type.
                     *
                     * Specifically, xo::ast::Apply::make() requires the expression in function position
                     * have suitable function type.
                     *
                     * Now: we have an llvm::Value (fn_value) representing the pointer.
                     * However it's not an llvm::Function instance,  and we can't get one.
                     *
                     * (Older LLVM versions allowed getting the element type from a pointer,
                     *  for some reasons that's deprecated at least in 18.1.5)
                     */
                }
            }

            if (!llvm_fnval) {
                return nullptr;
            }

#ifdef NOT_USING_DEBUG
            cerr << "MachPipeline::codegen_apply: fn:" << endl;
            fn->print(llvm::errs());
            cerr << endl;
#endif

            /* checks here will be redundant */

#ifdef REDUNDANT_TYPECHECK
            if (apply->argv().size() != ast_fn_td->n_fn_arg()) {
                cerr << "MachPipeline::codegen_apply: error: callee f expecting n1 args where n2 supplied"
                    //<< xtag("f", ast_fn->name())
                     << xtag("n1", ast_fn_td->n_fn_arg())
                     << xtag("n2", apply->argv().size())
                     << endl;

                return nullptr;
            }

            /** also check argument types **/
            for (size_t i = 0, n = ast_fn_td->n_fn_arg(); i < n; ++i) {
                if (apply->argv()[i]->valuetype() != ast_fn_td->fn_arg(i)) {
                    cerr << "MachPipeline::codegen_apply: error: callee F for arg# I seeeing U instead of expected T"
                         << xtag("F", apply->fn())
                         << xtag("I", i)
                         << xtag("U", apply->argv()[i]->valuetype()->short_name())
                         << xtag("T", ast_fn_td->fn_arg(i)->short_name())
                         << endl;

                    return nullptr;
                }
            }
#endif

            std::vector<llvm::Value *> args;
            args.reserve(apply->argv().size());

            int i = 0;
            for (const auto & arg_expr : apply->argv()) {
                auto * arg = this->codegen(arg_expr, ir_builder);

                if (log) {
                    /* TODO: print helper for llvm::Value* */
                    std::string llvm_value_str;
                    llvm::raw_string_ostream ss(llvm_value_str);
                    arg->print(ss);

                    log(xtag("i_arg", i),
                        xtag("arg", llvm_value_str));
                }

                args.push_back(arg);
                ++i;
            }

            /* if we have an intrinsic hint,
             * then instead of invoking a function,
             * we use some native machine instruction instead.
             */
            switch(intrinsic) {
            case llvmintrinsic::i_neg:
                return ir_builder.CreateNeg(args[0]);
            case llvmintrinsic::i_add:
                return ir_builder.CreateAdd(args[0], args[1]);
            case llvmintrinsic::i_sub:
                return ir_builder.CreateSub(args[0], args[1]);
            case llvmintrinsic::i_mul:
                return ir_builder.CreateMul(args[0], args[1]);
            case llvmintrinsic::i_sdiv:
                return ir_builder.CreateSDiv(args[0], args[1]);
            case llvmintrinsic::i_udiv:
                return ir_builder.CreateUDiv(args[0], args[1]);
            case llvmintrinsic::fp_add:
                return ir_builder.CreateFAdd(args[0], args[1]);
            case llvmintrinsic::fp_mul:
                return ir_builder.CreateFMul(args[0], args[1]);
            case llvmintrinsic::fp_div:
                return ir_builder.CreateFDiv(args[0], args[1]);
            case llvmintrinsic::invalid:
            case llvmintrinsic::fp_sqrt:
            case llvmintrinsic::fp_pow:
            case llvmintrinsic::fp_sin:
            case llvmintrinsic::fp_cos:
            case llvmintrinsic::fp_tan:
            case llvmintrinsic::n_intrinsic: /* n_intrinsic: not reachable */
                break;
            }

            /* At least as of 18.1.5,  LLVM needs us to supply function type
             * when making a function call.   In particular it doesn't remember
             * the function type with each function pointer
             */

            llvm::FunctionType * llvm_fn_type
                = type2llvm::function_td_to_llvm_type(this->llvm_cx_, ast_fn_td);

            return ir_builder.CreateCall(llvm_fn_type,
                                         llvm_fnval,
                                         args,
                                         "calltmp");

        } /*codegen_apply*/

#ifdef OBSOLETE
        /* in kaleidoscope7.cpp: CreateEntryBlockAlloca */
        llvm::AllocaInst *
        MachPipeline::create_entry_block_alloca(llvm::Function * llvm_fn,
                                                const std::string & var_name,
                                                TypeDescr var_type)
        {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("llvm_fn", (void*)llvm_fn),
                      xtag("var_name", var_name),
                      xtag("var_type", var_type->short_name()));

            llvm::IRBuilder<> tmp_ir_builder(&llvm_fn->getEntryBlock(),
                                             llvm_fn->getEntryBlock().begin());

            llvm::Type * llvm_var_type = type2llvm::td_to_llvm_type(llvm_cx_.borrow(),
                                                                    var_type);

            log && log(xtag("addr(llvm_var_type)", (void*)llvm_var_type));
            if (log) {
                std::string llvm_var_type_str;
                llvm::raw_string_ostream ss(llvm_var_type_str);
                llvm_var_type->print(ss);

                log(xtag("llvm_var_type", llvm_var_type_str));
            }

            if (!llvm_var_type)
                return nullptr;

            llvm::AllocaInst * retval = tmp_ir_builder.CreateAlloca(llvm_var_type,
                                                                    nullptr,
                                                                    var_name);
            log && log(xtag("alloca", (void*)retval),
                       xtag("align", retval->getAlign().value()),
                       xtag("size", retval->getAllocationSize(jit_->data_layout()).value()));

            return retval;
        } /*create_entry_block_alloca*/
#endif


        std::vector<ref::brw<Lambda>>
        MachPipeline::find_lambdas(ref::brw<Expression> expr) const
        {
            std::vector<ref::brw<Lambda>> retval_v;

            expr->visit_preorder(
                [&retval_v](ref::brw<Expression> x)
                    {
                        if (x->extype() == exprtype::lambda) {
                            retval_v.push_back(Lambda::from(x));
                        }
                    });

            return retval_v;
        } /*find_lambdas*/

        llvm::Function *
        MachPipeline::codegen_lambda_decl(ref::brw<Lambda> lambda)
        {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("lambda-name", lambda->name()));

            this->global_env_->require_global(lambda->name(), lambda);

            /* do we already know a function with this name? */
            auto * fn = llvm_module_->getFunction(lambda->name());

            if (fn) {
                return fn;
            }

            /* establish prototype for this function */

            llvm::FunctionType * llvm_fn_type
                = type2llvm::function_td_to_llvm_type(llvm_cx_.borrow(),
                                                      lambda->valuetype());

            /* create (initially empty) function */
            fn = llvm::Function::Create(llvm_fn_type,
                                        llvm::Function::ExternalLinkage,
                                        lambda->name(),
                                        llvm_module_.get());
            /* also capture argument names */
            {
                int i = 0;
                for (auto & arg : fn->args()) {
                    log && log("llvm formal param names",
                               xtag("i", i),
                               xtag("param", lambda->argv().at(i)));

                    arg.setName(lambda->argv().at(i)->name());
                    ++i;
                }
            }

            return fn;
        } /*codegen_lambda_decl*/

        llvm::Function *
        MachPipeline::codegen_lambda_defn(ref::brw<Lambda> lambda,
                                          llvm::IRBuilder<> & ir_builder)
        {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("lambda-name", lambda->name()));

            global_env_->require_global(lambda->name(), lambda.get());

            /* do we already know a function with this name? */
            auto * llvm_fn = llvm_module_->getFunction(lambda->name());

            if (!llvm_fn) {
                /** function with this name not declared? **/
                cerr << "MachPipeline::codegen_lambda: function f not declared"
                     << xtag("f", lambda->name())
                     << endl;

                return nullptr;
            }


            /* generate function body */

            auto block = llvm::BasicBlock::Create(llvm_cx_->llvm_cx_ref(), "entry", llvm_fn);

            ir_builder.SetInsertPoint(block);

            /** Actual parameters will need their own activation record.
             *  Track its shape + setup/teardown here.
             **/
            this->env_stack_.push(activation_record(lambda.get()));

            bool ok_flag = this->env_stack_.top().bind_locals(llvm_cx_, llvm_fn, ir_builder);

            if (!ok_flag) {
                this->env_stack_.pop();
                return nullptr;
            }

#ifdef OBSOLETE
            {
                log && log("lambda: stack size Z", xtag("Z", env_stack_.size()));

                int i = 0;
                for (auto & arg : llvm_fn->args()) {
                    log && log("nested environment",
                               xtag("i", i),
                               xtag("param", std::string(arg.getName())));

                    std::string arg_name = std::string(arg.getName());

                    /* stack location for arg[i] */
                    llvm::AllocaInst * alloca
                        = create_entry_block_alloca(llvm_fn,
                                                    arg_name,
                                                    lambda->fn_arg(i));

                    if (!alloca) {
                        this->env_stack_.pop();
                        return nullptr;
                    }

                    /* store on function entry
                     *   see codegen_variable() for corresponding load
                     */
                    ir_builder.CreateStore(&arg, alloca);

                    /* remember stack location for reference + assignment
                     * in lambda body.
                     *
                     */
                    env_stack_.top().alloc_var(i, arg_name, alloca);
                    ++i;
                }
            }
#endif

            llvm::Value * retval = this->codegen(lambda->body(), ir_builder);

            if (retval) {
                /* completes the function.. */
                ir_builder.CreateRet(retval);

                /* validate!  always validate! */
                llvm::verifyFunction(*llvm_fn);

                if (log) {
                    std::string buf;
                    llvm::raw_string_ostream ss(buf);
                    llvm_fn->print(ss);

                    log(xtag("IR-before-opt", buf));
                }

                /* optimize!  improves IR */
                ir_pipeline_->run_pipeline(*llvm_fn); // llvm_fpmgr_->run(*llvm_fn, *llvm_famgr_);

                if (log) {
                    std::string buf;
                    llvm::raw_string_ostream ss(buf);
                    llvm_fn->print(ss);

                    log(xtag("IR-after-opt", buf));
                }
            } else {
                /* oops,  something went wrong */
                llvm_fn->eraseFromParent();

                llvm_fn = nullptr;
            }

            this->env_stack_.pop();

            log && log("after pop, env stack size Z", xtag("Z", env_stack_.size()));

            return llvm_fn;
        } /*codegen_lambda_defn*/

        llvm::Value *
        MachPipeline::codegen_variable(ref::brw<Variable> var,
                                       llvm::IRBuilder<> & ir_builder)
        {
            if (env_stack_.empty()) {
                cerr << "MachPipeline::codegen_variable: expected non-empty environment stack"
                     << xtag("x", var->name())
                     << endl;

                return nullptr;
            }

            activation_record & ar = env_stack_.top();

            const runtime_binding_detail * binding = ar.lookup_var(var->name());

            if (!binding)
                return nullptr;

            /* code to load value from stack */
            return ir_builder.CreateLoad(binding->llvm_type_,
                                         binding->llvm_addr_,
                                         var->name().c_str());
        } /*codegen_variable*/

        llvm::Value *
        MachPipeline::codegen_ifexpr(ref::brw<IfExpr> expr, llvm::IRBuilder<> & ir_builder)
        {
            llvm::Value * test_ir = this->codegen(expr->test(), ir_builder);

            /** need test result in a variable **/
            llvm::Value * test_with_cmp_ir
                = ir_builder.CreateFCmpONE(test_ir,
                                           llvm::ConstantFP::get(llvm_cx_->llvm_cx_ref(),
                                                                 llvm::APFloat(0.0)),
                                           "iftest");

            llvm::Function * parent_fn = ir_builder.GetInsertBlock()->getParent();

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
            ir_builder.CreateCondBr(test_with_cmp_ir,
                                    when_true_bb,
                                    when_false_bb);

            /* populate when_true_bb */
            ir_builder.SetInsertPoint(when_true_bb);

            llvm::Value * when_true_ir = this->codegen(expr->when_true(),
                                                       ir_builder);

            if (!when_true_ir)
                return nullptr;

            /* at end of when-true sequence, jump to merge suffix */
            ir_builder.CreateBr(merge_bb);
            /* note: codegen for expr->when_true() may have altered builder's "current block" */
            when_true_bb = ir_builder.GetInsertBlock();

            /* populate when_false_bb */
            parent_fn->insert(parent_fn->end(), when_false_bb);
            ir_builder.SetInsertPoint(when_false_bb);

            llvm::Value * when_false_ir = this->codegen(expr->when_false(), ir_builder);
            if (!when_false_ir)
                return nullptr;

            /* at end of when-false sequence, jump to merge suffix */
            ir_builder.CreateBr(merge_bb);
            /* note: codegen for expr->when_false() may have altered builder's "current block" */
            when_false_bb = ir_builder.GetInsertBlock();

            /* merged suffix sequence */
            parent_fn->insert(parent_fn->end(), merge_bb);
            ir_builder.SetInsertPoint(merge_bb);

            /** TODO: switch to getInt1Ty here **/
            llvm::PHINode * phi_node
                = ir_builder.CreatePHI(llvm::Type::getDoubleTy(llvm_cx_->llvm_cx_ref()),
                                       2 /*#of branches being merged (?)*/,
                                       "iftmp");
            phi_node->addIncoming(when_true_ir, when_true_bb);
            phi_node->addIncoming(when_false_ir, when_false_bb);

            return phi_node;
        } /*codegen_ifexpr*/

        llvm::Value *
        MachPipeline::codegen(ref::brw<Expression> expr, llvm::IRBuilder<> & ir_builder)
        {
            switch(expr->extype()) {
            case exprtype::constant:
                return this->codegen_constant(ConstantInterface::from(expr));
            case exprtype::primitive:
                return this->codegen_primitive(PrimitiveInterface::from(expr));
            case exprtype::apply:
                return this->codegen_apply(Apply::from(expr), ir_builder);
            case exprtype::lambda:
                return this->codegen_lambda_decl(Lambda::from(expr));
            case exprtype::variable:
                return this->codegen_variable(Variable::from(expr), ir_builder);
            case exprtype::ifexpr:
                return this->codegen_ifexpr(IfExpr::from(expr), ir_builder);
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

        llvm::Value *
        MachPipeline::codegen_toplevel(ref::brw<Expression> expr)
        {
            /* - Pass 1.
             *   get set of lambdas.
             *   Generate decls for all.
             *
             *   TODO: for lexical scoping (not implemented yet)
             *         will need traversal that maintains stack
             *         of ancestor lambdas,  or at least their
             *         activation records.  May want to generalize
             *         activation_record so we can track the set of variables
             *         before generating AllocaInst's.
             *
             * - Pass 2.
             *   Generate code for lambdas.
             *
             * - Pass 3.
             *   If toplevel expressions isn't a lambda
             *   (? won't make sense at present when called from python)
             *   generate code for it too
             */

            /* Pass 1. */
            auto fn_v = this->find_lambdas(expr);

            for (auto lambda : fn_v) {
                this->codegen_lambda_decl(lambda);
            }

            /* Pass 2 */
            for (auto lambda : fn_v) {
                this->codegen_lambda_defn(lambda,
                                          *(this->llvm_toplevel_ir_builder_.get()));
            }

            /* Pass 3 */
            if (expr->extype() == exprtype::lambda) {
                /* code already generated in pass 2;
                 * look it up
                 */

                return llvm_module_->getFunction(Lambda::from(expr)->name());
            } else {
                /* toplevel expression isn't a lambda,
                 * so code for it hasn't been generated.
                 * Do that now
                 */
                return this->codegen(expr,
                                     *(this->llvm_toplevel_ir_builder_.get()));
            }
        } /*codegen_toplevel*/

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
        } /*machgen_current_module*/

        std::string_view
        MachPipeline::mangle(const std::string & sym) const
        {
            return this->jit_->mangle(sym);
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
