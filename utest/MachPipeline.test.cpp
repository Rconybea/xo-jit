/* @file MachPipeline.test.cpp */

#include "xo/jit/MachPipeline.hpp"
#include "xo/expression/Primitive.hpp"
#include "xo/ratio/ratio.hpp"
#include "xo/ratio/ratio_reflect.hpp"
#include "xo/reflect/reflect_struct.hpp"
#include "xo/indentlog/scope.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::jit::MachPipeline;
    using xo::ast::make_apply;
    using xo::ast::make_var;
    using xo::ast::make_primitive;
    using xo::ast::llvmintrinsic;
    using xo::ast::Expression;
    using xo::ast::Lambda;
    using xo::ast::exprtype;
    using xo::reflect::Reflect;
    using xo::reflect::reflect_struct;
    using xo::ref::rp;
    using xo::ref::brw;
    using std::cerr;
    using std::endl;

    namespace ut {

        /* abstract syntax tree for a function:
         *   def root4(x :: double) { sqrt(sqrt(x)); }
         */
        rp<Expression>
        root4_ast() {
            auto sqrt = make_primitive("sqrt",
                                       ::sqrt,
                                       false /*!explicit_symbol_def*/,
                                       llvmintrinsic::fp_sqrt);
            auto x_var = make_var("x", Reflect::require<double>());
            auto call1 = make_apply(sqrt, {x_var});
            auto call2 = make_apply(sqrt, {call1});

            auto fn_ast = make_lambda("root4",
                                      {x_var},
                                      call2);

            return fn_ast;
        }

        /* abstract syntax tree for a function:
         *   def twice(f :: double->double, x :: double) { f(f(x)); }
         */
        rp<Expression>
        root_2x_ast() {
            auto root = make_primitive("sqrt",
                                       ::sqrt,
                                       false /*!explicit_symbol_def*/,
                                       llvmintrinsic::fp_sqrt);

            /* def twice(f :: double->double, x :: double) { f(f(x)) } */
            auto f_var = make_var("f", Reflect::require<double (*)(double) noexcept>());
            auto x_var = make_var("x", Reflect::require<double>());
            auto call1 = make_apply(f_var, {x_var}); /* (f x) */
            auto call2 = make_apply(f_var, {call1}); /* (f (f x)) */

            /*   def twice(f :: double->double, x :: double) { f(f(x)); } */
            auto twice = make_lambda("twice",
                                     {f_var, x_var},
                                     call2);

            auto x2_var = make_var("x2", Reflect::require<double>());
            auto call3 = make_apply(twice, {root, x2_var});

            /*   def root4(x2 :: double) {
             *     def twice(f :: double->double, x :: double) { f(f(x)); }};
             *     twice(sqrt, x2)
             *   }
             */
            auto fn_ast = make_lambda("root_2x",
                                      {x2_var},
                                      call3);

            return fn_ast;
        }

        struct TestCase {
            rp<Expression> (*make_ast_)();
            /* each pair is (input, output) for function double->double */
            std::vector<std::pair<double,double>> call_v_;
        };

        static std::vector<TestCase>
        s_testcase_v = {
            {&root4_ast,
             {std::make_pair(1.0, 1.0),
              std::make_pair(16.0, 2.0),
              std::make_pair(81.0, 3.0)}},
            {&root_2x_ast,
             {std::make_pair(1.0, 1.0),
              std::make_pair(16.0, 2.0),
              std::make_pair(81.0, 3.0)}}
        };

        /** testcase root_ast tests:
         *  - nested function call
         *
         *  testcase root_2x_ast relies on:
         *  - lambda in function position
         *  - argument with function type
         **/
        TEST_CASE("machpipeline.fptr", "[llvm][llvm_fnptr]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.machpipeline"));
            //log && log("(A)", xtag("foo", foo));

            auto jit = MachPipeline::make();

            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                TestCase const & testcase = s_testcase_v[i_tc];

                INFO(tostr(xtag("i_tc", i_tc)));

                auto ast = (*testcase.make_ast_)();

                REQUIRE(ast.get());

                log && log(xtag("ast", ast));

                REQUIRE(ast->extype() == exprtype::lambda);

                brw<Lambda> fn_ast = Lambda::from(ast);

                llvm::Value * llvm_ircode = jit->codegen_toplevel(fn_ast);

                /* TODO: printer for llvm::Value* */
                if (llvm_ircode) {
                    /* note: llvm:errs() is 'raw stderr stream' */
                    cerr << "llvm_ircode:" << endl;
                    llvm_ircode->print(llvm::errs());
                    cerr << endl;
                } else {
                    cerr << "code generation failed"
                         << xtag("fn_ast", fn_ast)
                         << endl;
                }

                REQUIRE(llvm_ircode);

                jit->machgen_current_module();

                /** lookup compiled function pointer in jit **/
                auto llvm_addr = jit->lookup_symbol(fn_ast->name());

                if (!llvm_addr) {
                    cerr << "ex2: lookup: symbol not found"
                         << xtag("symbol", fn_ast->name())
                         << endl;
                } else {
                    cerr << "ex2: lookup: symbol found"
                         << xtag("llvm_addr", llvm_addr.get().getValue())
                         << xtag("symbol", fn_ast->name())
                         << endl;
                }

                auto fn_ptr = llvm_addr.get().toPtr<double(*)(double)>();

                REQUIRE(fn_ptr);

                for (std::size_t j_call = 0, n_call = testcase.call_v_.size(); j_call < n_call; ++j_call) {
                    double input = testcase.call_v_[j_call].first;
                    double expected = testcase.call_v_[j_call].second;

                    INFO(tostr(xtag("j_call", j_call), xtag("input", input), xtag("expected", expected)));

                    auto actual = (*fn_ptr)(input);

                    REQUIRE(actual == expected);
                }
            }
        } /*TEST_CASE(machpipeline)*/

        rp<Lambda>
        make_ratio() {
            auto make_ratio_impl = make_primitive("make_ratio_impl",
                                                  xo::ratio::make_ratio<int, int>,
                                                  true /*explicit_symbol_def*/,
                                                  llvmintrinsic::invalid);
            REQUIRE(make_ratio_impl.get());
            REQUIRE(make_ratio_impl->explicit_symbol_def());

            /* jit-prepared library:
             * 1. *uses* make_ratio_impl
             * 2. *provides* make_ratio  (can do jit->lookup_symbol("make_ratio"))
             */
            auto n_var = make_var("n", Reflect::require<int>());
            auto d_var = make_var("d", Reflect::require<int>());
            auto call1 = make_apply(make_ratio_impl, {n_var, d_var}); /*make_ratio(n,d)*/

            auto make_ratio = make_lambda("make_ratio",
                                          {n_var, d_var},
                                          call1);

            return make_ratio;
        }

        TEST_CASE("machpipeline.struct", "[llvm][llvm_struct]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.machpipeline.struct"));
            //log && log("(A)", xtag("foo", foo));

            auto jit = MachPipeline::make();

            /* let's reflect xo::ratio::ratio<int> */

            using ratio_type = xo::ratio::ratio<int>;

            auto struct_td = reflect_struct<ratio_type>();

            REQUIRE(struct_td);

            auto fn_ast = make_ratio();

            llvm::Value * llvm_ircode = jit->codegen_toplevel(fn_ast);

            /* TODO: printer for llvm::Value* */
            if (llvm_ircode) {
                /* note: llvm:errs() is 'raw stderr stream' */
                cerr << "llvm_ircode:" << endl;
                llvm_ircode->print(llvm::errs());
                cerr << endl;
            } else {
                cerr << "code generation failed"
                     << xtag("fn_ast", fn_ast)
                     << endl;
            }

            REQUIRE(llvm_ircode);

            jit->machgen_current_module();

            log && log("execution session after codegen:");
            //log && log(jit->xsession());  // segfaults
            jit->dump_execution_session();

            /** lookup compiled function pointer in jit **/
            auto llvm_addr = jit->lookup_symbol(fn_ast->name());

            log && log("execution session after lookup attempt:");
            jit->dump_execution_session();

            if (!llvm_addr) {
                cerr << "ex2: lookup: symbol not found"
                     << xtag("symbol", fn_ast->name())
                     << endl;
            } else {
                cerr << "ex2: lookup: symbol found"
                     << xtag("llvm_addr", llvm_addr.get().getValue())
                     << xtag("symbol", fn_ast->name())
                     << endl;
            }

            auto fn_ptr = llvm_addr.get().toPtr<ratio_type(*)(int,int)>();

            REQUIRE(fn_ptr);

            auto value = (*fn_ptr)(2, 3);

            log && log(xtag("value.num", value.num()),
                       xtag("value.den", value.den()));

        } /*TEST_CASE(machpipeline.struct)*/
    } /*namespace ut*/
} /*namespace xo*/


/* end MachPipeline.test.cpp */
