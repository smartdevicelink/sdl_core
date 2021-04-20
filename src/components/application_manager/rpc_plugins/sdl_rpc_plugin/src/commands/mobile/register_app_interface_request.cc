/*

 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/mobile/register_app_interface_request.h"

#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <map>

#include "application_manager/app_launch/app_launch_ctrl.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/message_helper.h"
#include "application_manager/plugin_manager/plugin_keys.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/rpc_service.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/generated_msg_version.h"
#include "utils/file_system.h"

namespace {
namespace custom_str = utils::custom_string;

struct AppHMITypeInserter {
  explicit AppHMITypeInserter(smart_objects::SmartObject& so_array)
      : index_(0), so_array_(so_array) {}

  bool operator()(const std::string& app_hmi_type) {
    so_array_[index_] =
        application_manager::StringToEnum<mobile_apis::AppHMIType::eType>(
            app_hmi_type);
    ++index_;
    return true;
  }

 private:
  uint32_t index_;
  smart_objects::SmartObject& so_array_;
};

struct CheckMissedTypes {
  CheckMissedTypes(const policy::StringArray& policy_app_types,
                   std::string& log)
      : policy_app_types_(policy_app_types), log_(log) {}

  bool operator()(const smart_objects::SmartArray::value_type& value) {
    std::string app_type_str = application_manager::EnumToString(
        static_cast<mobile_apis::AppHMIType::eType>(value.asInt()));
    if (!app_type_str.empty()) {
      policy::StringArray::const_iterator it = policy_app_types_.begin();
      policy::StringArray::const_iterator it_end = policy_app_types_.end();
      for (; it != it_end; ++it) {
        if (app_type_str == *it) {
          return true;
        }
      }
    }

    log_ += app_type_str;
    log_ += ",";

    return true;
  }

 private:
  const policy::StringArray& policy_app_types_;
  std::string& log_;
};

class SmartArrayValueExtractor {
 public:
  AppHmiType operator()(const smart_objects::SmartObject& so) const {
    return static_cast<AppHmiType>(so.asInt());
  }
};

struct IsSameNickname {
  explicit IsSameNickname(const custom_str::CustomString app_name)
      : app_name_(app_name) {}
  bool operator()(const policy::StringArray::value_type& nickname) const {
    return app_name_.CompareIgnoreCase(nickname.c_str());
  }

 private:
  const custom_str::CustomString app_name_;
};
}  // namespace

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

RegisterAppInterfaceRequest::RegisterAppInterfaceRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , are_tts_chunks_invalid_(false)
    , are_hmi_types_invalid_(false)
    , is_resumption_failed_(false)
    , is_wrong_language_(false)
    , device_handle_(0) {}

RegisterAppInterfaceRequest::~RegisterAppInterfaceRequest() {}

bool RegisterAppInterfaceRequest::Init() {
  SDL_LOG_AUTO_TRACE();
  return true;
}

uint32_t RegisterAppInterfaceRequest::default_timeout() const {
  // As RAI request does not depend on any HMI response there is no need to
  // track any timeout for it. RAI request will be removed from
  // RequestController queue upon RAI response which will be sent anyway
  return 0;
}

void RegisterAppInterfaceRequest::FillApplicationParams(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  const auto& msg_params = (*message_)[strings::msg_params];
  application->set_is_media_application(
      msg_params[strings::is_media_application].asBool());

  if (msg_params.keyExists(strings::vr_synonyms)) {
    application->set_vr_synonyms(msg_params[strings::vr_synonyms]);
  }

  if (msg_params.keyExists(strings::ngn_media_screen_app_name)) {
    application->set_ngn_media_screen_name(
        msg_params[strings::ngn_media_screen_app_name]);
  }

  if (msg_params.keyExists(strings::tts_name)) {
    smart_objects::SmartObject& tts_name =
        (*message_)[strings::msg_params][strings::tts_name];
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyTtsFiles(
            tts_name, application, application_manager_);

    if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
      SDL_LOG_WARN("MessageHelper::VerifyTtsFiles return "
                   << verification_result);
      response_info_ = "One or more files needed for tts_name are not present";
      are_tts_chunks_invalid_ = true;
    }
    application->set_tts_name(tts_name);
  }

  if (msg_params.keyExists(strings::app_hmi_type)) {
    application->set_app_types(msg_params[strings::app_hmi_type]);

    // check app type
    const smart_objects::SmartObject& app_type =
        msg_params.getElement(strings::app_hmi_type);

    for (size_t i = 0; i < app_type.length(); ++i) {
      mobile_apis::AppHMIType::eType current_app_type =
          static_cast<mobile_apis::AppHMIType::eType>(
              app_type.getElement(i).asUInt());

      switch (current_app_type) {
        case mobile_apis::AppHMIType::NAVIGATION: {
          application->set_is_navi(true);
          break;
        }
        case mobile_apis::AppHMIType::COMMUNICATION: {
          application->set_voice_communication_supported(true);
          break;
        }
        case mobile_apis::AppHMIType::PROJECTION: {
          application->set_mobile_projection_enabled(true);
          break;
        }
        case mobile_apis::AppHMIType::REMOTE_CONTROL: {
          application->set_remote_control_supported(true);
          break;
        }
        case mobile_apis::AppHMIType::WEB_VIEW: {
          application->set_webengine_projection_enabled(true);
          break;
        }
        default: {}
      }
    }
  }

  if (msg_params.keyExists(strings::day_color_scheme)) {
    application->set_day_color_scheme(msg_params[strings::day_color_scheme]);
  }

  if (msg_params.keyExists(strings::night_color_scheme)) {
    application->set_night_color_scheme(
        msg_params[strings::night_color_scheme]);
  }
}

void RegisterAppInterfaceRequest::SetupAppDeviceInfo(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  const auto& msg_params = (*message_)[strings::msg_params];

  policy::DeviceParams dev_params;
  if (-1 == application_manager_.connection_handler()
                .get_session_observer()
                .GetDataOnDeviceID(application->device(),
                                   &dev_params.device_name,
                                   NULL,
                                   &dev_params.device_mac_address,
                                   &dev_params.device_connection_type)) {
    SDL_LOG_ERROR("Failed to extract information for device "
                  << application->device());
  }
  policy::DeviceInfo device_info;
  device_info.AdoptDeviceType(dev_params.device_connection_type);
  if (msg_params.keyExists(strings::device_info)) {
    FillDeviceInfo(&device_info);
  }
  const std::string& device_mac = application->mac_address();
  GetPolicyHandler().SetDeviceInfo(device_mac, device_info);
}

bool RegisterAppInterfaceRequest::ApplicationDataShouldBeResumed(
    std::string& add_info) {
  SDL_LOG_AUTO_TRACE();
  const auto& msg_params = (*message_)[strings::msg_params];
  resumption::ResumeCtrl& resumer = application_manager_.resume_controller();

  const uint32_t key = connection_key();
  ApplicationSharedPtr application = application_manager_.application(key);

  const bool hash_id_present = msg_params.keyExists(strings::hash_id);
  const std::string hash_id = msg_params[strings::hash_id].asString();
  const bool resumption = hash_id_present && !hash_id.empty();

  if (!resumption) {
    SDL_LOG_DEBUG("Hash id is missing, no resumption required");
    return false;
  }

  if (!resumer.CheckApplicationHash(application, hash_id)) {
    add_info = "Hash from RAI does not match to saved resume data.";
    SDL_LOG_WARN(add_info);
    is_resumption_failed_ = true;
    return false;
  }

  if (!resumer.CheckPersistenceFilesForResumption(application)) {
    add_info = "Persistent data is missing.";
    SDL_LOG_WARN(add_info);
    is_resumption_failed_ = true;
    return false;
  }

  add_info = "Resume succeeded.";
  SDL_LOG_DEBUG(add_info);
  application->set_app_data_resumption_allowance(true);
  application->set_is_resuming(true);

  return true;
}

mobile_apis::Result::eType
RegisterAppInterfaceRequest::CalculateFinalResultCode() const {
  if (is_wrong_language_) {
    SDL_LOG_DEBUG("Language was wrong");
    return mobile_apis::Result::WRONG_LANGUAGE;
  }

  if (are_hmi_types_invalid_ || are_tts_chunks_invalid_) {
    SDL_LOG_DEBUG("HMI types or TTS chunks are invalid");
    return mobile_apis::Result::WARNINGS;
  }

  if (is_resumption_failed_) {
    SDL_LOG_DEBUG("Resumption has been failed");
    return mobile_apis::Result::RESUME_FAILED;
  }

  return mobile_apis::Result::SUCCESS;
}

policy::StatusNotifier RegisterAppInterfaceRequest::AddApplicationDataToPolicy(
    application_manager::ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();

  AppHmiTypes hmi_types;
  if ((*message_)[strings::msg_params].keyExists(strings::app_hmi_type)) {
    smart_objects::SmartArray* hmi_types_ptr =
        (*message_)[strings::msg_params][strings::app_hmi_type].asArray();
    if (hmi_types_ptr) {
      SmartArrayValueExtractor extractor;
      if (hmi_types_ptr && 0 < hmi_types_ptr->size()) {
        std::transform(hmi_types_ptr->begin(),
                       hmi_types_ptr->end(),
                       std::back_inserter(hmi_types),
                       extractor);
      }
    }
  }

  return policy_handler_.AddApplication(
      application->mac_address(), application->policy_app_id(), hmi_types);
}

void RegisterAppInterfaceRequest::CheckLanguage() {
  ApplicationSharedPtr application =
      application_manager_.application(connection_key());
  DCHECK_OR_RETURN_VOID(application);
  const auto& msg_params = (*message_)[strings::msg_params];
  if (msg_params[strings::language_desired].asInt() !=
          hmi_capabilities_.active_vr_language() ||
      msg_params[strings::hmi_display_language_desired].asInt() !=
          hmi_capabilities_.active_ui_language()) {
    SDL_LOG_WARN("Wrong language on registering application "
                 << application->name().c_str());

    SDL_LOG_ERROR("VR language desired code is "
                  << msg_params[strings::language_desired].asInt()
                  << " , active VR language code is "
                  << hmi_capabilities_.active_vr_language()
                  << ", UI language desired code is "
                  << msg_params[strings::hmi_display_language_desired].asInt()
                  << " , active UI language code is "
                  << hmi_capabilities_.active_ui_language());

    is_wrong_language_ = true;
  }
}

smart_objects::SmartObjectSPtr GetLockScreenIconUrlNotification(
    policy::PolicyHandlerInterface& policy_handler,
    const uint32_t connection_key,
    ApplicationSharedPtr app) {
  DCHECK_OR_RETURN(app.get(), smart_objects::SmartObjectSPtr());
  smart_objects::SmartObjectSPtr message =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemRequestID;
  (*message)[strings::params][strings::connection_key] = connection_key;
  (*message)[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  (*message)[strings::params][strings::protocol_type] =
      application_manager::commands::CommandImpl::mobile_protocol_type_;
  (*message)[strings::params][strings::protocol_version] =
      app->protocol_version();
  (*message)[strings::msg_params][strings::request_type] =
      mobile_apis::RequestType::LOCK_SCREEN_ICON_URL;
  (*message)[strings::msg_params][strings::url] =
      policy_handler.GetLockScreenIconUrl(app->policy_app_id());
  return message;
}

void SendDriverDistractionAndIconUrlNotifications(
    ApplicationManager& app_manager,
    const uint32_t connection_key,
    ApplicationSharedPtr app) {
  policy::PolicyHandlerInterface& policy_handler =
      app_manager.GetPolicyHandler();
  smart_objects::SmartObjectSPtr so =
      GetLockScreenIconUrlNotification(policy_handler, connection_key, app);
  app_manager.GetRPCService().ManageMobileCommand(
      so, app_mngr::commands::Command::SOURCE_SDL);
  app_manager.SendDriverDistractionState(app);
  // Create onSystemRequest to mobile to obtain cloud app icons
  app_manager.SendGetIconUrlNotifications(connection_key, app);
}

/**
 * @brief FinishSendingResponseToMobile is needed because by the time
 SendRegisterAppInterfaceResponseToMobile() is called from callback
 in Run(), request object is deleted, which results in SDL crash. This function
 is intended to prevent that, safely conclude sending response to mobile and
 perform other related operations (sending notifications, setting HMI level
 etc.)
 * @param msg_params copy of request object
 * @param app_manager reference to application manager
 * @param connection_key connection key
 * @param notify_upd_manager pointer to status notifier callback function
 */
