#include "can_cooperation/can_module.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;

CANModule::CANModule()
  : GenericModule(kCANModuleID) {
  plugin_info_.name = "ReverseSDLPlugin";
  plugin_info_.version = 1;
  plugin_info_.plugin = this;
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_RADIO);
}

CANModule::~CANModule() {
}

functional_modules::PluginInfo CANModule::GetPluginInfo() const {
  return plugin_info_;
}

ProcessResult CANModule::ProcessMessage(application_manager::MessagePtr msg) {
  return ProcessResult::CANNOT_PROCESS;
}

void CANModule::RemoveAppExtensions() {

}

}  //  namespace can_cooperation

