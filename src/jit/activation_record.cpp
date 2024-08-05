/* @file activation_record.cpp */

#include "activation_record.hpp"
#include "type2llvm.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <iostream>

namespace xo {
    namespace jit {
        using std::cerr;
        using std::endl;

        activation_record::activation_record(const ref::rp<Lambda> & lm)
            : lambda_{lm},
              binding_v_(lm->n_arg())
        {
            /* populate binding_v_ */
            int n_arg = lm->n_arg();
            binding_v_.resize(n_arg);

            /* next slot# to use in explicit activation record */
            int rt_env_slot = 0;

            for (int i_arg = 0; i_arg < n_arg; ++i_arg) {
                if (lm->is_captured(lm->i_argname(i_arg))) {
                    /* local param #i_arg needs a slot in explicit activation record */
                    binding_v_[i_arg] = runtime_binding_path::local(rt_env_slot);
                    ++rt_env_slot;
                } else {
                    binding_v_[i_arg] = runtime_binding_path::stackonly();
                }
            }
        } /*ctor*/

        const runtime_binding_detail *
        activation_record::lookup_var(const std::string & x) const
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag));

            auto ix = frame_.find(x);

            if (ix == frame_.end()) {
                cerr << "activation_record::lookup_var: no binding for variable x"
                     << xtag("x", x)
                     << endl;
                cerr << "frame:";
                for (const auto & ix : frame_)
                    cerr << xtag("var", ix.first) << xtag("->", ix.second) << endl;

                return nullptr;
            }

            return &(ix->second);
        } /*lookup_var*/

        const runtime_binding_detail *
        activation_record::alloc_var(const std::string & x,
                                     const runtime_binding_detail & binding)
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag));

            log && log(xtag("var", x),
                       xtag("binding", binding));

            if (frame_.find(x) != frame_.end()) {
                cerr << "activation_record::alloc_var: variable x already present in frame"
                     << xtag("x", x)
                     << endl;
                return nullptr;
            }

            frame_[x] = binding;

            return &(frame_[x]);
        } /*alloc_var*/

        /* in kaleidoscope7.cpp: CreateEntryBlockAlloca */
        runtime_binding_detail
        activation_record::create_entry_block_alloca(ref::brw<LlvmContext> llvm_cx,
                                                     //const llvm::DataLayout & data_layout,
                                                     llvm::Function * llvm_fn,
                                                     llvm::IRBuilder<> & fn_ir_builder,
                                                     int i_arg,
                                                     const std::string & var_name,
                                                     TypeDescr var_type)
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag));

            log && log(xtag("llvm_fn", (void*)llvm_fn),
                       xtag("i_arg", i_arg),
                       xtag("var_name", var_name),
                       xtag("var_type", var_type->short_name()));

            llvm::Type * llvm_var_type = type2llvm::td_to_llvm_type(llvm_cx,
                                                                    var_type);

            log && log(xtag("addr(llvm_var_type)", (void*)llvm_var_type));
            if (log) {
                std::string llvm_var_type_str;
                llvm::raw_string_ostream ss(llvm_var_type_str);
                llvm_var_type->print(ss);

                log(xtag("llvm_var_type", llvm_var_type_str));
            }

            if (!llvm_var_type)
                return runtime_binding_detail{}; /*sentinel*/

            llvm::AllocaInst * stackaddr = fn_ir_builder.CreateAlloca(llvm_var_type,
                                                                      nullptr,
                                                                      var_name);

            log && log(xtag("alloca", (void*)stackaddr),
                       xtag("align", stackaddr->getAlign().value())
                       //xtag("size", retval->getAllocationSize(data_layout).value())
                );

            return {i_arg, stackaddr, llvm_var_type};
        } /*create_entry_block_alloca*/

#ifdef NOT_USING
        llvm::AllocaInst *
        activation_record::create_runtime_localenv_alloca(ref::brw<LlvmContext> llvm_cx,
                                                          //const llvm::DataLayout & data_layout,
                                                          llvm::Function * llvm_fn,
                                                          llvm::IRBuilder<> & ir_builder)

        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("llvm_fn", (void*)llvm_fn));

            llvm::StructType * localenv_llvm_type
                = type2llvm::create_localenv_llvm_type(llvm_cx, lambda_.borrow());

            if (!localenv_llvm_type)
                return nullptr;

            llvm::AllocaInst * retval = ir_builder.CreateAlloca(localenv_llvm_type,
                                                                nullptr /*ArraySize*/,
                                                                "_localenv");

            log && log(xtag("alloca", (void*)retval),
                       xtag("align", retval->getAlign().value())
                       //xtag("size", retval->getAllocationSize(data_layout).value())
                );

            return retval;
        } /*create_runtime_localenv_alloca*/