void FinishSendingResponseToMobile(const smart_objects::SmartObject& msg_params,
                                   ApplicationManager& app_manager,
                                   const uint32_t connection_key,
                                   policy::StatusNotifier notify_upd_manager) {
  resumption::ResumeCtrl& resume_ctrl = app_manager.resume_controller();
  auto application = app_manager.application(connection_key);

  policy::PolicyHandlerInterface& policy_handler =
      app_manager.GetPolicyHandler();

  if (msg_params.keyExists(strings::app_hmi_type)) {
    policy_handler.SetDefaultHmiTypes(application->device(),
                                      application->policy_app_id(),
                                      &(msg_params[strings::app_hmi_type]));
  }

  // Default HMI level should be set before any permissions validation, since
  // it relies on HMI level.
  app_manager.OnApplicationRegistered(application);

  // Once HMI level is set we can safely forward system capabilities for the
  // main window and it won't be blocked by policies
  application->display_capabilities_builder().StopWaitingForWindow(
      mobile_apis::PredefinedWindows::DEFAULT_WINDOW);

  if (notify_upd_manager) {
    (*notify_upd_manager)();
  }

  auto send_rc_status = [application](plugin_manager::RPCPlugin& plugin) {
    plugin.OnApplicationEvent(plugin_manager::kRCStatusChanged, application);
  };
  app_manager.ApplyFunctorForEachPlugin(send_rc_status);

  // Start PTU after successful registration
  // Sends OnPermissionChange notification to mobile right after RAI response
  // and HMI level set-up
  policy_handler.OnAppRegisteredOnMobile(application->mac_address(),
                                         application->policy_app_id());
  SendDriverDistractionAndIconUrlNotifications(
      app_manager, connection_key, application);
  const bool is_app_saved_in_resumption = resume_ctrl.IsApplicationSaved(
      application->policy_app_id(), application->mac_address());
  if (is_app_saved_in_resumption) {
    resume_ctrl.StartResumptionOnlyHMILevel(application);
  }
}

