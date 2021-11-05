#include "application_manager/interrupt_manager.h"

#include "application_manager/rpc_handler_impl.h"
#include "config_profile/profile.h"
#include "utils/logger.h"

SDL_CREATE_LOG_VARIABLE("InterruptManager")
namespace application_manager {
namespace interrupt_manager {
InterruptManager::InterruptManager(
    ApplicationManager& app_manager,
    const policy::PolicySettings& policy_settings)
    : app_manager_(app_manager), policy_settings_(policy_settings) {
  SDL_LOG_AUTO_TRACE();
  highest_priority_ons_rpc_ = 0;
  reject_rpc_list_.clear();
}

InterruptManager::~InterruptManager() {
  SDL_LOG_AUTO_TRACE();
}

void InterruptManager::Init() {
  ReadRpcPriorityTable();
  ReadAppTypePriorityTable();
  ReadHmiStatusPriorityTable();
  ReadInterruptSetting();
}

IMStatus InterruptManager::GetRunningStatus() {
  SDL_LOG_AUTO_TRACE();
  return im_status_;
}

InterruptCheckResult InterruptManager::CheckRpcInterrupt(
    const std::shared_ptr<Message> outgoing_message) {
  SDL_LOG_AUTO_TRACE();
  InterruptCheckResult check_result = NO_INTERRUPT;
  if (IsSubjectMessageToPrioritized(outgoing_message)) {
    if (highest_priority_ons_rpc_) {
      SDL_LOG_INFO("check start");
      SDL_LOG_INFO("highest_priority_ons_rpc_" << highest_priority_ons_rpc_->function_id());
      namespace formatters = ns_smart_device_link::ns_json_handler::formatters;
      formatters::FormatterJsonRpc::FromString<mobile_apis::FunctionID::eType,
                                               mobile_apis::messageType::eType>(outgoing_message->json_message(), message_);
      
      int32_t connection_key = outgoing_message->connection_key();
      SDL_LOG_INFO("connection_key " << connection_key);

      app_id = (message_)[strings::msg_params][strings::app_id].asInt();
      SDL_LOG_INFO("app_id " << app_id);
      app = app_manager_.application(connection_key);
      
      std::string policy_app_id = app->policy_app_id();
      SDL_LOG_INFO("policy_app_id " << policy_app_id);
      
      policy::PolicyHandlerInterface& policy_handler_ = app_manager_.GetPolicyHandler();
      SDL_LOG_INFO("get policy_handler_");

      policy_handler_.GetPriority(policy_app_id, &appPriority);
      SDL_LOG_INFO("get appPriority " << appPriority);

      formatters::FormatterJsonRpc::FromString<mobile_apis::FunctionID::eType,
                                               mobile_apis::messageType::eType>(highest_priority_ons_rpc_->json_message(), first_message_);
      first_app_id = (first_message_)[strings::params][strings::connection_key].asInt();

      int32_t first_connection_key = highest_priority_ons_rpc_->connection_key();
      SDL_LOG_INFO("first_connection_key " << first_connection_key);
      first_app = app_manager_.application(first_connection_key);
      app_manager_.GetPolicyHandler().GetPriority(first_app->policy_app_id(), &firstAppPriority);
      SDL_LOG_INFO("get firstAppPriority " << firstAppPriority);
      
      if ((appPriority == "EMERGENCY" && firstAppPriority == "EMERGENCY") ||
          (appPriority != "EMERGENCY" && firstAppPriority != "EMERGENCY")) {
        check_result = CheckRpcPriority(outgoing_message);
      } else if (firstAppPriority == "EMERGENCY") {
        SDL_LOG_INFO("reject receved ons");
        reject_rpc_list_.push_front(outgoing_message->function_id());
        check_result = REJECT_RECEIVED_ONS;
      } else {
        SDL_LOG_INFO("reject in queue");
        reject_rpc_list_.push_front(outgoing_message->function_id());
        check_result = REJECT_ONS_IN_QUEUE;
      }
    } else {
      SDL_LOG_INFO("no thread message");
      highest_priority_ons_rpc_ = outgoing_message;
      check_result = NO_INTERRUPT;
    }
  } else {
    SDL_LOG_INFO("not subjectMessage");
    check_result = NO_INTERRUPT;
  }
  SDL_LOG_INFO("check_result" << check_result);
  return check_result;
}

bool InterruptManager::DoRpcReject(mobile_apis::FunctionID::eType function_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_INFO("function_id" << function_id);
  SDL_LOG_INFO("reject_rpc_list_" << reject_rpc_list_.front());
  auto reject_itr =
      find(reject_rpc_list_.begin(), reject_rpc_list_.end(), function_id);
  if (reject_itr != reject_rpc_list_.end()) {
    SDL_LOG_INFO("reject start");
    reject_rpc_list_.erase(reject_itr);
    SDL_LOG_INFO("reject_rpc_list_" << reject_rpc_list_.front());
    
    highest_priority_ons_rpc_ = 0;
    return true;
  }
  SDL_LOG_INFO("no rejecte message");
  return false;
}

bool InterruptManager::IsSubjectMessageToPrioritized(
    const std::shared_ptr<Message> outgoing_message) {
  SDL_LOG_AUTO_TRACE();
  function_id = outgoing_message->function_id();
  SDL_LOG_INFO("outgoing_message function_id " << function_id);
  
  function_name = MessageHelper::StringifiedFunctionID(
      static_cast<mobile_apis::FunctionID::eType>(function_id));
  SDL_LOG_INFO("outgoing_message function_name " << function_name);

  if (rpc_priority_table_.find(function_name) != rpc_priority_table_.end() ||
      function_id == mobile_apis::FunctionID::SubtleAlertID) {
    if (outgoing_message->type() == kRequest) {
      SDL_LOG_INFO("subjectMessage");
      return true;
    } else {
      SDL_LOG_INFO("Message is not request");
      return false;
    }
  } else {
    return false;
  }
}

InterruptCheckResult InterruptManager::CheckRpcPriority(
    const std::shared_ptr<Message> outgoing_message) {
  SDL_LOG_AUTO_TRACE();
  InterruptCheckResult check_result = NO_INTERRUPT;

  function_id = outgoing_message->function_id();
  function_name = MessageHelper::StringifiedFunctionID(
      static_cast<mobile_apis::FunctionID::eType>(function_id));

  first_function_id = highest_priority_ons_rpc_->function_id();
  first_function_name = MessageHelper::StringifiedFunctionID(
      static_cast<mobile_apis::FunctionID::eType>(first_function_id));

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (function_name == "SubtleAlert") {
    if ((message_)[strings::msg_params].keyExists(strings::alert_text1) ||
        (message_)[strings::msg_params].keyExists(strings::alert_text1)) {
      function_name = "Alert";
    } else {
      function_name = "Speak";
    }
  }

  if (first_function_name == "SubtleAlert") {
    if ((first_message_)[strings::msg_params].keyExists(strings::alert_text1) ||
        (first_message_)[strings::msg_params].keyExists(strings::alert_text1)) {
      first_function_name = "Alert";
    } else {
      first_function_name = "Speak";
    }
  }

  SDL_LOG_INFO("rpc check start");
  SDL_LOG_INFO("check" << function_name << first_function_name);
  if (rpc_priority_table_[function_name] >
      rpc_priority_table_[first_function_name]) {
    reject_rpc_list_.push_front(outgoing_message->function_id());
    check_result = REJECT_RECEIVED_ONS;
    SDL_LOG_INFO("reject received message");
    SDL_LOG_INFO("reject message" << outgoing_message->function_id());
    SDL_LOG_INFO("reject list" << reject_rpc_list_.front());
    
  } else if (rpc_priority_table_[function_name] <
             rpc_priority_table_[first_function_name]) {
    SDL_LOG_INFO("reject queue message");
    SDL_LOG_INFO("reject message" << highest_priority_ons_rpc_->function_id());
    reject_rpc_list_.push_front(highest_priority_ons_rpc_->function_id());
    check_result = REJECT_ONS_IN_QUEUE;    
    SDL_LOG_INFO("reject list" << reject_rpc_list_.front());
  } else if (rpc_priority_table_[function_name] ==
             rpc_priority_table_[first_function_name]) {
    check_result = CheckAppTypePriority(outgoing_message);
  } else {
    check_result = INTERRUPT_ERROR;
  }
  return check_result;
}

InterruptCheckResult InterruptManager::CheckAppTypePriority(
    const std::shared_ptr<Message> outgoing_message) {
  SDL_LOG_AUTO_TRACE();
  InterruptCheckResult check_result = NO_INTERRUPT;
  if (apptype_priority_table_[appPriority] >
      apptype_priority_table_[firstAppPriority]) {
    reject_rpc_list_.push_front(outgoing_message->function_id());
    check_result = REJECT_RECEIVED_ONS;
  } else if (apptype_priority_table_[appPriority] <
             apptype_priority_table_[firstAppPriority]) {
    reject_rpc_list_.push_front(highest_priority_ons_rpc_->function_id());
    check_result = REJECT_ONS_IN_QUEUE;
  } else if (apptype_priority_table_[appPriority] ==
             apptype_priority_table_[firstAppPriority]) {
    check_result = CheckHmiStatusPriority(outgoing_message);
  } else {
    check_result = INTERRUPT_ERROR;
  }
  return check_result;
}

InterruptCheckResult InterruptManager::CheckHmiStatusPriority(
    const std::shared_ptr<Message> outgoing_message) {
  SDL_LOG_AUTO_TRACE();
  InterruptCheckResult check_result = NO_INTERRUPT;

  app_hmi_level =
      app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  first_app_hmi_level =
      first_app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);

