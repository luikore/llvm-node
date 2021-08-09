#include "indirect-br-inst.h"
#include "basic-block.h"
#include "nan.h"

NAN_MODULE_INIT(IndirectBrInstWrapper::Init) {
    auto indirectBrInst = Nan::GetFunction(Nan::New(indirectBrInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("IndirectBrInst").ToLocalChecked(), indirectBrInst);
}

NAN_METHOD(IndirectBrInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor IndirectBrInst cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("IndirectBrInst constructor needs to be called with: indirectBrInst: external");
    }

    auto* indirectBrInst = static_cast<llvm::IndirectBrInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new IndirectBrInstWrapper { indirectBrInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> IndirectBrInstWrapper::of(llvm::IndirectBrInst* inst) {
    Nan::EscapableHandleScope escapeScope {};
    auto constructorFunction = Nan::GetFunction(Nan::New(indirectBrInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(inst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    return escapeScope.Escape(instance);
}

bool IndirectBrInstWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(indirectBrInstTemplate())->HasInstance(value);
}

llvm::IndirectBrInst* IndirectBrInstWrapper::getIndirectBrInst() {
    return static_cast<llvm::IndirectBrInst*>(getValue());
}

NAN_METHOD(IndirectBrInstWrapper::addDestination) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("addDestination needs to be called with: basicBlock: BasicBlock");
    }

    auto* inst = IndirectBrInstWrapper::FromValue(info.Holder())->getIndirectBrInst();
    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();

    inst->addDestination(basicBlock);
}

Nan::Persistent<v8::FunctionTemplate>& IndirectBrInstWrapper::indirectBrInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(IndirectBrInstWrapper::New);
        localTemplate->SetClassName(Nan::New("IndirectBrInst").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(valueTemplate()));

        Nan::SetPrototypeMethod(localTemplate, "addDestination", IndirectBrInstWrapper::addDestination);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}