#pragma once
#include "utils/ilogger.h"

#include <assert.h>
#include <functional>
#include <iostream>

#include "utils/threads/message_loop_thread.h"

typedef std::queue<LogMessage<LocationInfo>> LogMessageQueue;

typedef threads::MessageLoopThread<LogMessageQueue>
    LogMessageLoopThreadTemplate;

template <typename T>
class LogMessageLoopThread : public LogMessageLoopThreadTemplate,
                             public LogMessageLoopThreadTemplate::Handler {
 public:
  LogMessageLoopThread(std::function<void(T)> handler)
      : LogMessageLoopThreadTemplate("Logger", this),
        blocking_call_(handler) {}

  void Push(const T& message) {
    this->PostMessage(message);
  }

  void Handle(const T message) {
    blocking_call_(message);
  }

  void setHandler(const std::function<void(T)>& force_log) {
    blocking_call_ = force_log;
  }

  ~LogMessageLoopThread() {
    // we'll have to drop messages
    // while deleting logger thread
    //logger_status = DeletingLoggerThread;
    LogMessageLoopThreadTemplate::Shutdown();
  }

 private:
  std::function<void(T)> blocking_call_;
};

template <typename LocationInfo, class ThirdPartyLogger>
class LoggerImplementation : public Logger<LocationInfo, ThirdPartyLogger> {
  typedef LogMessage<LocationInfo> LogMessageImpl;

 public:
  LoggerImplementation()
      : impl_(nullptr), loop_thread_([this](LogMessageImpl log_message) {
        std::cerr << "logger not initialized" << std::endl;
      }) {}

  void Init(ThirdPartyLogger* impl) override {
    assert(impl_ == nullptr);
    impl_ = impl;
    impl_->Init();
    loop_thread_.setHandler(
        [this](LogMessageImpl message) { impl_->PushLog(message); });
  }
  void DeInit() override {
    impl_->DeInit();
  }
  void Enable() {
    impl_->Enable();
  }

  bool Enabled() override {
    return impl_->Enabled();
    ;
  }

  void Disable() override {
    impl_->Disable();
  }
  void Flush() override {
    impl_->Flush();
  }

  void PushLog(const LogMessageImpl& log_message) override {
    loop_thread_.Push(log_message);
    // Optional to use blocking call :
    //    impl_->Log(log_message);
  }

  ThirdPartyLogger* impl_;
  LogMessageLoopThread<LogMessageImpl> loop_thread_;
};

template <typename LocationInfo, typename ThirdPartyLogger>
Logger<LocationInfo, ThirdPartyLogger>&
Logger<LocationInfo, ThirdPartyLogger>::instance() {
  static LoggerImplementation<LocationInfo, ThirdPartyLogger> inst;
  return inst;
}