void RegisterAppInterfaceRequest::Run() {
  using namespace helpers;
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Connection key is " << connection_key());

  if (!application_manager_.WaitForHmiIsReady()) {
    SDL_LOG_WARN("Failed to wait for HMI readiness");
    return;
  }

  // Cache the original app ID (for legacy behavior)
  const auto policy_app_id =
      application_manager_.GetCorrectMobileIDFromMessage(message_);

  if (application_manager_.IsApplicationForbidden(connection_key(),
                                                  policy_app_id)) {
    SendResponse(false, mobile_apis::Result::TOO_MANY_PENDING_REQUESTS);
    return;
  }

  if (!GetDataOnSessionKey(connection_key(), &device_handle_, &device_id_)) {
    SendResponse(false,
                 mobile_apis::Result::GENERIC_ERROR,
                 "Could not find a session for your connection key!");
    return;
  }

  SDL_LOG_DEBUG("device_handle: " << device_handle_
                                  << " device_id: " << device_id_);

  if (IsApplicationSwitched()) {
    return;
  }

  ApplicationSharedPtr application =
      application_manager_.application(device_id_, policy_app_id);

  if (application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_REGISTERED_ALREADY);
    return;
  }

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  // transform app IDs to lowercase for usage in policy checks later
  const std::string app_id_short = msg_params[strings::app_id].asString();
  std::string new_app_id_short = app_id_short;
  std::transform(app_id_short.begin(),
                 app_id_short.end(),
                 new_app_id_short.begin(),
                 ::tolower);
  (*message_)[strings::msg_params][strings::app_id] = new_app_id_short;
  // If full ID is present, shift that to lowercase too
  if (msg_params.keyExists(strings::full_app_id)) {
    const std::string app_id_full = msg_params[strings::full_app_id].asString();
    std::string new_app_id_full = app_id_full;
    std::transform(app_id_full.begin(),
                   app_id_full.end(),
                   new_app_id_full.begin(),
                   ::tolower);
    (*message_)[strings::msg_params][strings::full_app_id] = new_app_id_full;
  }

  if (IsApplicationWithSameAppIdRegistered()) {
    SendResponse(false, mobile_apis::Result::DISALLOWED);
    return;
  }

  mobile_apis::Result::eType policy_result = CheckWithPolicyData();

  if (Compare<mobile_apis::Result::eType, NEQ, ALL>(
          policy_result,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS)) {
    SendResponse(false, policy_result, response_info_.c_str());
    return;
  }

  mobile_apis::Result::eType coincidence_result = CheckCoincidence();

  std::vector<ApplicationSharedPtr> duplicate_apps;
  if (GetDuplicateNames(duplicate_apps)) {
    SDL_LOG_ERROR("Found duplicate app names, checking for hybrid apps.");
    // Default preference to BOTH
    mobile_apis::HybridAppPreference::eType preference =
        mobile_apis::HybridAppPreference::BOTH;
    ApplicationSharedPtr app =
        application_manager_.pending_application_by_policy_id(policy_app_id);
    bool is_cloud_app = app.use_count() != 0 && app->is_cloud_app();
    if (is_cloud_app) {
      // Retrieve hybrid app preference from registering app
      preference = app->hybrid_app_preference();
    } else {
      if (mobile_apis::Result::DUPLICATE_NAME == coincidence_result) {
        usage_statistics::AppCounter count_of_rejections_duplicate_name(
            GetPolicyHandler().GetStatisticManager(),
            policy_app_id,
            usage_statistics::REJECTIONS_DUPLICATE_NAME);
        ++count_of_rejections_duplicate_name;
      }
      // Search for the hybrid app preference in the duplicate app list
      for (auto duplicate_app : duplicate_apps) {
        if (duplicate_app->is_cloud_app()) {
          preference = duplicate_app->hybrid_app_preference();
          break;
        }
      }
    }

    if (preference == mobile_apis::HybridAppPreference::MOBILE ||
        preference == mobile_apis::HybridAppPreference::CLOUD) {
      bool cloud_app_exists = is_cloud_app;
      bool mobile_app_exists = !is_cloud_app;
      for (auto duplicate_app : duplicate_apps) {
        cloud_app_exists = cloud_app_exists || (duplicate_app->IsRegistered() &&
                                                duplicate_app->is_cloud_app());
        mobile_app_exists = mobile_app_exists || !duplicate_app->is_cloud_app();
        if (is_cloud_app && !duplicate_app->is_cloud_app() &&
            preference == mobile_apis::HybridAppPreference::CLOUD) {
          // Unregister mobile application and allow cloud application to
          // register in it's place
          SDL_LOG_ERROR(
              "Unregistering app because a preferred version is registered.");
          application_manager_.UnregisterApplication(
              duplicate_app->app_id(),
              mobile_apis::Result::USER_DISALLOWED,
              "App is disabled by user preferences");
        }
      }

      bool mobile_app_matches =
          !is_cloud_app &&
          preference == mobile_apis::HybridAppPreference::MOBILE;
      bool cloud_app_matches =
          is_cloud_app && preference == mobile_apis::HybridAppPreference::CLOUD;

      bool is_preferred_application = mobile_app_matches || cloud_app_matches;
      if (mobile_app_exists && cloud_app_exists && !is_preferred_application) {
        SendResponse(false,
                     mobile_apis::Result::USER_DISALLOWED,
                     "App is disabled by user preferences");
        return;
      }
    }
  }

  if (mobile_apis::Result::SUCCESS != coincidence_result) {
    SDL_LOG_ERROR("Coincidence check failed.");
    SendResponse(false, coincidence_result);
    return;
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_INFO("Incoming register app interface has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  uint16_t major =
      msg_params[strings::sync_msg_version][strings::major_version].asUInt();
  uint16_t minor =
      msg_params[strings::sync_msg_version][strings::minor_version].asUInt();
  uint16_t patch = 0;
  // Check if patch exists since it is not mandatory.
  if (msg_params[strings::sync_msg_version].keyExists(strings::patch_version)) {
    patch =
        msg_params[strings::sync_msg_version][strings::patch_version].asUInt();
  }

  utils::SemanticVersion mobile_version(major, minor, patch);
  utils::SemanticVersion min_module_version(
      minimum_major_version, minimum_minor_version, minimum_patch_version);

  if (mobile_version < min_module_version) {
    SDL_LOG_WARN("Application RPC Version does not meet minimum requirement");
    SendResponse(false, mobile_apis::Result::REJECTED);
  }

  application = application_manager_.RegisterApplication(message_);

  if (!application) {
    SDL_LOG_ERROR("Application hasn't been registered!");
    return;
  }

  // Version negotiation
  utils::SemanticVersion module_version(
      major_version, minor_version, patch_version);
  if (mobile_version < utils::rpc_version_5) {
    // Mobile versioning did not exist for
    // versions before 5.0
    application->set_msg_version(utils::base_rpc_version);
  } else if (mobile_version < module_version) {
    // Use mobile RPC version as negotiated version
    application->set_msg_version(mobile_version);
  } else {
    // Use module version as negotiated version
    application->set_msg_version(module_version);
  }

  FillApplicationParams(application);
  SetupAppDeviceInfo(application);
  auto status_notifier = AddApplicationDataToPolicy(application);

  auto on_app_registered = [application](plugin_manager::RPCPlugin& plugin) {
    plugin.OnApplicationEvent(plugin_manager::kApplicationRegistered,
                              application);
  };
  // To prevent timing issues, this event is called before an app is accessible
  // by the applications accessor. This prevents incoming hmi rpcs from
  // attempting to access an app before it has been fully initialized.
  application_manager_.ApplyFunctorForEachPlugin(on_app_registered);
  application_manager_.FinalizeAppRegistration(application, connection_key());

  std::string add_info;
  const auto is_resumption_required = ApplicationDataShouldBeResumed(add_info);

  auto& resume_ctrl = application_manager_.resume_controller();

  // DisplayCapabilitiesBuilder has to collect all the information
  // from incoming HMI notifications and send only one notification
  // to mobile app, even if hash does not match, which means that app data
  // will not be resumed, notification should be sent for default window as
  // it will be resumed in any case
  resume_ctrl.StartWaitingForDisplayCapabilitiesUpdate(application,
                                                       is_resumption_required);

  SendOnAppRegisteredNotificationToHMI(
      application, is_resumption_required && !is_resumption_failed_);

  // By default app subscribed to CUSTOM_BUTTON
  SendSubscribeCustomButtonNotification();
  SendChangeRegistrationOnHMI(application);

  if (is_resumption_required) {
    const auto& msg_params = (*message_)[strings::msg_params];
    const auto& hash_id = msg_params[strings::hash_id].asString();
    SDL_LOG_WARN("Start Data Resumption");
    auto send_response = [this, application, status_notifier](
                             mobile_apis::Result::eType result_code,
                             const std::string info) {
      SDL_LOG_DEBUG("Invoking lambda callback for: " << this);
      if (result_code != mobile_apis::Result::SUCCESS) {
        is_resumption_failed_ = true;
      }

      SendRegisterAppInterfaceResponseToMobile(
          ApplicationType::kNewApplication, status_notifier, info);
      application->UpdateHash();
    };
    resume_ctrl.StartResumption(application, hash_id, send_response);

    return;
  }

  CheckLanguage();

  SendRegisterAppInterfaceResponseToMobile(
      ApplicationType::kNewApplication, status_notifier, add_info);
}

void FillVRRelatedFields(smart_objects::SmartObject& response_params,
                         const HMICapabilities& hmi_capabilities) {
  auto active_vr_lang = hmi_capabilities.active_vr_language();
  if (hmi_apis::Common_Language::INVALID_ENUM != active_vr_lang) {
    response_params[strings::language] = active_vr_lang;
  }
  auto vr_capabilities = hmi_capabilities.vr_capabilities();
  if (vr_capabilities) {
    response_params[strings::vr_capabilities] = *vr_capabilities;
  }
}

void FillVIRelatedFields(smart_objects::SmartObject& response_params,
                         const HMICapabilities& hmi_capabilities) {
  auto vehicle_type = hmi_capabilities.vehicle_type();
  if (vehicle_type) {
    response_params[hmi_response::vehicle_type] = *vehicle_type;
  }
}

void FillTTSRelatedFields(smart_objects::SmartObject& response_params,
                          const HMICapabilities& hmi_capabilities) {
  auto active_tts_lang = hmi_capabilities.active_tts_language();
  if (hmi_apis::Common_Language::INVALID_ENUM != active_tts_lang) {
    response_params[strings::language] = active_tts_lang;
  }
  auto speech_capabilities = hmi_capabilities.speech_capabilities();
  if (speech_capabilities) {
    response_params[strings::speech_capabilities] = *speech_capabilities;
  }
  auto prerecorded_speech = hmi_capabilities.prerecorded_speech();
  if (prerecorded_speech) {
    response_params[strings::prerecorded_speech] = *prerecorded_speech;
  }
}

void FillUIRelatedFields(smart_objects::SmartObject& response_params,
                         const HMICapabilities& hmi_capabilities) {
  auto active_ui_lang = hmi_capabilities.active_ui_language();
  if (hmi_apis::Common_Language::INVALID_ENUM != active_ui_lang) {
    response_params[strings::hmi_display_language] = active_ui_lang;
  }

  auto display_capabilities = hmi_capabilities.display_capabilities();
  if (display_capabilities) {
    response_params[hmi_response::display_capabilities] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& display_caps =
        response_params[hmi_response::display_capabilities];

    if (display_capabilities->keyExists(hmi_response::display_type)) {
      display_caps[hmi_response::display_type] =
          display_capabilities->getElement(hmi_response::display_type);
    }

    if (display_capabilities->keyExists(hmi_response::display_name)) {
      display_caps[hmi_response::display_name] =
          display_capabilities->getElement(hmi_response::display_name);
    }

    if (display_capabilities->keyExists(hmi_response::text_fields)) {
      display_caps[hmi_response::text_fields] =
          display_capabilities->getElement(hmi_response::text_fields);
    }

    if (display_capabilities->keyExists(hmi_response::image_fields)) {
      display_caps[hmi_response::image_fields] =
          display_capabilities->getElement(hmi_response::image_fields);
    }

    if (display_capabilities->keyExists(hmi_response::media_clock_formats)) {
      display_caps[hmi_response::media_clock_formats] =
          display_capabilities->getElement(hmi_response::media_clock_formats);
    }

    if (display_capabilities->keyExists(hmi_response::templates_available)) {
      display_caps[hmi_response::templates_available] =
          display_capabilities->getElement(hmi_response::templates_available);
    }

    if (display_capabilities->keyExists(hmi_response::screen_params)) {
      display_caps[hmi_response::screen_params] =
          display_capabilities->getElement(hmi_response::screen_params);
    }

    if (display_capabilities->keyExists(
            hmi_response::num_custom_presets_available)) {
      display_caps[hmi_response::num_custom_presets_available] =
          display_capabilities->getElement(
              hmi_response::num_custom_presets_available);
    }

    if (display_capabilities->keyExists(hmi_response::image_capabilities)) {
      display_caps[hmi_response::graphic_supported] =
          (display_capabilities->getElement(hmi_response::image_capabilities)
               .length() > 0);
    }
  }

  auto audio_pass_thru_capabilities =
      hmi_capabilities.audio_pass_thru_capabilities();
  if (audio_pass_thru_capabilities) {
    // hmi_capabilities json contains array and HMI response object
    response_params[strings::audio_pass_thru_capabilities] =
        *audio_pass_thru_capabilities;
  }
  response_params[strings::hmi_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  response_params[strings::hmi_capabilities][strings::navigation] =
      hmi_capabilities.navigation_supported();
  response_params[strings::hmi_capabilities][strings::phone_call] =
      hmi_capabilities.phone_call_supported();
  response_params[strings::hmi_capabilities][strings::video_streaming] =
      hmi_capabilities.video_streaming_supported();
  response_params[strings::hmi_capabilities][strings::remote_control] =
      hmi_capabilities.rc_supported();
  response_params[strings::hmi_capabilities][strings::app_services] = true;
  // Apps are automatically subscribed to the SystemCapability: DISPLAYS
  response_params[strings::hmi_capabilities][strings::displays] = true;
  response_params[strings::hmi_capabilities][strings::seat_location] =
      hmi_capabilities.seat_location_capability() ? true : false;
  response_params[strings::hmi_capabilities][strings::driver_distraction] =
      hmi_capabilities.driver_distraction_supported();
}

void RegisterAppInterfaceRequest::SendRegisterAppInterfaceResponseToMobile(
    ApplicationType app_type,
    policy::StatusNotifier status_notifier,
    const std::string& add_info) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);

  const HMICapabilities& hmi_capabilities = hmi_capabilities_;

  const uint32_t key = connection_key();
  ApplicationSharedPtr application = application_manager_.application(key);
  utils::SemanticVersion negotiated_version = application->msg_version();

  response_params[strings::sync_msg_version][strings::major_version] =
      negotiated_version.major_version_;
  response_params[strings::sync_msg_version][strings::minor_version] =
      negotiated_version.minor_version_;
  response_params[strings::sync_msg_version][strings::patch_version] =
      negotiated_version.patch_version_;

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_TTS)) {
    FillTTSRelatedFields(response_params, hmi_capabilities);
  }

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_VR)) {
    FillVRRelatedFields(response_params, hmi_capabilities);
  }

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_UI)) {
    FillUIRelatedFields(response_params, hmi_capabilities);
  }

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_VehicleInfo)) {
    FillVIRelatedFields(response_params, hmi_capabilities);
  }

  if (hmi_capabilities.button_capabilities()) {
    response_params[hmi_response::button_capabilities] =
        *hmi_capabilities.button_capabilities();
  }

  if (hmi_capabilities.soft_button_capabilities()) {
    response_params[hmi_response::soft_button_capabilities] =
        *hmi_capabilities.soft_button_capabilities();
  }

  if (hmi_capabilities.preset_bank_capabilities()) {
    response_params[hmi_response::preset_bank_capabilities] =
        *hmi_capabilities.preset_bank_capabilities();
  }

  if (hmi_capabilities.hmi_zone_capabilities()) {
    if (smart_objects::SmartType_Array ==
        hmi_capabilities.hmi_zone_capabilities()->getType()) {
      // hmi_capabilities json contains array and HMI response object
      response_params[hmi_response::hmi_zone_capabilities] =
          *hmi_capabilities.hmi_zone_capabilities();
    } else {
      response_params[hmi_response::hmi_zone_capabilities][0] =
          *hmi_capabilities.hmi_zone_capabilities();
    }
  }

  if (hmi_capabilities.pcm_stream_capabilities()) {
    response_params[strings::pcm_stream_capabilities] =
        *hmi_capabilities.pcm_stream_capabilities();
  }

  const std::vector<uint32_t>& diag_modes =
      application_manager_.get_settings().supported_diag_modes();
  if (!diag_modes.empty()) {
    std::vector<uint32_t>::const_iterator it = diag_modes.begin();
    uint32_t index = 0;
    for (; it != diag_modes.end(); ++it) {
      response_params[strings::supported_diag_modes][index] = *it;
      ++index;
    }
  }

  response_params[strings::sdl_version] =
      application_manager_.get_settings().sdl_version();
  const std::string ccpu_version = hmi_capabilities_.ccpu_version();
  if (!ccpu_version.empty()) {
    response_params[strings::system_software_version] = ccpu_version;
  }

  if (ApplicationType::kSwitchedApplicationWrongHashId == app_type) {
    SDL_LOG_DEBUG(
        "Application has been switched from another transport, "
        "but doesn't have correct hashID.");

    application_manager::DeleteApplicationData(application,
                                               application_manager_);

    SendResponse(
        true, mobile_apis::Result::RESUME_FAILED, NULL, &response_params);
    return;
  }

  if (ApplicationType::kSwitchedApplicationHashOk == app_type) {
    SDL_LOG_DEBUG(
        "Application has been switched from another transport "
        "and has correct hashID.");
    SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
    return;
  }

  response_info_ += add_info;

  response_params[strings::icon_resumed] =
      file_system::FileExists(application->app_icon_path());

  smart_objects::SmartObject msg_params_copy = msg_params;
  const auto result_code = CalculateFinalResultCode();

  SendResponse(true, result_code, response_info_.c_str(), &response_params);

  FinishSendingResponseToMobile(
      msg_params_copy, application_manager_, key, status_notifier);
}

