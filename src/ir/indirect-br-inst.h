//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_INDIRECT_BR_INST_H
#define LLVM_NODE_INDIRECT_BR_INST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class IndirectBrInstWrapper: public ValueWrapper, public FromValueMixin<IndirectBrInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    // static v8::Local<v8::Object> of(llvm::IndirectBrInst* indirectBrInst);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<IndirectBrInstWrapper>::FromValue;
    llvm::IndirectBrInst* getIndirectBrInst();

private:
    static NAN_METHOD(New);
    static NAN_METHOD(addDestination);
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;

    explicit IndirectBrInstWrapper(llvm::IndirectBrInst* value) : ValueWrapper { value } {}
};

#endif //LLVM_NODE_INDIRECT_BR_INST_H
