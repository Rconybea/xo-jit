/* @file type2llvm.cpp */

#include "type2llvm.hpp"
#include "xo/reflect/Reflect.hpp"
//#include "xo/reflect/struct/StructMember.hpp"

namespace xo {
    using xo::reflect::Reflect;
    using xo::reflect::TypeDescr;
    using xo::reflect::StructMember;
    using std::cerr;
    using std::endl;

    namespace jit {
        /** REMINDER:
         *  1. creation of llvm types is idempotent
         *     (duplicate calls will receive the same llvm::Type* pointer)
         *  2. llvm::Types are never deleted.
         **/

        llvm::Type *
        type2llvm::td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx, TypeDescr td) {
            auto & llvm_cx_ref = llvm_cx->llvm_cx_ref();

            if (td->is_function()) {
                /* in this context,  we're looking for a representation for a value,
                 * i.e. something that can be stored in a variable
                 */
                return function_td_to_llvm_fnptr_type(llvm_cx, td);
            } else if (td->is_struct()) {
                return struct_td_to_llvm_type(llvm_cx, td);
            } else if (td->is_pointer()) {
                return pointer_td_to_llvm_type(llvm_cx, td);
            } else if (Reflect::is_native<bool>(td)) {
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
        } /*td_to_llvm_type*/

        /** obtain llvm representation for a function type with the same signature as
         *  that represented by @p fn_td
         **/
        llvm::FunctionType *
        type2llvm::function_td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                            TypeDescr fn_td,
                                            bool wrapper_flag)
        {
            int n_fn_arg = fn_td->n_fn_arg();

            std::vector<llvm::Type *> llvm_argtype_v;
            llvm_argtype_v.reserve(n_fn_arg + (wrapper_flag ? 1 : 0));

            if (wrapper_flag)
                llvm_argtype_v.push_back(env_api_llvm_ptr_type(llvm_cx));

            /** check function args are all known **/
            for (int i = 0; i < n_fn_arg; ++i) {
                TypeDescr arg_td = fn_td->fn_arg(i);

                llvm::Type * llvm_argtype = type2llvm::td_to_llvm_type(llvm_cx, arg_td);

                if (!llvm_argtype)
                    return nullptr;

                llvm_argtype_v.push_back(llvm_argtype);
            }

            TypeDescr retval_td = fn_td->fn_retval();
            llvm::Type * llvm_retval = type2llvm::td_to_llvm_type(llvm_cx, retval_td);

            if (!llvm_retval)
                return nullptr;

            auto * llvm_fn_type = llvm::FunctionType::get(llvm_retval,
                                                          llvm_argtype_v,
                                                          false /*!varargs*/);
            return llvm_fn_type;
        } /*function_td_to_llvm_type*/

