/*

 Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/commands/mobile/register_app_interface_request.h"

#include <unistd.h>
#include <algorithm>
#include <map>
#include <string.h>

#include <utils/make_shared.h>
#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/application_impl.h"
#include "application_manager/app_launch/app_launch_ctrl.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/helpers/application_helper.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/generated_msg_version.h"

namespace {
namespace custom_str = utils::custom_string;

mobile_apis::AppHMIType::eType StringToAppHMIType(const std::string& str) {
  if ("DEFAULT" == str) {
    return mobile_apis::AppHMIType::DEFAULT;
  } else if ("COMMUNICATION" == str) {
    return mobile_apis::AppHMIType::COMMUNICATION;
  } else if ("MEDIA" == str) {
    return mobile_apis::AppHMIType::MEDIA;
  } else if ("MESSAGING" == str) {
    return mobile_apis::AppHMIType::MESSAGING;
  } else if ("NAVIGATION" == str) {
    return mobile_apis::AppHMIType::NAVIGATION;
  } else if ("INFORMATION" == str) {
    return mobile_apis::AppHMIType::INFORMATION;
  } else if ("SOCIAL" == str) {
    return mobile_apis::AppHMIType::SOCIAL;
  } else if ("BACKGROUND_PROCESS" == str) {
    return mobile_apis::AppHMIType::BACKGROUND_PROCESS;
  } else if ("TESTING" == str) {
    return mobile_apis::AppHMIType::TESTING;
  } else if ("SYSTEM" == str) {
    return mobile_apis::AppHMIType::SYSTEM;
  } else if ("PROJECTION" == str) {
    return mobile_apis::AppHMIType::PROJECTION;
#ifdef SDL_REMOTE_CONTROL
  } else if ("REMOTE_CONTROL" == str) {
    return mobile_apis::AppHMIType::REMOTE_CONTROL;
#endif
  } else {
    return mobile_apis::AppHMIType::INVALID_ENUM;
  }
}

std::string AppHMITypeToString(mobile_apis::AppHMIType::eType type) {
  const std::map<mobile_apis::AppHMIType::eType, std::string> app_hmi_type_map =
      {{mobile_apis::AppHMIType::DEFAULT, "DEFAULT"},
#ifdef SDL_REMOTE_CONTROL
       {mobile_apis::AppHMIType::REMOTE_CONTROL, "REMOTE_CONTROL"},
#endif  // SDL_REMOTE_CONTROL
       {mobile_apis::AppHMIType::COMMUNICATION, "COMMUNICATION"},
       {mobile_apis::AppHMIType::MEDIA, "MEDIA"},
       {mobile_apis::AppHMIType::MESSAGING, "MESSAGING"},
       {mobile_apis::AppHMIType::NAVIGATION, "NAVIGATION"},
       {mobile_apis::AppHMIType::INFORMATION, "INFORMATION"},
       {mobile_apis::AppHMIType::SOCIAL, "SOCIAL"},
       {mobile_apis::AppHMIType::BACKGROUND_PROCESS, "BACKGROUND_PROCESS"},
       {mobile_apis::AppHMIType::TESTING, "TESTING"},
       {mobile_apis::AppHMIType::SYSTEM, "SYSTEM"},
       {mobile_apis::AppHMIType::PROJECTION, "PROJECTION"}};

  std::map<mobile_apis::AppHMIType::eType, std::string>::const_iterator iter =
      app_hmi_type_map.find(type);

  return app_hmi_type_map.end() != iter ? iter->second : std::string("");
}

struct AppHMITypeInserter {
  AppHMITypeInserter(smart_objects::SmartObject& so_array)
      : index_(0), so_array_(so_array) {}

  bool operator()(const std::string& app_hmi_type) {
    so_array_[index_] = StringToAppHMIType(app_hmi_type);
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
    std::string app_type_str = AppHMITypeToString(
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
  IsSameNickname(const custom_str::CustomString& app_id) : app_id_(app_id) {}
  bool operator()(const policy::StringArray::value_type& nickname) const {
    return app_id_.CompareIgnoreCase(nickname.c_str());
  }

 private:
  const custom_str::CustomString& app_id_;
};
}

namespace application_manager {

namespace commands {

RegisterAppInterfaceRequest::RegisterAppInterfaceRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , result_checking_app_hmi_type_(mobile_apis::Result::INVALID_ENUM) {}

RegisterAppInterfaceRequest::~RegisterAppInterfaceRequest() {}

bool RegisterAppInterfaceRequest::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  return true;
}

void RegisterAppInterfaceRequest::Run() {
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Connection key is " << connection_key());

  // Fix problem with SDL and HMI HTML. This problem is not actual for HMI PASA.
  // Flag conditional compilation specific to customer is used in order to
  // exclude hit code
  // to RTC
  // FIXME(EZamakhov): on shutdown - get freez

  // wait till HMI started
  while (!application_manager_.IsStopping() &&
         !application_manager_.IsHMICooperating()) {
    LOG4CXX_DEBUG(logger_,
                  "Waiting for the HMI... conn_key="
                      << connection_key()
                      << ", correlation_id=" << correlation_id()
                      << ", default_timeout=" << default_timeout()
                      << ", thread=" << pthread_self());
    application_manager_.updateRequestTimeout(
        connection_key(), correlation_id(), default_timeout());
    sleep(1);
    // TODO(DK): timer_->StartWait(1);
  }

  if (application_manager_.IsStopping()) {
    LOG4CXX_WARN(logger_, "The ApplicationManager is stopping!");
    return;
  }

  if (IsApplicationSwitched()) {
    return;
  }

  const std::string mobile_app_id =
      (*message_)[strings::msg_params][strings::app_id].asString();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_REGISTERED_ALREADY);
    return;
  }

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  const std::string policy_app_id = msg_params[strings::app_id].asString();
  std::string new_policy_app_id = policy_app_id;
  std::transform(policy_app_id.begin(),
                 policy_app_id.end(),
                 new_policy_app_id.begin(),
                 ::tolower);
  (*message_)[strings::msg_params][strings::app_id] = new_policy_app_id;

  if (application_manager_.IsApplicationForbidden(connection_key(),
                                                  policy_app_id)) {
    SendResponse(false, mobile_apis::Result::TOO_MANY_PENDING_REQUESTS);
    return;
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
    SendResponse(false, policy_result);
    return;
  }

  mobile_apis::Result::eType coincidence_result = CheckCoincidence();

  if (mobile_apis::Result::SUCCESS != coincidence_result) {
    LOG4CXX_ERROR(logger_, "Coincidence check failed.");
    if (mobile_apis::Result::DUPLICATE_NAME == coincidence_result) {
      usage_statistics::AppCounter count_of_rejections_duplicate_name(
          GetPolicyHandler().GetStatisticManager(),
          policy_app_id,
          usage_statistics::REJECTIONS_DUPLICATE_NAME);
      ++count_of_rejections_duplicate_name;
    }
    SendResponse(false, coincidence_result);
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_INFO(logger_,
                 "Incoming register app interface has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  application = application_manager_.RegisterApplication(message_);

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application hasn't been registered!");
    return;
  }
  // For resuming application need to restore hmi_app_id from resumeCtrl
  resumption::ResumeCtrl& resumer = application_manager_.resume_controller();
  const std::string& device_mac = application->mac_address();

  // there is side affect with 2 mobile app with the same mobile app_id
  if (resumer.IsApplicationSaved(policy_app_id, device_mac)) {
    application->set_hmi_application_id(
        resumer.GetHMIApplicationID(policy_app_id, device_mac));
  } else {
    application->set_hmi_application_id(
        application_manager_.GenerateNewHMIAppID());
  }

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
    application->set_tts_name(msg_params[strings::tts_name]);
  }

  if (msg_params.keyExists(strings::app_hmi_type)) {
    application->set_app_types(msg_params[strings::app_hmi_type]);

    // check app type
    const smart_objects::SmartObject& app_type =
        msg_params.getElement(strings::app_hmi_type);

    for (size_t i = 0; i < app_type.length(); ++i) {
      if (mobile_apis::AppHMIType::NAVIGATION ==
          static_cast<mobile_apis::AppHMIType::eType>(
              app_type.getElement(i).asUInt())) {
        application->set_is_navi(true);
      }
      if (mobile_apis::AppHMIType::COMMUNICATION ==
          static_cast<mobile_apis::AppHMIType::eType>(
              app_type.getElement(i).asUInt())) {
        application->set_voice_communication_supported(true);
      }
      if (mobile_apis::AppHMIType::PROJECTION ==
          static_cast<mobile_apis::AppHMIType::eType>(
              app_type.getElement(i).asUInt())) {
        application->set_mobile_projection_enabled(true);
      }
    }
  }

  // Add device to policy table and set device info, if any
  policy::DeviceParams dev_params;
  if (-1 ==
      application_manager_.connection_handler()
          .get_session_observer()
          .GetDataOnDeviceID(application->device(),
                             &dev_params.device_name,
                             NULL,
                             &dev_params.device_mac_address,
                             &dev_params.device_connection_type)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to extract information for device "
                      << application->device());
  }
  policy::DeviceInfo device_info;
  device_info.AdoptDeviceType(dev_params.device_connection_type);
  if (msg_params.keyExists(strings::device_info)) {
    FillDeviceInfo(&device_info);
  }

  GetPolicyHandler().SetDeviceInfo(device_mac, device_info);

  SendRegisterAppInterfaceResponseToMobile(AppicationType::kNewApplication);
  smart_objects::SmartObjectSPtr so =
      GetLockScreenIconUrlNotification(connection_key(), application);
  application_manager_.ManageMobileCommand(so, commands::Command::ORIGIN_SDL);
}

smart_objects::SmartObjectSPtr
RegisterAppInterfaceRequest::GetLockScreenIconUrlNotification(
    const uint32_t connection_key, ApplicationSharedPtr app) {
  DCHECK_OR_RETURN(app.get(), smart_objects::SmartObjectSPtr());
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemRequestID;
  (*message)[strings::params][strings::connection_key] = connection_key;
  (*message)[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  (*message)[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  (*message)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  (*message)[strings::msg_params][strings::request_type] =
      mobile_apis::RequestType::LOCK_SCREEN_ICON_URL;
  (*message)[strings::msg_params][strings::url] =
      GetPolicyHandler().GetLockScreenIconUrl();
  return message;
}

void FillVRRelatedFields(smart_objects::SmartObject& response_params,
                         const HMICapabilities& hmi_capabilities) {
  response_params[strings::language] = hmi_capabilities.active_vr_language();
  if (hmi_capabilities.vr_capabilities()) {
    response_params[strings::vr_capabilities] =
        *hmi_capabilities.vr_capabilities();
  }
}

void FillVIRelatedFields(smart_objects::SmartObject& response_params,
                         const HMICapabilities& hmi_capabilities) {
  if (hmi_capabilities.vehicle_type()) {
    response_params[hmi_response::vehicle_type] =
        *hmi_capabilities.vehicle_type();
  }
}

void FillTTSRelatedFields(smart_objects::SmartObject& response_params,
                          const HMICapabilities& hmi_capabilities) {
  response_params[strings::language] = hmi_capabilities.active_tts_language();
  if (hmi_capabilities.speech_capabilities()) {
    response_params[strings::speech_capabilities] =
        *hmi_capabilities.speech_capabilities();
  }
  if (hmi_capabilities.prerecorded_speech()) {
    response_params[strings::prerecorded_speech] =
        *(hmi_capabilities.prerecorded_speech());
  }
}

void FillUIRelatedFields(smart_objects::SmartObject& response_params,
                         const HMICapabilities& hmi_capabilities) {
  response_params[strings::hmi_display_language] =
      hmi_capabilities.active_ui_language();
  if (hmi_capabilities.display_capabilities()) {
    response_params[hmi_response::display_capabilities] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& display_caps =
        response_params[hmi_response::display_capabilities];

    display_caps[hmi_response::display_type] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::display_type);

    display_caps[hmi_response::text_fields] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::text_fields);

    display_caps[hmi_response::image_fields] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::image_fields);

    display_caps[hmi_response::media_clock_formats] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::media_clock_formats);

    display_caps[hmi_response::templates_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::templates_available);

    display_caps[hmi_response::screen_params] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::screen_params);

    display_caps[hmi_response::num_custom_presets_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::num_custom_presets_available);

    display_caps[hmi_response::graphic_supported] =
        (hmi_capabilities.display_capabilities()
             ->getElement(hmi_response::image_capabilities)
             .length() > 0);

    display_caps[hmi_response::templates_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::templates_available);

    display_caps[hmi_response::screen_params] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::screen_params);

    display_caps[hmi_response::num_custom_presets_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::num_custom_presets_available);
  }

  if (hmi_capabilities.audio_pass_thru_capabilities()) {
    if (smart_objects::SmartType_Array ==
        hmi_capabilities.audio_pass_thru_capabilities()->getType()) {
      // hmi_capabilities json contains array and HMI response object
      response_params[strings::audio_pass_thru_capabilities] =
          *hmi_capabilities.audio_pass_thru_capabilities();
    } else {
      response_params[strings::audio_pass_thru_capabilities][0] =
          *hmi_capabilities.audio_pass_thru_capabilities();
    }
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
}

void RegisterAppInterfaceRequest::SendRegisterAppInterfaceResponseToMobile(
    AppicationType app_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);

  mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;

  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();

  const uint32_t key = connection_key();
  ApplicationSharedPtr application = application_manager_.application(key);

  resumption::ResumeCtrl& resumer = application_manager_.resume_controller();

  if (!application) {
    LOG4CXX_ERROR(logger_,
                  "There is no application for such connection key" << key);
    LOG4CXX_DEBUG(logger_, "Need to start resume data persistent timer");
    resumer.OnAppRegistrationEnd();
    return;
  }

  response_params[strings::sync_msg_version][strings::major_version] =
      major_version;  // From generated file interfaces/generated_msg_version.h
  response_params[strings::sync_msg_version][strings::minor_version] =
      minor_version;  // From generated file interfaces/generated_msg_version.h
  response_params[strings::sync_msg_version][strings::patch_version] =
      patch_version;  // From generated file interfaces/generated_msg_version.h

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (msg_params[strings::language_desired].asInt() !=
          hmi_capabilities.active_vr_language() ||
      msg_params[strings::hmi_display_language_desired].asInt() !=
          hmi_capabilities.active_ui_language()) {
    LOG4CXX_WARN(logger_,
                 "Wrong language on registering application "
                     << application->name().c_str());

    LOG4CXX_ERROR(
        logger_,
        "VR language desired code is "
            << msg_params[strings::language_desired].asInt()
            << " , active VR language code is "
            << hmi_capabilities.active_vr_language() << ", UI language code is "
            << msg_params[strings::hmi_display_language_desired].asInt()
            << " , active UI language code is "
            << hmi_capabilities.active_ui_language());

    result_code = mobile_apis::Result::WRONG_LANGUAGE;
  }

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

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_TTS)) {
    FillTTSRelatedFields(response_params, hmi_capabilities);
  }

  if (hmi_capabilities.pcm_stream_capabilities()) {
    response_params[strings::pcm_stream_capabilities] =
        *hmi_capabilities.pcm_stream_capabilities();
  }

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_VehicleInfo)) {
    FillVIRelatedFields(response_params, hmi_capabilities);
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
  const std::string ccpu_version =
      application_manager_.hmi_capabilities().ccpu_version();
  if (!ccpu_version.empty()) {
    response_params[strings::system_software_version] = ccpu_version;
  }

  bool resumption =
      (*message_)[strings::msg_params].keyExists(strings::hash_id);

  bool need_restore_vr = resumption;

  std::string hash_id;
  std::string add_info;
  if (resumption) {
    hash_id = (*message_)[strings::msg_params][strings::hash_id].asString();
    if (!resumer.CheckApplicationHash(application, hash_id)) {
      LOG4CXX_WARN(logger_,
                   "Hash from RAI does not match to saved resume data.");
      result_code = mobile_apis::Result::RESUME_FAILED;
      add_info = "Hash from RAI does not match to saved resume data.";
      need_restore_vr = false;
    } else if (!resumer.CheckPersistenceFilesForResumption(application)) {
      LOG4CXX_WARN(logger_, "Persistent data is missing.");
      result_code = mobile_apis::Result::RESUME_FAILED;
      add_info = "Persistent data is missing.";
      need_restore_vr = false;
    } else {
      add_info = "Resume succeeded.";
    }
  }
  if ((mobile_apis::Result::SUCCESS == result_code) &&
      (mobile_apis::Result::INVALID_ENUM != result_checking_app_hmi_type_)) {
    add_info += response_info_;
    result_code = result_checking_app_hmi_type_;
  }

  if (AppicationType::kSwitchedApplication == app_type) {
    LOG4CXX_DEBUG(logger_,
                  "Application has been switched from another transport.");

    if (hash_id.empty() && resumer.CheckApplicationHash(application, hash_id)) {
      LOG4CXX_INFO(logger_,
                   "Application does not have hashID saved and neither "
                   "provided it with RAI. Nothing to resume.");
      result_code = mobile_apis::Result::SUCCESS;
    } else if (!resumption ||
               mobile_apis::Result::RESUME_FAILED == result_code) {
      application_manager::RecallApplicationData(application,
                                                 application_manager_);
      resumer.RemoveApplicationFromSaved(application);
      result_code = mobile_apis::Result::RESUME_FAILED;
    }

    SendResponse(true, result_code, add_info.c_str(), &response_params);
    return;
  }

  // in case application exist in resumption we need to send resumeVrgrammars
  if (false == resumption) {
    resumption = resumer.IsApplicationSaved(application->policy_app_id(),
                                            application->mac_address());
  }

  AppHmiTypes hmi_types;
  if ((*message_)[strings::msg_params].keyExists(strings::app_hmi_type)) {
    smart_objects::SmartArray* hmi_types_ptr =
        (*message_)[strings::msg_params][strings::app_hmi_type].asArray();
    DCHECK_OR_RETURN_VOID(hmi_types_ptr);
    SmartArrayValueExtractor extractor;
    if (hmi_types_ptr && 0 < hmi_types_ptr->size()) {
      std::transform(hmi_types_ptr->begin(),
                     hmi_types_ptr->end(),
                     std::back_inserter(hmi_types),
                     extractor);
    }
  }
  policy::StatusNotifier notify_upd_manager = GetPolicyHandler().AddApplication(
      application->policy_app_id(), hmi_types);
  SendResponse(true, result_code, add_info.c_str(), &response_params);
  SendOnAppRegisteredNotificationToHMI(
      *(application.get()), resumption, need_restore_vr);
#ifdef SDL_REMOTE_CONTROL
  if (msg_params.keyExists(strings::app_hmi_type)) {
    GetPolicyHandler().SetDefaultHmiTypes(application->policy_app_id(),
                                          &(msg_params[strings::app_hmi_type]));
  }
#endif  // SDL_REMOTE_CONTROL

  // Default HMI level should be set before any permissions validation, since it
  // relies on HMI level.
  application_manager_.OnApplicationRegistered(application);
  (*notify_upd_manager)();

  // Start PTU after successfull registration
  // Sends OnPermissionChange notification to mobile right after RAI response
  // and HMI level set-up
  GetPolicyHandler().OnAppRegisteredOnMobile(application->policy_app_id());

  if (result_code != mobile_apis::Result::RESUME_FAILED) {
    resumer.StartResumption(application, hash_id);
  } else {
    resumer.StartResumptionOnlyHMILevel(application);
  }

  // By default app subscribed to CUSTOM_BUTTON
  SendSubscribeCustomButtonNotification();
  SendChangeRegistrationOnHMI(application);
}

// DEPRECATED
void RegisterAppInterfaceRequest::SendRegisterAppInterfaceResponseToMobile() {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);

  mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;

  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();

  const uint32_t key = connection_key();
  ApplicationSharedPtr application = application_manager_.application(key);

  resumption::ResumeCtrl& resumer = application_manager_.resume_controller();

  if (!application) {
    LOG4CXX_ERROR(logger_,
                  "There is no application for such connection key" << key);
    LOG4CXX_DEBUG(logger_, "Need to start resume data persistent timer");
    resumer.OnAppRegistrationEnd();
    return;
  }

  response_params[strings::sync_msg_version][strings::major_version] =
      major_version;  // From generated file interfaces/generated_msg_version.h
  response_params[strings::sync_msg_version][strings::minor_version] =
      minor_version;  // From generated file interfaces/generated_msg_version.h
  response_params[strings::sync_msg_version][strings::patch_version] =
      patch_version;  // From generated file interfaces/generated_msg_version.h

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (msg_params[strings::language_desired].asInt() !=
          hmi_capabilities.active_vr_language() ||
      msg_params[strings::hmi_display_language_desired].asInt() !=
          hmi_capabilities.active_ui_language()) {
    LOG4CXX_WARN(logger_,
                 "Wrong language on registering application "
                     << application->name().c_str());

    LOG4CXX_ERROR(
        logger_,
        "VR language desired code is "
            << msg_params[strings::language_desired].asInt()
            << " , active VR language code is "
            << hmi_capabilities.active_vr_language() << ", UI language code is "
            << msg_params[strings::hmi_display_language_desired].asInt()
            << " , active UI language code is "
            << hmi_capabilities.active_ui_language());

    result_code = mobile_apis::Result::WRONG_LANGUAGE;
  }

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

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_TTS)) {
    FillTTSRelatedFields(response_params, hmi_capabilities);
  }

  if (hmi_capabilities.pcm_stream_capabilities()) {
    response_params[strings::pcm_stream_capabilities] =
        *hmi_capabilities.pcm_stream_capabilities();
  }

  if (HmiInterfaces::STATE_NOT_AVAILABLE !=
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_VehicleInfo)) {
    FillVIRelatedFields(response_params, hmi_capabilities);
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
  const std::string ccpu_version =
      application_manager_.hmi_capabilities().ccpu_version();
  if (!ccpu_version.empty()) {
    response_params[strings::system_software_version] = ccpu_version;
  }

  bool resumption =
      (*message_)[strings::msg_params].keyExists(strings::hash_id);

  bool need_restore_vr = resumption;

  std::string hash_id;
  std::string add_info;
  if (resumption) {
    hash_id = (*message_)[strings::msg_params][strings::hash_id].asString();
    if (!resumer.CheckApplicationHash(application, hash_id)) {
      LOG4CXX_WARN(logger_,
                   "Hash from RAI does not match to saved resume data.");
      result_code = mobile_apis::Result::RESUME_FAILED;
      add_info = "Hash from RAI does not match to saved resume data.";
      need_restore_vr = false;
    } else if (!resumer.CheckPersistenceFilesForResumption(application)) {
      LOG4CXX_WARN(logger_, "Persistent data is missing.");
      result_code = mobile_apis::Result::RESUME_FAILED;
      add_info = "Persistent data is missing.";
      need_restore_vr = false;
    } else {
      add_info = "Resume succeeded.";
    }
  }
  if ((mobile_apis::Result::SUCCESS == result_code) &&
      (mobile_apis::Result::INVALID_ENUM != result_checking_app_hmi_type_)) {
    add_info += response_info_;
    result_code = result_checking_app_hmi_type_;
  }

  // in case application exist in resumption we need to send resumeVrgrammars
  if (false == resumption) {
    resumption = resumer.IsApplicationSaved(application->policy_app_id(),
                                            application->mac_address());
  }

  AppHmiTypes hmi_types;
  if ((*message_)[strings::msg_params].keyExists(strings::app_hmi_type)) {
    smart_objects::SmartArray* hmi_types_ptr =
        (*message_)[strings::msg_params][strings::app_hmi_type].asArray();
    DCHECK_OR_RETURN_VOID(hmi_types_ptr);
    SmartArrayValueExtractor extractor;
    if (hmi_types_ptr && 0 < hmi_types_ptr->size()) {
      std::transform(hmi_types_ptr->begin(),
                     hmi_types_ptr->end(),
                     std::back_inserter(hmi_types),
                     extractor);
    }
  }
  policy::StatusNotifier notify_upd_manager = GetPolicyHandler().AddApplication(
      application->policy_app_id(), hmi_types);
  SendResponse(true, result_code, add_info.c_str(), &response_params);
  SendOnAppRegisteredNotificationToHMI(
      *(application.get()), resumption, need_restore_vr);
#ifdef SDL_REMOTE_CONTROL
  if (msg_params.keyExists(strings::app_hmi_type)) {
    GetPolicyHandler().SetDefaultHmiTypes(application->policy_app_id(),
                                          &(msg_params[strings::app_hmi_type]));
  }
#endif  // SDL_REMOTE_CONTROL

  // Default HMI level should be set before any permissions validation, since it
  // relies on HMI level.
  application_manager_.OnApplicationRegistered(application);
  (*notify_upd_manager)();

  // Start PTU after successfull registration
  // Sends OnPermissionChange notification to mobile right after RAI response
  // and HMI level set-up
  GetPolicyHandler().OnAppRegisteredOnMobile(application->policy_app_id());

  if (result_code != mobile_apis::Result::RESUME_FAILED) {
    resumer.StartResumption(application, hash_id);
  } else {
    resumer.StartResumptionOnlyHMILevel(application);
  }

  // By default app subscribed to CUSTOM_BUTTON
  SendSubscribeCustomButtonNotification();
  SendChangeRegistrationOnHMI(application);
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
    LOG4CXX_DEBUG(logger_, "Interface " << interface << "is not avaliable");
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
    const Application& application_impl,
    bool resumption,
    bool need_restore_vr) {
  using namespace smart_objects;
  SmartObjectSPtr notification = utils::MakeShared<SmartObject>(SmartType_Map);
  if (!notification) {
    LOG4CXX_ERROR(logger_, "Failed to create smart object");
    return;
  }

  (*notification)[strings::params] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& params = (*notification)[strings::params];
  params[strings::function_id] = static_cast<int32_t>(
      hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
  params[strings::message_type] = static_cast<int32_t>(kNotification);
  params[strings::protocol_version] = commands::CommandImpl::protocol_version_;
  params[strings::protocol_type] = commands::CommandImpl::hmi_protocol_type_;

  (*notification)[strings::msg_params] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& msg_params = (*notification)[strings::msg_params];
  // Due to current requirements in case when we're in resumption mode
  // we have to always send resumeVRGrammar field.
  if (resumption) {
    msg_params[strings::resume_vr_grammars] = need_restore_vr;
  }

  if (application_impl.vr_synonyms()) {
    msg_params[strings::vr_synonyms] = *(application_impl.vr_synonyms());
  }

  if (application_impl.tts_name()) {
    msg_params[strings::tts_name] = *(application_impl.tts_name());
  }

  std::string priority;
  GetPolicyHandler().GetPriority(application_impl.policy_app_id(), &priority);

  if (!priority.empty()) {
    msg_params[strings::priority] = MessageHelper::GetPriorityCode(priority);
  }

  msg_params[strings::msg_params] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& application = msg_params[strings::application];
  application[strings::app_name] = application_impl.name();
  application[strings::app_id] = application_impl.app_id();
  application[hmi_response::policy_app_id] = application_impl.policy_app_id();
  application[strings::icon] = application_impl.app_icon_path();

  const smart_objects::SmartObject* ngn_media_screen_name =
      application_impl.ngn_media_screen_name();
  if (ngn_media_screen_name) {
    application[strings::ngn_media_screen_app_name] = *ngn_media_screen_name;
  }

  application[strings::hmi_display_language_desired] =
      static_cast<int32_t>(application_impl.ui_language());

  application[strings::is_media_application] =
      application_impl.is_media_application();

  const smart_objects::SmartObject* app_type = application_impl.app_types();
  if (app_type) {
    application[strings::app_type] = *app_type;
  }

  std::vector<std::string> request_types =
      GetPolicyHandler().GetAppRequestTypes(application_impl.policy_app_id());

  application[strings::request_type] = SmartObject(SmartType_Array);
  smart_objects::SmartObject& request_array =
      application[strings::request_type];

  uint32_t index = 0;
  std::vector<std::string>::const_iterator it = request_types.begin();
  for (; request_types.end() != it; ++it) {
    request_array[index] = *it;
    ++index;
  }

  application[strings::device_info] = SmartObject(SmartType_Map);
  smart_objects::SmartObject& device_info = application[strings::device_info];
  const protocol_handler::SessionObserver& session_observer =
      application_manager_.connection_handler().get_session_observer();
  std::string device_name;
  std::string mac_address;
  std::string transport_type;
  const connection_handler::DeviceHandle handle = application_impl.device();
  if (-1 ==
      session_observer.GetDataOnDeviceID(
          handle, &device_name, NULL, &mac_address, &transport_type)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to extract information for device " << handle);
  }

  device_info[strings::name] = device_name;
  device_info[strings::id] = mac_address;

  const policy::DeviceConsent device_consent =
      GetPolicyHandler().GetUserConsentForDevice(mac_address);
  device_info[strings::isSDLAllowed] =
      policy::DeviceConsent::kDeviceAllowed == device_consent;

  device_info[strings::transport_type] =
      application_manager_.GetDeviceTransportType(transport_type);

  DCHECK(application_manager_.ManageHMICommand(notification));
}

mobile_apis::Result::eType RegisterAppInterfaceRequest::CheckCoincidence() {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  ApplicationSet accessor = application_manager_.applications().GetData();

  ApplicationSetConstIt it = accessor.begin();
  const custom_str::CustomString& app_name =
      msg_params[strings::app_name].asCustomString();

  for (; accessor.end() != it; ++it) {
    // name check
    const custom_str::CustomString& cur_name = (*it)->name();
    if (app_name.CompareIgnoreCase(cur_name)) {
      LOG4CXX_ERROR(logger_, "Application name is known already.");
      return mobile_apis::Result::DUPLICATE_NAME;
    }

    const smart_objects::SmartObject* vr = (*it)->vr_synonyms();
    const std::vector<smart_objects::SmartObject>* curr_vr = NULL;
    if (NULL != vr) {
      curr_vr = vr->asArray();
      CoincidencePredicateVR v(app_name);

      if (0 != std::count_if(curr_vr->begin(), curr_vr->end(), v)) {
        LOG4CXX_ERROR(logger_, "Application name is known already.");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }

    // vr check
    if (msg_params.keyExists(strings::vr_synonyms)) {
      const std::vector<smart_objects::SmartObject>* new_vr =
          msg_params[strings::vr_synonyms].asArray();

      CoincidencePredicateVR v(cur_name);
      if (0 != std::count_if(new_vr->begin(), new_vr->end(), v)) {
        LOG4CXX_ERROR(logger_, "vr_synonyms duplicated with app_name .");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }  // end vr check

  }  // application for end

  return mobile_apis::Result::SUCCESS;
}  // method end

mobile_apis::Result::eType RegisterAppInterfaceRequest::CheckWithPolicyData() {
  LOG4CXX_AUTO_TRACE(logger_);
  // TODO(AOleynik): Check is necessary to allow register application in case
  // of disabled policy
  // Remove this check, when HMI will support policy
  if (!GetPolicyHandler().PolicyEnabled()) {
    return mobile_apis::Result::WARNINGS;
  }

  smart_objects::SmartObject& message = *message_;
  policy::StringArray app_nicknames;
  policy::StringArray app_hmi_types;

  const std::string mobile_app_id =
      message[strings::msg_params][strings::app_id].asString();
  const bool init_result = GetPolicyHandler().GetInitialAppData(
      mobile_app_id, &app_nicknames, &app_hmi_types);

  if (!init_result) {
    LOG4CXX_ERROR(logger_, "Error during initial application data check.");
    return mobile_apis::Result::INVALID_DATA;
  }

  if (!app_nicknames.empty()) {
    IsSameNickname compare(
        message[strings::msg_params][strings::app_name].asCustomString());
    policy::StringArray::const_iterator it =
        std::find_if(app_nicknames.begin(), app_nicknames.end(), compare);
    if (app_nicknames.end() == it) {
      LOG4CXX_WARN(logger_,
                   "Application name was not found in nicknames list.");
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

  mobile_apis::Result::eType result = mobile_apis::Result::SUCCESS;

  // If AppHMIType is not included in policy - allow any type
  if (!app_hmi_types.empty()) {
    if (message[strings::msg_params].keyExists(strings::app_hmi_type)) {
      // If AppHmiTypes are partially same, the system should allow those listed
      // in the policy table and send warning info on missed values
      smart_objects::SmartArray app_types =
          *(message[strings::msg_params][strings::app_hmi_type].asArray());

      std::string log;
      CheckMissedTypes checker(app_hmi_types, log);
      std::for_each(app_types.begin(), app_types.end(), checker);
      if (!log.empty()) {
        response_info_ =
            "Following AppHmiTypes are not present in policy "
            "table:" +
            log;
        result_checking_app_hmi_type_ = mobile_apis::Result::WARNINGS;
      }
    }
    // Replace AppHmiTypes in request with values allowed by policy table
    message[strings::msg_params][strings::app_hmi_type] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    smart_objects::SmartObject& app_hmi_type =
        message[strings::msg_params][strings::app_hmi_type];

    AppHMITypeInserter inserter(app_hmi_type);
    std::for_each(app_hmi_types.begin(), app_hmi_types.end(), inserter);
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
  LOG4CXX_AUTO_TRACE(logger_);

  const custom_string::CustomString mobile_app_id =
      (*message_)[strings::msg_params][strings::app_id].asCustomString();

  const ApplicationSet& applications =
      application_manager_.applications().GetData();

  ApplicationSetConstIt it = applications.begin();
  ApplicationSetConstIt it_end = applications.end();

  for (; it != it_end; ++it) {
    if (mobile_app_id.CompareIgnoreCase((*it)->policy_app_id().c_str())) {
      return true;
    }
  }

  return false;
}

bool RegisterAppInterfaceRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  str = (*message_)[strings::msg_params][strings::app_name].asCharArray();
  if (!CheckSyntax(str)) {
    LOG4CXX_ERROR(logger_, "Invalid app_name syntax check failed");
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
        LOG4CXX_ERROR(logger_, "Invalid tts_name syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::ngn_media_screen_app_name)) {
    str = (*message_)[strings::msg_params][strings::ngn_media_screen_app_name]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid ngn_media_screen_app_name syntax check failed");
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
        LOG4CXX_ERROR(logger_, "Invalid vr_synonyms syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::hash_id)) {
    str = (*message_)[strings::msg_params][strings::hash_id].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid hash_id syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::device_info)) {
    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::hardware)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::hardware].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info hardware syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::firmware_rev)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::firmware_rev].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info firmware_rev syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::os)) {
      str = (*message_)[strings::msg_params][strings::device_info][strings::os]
                .asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid device_info os syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::os_version)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::os_version].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info os_version syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].keyExists(
            strings::carrier)) {
      str = (*message_)[strings::msg_params][strings::device_info]
                       [strings::carrier].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info carrier syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::app_id)) {
    str = (*message_)[strings::msg_params][strings::app_id].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid app_id syntax check failed");
      return true;
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
      LOG4CXX_DEBUG(logger_,
                    param << " is missing."
                             "Will be replaced with policy table value.");
      vehicle_type[param] = backup_value;
    } else {
      vehicle_type.erase(param);
    }
  }
}

void RegisterAppInterfaceRequest::SendSubscribeCustomButtonNotification() {
  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObject msg_params = SmartObject(SmartType_Map);
  msg_params[strings::app_id] = connection_key();
  msg_params[strings::name] = Common_ButtonName::CUSTOM_BUTTON;
  msg_params[strings::is_suscribed] = true;
  CreateHMINotification(FunctionID::Buttons_OnButtonSubscription, msg_params);
}

bool RegisterAppInterfaceRequest::IsApplicationSwitched() {
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  const std::string& policy_app_id = msg_params[strings::app_id].asString();

  LOG4CXX_DEBUG(logger_, "Looking for application id " << policy_app_id);

  auto app = application_manager_.application_by_policy_id(policy_app_id);

  if (!app) {
    LOG4CXX_DEBUG(logger_,
                  "Application with policy id " << policy_app_id
                                                << " is not found.");
    return false;
  }

  LOG4CXX_DEBUG(logger_,
                "Application with policy id " << policy_app_id << " is found.");
  if (!application_manager_.IsAppInReconnectMode(policy_app_id)) {
    LOG4CXX_DEBUG(logger_,
                  "Policy id " << policy_app_id
                               << " is not found in reconnection list.");
    SendResponse(false, mobile_apis::Result::APPLICATION_REGISTERED_ALREADY);
    return false;
  }

  LOG4CXX_DEBUG(logger_, "Application is found in reconnection list.");
  application_manager_.ProcessReconnection(app, connection_key());
  SendRegisterAppInterfaceResponseToMobile(
      AppicationType::kSwitchedApplication);

  application_manager_.SendHMIStatusNotification(app);

  application_manager_.OnApplicationSwitched(app);

  return true;
}

policy::PolicyHandlerInterface&
RegisterAppInterfaceRequest::GetPolicyHandler() {
  return application_manager_.GetPolicyHandler();
}

}  // namespace commands

}  // namespace application_manager
