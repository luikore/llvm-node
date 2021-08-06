//
// Created by Micha Reiser on 13.03.17.
//

#include "indirect-br-inst.h"
#include "basic-block.h"

Nan::Persistent<v8::FunctionTemplate> IndirectBrInstWrapper::functionTemplate {};

NAN_MODULE_INIT(IndirectBrInstWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

    tpl->SetClassName(Nan::New("IndirectBrInst").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Nan::SetPrototypeMethod(tpl, "addDestination", IndirectBrInstWrapper::addDestination);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("IndirectBrInst").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

// v8::Local<v8::Object> IndirectBrInstWrapper::of(llvm::IndirectBrInst* inst) {
//     Nan::EscapableHandleScope escapeScope {};
//     auto constructorFunction = Nan::GetFunction(Nan::New(functionTemplate())).ToLocalChecked();
//     v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(inst) };
//     auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

//     return escapeScope.Escape(instance);
// }

bool IndirectBrInstWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}

llvm::IndirectBrInst* IndirectBrInstWrapper::getIndirectBrInst() {
    return static_cast<llvm::IndirectBrInst*>(getValue());
}

NAN_METHOD(IndirectBrInstWrapper::addDestination) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("addDestination needs to be called with: basicBlock: BasicBlock");
    }

    auto* inst = IndirectBrInstWrapper::FromValue(info.Holder())->getIndirectBrInst();
    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();

    inst->addDestination(basicBlock);
}
