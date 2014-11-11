#include "can_cooperation/can_module.h"
#include "./can_tcp_connection.h"
#include "utils/logger.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;

CREATE_LOGGERPTR_GLOBAL(logger_, "CanModule");

CANModule::CANModule()
  : GenericModule(kCANModuleID)
  , can_connection() {
  	can_connection = new CANTCPConnection;
  	if (ConnectionState::OPENED != can_connection->OpenConnection()) {
  		LOG4CXX_ERROR(logger_, "Failed to connect to CAN");
  	}
plugin_info_.name = "ReverseSDLPlugin";
  plugin_info_.version = 1;
  plugin_info_.plugin = this;
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_RADIO);
 }

CANModule::~CANModule() {
  if (can_connection) {
		can_connection->CloseConnection();
	}
}

functional_modules::PluginInfo CANModule::GetPluginInfo() const {
  return plugin_info_;
}

ProcessResult CANModule::ProcessMessage(application_manager::MessagePtr msg) {
  //static_cast<CANTCPConnection*>(can_connection.get())->WriteData(msg);
  if (ConnectionState::OPENED != can_connection->Flash()) {
    LOG4CXX_ERROR(logger_, "Failed to send message to CAN");
    return ProcessResult::FAILED;
  }
  return ProcessResult::CANNOT_PROCESS;
}

void CANModule::RemoveAppExtensions() {

}

}  //  namespace can_cooperation

