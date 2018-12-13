#include <napi.h>
#include <chrono>
#include <thread>

class MyWorker : public Napi::AsyncWorker {
 public:

  MyWorker(Napi::Function& callback, int points): Napi::AsyncWorker(callback) {
    result=points;
  }
  
  ~MyWorker() {}

  void Execute () {
  for(int i=0;i<5;i++){
    result++;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
  }

  void OnOK() {
    Napi::HandleScope scope(Env());
    Callback().Call({Env().Undefined(), Napi::Number::New(Env(), result)});
  }

 private:
  int result;
};

Napi::String IncrementAsync(const Napi::CallbackInfo& info) {
  int val = info[0].As<Napi::Number>().Uint32Value();
  Napi::Function callback = info[1].As<Napi::Function>();
  MyWorker* myWorker = new MyWorker(callback, val);
  myWorker->Queue();
  return Napi::String::New(info.Env(),"Called");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "IncrementAsync"), Napi::Function::New(env, IncrementAsync));
  return exports;
}

NODE_API_MODULE(async, Init)
