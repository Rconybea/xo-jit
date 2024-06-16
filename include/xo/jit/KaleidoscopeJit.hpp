//===- KaleidoscopeJIT.h - A simple JIT for Kaleidoscope --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Contains a simple JIT definition for use in the kaleidoscope tutorials.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>

namespace xo {
    namespace jit {

        class KaleidoscopeJIT {
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
            IRCompileLayer compile_layer_;

            JITDylib &MainJD;

        public:
            KaleidoscopeJIT(std::unique_ptr<ExecutionSession> xsession,
                            JITTargetMachineBuilder jtmb,
                            DataLayout data_layout_)
                : xsession_{std::move(xsession)},
                  data_layout_(std::move(data_layout_)),
                  mangler_(*this->xsession_, this->data_layout_),
                  object_layer_(*this->xsession_,
                                []() { return std::make_unique<SectionMemoryManager>(); }),
                  compile_layer_(*this->xsession_, object_layer_,
                                 std::make_unique<ConcurrentIRCompiler>(std::move(jtmb))),
                  MainJD(this->xsession_->createBareJITDylib("<main>"))
                {
                    MainJD.addGenerator(
                        cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(
                                     data_layout_.getGlobalPrefix())));
                    if (jtmb.getTargetTriple().isOSBinFormatCOFF()) {
                        object_layer_.setOverrideObjectFlagsWithResponsibilityFlags(true);
                        object_layer_.setAutoClaimResponsibilityForObjectSymbols(true);
                    }
                }

            ~KaleidoscopeJIT() {
                if (auto Err = this->xsession_->endSession())
                    this->xsession_->reportError(std::move(Err));
            }

            static llvm::Expected<std::unique_ptr<KaleidoscopeJIT>> Create() {
                auto EPC = SelfExecutorProcessControl::Create();
                if (!EPC)
                    return EPC.takeError();

                auto xsession = std::make_unique<ExecutionSession>(std::move(*EPC));

                JITTargetMachineBuilder jtmb
                    (xsession->getExecutorProcessControl().getTargetTriple());

                auto data_layout = jtmb.getDefaultDataLayoutForTarget();
                if (!data_layout)
                    return data_layout.takeError();

                return std::make_unique<KaleidoscopeJIT>(std::move(xsession),
                                                         std::move(jtmb),
                                                         std::move(*data_layout));
            }

            const DataLayout & data_layout() const { return data_layout_; }

            JITDylib &getMainJITDylib() { return MainJD; }

            llvm::Error
            addModule(ThreadSafeModule ts_module,
                      ResourceTrackerSP RT = nullptr) {
                if (!RT)
                    RT = MainJD.getDefaultResourceTracker();

                return compile_layer_.add(RT, std::move(ts_module));
            }

            llvm::Expected<ExecutorSymbolDef> lookup(StringRef name) {
                return this->xsession_->lookup({&MainJD},
                                               this->mangler_(name.str()));
            }

            /* dump */
            void dump_execution_session() {
                this->xsession_->dump(llvm::errs());
            }
        };

    } // end namespace jit
} // end namespace xo
