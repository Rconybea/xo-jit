/** @file VirtualSchematikaMachine.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "VirtualSchematikaMachine.hpp"
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/expression2/DConstant.hpp>
#include <xo/gc/DX1Collector.hpp>
#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <cassert>

namespace xo {
    using xo::print::APrintable;
    using xo::print::ppconfig;
    using xo::print::ppstate_standalone;
    using xo::mm::AGCObject;
    using xo::mm::MemorySizeInfo;
    using xo::mm::DX1Collector;
    using xo::facet::FacetRegistry;
    using std::cout;

    namespace scm {

        VirtualSchematikaMachine::VirtualSchematikaMachine(const VsmConfig & config)
        : config_{config},
          mm_(box<AAllocator,DX1Collector>(new DX1Collector(config.x1_config_))),
          reader_{config.rdr_config_, mm_.to_op()}
        {}

        std::size_t
        VirtualSchematikaMachine::_n_store() const noexcept
        {
            // oops. need something that goes through AAllocator api

            return reader_._n_store();
        }

        MemorySizeInfo
        VirtualSchematikaMachine::_store_info(std::size_t i) const noexcept
        {
            // oops. need something poly that goes through AAllocator api

            return reader_._store_info(i);
        }

        void
        VirtualSchematikaMachine::begin_interactive_session()
        {
            reader_.begin_interactive_session();
        }

        void
        VirtualSchematikaMachine::begin_batch_session()
        {
            reader_.begin_batch_session();
        }

        VsmResultExt
        VirtualSchematikaMachine::read_eval_print(span_type input, bool eof)
        {
            if (input.empty()) {
                return VsmResultExt();
            }

            auto [expr, remaining, error1]
                = reader_.read_expr(input, eof);

            if (!expr) {
                /* tokenizer error */

                return VsmResultExt(VsmResult(error1), remaining);
            }

            VsmResult evalresult = this->start_eval(expr);

            if (evalresult.is_eval_error() || evalresult.is_tk_error()) {
                return VsmResultExt(evalresult, remaining);
            }

            assert(evalresult.is_value());

            obj<AGCObject> * p_value = std::get_if<obj<AGCObject>>(&(evalresult.result_));

            assert(p_value);

            obj<APrintable> value_pr
                = FacetRegistry::instance().variant<APrintable,AGCObject>(*p_value);

            // pretty_toplevel(value_pr, &cout, ppconfig());
            ppconfig ppc;
            ppstate_standalone pps(&cout, 0, &ppc);
            pps.prettyn(value_pr);

            return VsmResultExt(VsmResult(*p_value), remaining);
        }

        VsmResult
        VirtualSchematikaMachine::start_eval(obj<AExpression> expr)
        {
            this->pc_ = VsmInstr::c_eval;
            this->expr_ = expr;
            this->value_ = obj<AGCObject>();
            this->cont_ = VsmInstr::c_halt;

            this->run();

            return value_;
        }

        void
        VirtualSchematikaMachine::run()
        {
            while (this->execute_one())
                ;
        }

        bool
        VirtualSchematikaMachine::execute_one()
        {
            switch (pc_.opcode()) {
            case vsm_opcode::halt:
            case vsm_opcode::N:
                return false;
            case vsm_opcode::eval:
                _do_eval_op();
            }

            return true;
        }

        void
        VirtualSchematikaMachine::_do_eval_op()
        {
            switch(expr_.extype()) {
            case exprtype::invalid:
            case exprtype::N:
                break;
            case exprtype::constant:
                _do_eval_constant_op();
                break;
            case exprtype::define:
                _do_eval_define_op();
                break;
            case exprtype::lambda:
                _do_eval_lambda_op();
                break;
            case exprtype::variable:
                _do_eval_variable_op();
                break;
            case exprtype::apply:
                _do_eval_apply_op();
                break;
            case exprtype::ifexpr:
                _do_eval_if_else_op();
                break;
            case exprtype::sequence:
                _do_eval_sequence_op();
                break;
            }
        }

        void
        VirtualSchematikaMachine::_do_eval_constant_op()
        {
            auto expr
                = obj<AExpression,DConstant>::from(expr_);

            this->value_ = expr.data()->value();
            this->pc_ = this->cont_;
        }

        void
        VirtualSchematikaMachine::_do_eval_define_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_lambda_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_variable_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_apply_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_if_else_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_sequence_op()
        {
            // not implemented
            assert(false);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
