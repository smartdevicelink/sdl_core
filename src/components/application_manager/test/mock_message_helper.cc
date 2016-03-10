/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/message_helper.h"
#include "mock_message_helper.h"

namespace application_manager {

void MessageHelper::SendHashUpdateNotification(uint32_t const app_id) {
  MockMessageHelper::message_helper_mock()->SendHashUpdateNotification(app_id);
}
void MessageHelper::SendNaviStartStream(int32_t connection_key) {
  MockMessageHelper::message_helper_mock()->SendNaviStartStream(connection_key);
}
void MessageHelper::SendNaviStopStream(int32_t connection_key) {
  MockMessageHelper::message_helper_mock()->SendNaviStopStream(connection_key);
}
void MessageHelper::SendAudioStartStream(int32_t connection_key) {
  MockMessageHelper::message_helper_mock()->SendAudioStartStream(
      connection_key);
}
void MessageHelper::SendAudioStopStream(int32_t connection_key) {
  MockMessageHelper::message_helper_mock()->SendAudioStopStream(connection_key);
}
void MessageHelper::SendOnDataStreaming(protocol_handler::ServiceType service,
                                        bool available) {
  MockMessageHelper::message_helper_mock()->SendOnDataStreaming(service,
                                                                available);
}

uint32_t MessageHelper::GetAppCommandLimit(const std::string& policy_app_id) {
  return MockMessageHelper::message_helper_mock()->GetAppCommandLimit(
      policy_app_id);
}

smart_objects::SmartObjectSPtr GetHashUpdateNotification(
    const uint32_t app_id) {
  return MockMessageHelper::message_helper_mock()->GetHashUpdateNotification(
      app_id);
}

std::string MessageHelper::HMIResultToString(
    hmi_apis::Common_Result::eType hmi_result) {
  return MockMessageHelper::message_helper_mock()->HMIResultToString(
      hmi_result);
}

hmi_apis::Common_Result::eType MessageHelper::HMIResultFromString(
    const std::string& hmi_result) {
  return MockMessageHelper::message_helper_mock()->HMIResultFromString(
      hmi_result);
}

std::string MessageHelper::MobileResultToString(
    mobile_apis::Result::eType mobile_result) {
  return MockMessageHelper::message_helper_mock()->MobileResultToString(
      mobile_result);
}

mobile_api::Result::eType MessageHelper::MobileResultFromString(
    const std::string& mobile_result) {
  return MockMessageHelper::message_helper_mock()->MobileResultFromString(
      mobile_result);
}

mobile_api::Result::eType MessageHelper::HMIToMobileResult(
    const hmi_apis::Common_Result::eType hmi_result) {
  return MockMessageHelper::message_helper_mock()->HMIToMobileResult(
      hmi_result);
}

hmi_apis::Common_Result::eType MessageHelper::MobileToHMIResult(
    const mobile_api::Result::eType mobile_result) {
  return MockMessageHelper::message_helper_mock()->MobileToHMIResult(
      mobile_result);
}

mobile_api::HMILevel::eType MessageHelper::StringToHMILevel(
    const std::string& hmi_level) {
  return MockMessageHelper::message_helper_mock()->StringToHMILevel(hmi_level);
}

smart_objects::SmartObjectSPtr CreateDeviceListSO(
    const connection_handler::DeviceMap& devices) {
  return MockMessageHelper::message_helper_mock()->CreateDeviceListSO(devices);
}

void MessageHelper::SendOnAppPermissionsChangedNotification(
    uint32_t connection_key, const policy::AppPermissions& permissions) {
  MockMessageHelper::message_helper_mock()
      ->SendOnAppPermissionsChangedNotification(connection_key, permissions);
}

void MessageHelper::SendGetUserFriendlyMessageResponse(
    const std::vector<policy::UserFriendlyMessage>& msg,
    uint32_t correlation_id) {
  MockMessageHelper::message_helper_mock()->SendGetUserFriendlyMessageResponse(
      msg, correlation_id);
}

void MessageHelper::SendGetStatusUpdateResponse(const std::string& status,
                                                uint32_t correlation_id) {
  MockMessageHelper::message_helper_mock()->SendGetStatusUpdateResponse(
      status, correlation_id);
}

void MessageHelper::SendOnStatusUpdate(const std::string& status) {
  MockMessageHelper::message_helper_mock()->SendOnStatusUpdate(status);
}

void MessageHelper::SendGetSystemInfoRequest() {
  MockMessageHelper::message_helper_mock()->SendGetSystemInfoRequest();
}

void MessageHelper::CreateGetVehicleDataRequest(
    uint32_t correlation_id, const std::vector<std::string>& params) {
  MockMessageHelper::message_helper_mock()->CreateGetVehicleDataRequest(
      correlation_id, params);
}

void MessageHelper::SendGetListOfPermissionsResponse(
    const std::vector<policy::FunctionalGroupPermission>& permissions,
    uint32_t correlation_id) {
  MockMessageHelper::message_helper_mock()->SendGetListOfPermissionsResponse(
      permissions, correlation_id);
}

void MessageHelper::SendOnPermissionsChangeNotification(
    uint32_t connection_key, const policy::Permissions& permissions) {
  MockMessageHelper::message_helper_mock()->SendOnPermissionsChangeNotification(
      connection_key, permissions);
}

void MessageHelper::SendPolicySnapshotNotification(
    unsigned int connection_key,
    const std::vector<uint8_t>& policy_data,
    const std::string& url) {
  MockMessageHelper::message_helper_mock()->SendPolicySnapshotNotification(
      connection_key, policy_data, url);
}
void MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
    int32_t connection_key,
    mobile_apis::AppInterfaceUnregisteredReason::eType reason) {
  MockMessageHelper::message_helper_mock()
      ->SendOnAppInterfaceUnregisteredNotificationToMobile(connection_key,
                                                           reason);
}

