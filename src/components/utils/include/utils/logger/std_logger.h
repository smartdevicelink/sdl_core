#pragma once
#include "utils/ilogger.h"

class STDLogger : public Logger<std::string, void> {
 public:
  STDLogger();

  void Init(void* unused = 0) override;
  void DeInit() override;
  void Enable() override;
  bool Enabled() override {
    return true;
  }
  void Disable() override;
  void Flush() override;
  void PushLog(const LogMessageImpl& log_message) override;

 private:
  bool enable_;
};
