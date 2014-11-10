#include "can_cooperation/can_module.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;

CANModule::CANModule()
  : GenericModule(kCANModuleID) {}

CANModule::~CANModule() {
}

PluginInfo CANModule::GetPluginInfo() const {
	PluginInfo info;
	info.name = "ReverseSDLPlugin";
	info.version = 1;
	info.id = kCANModuleID;
	return info;
}

ProcessResult CANModule::ProcessMessage(const Json::Value& msg) {
  return ProcessResult::CANNOT_PROCESS;
}

void CANModule::RemoveAppExtensions() {

}

}  //  namespace can_cooperation