        llvm::PointerType *
        type2llvm::function_td_to_llvm_fnptr_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                  TypeDescr fn_td)
        {
            auto * llvm_fn_type = function_td_to_llvm_type(llvm_cx, fn_td);

            /** like C: llvm IR doesn't support function-valued variables;
             *  it does however support pointer-to-function-valued variables
             **/
            auto * llvm_ptr_type
                = llvm::PointerType::get(llvm_fn_type,
                                         0 /*numbered address space*/);

            return llvm_ptr_type;
        }

        /**
         *  Generate llvm::Type correspoinding to a TypeDescr for a struct.
         **/
        llvm::StructType *
        type2llvm::struct_td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                          TypeDescr struct_td)
        {
            // see
            //   [[https://stackoverflow.com/questions/32299166/accessing-struct-members-and-arrays-of-structs-from-llvm-ir]]

            auto & llvm_cx_ref = llvm_cx->llvm_cx_ref();

            /* note: object pointer ignored for struct types,
             *       since number of members is known at compile time
             */
            int n_member = struct_td->n_child(nullptr /*&object*/);

            /* one type for each struct member */
            std::vector<llvm::Type *> llvm_membertype_v;
            llvm_membertype_v.reserve(n_member);

            for (int i = 0; i < n_member; ++i) {
                StructMember const & sm = struct_td->struct_member(i);

                llvm_membertype_v.push_back(type2llvm::td_to_llvm_type(llvm_cx,
                                                                       sm.get_member_td()));
            }

            std::string struct_name = std::string(struct_td->short_name());

            /* structs with names:  within an llvmcontext, must be unique
             *
             * We can however compare the offsets recorded in xo::reflect with
             * offsets chosen by llvm, *once we've created the llvm type*
             *
             * Also,  we can't guarantee that a c++ type was completely reflected --
             * it's possible one or more members were omitted, in which case
             * it's unlikely at best that llvm chooses the same layout.
             *
             * Instead: tell llvm to make packed struct,
             *          and introduce dummy members for padding.
             *
             * A consequence is we have to maintain mapping between llvm's
             * member numbering and xo::reflect's
             */
            llvm::StructType * llvm_struct_type
                = llvm::StructType::create(llvm_cx_ref,
                                           llvm_membertype_v,
                                           llvm::StringRef(struct_name),
                                           false /*!isPacked*/);

            /* TODO: inspect (how) offsets that llvm is using
             * we need them to match what C++ chose
             *
             * (because we want jitted llvm code to interoperate with
             *  C++ library code that has structs)
             */

            // GetElementPtrInst is interesting,
            // but I think that's for generating code

            return llvm_struct_type;
        } /*struct_td_to_llvm_type*/

        llvm::PointerType *
        type2llvm::pointer_td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                           TypeDescr pointer_td)
        {
            assert(pointer_td->is_pointer());

            TypeDescr dest_td = pointer_td->fixed_child_td(0);

            llvm::Type * llvm_dest_type = type2llvm::td_to_llvm_type(llvm_cx, dest_td);

            llvm::PointerType * llvm_ptr_type
                = llvm::PointerType::getUnqual(llvm_dest_type);

            return llvm_ptr_type;
        } /*pointer_td_llvm_type*/

        llvm::PointerType *
        type2llvm::require_localenv_unwind_llvm_fnptr_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                           llvm::PointerType * envptr_llvm_type)
        {
            if (!envptr_llvm_type)
                envptr_llvm_type = env_api_llvm_ptr_type(llvm_cx);

            std::vector<llvm::Type *> llvm_argtype_v;
            llvm_argtype_v.reserve(2);

            /* 1st arg is _env_api pointer */
            llvm_argtype_v.push_back(envptr_llvm_type);

            /* 2nd arg is i32 */
            llvm_argtype_v.push_back(llvm::Type::getInt32Ty(llvm_cx->llvm_cx_ref()));

            /* return value is _env_api pointer */
            llvm::Type * retval_llvm_type = envptr_llvm_type;

            /* _env_api* (_env_api*, i32) */
            auto * unwind_llvm_type
                = llvm::FunctionType::get(retval_llvm_type,
                                          llvm_argtype_v,
                                          false /*!varargs*/);

            /* _env_api* (*)(_env_api*, i32) */
            auto * unwind_llvm_fnptr_type
                = llvm::PointerType::getUnqual(unwind_llvm_type);

            return unwind_llvm_fnptr_type;
        } /*require_localenv_unwind_llvm_fnptr_type*/

        llvm::StructType *
        type2llvm::env_api_llvm_type(xo::ref::brw<LlvmContext> llvm_cx)
        {
            /* _env_api: base type for a local environment */
            llvm::StructType * env_llvm_type
                = llvm::StructType::get(llvm_cx->llvm_cx_ref(),
                    "_env_api");

            /* _env_api[0]: pointer to a local environment */
            llvm::PointerType * envptr_llvm_type
                = llvm::PointerType::getUnqual(env_llvm_type);

            /* _env_api[1]: unwwind/copy function */
            llvm::PointerType * unwind_llvm_fnptr_type
                = type2llvm::require_localenv_unwind_llvm_fnptr_type(llvm_cx,
                                                                     envptr_llvm_type);

            /* now supply _env_api members */
            env_llvm_type->setBody(envptr_llvm_type /*_env_api[0]*/,
                                   unwind_llvm_fnptr_type /*_env_api[1]*/);

            return env_llvm_type;
        } /*env_api_llvm_type*/

        llvm::PointerType *
        type2llvm::env_api_llvm_ptr_type(xo::ref::brw<LlvmContext> llvm_cx)
        {
            llvm::StructType * env_llvm_type = env_api_llvm_type(llvm_cx);

            return llvm::PointerType::getUnqual(env_llvm_type);
        } /*env_api_llvm_ptr_type*/

