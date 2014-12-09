#include "can_cooperation/can_module.h"
#include "can_cooperation/mobile_command_factory.h"
#include "./can_tcp_connection.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;
namespace hmi_api = functional_modules::hmi_api;

EXPORT_FUNCTION_IMPL(CANModule);
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

uint32_t CANModule::next_correlation_id_ = 1;

CANModule::CANModule()
  : GenericModule(kCANModuleID)
  , can_connection()
  , from_can_("FromCan To Mobile", this)
  , from_mobile_("FromMobile To Can", this)
  , thread_(NULL)
  , is_scan_started_(false) {
  	can_connection = new CANTCPConnection;
  	if (ConnectionState::OPENED != can_connection->OpenConnection()) {
  		LOG4CXX_ERROR(logger_, "Failed to connect to CAN");
  	} else {
      thread_ = new threads::Thread("CANClientListener", new TCPClientDelegate(this));
      const size_t kStackSize = 16384;
      thread_->startWithOptions(threads::ThreadOptions(kStackSize));
    }
  plugin_info_.name = "ReverseSDLPlugin";
  plugin_info_.version = 1;
  SubscribeOnFunctions();
}

void CANModule::SubscribeOnFunctions() {
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_RADIO);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_UP);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_DOWN);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::GRANT_ACCESS);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::CANCEL_ACCESS);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::START_SCAN);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::STOP_SCAN);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::ON_CONTROL_CHANGED);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::ON_RADIO_DETAILS);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::ON_PRESET_CHANGED);

  plugin_info_.hmi_function_list.push_back(hmi_api::grant_access);
  plugin_info_.hmi_function_list.push_back(hmi_api::cancel_access);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_control_changed);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_preset_changed);
}

CANModule::~CANModule() {
  if (can_connection) {
    can_connection->CloseConnection();
  }
  if (thread_ ) {
    thread_->stop();
    delete thread_;
  }
  RemoveAppExtensions();
}

functional_modules::PluginInfo CANModule::GetPluginInfo() const {
  return plugin_info_;
}

ProcessResult CANModule::ProcessMessage(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message received.");
    return ProcessResult::FAILED;
  }

  commands::Command* command = MobileCommandFactory::CreateCommand(msg);
  if (command) {
    request_controller_.AddRequest(msg->correlation_id(), command);
    command->Run();
  }

/*  std::string msg_to_send;

  switch (msg->function_id()) {
    case MobileFunctionID::TUNE_RADIO: {
      Json::Value can_msg;

      HMIResponseSubscriberInfo subscriber_info;
      subscriber_info.connection_key_ = msg->connection_key();
      subscriber_info.correlation_id_ = msg->correlation_id();
      subscriber_info.function_id_ = msg->function_id();

      hmi_response_subscribers_[next_correlation_id_] = subscriber_info;

      can_msg["id"] = next_correlation_id_++;
      can_msg["jsonrpc"] = "2.0";
      can_msg["method"] = "CAN.TuneRadio";
      can_msg["params"] = msg->json_message();

      Json::FastWriter writer;
      msg_to_send = writer.write(can_msg);
      break;
    }
    default: {
      return ProcessResult::CANNOT_PROCESS;
    }
  }

  from_mobile_.PostMessage(msg_to_send);*/
  return ProcessResult::PROCESSED;
}

ProcessResult CANModule::ProcessHMIMessage(application_manager::MessagePtr msg) {

  return ProcessResult::PROCESSED;
}

void CANModule::ProcessCANMessage(const MessageFromCAN& can_msg) {
  DCHECK(Json::ValueType::objectValue == can_msg.type());
  from_can_.PostMessage(can_msg);
}

void CANModule::Handle(const std::string message) {
  static_cast<CANTCPConnection*>(can_connection.get())->WriteData(
      message);

  if (ConnectionState::OPENED != can_connection->Flash()) {
    LOG4CXX_ERROR(logger_, "Failed to send message to CAN");
  }
}
  
void CANModule::Handle(const MessageFromCAN can_msg) {
  if (can_msg.isMember("id")) {
    std::map<uint32_t, HMIResponseSubscriberInfo>::iterator it =
        hmi_response_subscribers_.find(can_msg["id"].asInt());
    if (it != hmi_response_subscribers_.end()) {
      application_manager::MessagePtr msg(new application_manager::Message(
              protocol_handler::MessagePriority::kDefault));
      msg->set_connection_key(it->second.connection_key_);
      msg->set_correlation_id(it->second.correlation_id_);
      msg->set_function_id(it->second.function_id_);
      msg->set_protocol_version(application_manager::ProtocolVersion::kV3);
      msg->set_message_type(application_manager::MessageType::kResponse);

      Json::Value msg_params;

      if ((can_msg.isMember("result")) &&
          (can_msg["result"].isMember("code")) &&
          (0 == can_msg["result"]["code"].asInt())) {

        msg_params["success"] = true;
        msg_params["resultCode"] = "SUCCESS";
      } else {
        msg_params["success"] = false;
        msg_params["resultCode"] = "GENERIC_ERROR";
      }

      Json::FastWriter writer;
      std::string msg_to_send = writer.write(msg_params);
      msg->set_json_message(msg_to_send);
      service_->SendMessageToMobile(msg);
    }
  }
}

void CANModule::SendResponseToMobile(application_manager::MessagePtr msg) {
  service_->SendMessageToMobile(msg);
  request_controller_.DeleteRequest(msg->correlation_id());
}

bool CANModule::IsScanStarted() const {
  return is_scan_started_;
}

void CANModule::SetScanStarted(bool is_scan_started) {
  is_scan_started_ = is_scan_started;
}

void CANModule::RemoveAppExtensions() {

}

void CANModule::RemoveAppExtension(uint32_t app_id) {

}

}  //  namespace can_cooperation

