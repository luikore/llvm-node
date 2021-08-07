//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-block-address.h"
#include "basic-block.h"

NAN_MODULE_INIT(ConstantBlockAddressWrapper::Init) {
    auto constantBlockAddress = Nan::GetFunction(Nan::New(constantBlockAddressTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("ConstantBlockAddress").ToLocalChecked(), constantBlockAddress);
}

NAN_METHOD(ConstantBlockAddressWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor ConstantBlockAddress cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantBlockAddress constructor needs to be called with: constantBlockAddress: external");
    }

    auto* constantBlockAddress = static_cast<llvm::BlockAddress*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantBlockAddressWrapper { constantBlockAddress };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantBlockAddressWrapper::get) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("get needs to be called with: basicBlock: BasicBlock");
    }

    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* constant = llvm::BlockAddress::get(basicBlock);

    info.GetReturnValue().Set(ConstantBlockAddressWrapper::of(constant));
}

llvm::BlockAddress *ConstantBlockAddressWrapper::getConstantBlockAddress() {
    return static_cast<llvm::BlockAddress*>(getValue());
}

v8::Local<v8::Object> ConstantBlockAddressWrapper::of(llvm::BlockAddress *constantBlockAddress) {
    auto constructorFunction = Nan::GetFunction(Nan::New(constantBlockAddressTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(constantBlockAddress) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& ConstantBlockAddressWrapper::constantBlockAddressTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(ConstantBlockAddressWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantBlockAddress").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", ConstantBlockAddressWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}