#endif

        llvm::Value *
        activation_record::runtime_localenv_slot_addr(ref::brw<LlvmContext> llvm_cx,
                                                      llvm::StructType * localenv_llvm_type,
                                                      llvm::AllocaInst * localenv_alloca,
                                                      int i_slot,
#ifdef NOT_HERE
                                                      llvm::Value * llvm_slot_value,
#endif
                                                      llvm::IRBuilder<> & tmp_ir_builder)
        {
            llvm::Value * i0_slot
                = llvm::ConstantInt::get(llvm_cx->llvm_cx_ref(),
                                         llvm::APInt(32 /*bits*/, 0));

            llvm::Value * i32_slot
                = llvm::ConstantInt::get(llvm_cx->llvm_cx_ref(),
                                         llvm::APInt(32 /*bits*/,
                                                     i_slot /*value*/));
            std::array<llvm::Value*, 2> index_v = {
                {i0_slot, i32_slot /*environment slot #0*/}};

            llvm::Value * llvm_localenv_slot_ptr
                = tmp_ir_builder.CreateInBoundsGEP(localenv_llvm_type,
                                                   localenv_alloca,
                                                   index_v);

            return llvm_localenv_slot_ptr;

#ifdef NOT_HERE
            tmp_ir_builder.CreateStore(llvm_value, //llvm_0ptr,
                                       llvm_parent_env_ptr);
#endif
        } /*runtime_localenv_slot_addr*/

        bool
        activation_record::bind_locals(ref::brw<LlvmContext> llvm_cx,
                                       //const llvm::DataLayout & data_layout,
                                       llvm::Function * llvm_fn,
                                       llvm::IRBuilder<> & ir_builder)
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag),
                      xtag("lambda-name", lambda_->name()));

            llvm::IRBuilder<> tmp_ir_builder(&llvm_fn->getEntryBlock(),
                                             llvm_fn->getEntryBlock().begin());

            /* 1st pass: handle stackonly variables
             *
             * We presume this must come first,
             * for subsequent mem2reg optimization pass to consider
             */
            {
                int i_arg = 0;
                for (auto & arg : llvm_fn->args()) {
                    if (i_arg == 0) {
                        /* 1st argument is injected environment pointer.
                         * we don't need that to be on the stack,
                         * since not modifiable and not user-referencable.
                         */
                    } else {
                        std::string arg_name = std::string(arg.getName());

                        log && log("nested environment",
                                   xtag("i", i_arg),
                                   xtag("arg[i]", arg_name),
                                   xtag("stackonly(i)", binding_v_[i_arg-1].is_stackonly()));

                        if (binding_v_[i_arg-1].is_stackonly()) {
                            /* stack location for arg[i] */
                            runtime_binding_detail binding
                                = create_entry_block_alloca(llvm_cx,
                                                            //data_layout,
                                                            llvm_fn,
                                                            tmp_ir_builder,
                                                            i_arg,
                                                            arg_name,
                                                            lambda_->fn_arg(i_arg-1));

                            if (!binding.llvm_addr_)
                                return false;

                            /* store on function entry
                             *   see codegen_variable() for corresponding load
                             */
                            ir_builder.CreateStore(&arg, binding.llvm_addr_);

                            /* remember stack location for reference + assignment
                             * in lambda body.
                             *
                             */
                            this->alloc_var(arg_name, binding);
                        }
                    }

                    ++i_arg;
                }
            }

            /* REMINDER: all functions need to follow the closure pattern,
             *           to accomodate cases where we don't know until runtime
             *           what kind of function we are invoking.
             *
             * This means:
             * - always represent function in IR by a closure-shaped object
             *
             *                  +-------+
             *                  |   o---------> native function
             *                  +-------+
             *                  |   o---------> runtime localenv
             *                  +-------+       (possibly nullptr)
             *
             * We hope to optimize away the closures in cases where we know
             * their contents at compile time
             *
             */

            /* 2nd pass: handle captured formal parameters */
            if (lambda_->needs_closure_flag()) {
                llvm::StructType * localenv_llvm_type
                    = type2llvm::create_localenv_llvm_type(llvm_cx, lambda_.borrow());
#ifdef NOT_USING
                llvm::PointerType * envapiptr_llvm_type
                    = type2llvm::env_api_llvm_ptr_type(llvm_cx);
#endif

                if (!localenv_llvm_type)
                    return false;

                /*
                 * runtime localenv:            ^
                 *                              | parent
                 *                  +-------+   |
                 *   parent_env [0] |   o-------/
                 *                  +-------+
                 *   unwind_fn  [1] |   o-------> env * (*)(env*, ctl)
                 *                  +-------+
                 *   arg[i]   [2+i] .  ...  .
                 *                  .  ...  .
                 *                  +-------+
                 *
                 *   ctl=0  unwind.  finalization for any arg[i] that requires it.
                 *                   returns nullptr
                 *   ctl=1  copy.    copy runtime environment to heap destination
                 *                   and return address of the copy
                 *
                 *   arg[] comprises the subset of lambda arg names arg[j] for which
                 *   lambda->is_captured(arg[j]) is true
                 */
                llvm::AllocaInst * localenv_alloca
                    = tmp_ir_builder.CreateAlloca(localenv_llvm_type,
                                                  nullptr /*ArraySize*/,
                                                  "_localenv");

                if (!localenv_alloca)
                    return false;

                /* remember environemnt location.
                 * Will need this if need to copy-to-stack
                 */
                this->localenv_alloca_ = localenv_alloca;

                int i_localenv_slot = 0;

                /* store localenv->parent_env */
                {
                    llvm::Value * slot_addr
                        = runtime_localenv_slot_addr(llvm_cx,
                                                     localenv_llvm_type,
                                                     localenv_alloca,
                                                     i_localenv_slot,
                                                     //llvm_0ptr,
                                                     tmp_ir_builder);

                    if (!slot_addr)
                        return false;

                    ++i_localenv_slot;

                    /* null pointer for now */
                    /* TODO: get parent environment (from runtime closure created for this function) */
                    llvm::Value * llvm_0ptr
                        = llvm::ConstantPointerNull::get(type2llvm::env_api_llvm_ptr_type(llvm_cx));

                    tmp_ir_builder.CreateStore(llvm_0ptr,
                                               slot_addr);
                }

                /* store localenv->unwind_fn */
                {
                    llvm::Value * slot_addr
                        = runtime_localenv_slot_addr(llvm_cx,
                                                     localenv_llvm_type,
                                                     localenv_alloca,
                                                     i_localenv_slot,
                                                     //llvm_0ptr,
                                                     tmp_ir_builder);

                    if (!slot_addr)
                        return false;

                    ++i_localenv_slot;

                    /* null function pointer for now */
                    /* TODO: construct unwind function */
                    llvm::Value * llvm_0ptr
                        = (llvm::ConstantPointerNull::get
                           (type2llvm::require_localenv_unwind_llvm_fnptr_type(llvm_cx)));

                    tmp_ir_builder.CreateStore(llvm_0ptr,
                                               slot_addr);
                }

                int i_arg = 0;

                for (llvm::Argument & arg : llvm_fn->args()) {
                    if (i_arg == 0) {
                        /* to remove all doubt,  ignore first arg here.
                         * it's non-captureable environment pointer
                         */
                    } else {
                        std::string arg_name = std::string(arg.getName());

                        log && log("nested environment",
                                   xtag("i", i_arg),
                                   xtag("arg[i]", arg_name),
                                   xtag("captured(i)", binding_v_[i_arg-1].is_captured()));

                        if (binding_v_[i_arg-1].is_captured()) {
                            // do something with runtime-local-env for this llvm_fn

                            /* remember stack location for reference + assignment
                             * in lambda body.
                             *
                             */

                            TypeDescr arg_td = lambda_->fn_arg(i_arg-1);

                            llvm::Type * llvm_var_type = type2llvm::td_to_llvm_type(llvm_cx, arg_td);

                            llvm::Value * slot_addr
                                = runtime_localenv_slot_addr(llvm_cx,
                                                             localenv_llvm_type,
                                                             localenv_alloca,
                                                             i_localenv_slot,
                                                             tmp_ir_builder);

                            if (!slot_addr)
                                return false;

                            ++i_localenv_slot;

                            tmp_ir_builder.CreateStore(&arg, slot_addr);

                            runtime_binding_detail binding = { i_arg, slot_addr, llvm_var_type };

                            this->alloc_var(arg_name, binding);
                        }
                    }

                    ++i_arg;
                }
            }

            return true;
        } /*bind_locals*/
    } /*namespace jit*/
} /*namespace xo*/


/* end activation_record.cpp */