void MessageHelper::SendSDLActivateAppResponse(
    policy::AppPermissions& permissions, uint32_t correlation_id) {
  MockMessageHelper::message_helper_mock()->SendSDLActivateAppResponse(
      permissions, correlation_id);
}

void MessageHelper::SendPolicyUpdate(const std::string& file_path,
                                     int timeout,
                                     const std::vector<int>& retries) {
  MockMessageHelper::message_helper_mock()->SendPolicyUpdate(
      file_path, timeout, retries);
}

void MessageHelper::SendUpdateSDLResponse(const std::string& result,
                                          uint32_t correlation_id) {
  MockMessageHelper::message_helper_mock()->SendUpdateSDLResponse(
      result, correlation_id);
}

void MessageHelper::SendOnLanguageChangeToMobile(uint32_t connection_key) {
  MockMessageHelper::message_helper_mock()->SendOnLanguageChangeToMobile(connection_key);
}

void MessageHelper::SendDecryptCertificateToHMI(const std::string& file_name) {
  MockMessageHelper::message_helper_mock()->SendDecryptCertificateToHMI(
      file_name);
}

hmi_apis::Common_Language::eType MessageHelper::CommonLanguageFromString(const std::string& language) {
   return MockMessageHelper::message_helper_mock()->CommonLanguageFromString(
        language);
}

std::string MessageHelper::MobileLanguageToString(const mobile_apis::Language::eType language) {
  return  MockMessageHelper::message_helper_mock()->MobileLanguageToString(language);
}

mobile_apis::Language::eType MessageHelper::MobileLanguageFromString(const std::string& language) {
  return  MockMessageHelper::message_helper_mock()->MobileLanguageFromString(language);
}

hmi_apis::Common_Language::eType MessageHelper::MobileToCommonLanguage(const mobile_apis::Language::eType language) {
  return MockMessageHelper::message_helper_mock()->MobileToCommonLanguage(language);
}

mobile_apis::Language::eType MessageHelper::CommonToMobileLanguage(const hmi_apis::Common_Language::eType language) {
  return  MockMessageHelper::message_helper_mock()->CommonToMobileLanguage(language);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateModuleInfoSO (uint32_t function_id) {
    return  MockMessageHelper::message_helper_mock()->CreateModuleInfoSO(
                function_id);
}

MockMessageHelper* MockMessageHelper::message_helper_mock() {
  static MockMessageHelper message_helper_mock;
  return &message_helper_mock;
}
void MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
    ApplicationConstSharedPtr app) {
  MockMessageHelper::message_helper_mock()
      ->SendAllOnButtonSubscriptionNotificationsForApp(app);
}

void MessageHelper::SendOnResumeAudioSourceToHMI(const uint32_t app_id) {
  MockMessageHelper::message_helper_mock()->SendOnResumeAudioSourceToHMI(
      app_id);
}

smart_objects::SmartObjectList MessageHelper::CreateAddSubMenuRequestToHMI(
    ApplicationConstSharedPtr app) {
  return MockMessageHelper::message_helper_mock()->CreateAddSubMenuRequestToHMI(
      app);
}

smart_objects::SmartObjectList MessageHelper::CreateAddCommandRequestToHMI(
    ApplicationConstSharedPtr app) {
  return MockMessageHelper::message_helper_mock()->CreateAddCommandRequestToHMI(
      app);
}

smart_objects::SmartObjectList
MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
    ApplicationConstSharedPtr app) {
  return MockMessageHelper::message_helper_mock()
      ->CreateAddVRCommandRequestFromChoiceToHMI(app);
}

void MessageHelper::SendGlobalPropertiesToHMI(ApplicationConstSharedPtr app) {
  return MockMessageHelper::message_helper_mock()->SendGlobalPropertiesToHMI(
      app);
}

smart_objects::SmartObjectList MessageHelper::GetIVISubscriptionRequests(
    ApplicationSharedPtr app) {
  return MockMessageHelper::message_helper_mock()->GetIVISubscriptionRequests(
      app);
}

mobile_apis::Result::eType MessageHelper::VerifyImageFiles(
    smart_objects::SmartObject& message, ApplicationConstSharedPtr app) {
  return MockMessageHelper::message_helper_mock()->VerifyImageFiles(message,
                                                                    app);
}
std::string MessageHelper::CommonLanguageToString(
    hmi_apis::Common_Language::eType lang) {
  return MockMessageHelper::message_helper_mock()->CommonLanguageToString(lang);
}

bool MessageHelper::CheckWithPolicy(
    mobile_apis::SystemAction::eType system_action,
    const std::string& app_mobile_id) {
  return MockMessageHelper::message_helper_mock()->CheckWithPolicy(
        system_action, app_mobile_id);
}

smart_objects::SmartObjectSPtr MessageHelper::GetBCActivateAppRequestToHMI(
    ApplicationConstSharedPtr app,
    hmi_apis::Common_HMILevel::eType hmi_level,
    bool send_policy_priority) {
  return MockMessageHelper::message_helper_mock()->
      GetBCActivateAppRequestToHMI(app, hmi_level, send_policy_priority);
}

}  // namespace application_manager
