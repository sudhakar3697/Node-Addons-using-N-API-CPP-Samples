#include <napi.h>
#include <chrono>
#include <thread>

Napi::Number IncrementSync(const Napi::CallbackInfo& info) {
  int val = info[0].As<Napi::Number>().Uint32Value();
  for(int i=0;i<5;i++){
    val++;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
  return Napi::Number::New(info.Env(),val);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "IncrementSync"), Napi::Function::New(env, IncrementSync));
  return exports;
}

NODE_API_MODULE(sync, Init)
