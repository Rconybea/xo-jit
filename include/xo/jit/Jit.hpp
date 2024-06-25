/** @file Jit.hpp **/

/** Adapted from LLVM KaleidoscopeJIT.h **/

#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
# include "llvm/ADT/StringRef.h"
# include "llvm/ExecutionEngine/JITSymbol.h"
# include "llvm/ExecutionEngine/Orc/CompileUtils.h"
# include "llvm/ExecutionEngine/Orc/Core.h"
# include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
# include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
# include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
# include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
# include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
# include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
# include "llvm/ExecutionEngine/SectionMemoryManager.h"
# include "llvm/IR/DataLayout.h"
# include "llvm/IR/LLVMContext.h"
#pragma GCC diagnostic pop
#include <memory>

namespace xo {
    namespace jit {

        class Jit {
        private:
            using StringRef = llvm::StringRef;
            using SectionMemoryManager = llvm::SectionMemoryManager;
            using DynamicLibrarySearchGenerator = llvm::orc::DynamicLibrarySearchGenerator;
            using ConcurrentIRCompiler = llvm::orc::ConcurrentIRCompiler;
            using ExecutionSession = llvm::orc::ExecutionSession;
            using DataLayout = llvm::DataLayout;
            using MangleAndInterner = llvm::orc::MangleAndInterner;
            using RTDyldObjectLinkingLayer = llvm::orc::RTDyldObjectLinkingLayer;
            using IRCompileLayer = llvm::orc::IRCompileLayer;
            using JITDylib = llvm::orc::JITDylib;
            using JITTargetMachineBuilder = llvm::orc::JITTargetMachineBuilder;
            using ThreadSafeModule = llvm::orc::ThreadSafeModule;
            using ResourceTrackerSP = llvm::orc::ResourceTrackerSP;
            using ExecutorSymbolDef = llvm::orc::ExecutorSymbolDef;
            using SelfExecutorProcessControl = llvm::orc::SelfExecutorProcessControl;

        private:
            /** execution session - represents a currently-running jit program **/
            std::unique_ptr<ExecutionSession> xsession_;

            /** (?) needed for name mangling (?) **/
            DataLayout data_layout_;
            /** symbol mangling and unique-ifying */
            MangleAndInterner mangler_;

            /** in-process linking layer
             *  (? specialized for jit in running process ?)
             **/
            RTDyldObjectLinkingLayer object_layer_;
            /** compilation layer (sits above linking layer) **/
            IRCompileLayer compile_layer_;

            /** destination library **/
            JITDylib & dest_dynamic_lib_; //MainJD;

        public:
            Jit(std::unique_ptr<ExecutionSession> xsession,
                JITTargetMachineBuilder jtmb,
                DataLayout data_layout)
                : xsession_{std::move(xsession)},
                  data_layout_(std::move(data_layout)),
                  mangler_(*this->xsession_, this->data_layout_),
                  object_layer_(*this->xsession_,
                                []() { return std::make_unique<SectionMemoryManager>(); }),
                  compile_layer_(*this->xsession_, object_layer_,
                                 std::make_unique<ConcurrentIRCompiler>(std::move(jtmb))),
                  dest_dynamic_lib_(this->xsession_->createBareJITDylib("<main>"))
                {
                    dest_dynamic_lib_.addGenerator
                        (cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess
                                  (data_layout_.getGlobalPrefix())));

                    if (jtmb.getTargetTriple().isOSBinFormatCOFF()) {
                        object_layer_.setOverrideObjectFlagsWithResponsibilityFlags(true);
                        object_layer_.setAutoClaimResponsibilityForObjectSymbols(true);
                    }
                }

            ~Jit() {
                if (auto Err = this->xsession_->endSession())
                    this->xsession_->reportError(std::move(Err));
            }

            static llvm::Expected<std::unique_ptr<Jit>> Create() {
                auto EPC = SelfExecutorProcessControl::Create();
                if (!EPC)
                    return EPC.takeError();

                auto xsession = std::make_unique<ExecutionSession>(std::move(*EPC));

                JITTargetMachineBuilder jtmb
                    (xsession->getExecutorProcessControl().getTargetTriple());

                auto data_layout = jtmb.getDefaultDataLayoutForTarget();
                if (!data_layout)
                    return data_layout.takeError();

                return std::make_unique<Jit>(std::move(xsession),
                                             std::move(jtmb),
                                             std::move(*data_layout));
            }

            /* exposing this for printing */
            const ExecutionSession * xsession() const { return xsession_.get(); }
            const DataLayout & data_layout() const { return data_layout_; }

            JITDylib & dest_dynamic_lib_ref() { return dest_dynamic_lib_; }
            const std::string & target_triple() const {
                return xsession_->getTargetTriple().getTriple();
            }


            /** compile module to machine code that's runnable from this process;
             *  incorporate into @ref dest_dynamic_lib_
             **/
            llvm::Error
            add_llvm_module(ThreadSafeModule ts_module,
                            ResourceTrackerSP rtracker = nullptr) {
                if (!rtracker)
                    rtracker = dest_dynamic_lib_.getDefaultResourceTracker();

                return compile_layer_.add(rtracker,
                                          std::move(ts_module));
            }

            /** intern @p symbol, binding it to address @p dest **/
            template <typename T>
            llvm::Error intern_symbol(const std::string & symbol, T * dest) {
                auto mangled_sym = mangler_(symbol);

                llvm::orc::SymbolMap symbol_map;
                symbol_map[mangled_sym]
                    = llvm::orc::ExecutorSymbolDef(llvm::orc::ExecutorAddr::fromPtr(dest),
                                                   llvm::JITSymbolFlags());

                auto materializer = llvm::orc::absoluteSymbols(symbol_map);

                return dest_dynamic_lib_.define(materializer);
            } /*intern_symbol*/

            /** report mangled symbol name **/
            std::string_view mangle(StringRef name) {
                auto tmp = *(this->mangler_(name.str()));

                return std::string_view(tmp.data(), tmp.size());
            }

            llvm::Expected<ExecutorSymbolDef> lookup(StringRef name) {
                return this->xsession_->lookup({&dest_dynamic_lib_},
                                               this->mangle(name));
            }

            /* dump */
            void dump_execution_session() {
                this->xsession_->dump(llvm::errs());
            }
        }; /*Jit*/

    } /*namespace jit*/
} /*namespace xo*/

/** end Jit.hpp **/
