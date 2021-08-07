//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_BLOCK_ADDRESS_H
#define LLVM_NODE_CONSTANT_BLOCK_ADDRESS_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class ConstantBlockAddressWrapper: public ConstantWrapper, public FromValueMixin<ConstantBlockAddressWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::BlockAddress* constantBlockAddress);
    using FromValueMixin<ConstantBlockAddressWrapper>::FromValue;
    llvm::BlockAddress* getConstantBlockAddress();

private:
    explicit ConstantBlockAddressWrapper(llvm::BlockAddress* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantBlockAddressTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_CONSTANT_BLOCK_ADDRESS_H
