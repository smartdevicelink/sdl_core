#include "can_cooperation/can_module.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;

CANModule::CANModule()
  : GenericModule(kCANModuleID) {}

CANModule::~CANModule() {
}

ProcessResult CANModule::ProcessMessage(const Json::Value& msg) {
  return ProcessResult::CANNOT_PROCESS;
}
}  //  namespace can_cooperation