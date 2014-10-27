/*

 Copyright (c) 2013, Ford Motor Company
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
#include <string.h>

#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"

namespace {

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
  } else {
    return mobile_apis::AppHMIType::INVALID_ENUM;
  }
}

struct AppHMITypeInserter {
    AppHMITypeInserter(smart_objects::SmartObject& so_array)
      : index_(0),
        so_array_(so_array) {
    }

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
      : policy_app_types_(policy_app_types),
        log_(log) {
    }

    bool operator()(const smart_objects::SmartArray::value_type& value) {
      std::string app_type_str = value.asString();
      policy::StringArray::const_iterator it = policy_app_types_.begin();
      policy::StringArray::const_iterator it_end = policy_app_types_.end();
      for (; it != it_end; ++it) {
        if (app_type_str == *it) {
          return true;
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
}

namespace application_manager {

namespace commands {

RegisterAppInterfaceRequest::RegisterAppInterfaceRequest(
  const MessageSharedPtr& message)
  : CommandRequestImpl(message) {
}

RegisterAppInterfaceRequest::~RegisterAppInterfaceRequest() {
}

bool RegisterAppInterfaceRequest::Init() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::Init");
  return true;
}

void RegisterAppInterfaceRequest::Run() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::Run " << connection_key());

  // Fix problem with SDL and HMI HTML. This problem is not actual for HMI PASA.
  // Flag conditional compilation "CUSTOMER_PASA" is used in order to exclude hit code
  // to RTC
  if (true == profile::Profile::instance()->launch_hmi()) {
    // wait till HMI started
    while (!ApplicationManagerImpl::instance()->IsHMICooperating()) {
      sleep(1);
      // TODO(DK): timer_->StartWait(1);
      ApplicationManagerImpl::instance()->updateRequestTimeout(connection_key(),
                                                               correlation_id(),
                                                               default_timeout());
    }
  }

  const std::string mobile_app_id = (*message_)[strings::msg_params][strings::app_id]
                                                               .asString();

  ApplicationSharedPtr application =
    ApplicationManagerImpl::instance()->application(connection_key());

  if (application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_REGISTERED_ALREADY);
    return;
  }

  if (IsApplicationWithSameAppIdRegistered()) {
    SendResponse(false, mobile_apis::Result::DISALLOWED);
    return;
  }

  mobile_apis::Result::eType policy_result = CheckWithPolicyData();
  if (mobile_apis::Result::SUCCESS != policy_result
      && mobile_apis::Result::WARNINGS != policy_result) {
    SendResponse(false, policy_result);
    return;
  }

  mobile_apis::Result::eType coincidence_result = CheckCoincidence();

  if (mobile_apis::Result::SUCCESS != coincidence_result) {
    LOG4CXX_ERROR_EXT(logger_, "Coincidence check failed.");
    if (mobile_apis::Result::DUPLICATE_NAME == coincidence_result) {
      usage_statistics::AppCounter count_of_rejections_duplicate_name(
        policy::PolicyHandler::instance()->GetStatisticManager(), mobile_app_id,
        usage_statistics::REJECTIONS_DUPLICATE_NAME);
      ++count_of_rejections_duplicate_name;
    }
    SendResponse(false, coincidence_result);
    return;
  }

  mobile_apis::Result::eType restriction_result = CheckRestrictions();
  if (mobile_apis::Result::SUCCESS != restriction_result) {
    LOG4CXX_ERROR_EXT(logger_, "Param names restrictions check failed.");
    SendResponse(false, restriction_result);
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_INFO(logger_,
                  "Incoming register app interface has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  const smart_objects::SmartObject& msg_params =
    (*message_)[strings::msg_params];

  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()->RegisterApplication(message_);

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "Application " <<
                      msg_params[strings::app_name].asString() <<
                      "  hasn't been registered!");
  } else {

    // For resuming application need to restore hmi_app_id from resumeCtrl
    const std::string mobile_app_id = msg_params[strings::app_id].asString();
    ResumeCtrl& resumer = ApplicationManagerImpl::instance()->resume_controller();

    // there is side affect with 2 mobile app with the same mobile app_id
    if (resumer.IsApplicationSaved(mobile_app_id)) {
      app->set_hmi_application_id(resumer.GetHMIApplicationID(mobile_app_id));
    } else {
      app->set_hmi_application_id(
        ApplicationManagerImpl::instance()->GenerateNewHMIAppID());
    }

    app->set_is_media_application(
      msg_params[strings::is_media_application].asBool());

    if (msg_params.keyExists(strings::vr_synonyms)) {
      app->set_vr_synonyms(msg_params[strings::vr_synonyms]);
    }

    if (msg_params.keyExists(strings::ngn_media_screen_app_name)) {
      app->set_ngn_media_screen_name(
        msg_params[strings::ngn_media_screen_app_name]);
    }

    if (msg_params.keyExists(strings::tts_name)) {
      app->set_tts_name(msg_params[strings::tts_name]);
    }

    if (msg_params.keyExists(strings::app_hmi_type)) {
      app->set_app_types(msg_params[strings::app_hmi_type]);

      // check app type
      const smart_objects::SmartObject& app_type =
        msg_params.getElement(strings::app_hmi_type);

      for (size_t i = 0; i < app_type.length(); ++i) {
        if (mobile_apis::AppHMIType::NAVIGATION ==
            static_cast<mobile_apis::AppHMIType::eType>(
                app_type.getElement(i).asUInt())) {
          app->set_allowed_support_navigation(true);
        }
        if (mobile_apis::AppHMIType::COMMUNICATION ==
            static_cast<mobile_apis::AppHMIType::eType>(
            app_type.getElement(i).asUInt())) {
          app->set_voice_communication_supported(true);
        }
      }
    }

    const connection_handler::DeviceHandle handle = app->device();
    // Add device to policy table and set device info, if any
    std::string device_mac_address =
      application_manager::MessageHelper::GetDeviceMacAddressForHandle(handle);
    policy::DeviceParams dev_params;
    application_manager::MessageHelper::GetDeviceInfoForHandle(handle,
                                                               &dev_params);
    policy::DeviceInfo device_info;
    device_info.connection_type = dev_params.device_connection_type;
    if (msg_params.keyExists(strings::device_info)) {
      FillDeviceInfo(&device_info);
    }

    policy::PolicyHandler::instance()->SetDeviceInfo(device_mac_address,
        device_info);

    SendRegisterAppInterfaceResponseToMobile();
    policy::PolicyHandler::instance()->PTExchangeAtRegistration(mobile_app_id);
  }
}

void RegisterAppInterfaceRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::on_event");
  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_Speak: {
      const smart_objects::SmartObject& message = event.smart_object();

      mobile_apis::Result::eType tts_result =
        static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      SendRegisterAppInterfaceResponseToMobile(tts_result);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}

void RegisterAppInterfaceRequest::SendRegisterAppInterfaceResponseToMobile(
  mobile_apis::Result::eType result) {
  smart_objects::SmartObject* params = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  ApplicationManagerImpl* app_manager = ApplicationManagerImpl::instance();
  const HMICapabilities& hmi_capabilities = app_manager->hmi_capabilities();
  const uint32_t key = connection_key();
  ApplicationSharedPtr application =
    ApplicationManagerImpl::instance()->application(key);

  if (!application.valid()) {
    LOG4CXX_ERROR(logger_, "There is no application for such connection key" <<
                  key);
    return;
  }

  smart_objects::SmartObject& response_params = *params;

  response_params[strings::sync_msg_version][strings::major_version] =
    APIVersion::kAPIV3;
  response_params[strings::sync_msg_version][strings::minor_version] =
    APIVersion::kAPIV0;

  response_params[strings::language] = hmi_capabilities.active_vr_language();
  response_params[strings::hmi_display_language] =
    hmi_capabilities.active_ui_language();

  const smart_objects::SmartObject& msg_params =
    (*message_)[strings::msg_params];

  if (msg_params[strings::language_desired].asInt() !=
      hmi_capabilities.active_vr_language() ||
      msg_params[strings::hmi_display_language_desired].asInt() !=
      hmi_capabilities.active_ui_language()) {

    LOG4CXX_WARN_EXT(
      logger_,
      "Wrong language on registering application " << application->name());

    LOG4CXX_ERROR_EXT(
      logger_,
      "vr "
      << msg_params[strings::language_desired].asInt()
      << " - "
      << hmi_capabilities.active_vr_language()
      << "ui "
      << msg_params[strings::hmi_display_language_desired].asInt()
      << " - "
      << hmi_capabilities.active_ui_language());

    result = mobile_apis::Result::WRONG_LANGUAGE;
  }

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

    if (hmi_capabilities.display_capabilities()->getElement(
          hmi_response::image_capabilities).length() > 0) {
      display_caps[hmi_response::graphic_supported] = true;
    } else {
      display_caps[hmi_response::graphic_supported] = false;
    }

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
    response_params[hmi_response::hmi_zone_capabilities] =
      *hmi_capabilities.hmi_zone_capabilities();
  }
  if (hmi_capabilities.speech_capabilities()) {
    response_params[strings::speech_capabilities] =
      *hmi_capabilities.speech_capabilities();
  }
  if (hmi_capabilities.vr_capabilities()) {
    response_params[strings::vr_capabilities] =
      *hmi_capabilities.vr_capabilities();
  }
  if (hmi_capabilities.audio_pass_thru_capabilities()) {
    response_params[strings::audio_pass_thru_capabilities] =
      *hmi_capabilities.audio_pass_thru_capabilities();
  }
  if (hmi_capabilities.vehicle_type()) {
    response_params[hmi_response::vehicle_type] =
      *hmi_capabilities.vehicle_type();
  }
  if (hmi_capabilities.prerecorded_speech()) {
    response_params[strings::prerecorded_speech] =
      *(hmi_capabilities.prerecorded_speech());
  }

  const std::vector<uint32_t>& diag_modes =
    profile::Profile::instance()->supported_diag_modes();
  if (!diag_modes.empty()) {
    std::vector<uint32_t>::const_iterator it = diag_modes.begin();
    uint32_t index = 0;
    for (; it != diag_modes.end(); ++it) {
      response_params[strings::supported_diag_modes][index] = *it;
      ++index;
    }
  }

  ResumeCtrl& resumer = ApplicationManagerImpl::instance()->resume_controller();
  uint32_t hash_id = 0;

  const char* add_info = "";
  const bool resumption = (*message_)[strings::msg_params].keyExists(strings::hash_id);
  bool need_restore_vr = resumption;
  if (resumption) {
    hash_id = (*message_)[strings::msg_params][strings::hash_id].asUInt();
    if (!resumer.CheckApplicationHash(application, hash_id)) {
      LOG4CXX_WARN(logger_, "Hash does not matches");
      result = mobile_apis::Result::RESUME_FAILED;
      add_info = "Hash does not matches";
      need_restore_vr = false;
    } else if (!resumer.CheckPersistenceFilesForResumption(application)) {
      LOG4CXX_WARN(logger_, "Persistent data is missed");
      result = mobile_apis::Result::RESUME_FAILED;
      add_info = "Persistent data is missed";
      need_restore_vr = false;
    } else {
      add_info = " Resume Succeed";
    }
  }

  SendResponse(true, result, add_info, params);

  MessageHelper::SendOnAppRegisteredNotificationToHMI(*(application.get()),
                                                      resumption,
                                                      need_restore_vr);

  MessageHelper::SendChangeRegistrationRequestToHMI(application);

  if (result != mobile_apis::Result::RESUME_FAILED) {
    resumer.StartResumption(application, hash_id);
  } else {
    resumer.StartResumptionOnlyHMILevel(application);
  }
}

mobile_apis::Result::eType
RegisterAppInterfaceRequest::CheckCoincidence() {
  LOG4CXX_TRACE_ENTER(logger_);
  const smart_objects::SmartObject& msg_params =
    (*message_)[strings::msg_params];

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const std::set<ApplicationSharedPtr> applications = accessor.applications();

  std::set<ApplicationSharedPtr>::const_iterator it = applications.begin();
  const std::string app_name = msg_params[strings::app_name].asString();

  for (; applications.end() != it; ++it) {

    // name check
    const std::string& cur_name = (*it)->name();
    if (!strcasecmp(app_name.c_str(), cur_name.c_str())) {
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
  LOG4CXX_INFO(logger_, "CheckWithPolicyData");
  // TODO(AOleynik): Check is necessary to allow register application in case
  // of disabled policy
  // Remove this check, when HMI will support policy
  if (!policy::PolicyHandler::instance()->PolicyEnabled()) {
    return mobile_apis::Result::WARNINGS;
  }

  smart_objects::SmartObject& message = *message_;
  policy::StringArray app_nicknames;
  policy::StringArray app_hmi_types;

  std::string mobile_app_id = message[strings::msg_params][strings::app_id].asString();
  const bool init_result = policy::PolicyHandler::instance()->GetInitialAppData(
        mobile_app_id, &app_nicknames, &app_hmi_types);

  if (!init_result) {
    LOG4CXX_ERROR(logger_, "Error during initial application data check.");
    return mobile_apis::Result::INVALID_DATA;
  }

  if (!app_nicknames.empty()) {
    policy::StringArray::const_iterator it = std::find(
          app_nicknames.begin(), app_nicknames.end(),
          message[strings::msg_params][strings::app_name].asString());
    if (app_nicknames.end() == it) {
      LOG4CXX_WARN(logger_,
                   "Application name was not found in nicknames list.");
      //App should be unregistered, if its name is not present in nicknames list
      usage_statistics::AppCounter count_of_rejections_nickname_mismatch(
        policy::PolicyHandler::instance()->GetStatisticManager(), mobile_app_id,
        usage_statistics::REJECTIONS_NICKNAME_MISMATCH);
      ++count_of_rejections_nickname_mismatch;
      return mobile_apis::Result::DISALLOWED;
    }
  }

  mobile_apis::Result::eType result = mobile_apis::Result::SUCCESS;

  // If AppHMIType is not included in policy - allow any type
  if (!app_hmi_types.empty()) {
    if (message[strings::msg_params].keyExists(strings::app_hmi_type)) {
      // If AppHMITypes are partially same, the system should allow those listed
      // in the policy table and send warning info on missed values
      smart_objects::SmartArray app_types =
        *(message[strings::msg_params][strings::app_hmi_type].asArray());

      std::string log;
      CheckMissedTypes checker(app_hmi_types, log);
      std::for_each(app_types.begin(), app_types.end(), checker);
      if (!log.empty()) {
        response_info_ = "Following AppHMITypes are not present in policy "
                         "table:" + log;
        result = mobile_apis::Result::WARNINGS;
      }
    }
    // Replace AppHMITypes in request with values allowed by policy table
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
    device_info->hardware = msg_params[strings::device_info][hardware].asString();
  }
  if (device_info_so.keyExists(firmware_rev)) {
    device_info->firmware_rev = msg_params[strings::device_info][firmware_rev].asString();
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

mobile_apis::Result::eType RegisterAppInterfaceRequest::CheckRestrictions() const {

  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::CheckRestrictions");

  const smart_objects::SmartObject& msg_params =
    (*message_)[strings::msg_params];

  const std::string& app_name = msg_params[strings::app_name].asString();

  if (ClearParamName(app_name).empty()) {
    printf("Application name is empty.\n");
    return mobile_apis::Result::INVALID_DATA;
  }

  if ((app_name[0] == '\n') ||
      ((app_name[0] == '\\') && (app_name[1] == 'n'))) {

    printf("Application name has invalid characters.");
    return mobile_apis::Result::INVALID_DATA;
  }

  if (msg_params.keyExists(strings::tts_name)) {

    const smart_objects::SmartArray* tts =
      msg_params[strings::tts_name].asArray();

    smart_objects::SmartArray::const_iterator it = tts->begin();
    smart_objects::SmartArray::const_iterator it_end = tts->end();

    for (; it != it_end; ++it) {

      const std::string& tts_name = (*it)[strings::text].asString();

      if (ClearParamName(tts_name).empty()) {
        printf("TTS value is empty.");
        return mobile_apis::Result::INVALID_DATA;
      }

      if ((tts_name[0] == '\n') ||
          ((tts_name[0] == '\\') && (tts_name[1] == 'n'))) {

        printf("TTS value(s) has invalid characters.");
        return mobile_apis::Result::INVALID_DATA;
      }
    }
  }

  return mobile_apis::Result::SUCCESS;
}

std::string
RegisterAppInterfaceRequest::ClearParamName(std::string param_name) const {

  // Expecting for chars different from newlines and spaces in the appName
  //
  // There is an agreement, that "\n" is not allowed symbols, so we have to
  // check for this case also

  std::string newline = "\\n";
  while (std::string::npos != param_name.find(newline)) {
    param_name.erase(param_name.find(newline), newline.length());
  }

  std::string::iterator param_name_new_end =
    std::remove_if(param_name.begin(), param_name.end(), ::isspace);

  return std::string(param_name.begin(), param_name_new_end);
}

bool RegisterAppInterfaceRequest::IsApplicationWithSameAppIdRegistered() {

  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::"
               "IsApplicationWithSameAppIdRegistered");

  const std::string mobile_app_id = (*message_)[strings::msg_params]
                                    [strings::app_id].asString();

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const std::set<ApplicationSharedPtr> applications = accessor.applications();

  std::set<ApplicationSharedPtr>::const_iterator it = applications.begin();
  std::set<ApplicationSharedPtr>::const_iterator it_end = applications.end();

  for (; it != it_end; ++it) {
    if (mobile_app_id == (*it)->mobile_app_id()->asString()) {
      return true;
    }
  }

  return false;
}

bool RegisterAppInterfaceRequest::IsWhiteSpaceExist() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::IsWhiteSpaceExist");
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

  if ((*message_)[strings::msg_params].
      keyExists(strings::ngn_media_screen_app_name)) {
    str = (*message_)[strings::msg_params]
                      [strings::ngn_media_screen_app_name].asCharArray();
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

    if ((*message_)[strings::msg_params][strings::device_info].
                                         keyExists(strings::hardware)) {
      str = (*message_)[strings::msg_params]
                  [strings::device_info][strings::hardware].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info hardware syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].
                                         keyExists(strings::firmware_rev)) {
      str = (*message_)[strings::msg_params]
                  [strings::device_info][strings::firmware_rev].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info firmware_rev syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].
                                         keyExists(strings::os)) {
      str = (*message_)[strings::msg_params]
                  [strings::device_info][strings::os].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info os syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].
                                         keyExists(strings::os_version)) {
      str = (*message_)[strings::msg_params]
                  [strings::device_info][strings::os_version].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid device_info os_version syntax check failed");
        return true;
      }
    }

    if ((*message_)[strings::msg_params][strings::device_info].
                                         keyExists(strings::carrier)) {
      str = (*message_)[strings::msg_params]
                  [strings::device_info][strings::carrier].asCharArray();
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

}  // namespace commands

}  // namespace application_manager