#ifdef NOT_USING
        llvm::StructType *
        type2llvm::function_td_to_llvm_closure_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                    TypeDescr fn_td)
        {
            constexpr bool c_debug_flag = true;
            using xo::scope;

            scope log(XO_DEBUG(c_debug_flag));

            /* closure type doesn't need a name.
             * (We might find it convenient to give one anyway)
             */
            llvm::StructType * closure_llvm_type
                = llvm::StructType::get(llvm_cx->llvm_cx_ref());

            llvm::PointerType * parent_llvm_type
        } /*function_td_to_llvm_fnptr_type*/
#endif

        llvm::StructType *
        type2llvm::create_localenv_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                             xo::ref::brw<Lambda> lambda)
        {
            constexpr const char * c_prefix = "e.";

            llvm::PointerType * parentenvptr_llvm_type = env_api_llvm_ptr_type(llvm_cx);
            llvm::PointerType * unwind_llvm_fnptr_type
                = type2llvm::require_localenv_unwind_llvm_fnptr_type(llvm_cx, parentenvptr_llvm_type);

            std::vector<llvm::Type *> member_llvm_type_v;
            member_llvm_type_v.push_back(parentenvptr_llvm_type);
            member_llvm_type_v.push_back(unwind_llvm_fnptr_type);

            for (const auto & var : lambda->argv()) {
                if (lambda->is_captured(var->name())) {
                    /* var needs a slot in localenv_llvm_type for lambda */

                    member_llvm_type_v.push_back(td_to_llvm_type(llvm_cx,
                                                                 var->valuetype()));
                }
            }

            /* e.g. "e.foo" */
            std::string env_name = std::string(c_prefix) + lambda->name();

            llvm::StructType * localenv_lvtype
                = llvm::StructType::get(llvm_cx->llvm_cx_ref());

            localenv_lvtype->setName(env_name);
            localenv_lvtype->setBody(member_llvm_type_v, false /*!is_packed*/);

            return localenv_lvtype;
        } /*create_localenv_llvm_type*/

        llvm::StructType *
        type2llvm::create_closure_lvtype(xo::ref::brw<LlvmContext> llvm_cx,
                                         xo::ref::brw<Lambda> lambda)
        {
            constexpr const char * c_prefix = "c.";

            /* would be precisely correct to use create_localenv_llvm_type()
             * here.  However judged not sufficiently helpful.
             * Would still
             * need environment cast whenever closure in apply position is
             * not known at compile time.
             */
            llvm::PointerType * fn_lvtype = function_td_to_llvm_fnptr_type(llvm_cx,
                                                                            lambda->valuetype());
            llvm::StructType * env_lvtype = env_api_llvm_type(llvm_cx);

            std::vector<llvm::Type *> member_lvtype_v = { fn_lvtype, env_lvtype };

            /* e.g. "c.foo" */
            std::string closure_name = std::string(c_prefix) + lambda->name();

            llvm::StructType * closure_lvtype
                = llvm::StructType::create(llvm_cx->llvm_cx_ref(),
                                           member_lvtype_v,
                                           llvm::StringRef(closure_name),
                                           false /*!is_packed*/);

            return closure_lvtype;
        } /*create_closure_lvtype*/

    } /*namespace jit*/
} /*namespace xo*/

/* end type2llvm.cpp */
