#include <napi.h>
#include <string>
#include "greeting.h"

class DetectWorker : public Napi::AsyncWorker {
private:
    void* bin;                    // 输入图像数据
    uint32_t width;               // 图像宽度
    uint32_t height;              // 图像高度
    std::string outbuf;           // 输出缓冲区
    std::vector<char> binCopy;    // 输入数据的副本

public:
    DetectWorker(Napi::Function& callback, void* binData, size_t binSize,
        uint32_t w, uint32_t h)
        : Napi::AsyncWorker(callback), width(w), height(h) {
        // 在构造函数中复制输入数据
        binCopy.resize(binSize);
        std::memcpy(binCopy.data(), binData, binSize);
        bin = binCopy.data();

        // 预分配输出缓冲区
        outbuf.resize(2 * 1024 * 1024);
    }

    // 在工作线程中执行
    void Execute() override {
        // 调用原始的检测函数
        size_t len = detect(bin, width, height, outbuf.data(), outbuf.size(), false);
        outbuf.resize(len);
    }

    // 在主线程中处理结果
    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({ Env().Null(), Napi::String::New(Env(), outbuf) });
    }

    // 处理错误情况
    void OnError(const Napi::Error& e) override {
        Napi::HandleScope scope(Env());
        Callback().Call({ e.Value(), Env().Undefined() });
    }
};

// 定义一个返回类型为 Napi String 的 greetHello 函数, 注意此处的 info
Napi::Number nodeInitYolo(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	auto result = initYolo();
	return Napi::Number::New(env, result);
}

Napi::String nodeGetMachineCode(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	char result[32];
	get_machine_code(result);
	return Napi::String::New(env, std::string(result, result + 32));
}

Napi::String nodeYoloDetect(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	auto buffer = info[0].As<Napi::ArrayBuffer>();
	auto bin = buffer.Data();

	std::string outbuf;
	outbuf.resize(2 * 1024 * 1024);
	auto len = detect(bin, info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value(), outbuf.data(), outbuf.size(), false);
	outbuf.resize(len);
	return Napi::String::New(env, outbuf);
}

Napi::Value nodeYoloDetectAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // 参数检查
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Undefined();
    }

    // 获取输入参数
    auto buffer = info[0].As<Napi::ArrayBuffer>();
    auto width = info[1].As<Napi::Number>().Uint32Value();
    auto height = info[2].As<Napi::Number>().Uint32Value();
    auto callback = info[3].As<Napi::Function>();

    // 创建异步工作对象
    auto worker = new DetectWorker(
        callback,
        buffer.Data(),
        buffer.ByteLength(),
        width,
        height
    );

    // 启动异步操作
    worker->Queue();

    return env.Undefined();
}

// 设置类似于 exports = {key:value}的模块导出
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(
    Napi::String::New(env, "init"), // key
    Napi::Function::New(env, nodeInitYolo)  // value
  );

  exports.Set(
	  Napi::String::New(env, "getMachineCode"), // key
	  Napi::Function::New(env, nodeGetMachineCode)  // value
  );

  exports.Set("detectAsync",
      Napi::Function::New(env, nodeYoloDetectAsync));

  exports.Set("detect",
      Napi::Function::New(env, nodeYoloDetect));
  return exports;
}

NODE_API_MODULE(greet, Init)
