#pragma once
#include "utils/ilogger.h"

#include <assert.h>
#include <functional>
#include <iostream>

#include "utils/logger_status.h"
#include "utils/threads/message_loop_thread.h"

namespace logger {

typedef std::queue<SDLLogMessage> LogMessageQueue;

typedef threads::MessageLoopThread<LogMessageQueue>
    LogMessageLoopThreadTemplate;

template <typename T>
using LoggerPtr = std::unique_ptr<T, std::function<void(T*)> >;

class LogMessageLoopThread : public LogMessageLoopThreadTemplate,
                             public LogMessageLoopThreadTemplate::Handler {
 public:
  LogMessageLoopThread(std::function<void(SDLLogMessage)> handler)
      : LogMessageLoopThreadTemplate("Logger", this), blocking_call_(handler) {}

  void Push(const SDLLogMessage& message) {
    this->PostMessage(message);
  }

  void Handle(const SDLLogMessage message) {
    blocking_call_(message);
  }

  ~LogMessageLoopThread() {
    // we'll have to drop messages
    // while deleting logger thread
    logger::logger_status = logger::DeletingLoggerThread;
    LogMessageLoopThreadTemplate::Shutdown();
  }

 private:
  std::function<void(SDLLogMessage)> blocking_call_;
};

template <class ThirdPartyLogger>
class LoggerImp : public Logger<ThirdPartyLogger> {
 public:
  LoggerImp() : impl_(nullptr), logs_enabled_(false) {}

  void Init(ThirdPartyLogger* impl) override {
    assert(impl_ == nullptr);
    impl_ = impl;
    impl_->Init();

    auto deinit_logger = [](LogMessageLoopThread* logMsgThread) {
      delete logMsgThread;
      logger::logger_status = logger::LoggerThreadNotCreated;
    };

    if (!loop_thread_) {
      loop_thread_ = LoggerPtr<LogMessageLoopThread>(
          new LogMessageLoopThread(
              [this](SDLLogMessage message) { impl_->PushLog(message); }),
          deinit_logger);
    }
    logger::logger_status = logger::LoggerThreadCreated;
  }

  void DeInit() override {
    Disable();
    if (logger::logger_status == logger::LoggerThreadCreated)
      Flush();
    loop_thread_.reset();
    impl_->DeInit();
    logger::logger_status = logger::LoggerThreadNotCreated;
  }

  void Enable() {
    logs_enabled_ = true;
  }

  bool Enabled() {
    return logs_enabled_;
  }

  void Disable() {
    logs_enabled_ = false;
  }

  void Flush() override {
    logger::LoggerStatus old_status = logger::logger_status;
    logger::logger_status = logger::DeletingLoggerThread;
    loop_thread_->WaitDumpQueue();
    logger::logger_status = old_status;
  }

  bool IsEnabledFor(const std::string& logger, LogLevel log_level) override {
    return impl_->IsEnabledFor(logger, log_level);
  }

  void PushLog(const SDLLogMessage& log_message) override {
    if (loop_thread_)
      loop_thread_->Push(log_message);
    // Optional to use blocking call :
    // impl_->PushLog(log_message);
  }

  ThirdPartyLogger* impl_;
  LoggerPtr<LogMessageLoopThread> loop_thread_;
  std::atomic_bool logs_enabled_;
};

template <typename ThirdPartyLogger>
Logger<ThirdPartyLogger>& Logger<ThirdPartyLogger>::instance() {
  static LoggerImp<ThirdPartyLogger> inst;
  return inst;
}

}  // namespace logger
