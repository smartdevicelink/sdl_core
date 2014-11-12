#include "utils/threads/thread.h"
#include "can_cooperation/can_module.h"
#include "./can_tcp_connection.h"
#include "utils/logger.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;

CREATE_LOGGERPTR_GLOBAL(logger_, "CanModule");

class TCPClientDelegate : public threads::ThreadDelegate {
public:
  explicit TCPClientDelegate(CANModule* can_module);
  ~TCPClientDelegate();
  void threadMain();
  bool exitThreadMain();
private:
  CANModule* can_module_;
  bool stop_flag_;
};

TCPClientDelegate::TCPClientDelegate(CANModule* can_module)
: can_module_(can_module)
, stop_flag_(false) {
  DCHECK(can_module);
}

TCPClientDelegate::~TCPClientDelegate() {
  can_module_ = NULL;
  stop_flag_ = true;
}

void TCPClientDelegate::threadMain() {
  while(!stop_flag_) {
    while(ConnectionState::OPENED == 
        can_module_->can_connection->GetData()) {
      can_module_->from_can_.PostMessage(
        static_cast<CANTCPConnection*>(can_module_->can_connection.get())
          ->ReadData());
    }
  }
}

bool TCPClientDelegate::exitThreadMain() {
  stop_flag_ = true;
  return false;
}

CANModule::CANModule()
  : GenericModule(kCANModuleID)
  , can_connection()
  , from_can_("FromCan To Mobile", this)
  , from_mobile_("FromMobile To Can", this)
  , thread_(NULL) {
  	can_connection = new CANTCPConnection;
  	if (ConnectionState::OPENED != can_connection->OpenConnection()) {
  		LOG4CXX_ERROR(logger_, "Failed to connect to CAN");
  	}
  plugin_info_.name = "ReverseSDLPlugin";
  plugin_info_.version = 1;
  plugin_info_.plugin = this;
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_RADIO);
  thread_ = new threads::Thread("CANClientListener", new TCPClientDelegate(this));
  const size_t kStackSize = 16384;
  thread_->startWithOptions(threads::ThreadOptions(kStackSize));
 }

CANModule::~CANModule() {
  thread_->stop();
  if (can_connection) {
		can_connection->CloseConnection();
	}
  delete thread_;
}

functional_modules::PluginInfo CANModule::GetPluginInfo() const {
  return plugin_info_;
}



ProcessResult CANModule::ProcessMessage(application_manager::MessagePtr msg) {
  Json::Value can_msg;

  switch (msg->function_id()) {
    case MobileFunctionID::TUNE_RADIO: {
      can_msg["id"] = 1;
      can_msg["jsonrpc"] = "2.0";
      can_msg["method"] = "CAN.TuneRadio";
      can_msg["params"] = msg->json_message();

      msg->set_json_message(can_msg.asString());
      break;
    }
    default: {
      return ProcessResult::CANNOT_PROCESS;
    }
  }

  from_mobile_.PostMessage(msg);
  return ProcessResult::PROCESSED;
}

void CANModule::ProcessCANMessage(const MessageFromCAN& can_msg) {
  from_can_.PostMessage(can_msg);
}

void CANModule::Handle(const application_manager::MessagePtr message) {
  static_cast<CANTCPConnection*>(can_connection.get())->WriteData(
      message->json_message());

  if (ConnectionState::OPENED != can_connection->Flash()) {
    LOG4CXX_ERROR(logger_, "Failed to send message to CAN");
  }
}
  
void CANModule::Handle(const MessageFromCAN message) {
  // TODO(PV): process and send response/notification to mobile
}

void CANModule::RemoveAppExtensions() {

}

}  //  namespace can_cooperation