void RegisterAppInterfaceRequest::SendChangeRegistration(
    const hmi_apis::FunctionID::eType function_id,
    const int32_t language,
    const uint32_t app_id) {
  using helpers::Compare;
  using helpers::EQ;
  using helpers::ONE;
  const HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  const HmiInterfaces::InterfaceID interface =
      hmi_interfaces.GetInterfaceFromFunction(function_id);
  if (hmi_interfaces.GetInterfaceState(interface) !=
      HmiInterfaces::STATE_NOT_AVAILABLE) {
    smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
    msg_params[strings::language] = language;
    msg_params[strings::app_id] = app_id;
    SendHMIRequest(function_id, &msg_params);
  } else {
    SDL_LOG_DEBUG("Interface " << interface << " is not available");
  }
}

void RegisterAppInterfaceRequest::SendChangeRegistrationOnHMI(
    ApplicationConstSharedPtr app) {
  using namespace hmi_apis::FunctionID;
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(mobile_apis::Language::INVALID_ENUM != app->language());
  SendChangeRegistration(VR_ChangeRegistration, app->language(), app->app_id());
  SendChangeRegistration(
      TTS_ChangeRegistration, app->language(), app->app_id());
  SendChangeRegistration(UI_ChangeRegistration, app->language(), app->app_id());
}