  if (hmists_priority_table_[app_hmi_level] >
      hmists_priority_table_[first_app_hmi_level]) {
    reject_rpc_list_.push_front(outgoing_message->function_id());
    check_result = REJECT_RECEIVED_ONS;
  } else if ((hmists_priority_table_[app_hmi_level] <
              hmists_priority_table_[first_app_hmi_level]) ||
             (hmists_priority_table_[app_hmi_level] ==
              hmists_priority_table_[first_app_hmi_level])) {

    reject_rpc_list_.push_front(highest_priority_ons_rpc_->function_id());
    check_result = REJECT_ONS_IN_QUEUE;
  } else {
    check_result = INTERRUPT_ERROR;
  }
  return check_result;
}

bool InterruptManager::ReadRpcPriorityTable() {
  SDL_LOG_AUTO_TRACE();
  rpc_priority_table_ = app_manager_.GetPolicyHandler().GetRpcPriority();
  if (rpc_priority_table_.size() == 0) {
    SDL_LOG_ERROR("Table read failed");
    return false;
  }
  return true;
}

bool InterruptManager::ReadAppTypePriorityTable() {
  SDL_LOG_AUTO_TRACE();    
  apptype_priority_table_ = app_manager_.GetPolicyHandler().GetAppPriority();
  if (apptype_priority_table_.size() == 0) {
    SDL_LOG_ERROR("Table read failed");
    return false;
  }
  return true;
}

bool InterruptManager::ReadHmiStatusPriorityTable() {
  SDL_LOG_AUTO_TRACE();
  hmists_priority_table_ =
      app_manager_.GetPolicyHandler().GetHmiStatusPriority();
  if (hmists_priority_table_.size() == 0) {
    SDL_LOG_ERROR("Table read failed");
    return false;
  }
  return true;
}

bool InterruptManager::ReadInterruptSetting() {
  SDL_LOG_AUTO_TRACE();
  if (policy_settings_.enable_rpc_conflict_manager()) {
    SDL_LOG_INFO("IM_ACTIVATE");
    im_status_ = IM_ACTIVATE;
    return true;
  }
  SDL_LOG_INFO("IM_DEACTIVATE");
  im_status_ = IM_DEACTIVATE;
  return false;
}

}  // namespace interrupt_manager
}  // namespace application_manager
