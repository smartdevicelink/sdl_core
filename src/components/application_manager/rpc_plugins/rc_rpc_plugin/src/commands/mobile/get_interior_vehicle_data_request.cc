#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "utils/macro.h"
#include "interfaces/MOBILE_API.h"

namespace rc_rpc_plugin {
namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

GetInteriorVehicleDataRequest::GetInteriorVehicleDataRequest(
        const app_mngr::commands::MessageSharedPtr& message,
        app_mngr::ApplicationManager& application_manager,
        rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager)
        : RCCommandRequest(
              resource_allocation_manager, message, application_manager) {}

bool CheckIfModuleTypeExistInCapabilities(
    const smart_objects::SmartObject& rc_capabilities,
    const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (enums_value::kRadio == module_type &&
      !rc_capabilities.keyExists(strings::kradioControlCapabilities)) {
    LOG4CXX_DEBUG(logger_, " Radio control capabilities not present");
    return false;
  }
  if (enums_value::kClimate == module_type &&
      !rc_capabilities.keyExists(strings::kclimateControlCapabilities)) {
    LOG4CXX_DEBUG(logger_, " Climate control capabilities not present");
    return false;
  }

  return true;
}

void GetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartObject* rc_capabilities =
      application_manager_.hmi_capabilities().rc_capability();
  const std::string module_type =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleType].asString();
  if (rc_capabilities &&
      !CheckIfModuleTypeExistInCapabilities(*rc_capabilities,
                                            module_type)) {
    LOG4CXX_WARN(logger_, "Accessing not supported module data");
    SendResponse(false,
                 mobile_apis::Result::UNSUPPORTED_RESOURCE,
                 "Accessing not supported module data");
    return;
  }
  if (HasRequestExcessiveSubscription()) {
    RemoveExcessiveSubscription();
  }

  SendHMIRequest(hmi_apis::FunctionID::RC_GetInteriorVehicleData,
              &(*message_)[app_mngr::strings::msg_params], true);
}

void GetInteriorVehicleDataRequest::on_event(const app_mngr::event_engine::Event& event)  {
  LOG4CXX_AUTO_TRACE(logger_);
  if (hmi_apis::FunctionID::RC_GetInteriorVehicleData != event.id()){
      return;
  }

    const smart_objects::SmartObject& hmi_response = event.smart_object();

    mobile_apis::Result::eType result_code =
        GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
            hmi_response[app_mngr::strings::params][app_mngr::hmi_response::code]
                .asUInt()));

    bool result =
        helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
            result_code,
            mobile_apis::Result::SUCCESS,
            mobile_apis::Result::WARNINGS);

    if (mobile_apis::Result::READ_ONLY == result_code) {
      result = false;
      result_code = mobile_apis::Result::GENERIC_ERROR;
    }

  if (result) {
    ProccessSubscription(hmi_response);
  }
  std::string response_info;
  GetInfo(hmi_response, response_info);
  SendResponse(result, result_code, response_info.c_str());
}

void GetInteriorVehicleDataRequest::ProccessSubscription(
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_response) {
  LOG4CXX_AUTO_TRACE(logger_);

  const bool is_subscribe_present_in_request =
          (*message_)[app_mngr::strings::msg_params].keyExists(message_params::kSubscribe);
  const bool isSubscribed_present_in_response =
           hmi_response[json_keys::kResult].keyExists(message_params::kIsSubscribed);

  if (!is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    return;
  }
  app_mngr::ApplicationSharedPtr app =
    application_manager_.application(CommandRequestImpl::connection_key());
  RCAppExtensionPtr extension = resource_allocation_manager_.GetApplicationExtention(app);
  if (is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    LOG4CXX_WARN(logger_,
                 "conditional mandatory parameter "
                     << message_params::kIsSubscribed << " missed in hmi response");
    return;
  }

  if (!is_subscribe_present_in_request && isSubscribed_present_in_response) {
    LOG4CXX_WARN(logger_,
                 "Parameter " << message_params::kIsSubscribed
                              << " is ignored due to absence '"
                              << message_params::kSubscribe
                              << "' parameter in request");
    return;
  }

  const bool request_subscribe =
          (*message_)[app_mngr::strings::msg_params][message_params::kSubscribe].asBool();
  const bool response_subscribe =
      hmi_response[json_keys::kResult][message_params::kIsSubscribed].asBool();
  LOG4CXX_TRACE(logger_, "request_subscribe = " << request_subscribe);
  LOG4CXX_TRACE(logger_, "response_subscribe = " << response_subscribe);
  if (request_subscribe == response_subscribe) {
  const std::string module_type =
          (*message_)[app_mngr::strings::msg_params][message_params::kModuleType].asString();
    if (response_subscribe) {
      LOG4CXX_DEBUG(logger_,
                    "SubscribeToInteriorVehicleData "
                        << app->app_id() << " "
                        << module_type);
      extension->SubscribeToInteriorVehicleData(module_type);
    } else {
      LOG4CXX_DEBUG(logger_,
                    "UnsubscribeFromInteriorVehicleData "
                        << app->app_id() << " "
                        << module_type);
      extension->UnsubscribeFromInteriorVehicleData(module_type);
    }
  }
}

bool GetInteriorVehicleDataRequest::HasRequestExcessiveSubscription() {
  LOG4CXX_AUTO_TRACE(logger_);
  const bool is_subscribe_present_in_request =
          (*message_)[app_mngr::strings::msg_params].keyExists(message_params::kSubscribe);

  if (is_subscribe_present_in_request) {
      app_mngr::ApplicationSharedPtr app =
        application_manager_.application(CommandRequestImpl::connection_key());
      RCAppExtensionPtr extension = resource_allocation_manager_.GetApplicationExtention(app);

    const bool is_app_already_subscribed =
        extension->IsSubscibedToInteriorVehicleData(
            (*message_)[app_mngr::strings::msg_params][message_params::kModuleType].asString());
    const bool app_wants_to_subscribe =
            (*message_)[app_mngr::strings::msg_params][message_params::kSubscribe].asBool();
    if (!app_wants_to_subscribe && !is_app_already_subscribed) {
      return true;
    }
    return app_wants_to_subscribe && is_app_already_subscribed;
  }
  return false;
}

void GetInteriorVehicleDataRequest::RemoveExcessiveSubscription() {
  LOG4CXX_AUTO_TRACE(logger_);
  (*message_)[app_mngr::strings::msg_params].erase(message_params::kSubscribe);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