void RegisterAppInterfaceRequest::SendOnAppRegisteredNotificationToHMI(
    ApplicationConstSharedPtr app, bool resumption) {
  using namespace smart_objects;
  SmartObjectSPtr notification = std::make_shared<SmartObject>(SmartType_Map);
  if (!notification) {
    SDL_LOG_ERROR("Failed to create smart object");
    return;
  }

  (*notification)[strings::params] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& params = (*notification)[strings::params];
  params[strings::function_id] = static_cast<int32_t>(
      hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
  params[strings::message_type] = static_cast<int32_t>(kNotification);
  params[strings::protocol_version] = protocol_version_;
  params[strings::protocol_type] = hmi_protocol_type_;

  (*notification)[strings::msg_params] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& msg_params = (*notification)[strings::msg_params];
  // Due to current requirements in case when we're in resumption mode
  // we have to always send resumeVRGrammar field.
  if (resumption) {
    msg_params[strings::resume_vr_grammars] = resumption;
  }

  if (app->vr_synonyms()) {
    msg_params[strings::vr_synonyms] = *(app->vr_synonyms());
  }

  if (app->tts_name()) {
    msg_params[strings::tts_name] = *(app->tts_name());
  }

  const std::string policy_app_id = app->policy_app_id();
  std::string priority;
  GetPolicyHandler().GetPriority(policy_app_id, &priority);

  if (!priority.empty()) {
    msg_params[strings::priority] = MessageHelper::GetPriorityCode(priority);
  }

  msg_params[strings::msg_params] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& application = msg_params[strings::application];
  const protocol_handler::SessionObserver& session_observer =
      application_manager_.connection_handler().get_session_observer();
  MessageHelper::CreateHMIApplicationStruct(app,
                                            session_observer,
                                            GetPolicyHandler(),
                                            &application,
                                            application_manager_);

  DCHECK(rpc_service_.ManageHMICommand(notification));
}

mobile_apis::Result::eType RegisterAppInterfaceRequest::CheckCoincidence() {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  auto compare_tts_name = [](const smart_objects::SmartObject& obj_1,
                             const smart_objects::SmartObject& obj_2) {
    return obj_1[application_manager::strings::text]
        .asCustomString()
        .CompareIgnoreCase(
            obj_2[application_manager::strings::text].asCustomString());
  };

  const auto& accessor = application_manager_.applications().GetData();
  const auto& app_name = msg_params[strings::app_name].asCustomString();

  for (const auto& app : accessor) {
    if (app->device() != device_handle_) {
      continue;
    }
    // Name check
    const auto& cur_name = app->name();
    if (app_name.CompareIgnoreCase(cur_name)) {
      SDL_LOG_ERROR("Application name is known already.");
      return mobile_apis::Result::DUPLICATE_NAME;
    }
    const auto vr = app->vr_synonyms();
    if (vr) {
      const auto curr_vr = vr->asArray();
      CoincidencePredicateVR v(app_name);

      if (0 != std::count_if(curr_vr->begin(), curr_vr->end(), v)) {
        SDL_LOG_ERROR("Application name is known already.");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }

    // VR check
    if (msg_params.keyExists(strings::vr_synonyms)) {
      const auto new_vr = msg_params[strings::vr_synonyms].asArray();

      CoincidencePredicateVR v(cur_name);
      if (0 != std::count_if(new_vr->begin(), new_vr->end(), v)) {
        SDL_LOG_ERROR("vr_synonyms duplicated with app_name .");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }  // End vr check

    // TTS check
    if (msg_params.keyExists(strings::tts_name) && app->tts_name()) {
      const auto tts_array = msg_params[strings::tts_name].asArray();
      const auto tts_curr = app->tts_name()->asArray();
      const auto& it_tts = std::find_first_of(tts_array->begin(),
                                              tts_array->end(),
                                              tts_curr->begin(),
                                              tts_curr->end(),
                                              compare_tts_name);
      if (it_tts != tts_array->end()) {
        SDL_LOG_ERROR("TTS name: "
                      << (*it_tts)[strings::text].asCustomString().AsMBString()
                      << " is known already");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }  // End tts check

  }  // Application for end

  return mobile_apis::Result::SUCCESS;
}  // method end

bool RegisterAppInterfaceRequest::GetDuplicateNames(
    std::vector<ApplicationSharedPtr>& out_duplicate_apps) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  const auto& app_name = msg_params[strings::app_name].asCustomString();
  {
    const auto& accessor = application_manager_.applications().GetData();

    for (const auto& app : accessor) {
      const auto& cur_name = app->name();
      if (app_name.CompareIgnoreCase(cur_name)) {
        out_duplicate_apps.push_back(app);
      }
    }
  }

  const std::string policy_app_id =
      application_manager_.GetCorrectMobileIDFromMessage(message_);
  {
    const auto& accessor =
        application_manager_.pending_applications().GetData();

    for (const auto& app : accessor) {
      const auto& cur_name = app->name();
      if (app_name.CompareIgnoreCase(cur_name) &&
          policy_app_id != app->policy_app_id()) {
        out_duplicate_apps.push_back(app);
      }
    }
  }
  return !out_duplicate_apps.empty();
}

mobile_apis::Result::eType RegisterAppInterfaceRequest::CheckWithPolicyData() {
  SDL_LOG_AUTO_TRACE();
  // TODO(AOleynik): Check is necessary to allow register application in case
  // of disabled policy
  // Remove this check, when HMI will support policy
  if (!GetPolicyHandler().PolicyEnabled()) {
    return mobile_apis::Result::WARNINGS;
  }

  auto& message = *message_;
  policy::StringArray app_nicknames;
  policy::StringArray app_hmi_types_in_policy;

  const std::string mobile_app_id =
      application_manager_.GetCorrectMobileIDFromMessage(message_);
  const bool init_result = GetPolicyHandler().GetInitialAppData(
      mobile_app_id, &app_nicknames, &app_hmi_types_in_policy);

  if (!init_result) {
    SDL_LOG_ERROR("Error during initial application data check.");
    return mobile_apis::Result::INVALID_DATA;
  }

  if (!app_nicknames.empty()) {
    IsSameNickname compare(
        message[strings::msg_params][strings::app_name].asCustomString());
    policy::StringArray::const_iterator it =
        std::find_if(app_nicknames.begin(), app_nicknames.end(), compare);
    if (app_nicknames.end() == it) {
      SDL_LOG_WARN("Application name was not found in nicknames list.");
      // App should be unregistered, if its name is not present in nicknames
      // list
      usage_statistics::AppCounter count_of_rejections_nickname_mismatch(
          GetPolicyHandler().GetStatisticManager(),
          mobile_app_id,
          usage_statistics::REJECTIONS_NICKNAME_MISMATCH);
      ++count_of_rejections_nickname_mismatch;
      return mobile_apis::Result::DISALLOWED;
    }
  }

  auto result = mobile_apis::Result::SUCCESS;

  // If AppHMIType is not included in policy - allow any type
  if (!app_hmi_types_in_policy.empty()) {
    result = ProcessingAppHMITypesPolicies(message, app_hmi_types_in_policy);
  } else {
    result = ProcessingAppHMITypesInMessage(message);
    if (mobile_apis::Result::DISALLOWED == result) {
      response_info_ =
          "WEB_VIEW AppHmiType is absent in application policies, because they "
          "are empty";
      SDL_LOG_DEBUG(response_info_);
      return result;
    }
  }

  return result;
}

void RegisterAppInterfaceRequest::FillDeviceInfo(
    policy::DeviceInfo* device_info) {
  const std::string hardware = "hardware";
  const std::string firmware_rev = "firmwareRev";
  const std::string os = "os";
  const std::string os_ver = "osVersion";
  const std::string carrier = "carrier";
  const std::string max_number_rfcom_ports = "maxNumberRFCOMMPorts";

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  const smart_objects::SmartObject& device_info_so =
      msg_params[strings::device_info];

  if (device_info_so.keyExists(hardware)) {
    device_info->hardware =
        msg_params[strings::device_info][hardware].asString();
  }
  if (device_info_so.keyExists(firmware_rev)) {
    device_info->firmware_rev =
        msg_params[strings::device_info][firmware_rev].asString();
  }
  if (device_info_so.keyExists(os)) {
    device_info->os = device_info_so[os].asString();
  }
  if (device_info_so.keyExists(os_ver)) {
    device_info->os_ver = device_info_so[os_ver].asString();
  }
  if (device_info_so.keyExists(carrier)) {
    device_info->carrier = device_info_so[carrier].asString();
  }
  if (device_info_so.keyExists(max_number_rfcom_ports)) {
    device_info->max_number_rfcom_ports =
        device_info_so[max_number_rfcom_ports].asInt();
  }
}

bool RegisterAppInterfaceRequest::IsApplicationWithSameAppIdRegistered() {
  SDL_LOG_AUTO_TRACE();

  const custom_string::CustomString mobile_app_id(
      application_manager_.GetCorrectMobileIDFromMessage(message_));

  const auto& applications = application_manager_.applications().GetData();

  for (const auto& app : applications) {
    if (mobile_app_id.CompareIgnoreCase(app->policy_app_id().c_str())) {
      if (app->device() != device_handle_) {
        SDL_LOG_DEBUG(
            "These policy_app_id equal, but applications have "
            "different device id"
            << " mobile_app_id: " << mobile_app_id.c_str() << " device_handle: "
            << device_handle_ << " device_handle: " << app->device());
        continue;
      }
      return true;
    }
  }

  return false;
}

bool RegisterAppInterfaceRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();

  const char* str =
      (*message_)[strings::msg_params][strings::app_name].asCharArray();
  if (!CheckSyntax(str)) {
    SDL_LOG_ERROR("Invalid app_name syntax check failed");
    return true;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::tts_name)) {
    const smart_objects::SmartArray* tn_array =
        (*message_)[strings::msg_params][strings::tts_name].asArray();

    smart_objects::SmartArray::const_iterator it_tn = tn_array->begin();
    smart_objects::SmartArray::const_iterator it_tn_end = tn_array->end();

    for (; it_tn != it_tn_end; ++it_tn) {
      str = (*it_tn)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid tts_name syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::ngn_media_screen_app_name)) {
    str = (*message_)[strings::msg_params][strings::ngn_media_screen_app_name]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid ngn_media_screen_app_name syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::vr_synonyms)) {
    const smart_objects::SmartArray* vs_array =
        (*message_)[strings::msg_params][strings::vr_synonyms].asArray();

    smart_objects::SmartArray::const_iterator it_vs = vs_array->begin();
    smart_objects::SmartArray::const_iterator it_vs_end = vs_array->end();

    for (; it_vs != it_vs_end; ++it_vs) {
      str = (*it_vs).asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid vr_synonyms syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::hash_id)) {
    str = (*message_)[strings::msg_params][strings::hash_id].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid hash_id syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::device_info)) {
    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::hardware)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::hardware]
                           .asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid device_info hardware syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::firmware_rev)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::firmware_rev]
                           .asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid device_info firmware_rev syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::os)) {
      str = (*message_)[strings::msg_params][strings::device_info][strings::os]
                .asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid device_info os syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::os_version)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::os_version]
                           .asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid device_info os_version syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::carrier)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::carrier]
                           .asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid device_info carrier syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::app_id)) {
    str = (*message_)[strings::msg_params][strings::app_id].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid app_id syntax check failed");
      return true;
    }
  }

  if (application_manager_.get_settings().use_full_app_id()) {
    if ((*message_)[strings::msg_params].keyExists(strings::full_app_id)) {
      str =
          (*message_)[strings::msg_params][strings::full_app_id].asCharArray();
      if (!CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid app_id syntax check failed");
        return true;
      }
    }
  }

  return false;
}

void RegisterAppInterfaceRequest::CheckResponseVehicleTypeParam(
    smart_objects::SmartObject& vehicle_type,
    const std::string& param,
    const std::string& backup_value) {
  using namespace hmi_response;
  if (!vehicle_type.keyExists(param) || vehicle_type[param].empty()) {
    if (!backup_value.empty()) {
      SDL_LOG_DEBUG(param << " is missing."
                             "Will be replaced with policy table value.");
      vehicle_type[param] = backup_value;
    } else {
      vehicle_type.erase(param);
    }
  }
}

void RegisterAppInterfaceRequest::SendSubscribeCustomButtonNotification() {
  using namespace smart_objects;
  SmartObject msg_params = SmartObject(SmartType_Map);
  msg_params[strings::app_id] = connection_key();
  msg_params[strings::name] = hmi_apis::Common_ButtonName::CUSTOM_BUTTON;
  msg_params[strings::is_suscribed] = true;
  CreateHMINotification(hmi_apis::FunctionID::Buttons_OnButtonSubscription,
                        msg_params);
}

bool RegisterAppInterfaceRequest::IsApplicationSwitched() {
  const std::string& policy_app_id =
      application_manager_.GetCorrectMobileIDFromMessage(message_);

  SDL_LOG_DEBUG("Looking for application id " << policy_app_id);
  auto app =
      application_manager_.reregister_application_by_policy_id(policy_app_id);

  if (!app) {
    SDL_LOG_DEBUG("Application with policy id " << policy_app_id
                                                << " is not found.");
    return false;
  }

  SDL_LOG_DEBUG("Application with policy id " << policy_app_id << " is found.");

  const auto app_device_handle = app->device();
  if (app_device_handle == device_handle_) {
    SDL_LOG_DEBUG("Application " << policy_app_id
                                 << " is already registered from this device.");
    SendResponse(false, mobile_apis::Result::APPLICATION_REGISTERED_ALREADY);
    return true;
  }

  SDL_LOG_DEBUG("Application is found in reconnection list.");

  auto app_type = ApplicationType::kSwitchedApplicationWrongHashId;
  if ((*message_)[strings::msg_params].keyExists(strings::hash_id)) {
    const auto hash_id =
        (*message_)[strings::msg_params][strings::hash_id].asString();

    auto& resume_ctrl = application_manager_.resume_controller();
    if (resume_ctrl.CheckApplicationHash(app, hash_id)) {
      app_type = ApplicationType::kSwitchedApplicationHashOk;
    }
  }

  application_manager_.ProcessReconnection(app, connection_key());

  const std::string additional_info;
  SendRegisterAppInterfaceResponseToMobile(
      app_type, policy::StatusNotifier(), additional_info);

  auto notification = MessageHelper::CreateHMIStatusNotification(
      app, mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  application_manager_.GetRPCService().ManageMobileCommand(notification,
                                                           SOURCE_SDL);

  application_manager_.OnApplicationSwitched(app);

  return true;
}

bool RegisterAppInterfaceRequest::GetDataOnSessionKey(
    const uint32_t key,
    connection_handler::DeviceHandle* device_id,
    std::string* mac_address) const {
  SDL_LOG_AUTO_TRACE();

  if ((nullptr == mac_address) && (nullptr == device_id)) {
    SDL_LOG_ERROR(
        "Can't get data on session key because device id and mac "
        "address are empty.");
    return false;
  }

  connection_handler::DeviceHandle device_handle = 0;
  auto& connect_handler = application_manager_.connection_handler();

  auto result = connect_handler.GetDataOnSessionKey(
      connection_key(), nullptr, nullptr, &device_handle);

  if (result) {
    SDL_LOG_DEBUG(
        "Failed to get device info for connection key: " << connection_key());
    return false;
  }

  if (mac_address) {
    result = connect_handler.get_session_observer().GetDataOnDeviceID(
        device_handle, nullptr, nullptr, mac_address, nullptr);
  }

  if (result) {
    SDL_LOG_DEBUG("Failed get unique address info for connection key: "
                  << connection_key());
    return false;
  }

  if (device_id) {
    *device_id = device_handle;
  }

  return true;
}

mobile_apis::Result::eType
RegisterAppInterfaceRequest::ProcessingAppHMITypesInMessage(
    const smart_objects::SmartObject& message) {
  const bool app_hmi_types_exist =
      message[strings::msg_params].keyExists(strings::app_hmi_type);

  const auto result = mobile_apis::Result::SUCCESS;

  if (!app_hmi_types_exist) {
    return result;
  }

  auto app_types_in_message =
      *(message[strings::msg_params][strings::app_hmi_type].asArray());

  auto it = std::find(app_types_in_message.begin(),
                      app_types_in_message.end(),
                      mobile_apis::AppHMIType::WEB_VIEW);
  if (app_types_in_message.end() != it) {
    return mobile_apis::Result::DISALLOWED;
  }

  return result;
}

mobile_apis::Result::eType
RegisterAppInterfaceRequest::ProcessingAppHMITypesPolicies(
    smart_objects::SmartObject& message,
    policy::StringArray& app_hmi_types_in_policy) {
  auto result = mobile_apis::Result::SUCCESS;
  const bool app_hmi_types_exist =
      message[strings::msg_params].keyExists(strings::app_hmi_type);

  if (app_hmi_types_exist) {
    // If AppHmiTypes are partially same, the system should allow those listed
    // in the policy table and send warning info on missed values
    auto app_types_in_message =
        *(message[strings::msg_params][strings::app_hmi_type].asArray());

    std::string log;
    CheckMissedTypes checker(app_hmi_types_in_policy, log);
    std::for_each(
        app_types_in_message.begin(), app_types_in_message.end(), checker);
    if (!log.empty()) {
      result = log.find("WEB_VIEW") != std::string::npos
                   ? mobile_apis::Result::DISALLOWED
                   : mobile_apis::Result::WARNINGS;

      if (mobile_apis::Result::DISALLOWED == result) {
        response_info_ =
            "WEB_VIEW AppHmiType is absent in application policies";
        SDL_LOG_DEBUG(response_info_);
        return result;
      }

      response_info_ =
          "Following AppHmiTypes are not present in policy "
          "table:" +
          log;
      are_hmi_types_invalid_ = true;
      result = mobile_apis::Result::WARNINGS;
    }
  }
  // Replace AppHmiTypes in request with values allowed by policy table
  message[strings::msg_params][strings::app_hmi_type] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  auto& app_hmi_type = message[strings::msg_params][strings::app_hmi_type];

  AppHMITypeInserter inserter(app_hmi_type);
  std::for_each(
      app_hmi_types_in_policy.begin(), app_hmi_types_in_policy.end(), inserter);

  return result;
}

policy::PolicyHandlerInterface&
RegisterAppInterfaceRequest::GetPolicyHandler() {
  return policy_handler_;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
