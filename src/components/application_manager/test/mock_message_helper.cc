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
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace application_manager {

void MessageHelper::SendHashUpdateNotification(uint32_t const app_id,
                                               ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendHashUpdateNotification(
      app_id, app_mngr);
}
void MessageHelper::SendNaviSetVideoConfig(
    int32_t app_id,
    ApplicationManager& app_mngr,
    const smart_objects::SmartObject& video_params) {
  MockMessageHelper::message_helper_mock()->SendNaviSetVideoConfig(
      app_id, app_mngr, video_params);
}
void MessageHelper::SendNaviStartStream(int32_t connection_key,
                                        ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendNaviStartStream(connection_key,
                                                                app_mngr);
}
void MessageHelper::SendNaviStopStream(int32_t connection_key,
                                       ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendNaviStopStream(connection_key,
                                                               app_mngr);
}
void MessageHelper::SendAudioStartStream(int32_t connection_key,
                                         ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendAudioStartStream(connection_key,
                                                                 app_mngr);
}
void MessageHelper::SendAudioStopStream(int32_t connection_key,
                                        ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendAudioStopStream(connection_key,
                                                                app_mngr);
}
void MessageHelper::SendOnDataStreaming(protocol_handler::ServiceType service,
                                        bool available,
                                        ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendOnDataStreaming(
      service, available, app_mngr);
}

void MessageHelper::SendDecryptCertificateToHMI(const std::string& file_name,
                                                ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendDecryptCertificateToHMI(
      file_name, app_mngr);
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

smart_objects::SmartObjectSPtr MessageHelper::CreateDeviceListSO(
    const connection_handler::DeviceMap& devices,
    const policy::PolicyHandlerInterface& policy_handler,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->CreateDeviceListSO(
      devices, policy_handler, app_mngr);
}

void MessageHelper::SendOnAppPermissionsChangedNotification(
    uint32_t connection_key,
    const policy::AppPermissions& permissions,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()
      ->SendOnAppPermissionsChangedNotification(
          connection_key, permissions, app_mngr);
}

void MessageHelper::SendGetUserFriendlyMessageResponse(
    const std::vector<policy::UserFriendlyMessage>& msg,
    uint32_t correlation_id,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendGetUserFriendlyMessageResponse(
      msg, correlation_id, app_mngr);
}

void MessageHelper::SendGetStatusUpdateResponse(const std::string& status,
                                                uint32_t correlation_id,
                                                ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendGetStatusUpdateResponse(
      status, correlation_id, app_mngr);
}

void MessageHelper::SendOnStatusUpdate(const std::string& status,
                                       ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendOnStatusUpdate(status,
                                                               app_mngr);
}

void MessageHelper::SendGetSystemInfoRequest(ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendGetSystemInfoRequest(app_mngr);
}

void MessageHelper::CreateGetVehicleDataRequest(
    uint32_t correlation_id,
    const std::vector<std::string>& params,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->CreateGetVehicleDataRequest(
      correlation_id, params, app_mngr);
}

void MessageHelper::SendGetListOfPermissionsResponse(
    const std::vector<policy::FunctionalGroupPermission>& permissions,
#ifdef EXTERNAL_PROPRIETARY_MODE
    const policy::ExternalConsentStatus& external_consent_status,
#endif  // EXTERNAL_PROPRIETARY_MODE
    uint32_t correlation_id,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendGetListOfPermissionsResponse(
      permissions,
#ifdef EXTERNAL_PROPRIETARY_MODE
      external_consent_status,
#endif  // EXTERNAL_PROPRIETARY_MODE
      correlation_id,
      app_mngr);
}

void MessageHelper::SendOnPermissionsChangeNotification(
    uint32_t connection_key,
    const policy::Permissions& permissions,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendOnPermissionsChangeNotification(
      connection_key, permissions, app_mngr);
}

void MessageHelper::SendPolicySnapshotNotification(
    uint32_t connection_key,
    const std::vector<uint8_t>& policy_data,
    const std::string& url,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendPolicySnapshotNotification(
      connection_key, policy_data, url, app_mngr);
}

void MessageHelper::SendSDLActivateAppResponse(
    policy::AppPermissions& permissions,
    uint32_t correlation_id,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendSDLActivateAppResponse(
      permissions, correlation_id, app_mngr);
}

void MessageHelper::SendPolicyUpdate(const std::string& file_path,
                                     const uint32_t timeout,
                                     const std::vector<int>& retries,
                                     ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendPolicyUpdate(
      file_path, timeout, retries, app_mngr);
}

#ifdef SDL_REMOTE_CONTROL
void MessageHelper::SendActivateAppToHMI(
    uint32_t const app_id,
    ApplicationManager& application_manager,
    hmi_apis::Common_HMILevel::eType level,
    bool send_policy_priority) {
  MockMessageHelper::message_helper_mock()->SendActivateAppToHMI(
      app_id, application_manager, level, send_policy_priority);
}

void MessageHelper::SendHMIStatusNotification(
    const Application& application_impl,
    ApplicationManager& application_manager) {
  MockMessageHelper::message_helper_mock()->SendHMIStatusNotification(
      application_impl, application_manager);
}

#endif  // SDL_REMOTE_CONTROL

void MessageHelper::SendUpdateSDLResponse(const std::string& result,
                                          uint32_t correlation_id,
                                          ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendUpdateSDLResponse(
      result, correlation_id, app_mngr);
}

hmi_apis::Common_Language::eType MessageHelper::CommonLanguageFromString(
    const std::string& language) {
  return MockMessageHelper::message_helper_mock()->CommonLanguageFromString(
      language);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateModuleInfoSO(
    uint32_t function_id, ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->CreateModuleInfoSO(
      function_id, app_mngr);
}

MockMessageHelper* MockMessageHelper::message_helper_mock() {
  static ::testing::NiceMock<MockMessageHelper> message_helper_mock;
  return &message_helper_mock;
}
void MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()
      ->SendAllOnButtonSubscriptionNotificationsForApp(app, app_mngr);
}

void MessageHelper::SendOnResumeAudioSourceToHMI(const uint32_t app_id,
                                                 ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendOnResumeAudioSourceToHMI(
      app_id, app_mngr);
}

smart_objects::SmartObjectList MessageHelper::CreateAddSubMenuRequestToHMI(
    ApplicationConstSharedPtr app, const uint32_t correlation_id) {
  return MockMessageHelper::message_helper_mock()->CreateAddSubMenuRequestToHMI(
      app, correlation_id);
}

smart_objects::SmartObjectList MessageHelper::CreateAddCommandRequestToHMI(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->CreateAddCommandRequestToHMI(
      app, app_mngr);
}

smart_objects::SmartObjectList
MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()
      ->CreateAddVRCommandRequestFromChoiceToHMI(app);
}

void MessageHelper::SendGlobalPropertiesToHMI(ApplicationConstSharedPtr app,
                                              ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendGlobalPropertiesToHMI(
      app);
}

smart_objects::SmartObjectList MessageHelper::GetIVISubscriptionRequests(
    ApplicationSharedPtr app, ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->GetIVISubscriptionRequests(
      app);
}

mobile_apis::Result::eType MessageHelper::VerifyImage(
    smart_objects::SmartObject& message,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->VerifyImage(
      message, app, app_mngr);
}

mobile_apis::Result::eType MessageHelper::VerifyImageFiles(
    smart_objects::SmartObject& message,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->VerifyImageFiles(
      message, app, app_mngr);
}

std::string MessageHelper::CommonLanguageToString(
    hmi_apis::Common_Language::eType lang) {
  return MockMessageHelper::message_helper_mock()->CommonLanguageToString(lang);
}

smart_objects::SmartObjectSPtr MessageHelper::GetBCActivateAppRequestToHMI(
    ApplicationConstSharedPtr app,
    const protocol_handler::SessionObserver& session_observer,
    const policy::PolicyHandlerInterface& policy_handler,
    hmi_apis::Common_HMILevel::eType level,
    bool send_policy_priority,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->GetBCActivateAppRequestToHMI(
      app,
      session_observer,
      policy_handler,
      level,
      send_policy_priority,
      app_mngr);
}

NsSmartDeviceLink::NsSmartObjects::SmartObjectSPtr
MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
    int32_t connection_key,
    mobile_apis::AppInterfaceUnregisteredReason::eType reason) {
  return MockMessageHelper::message_helper_mock()
      ->GetOnAppInterfaceUnregisteredNotificationToMobile(connection_key,
                                                          reason);
}

mobile_apis::Result::eType MessageHelper::ProcessSoftButtons(
    smart_objects::SmartObject& message_params,
    ApplicationConstSharedPtr app,
    const policy::PolicyHandlerInterface& policy_handler,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->ProcessSoftButtons(
      message_params, app, policy_handler, app_mngr);
}

void MessageHelper::SubscribeApplicationToSoftButton(
    smart_objects::SmartObject& message_params,
    ApplicationSharedPtr app,
    int32_t function_id) {
  return MockMessageHelper::message_helper_mock()
      ->SubscribeApplicationToSoftButton(message_params, app, function_id);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateNegativeResponse(
    uint32_t connection_key,
    int32_t function_id,
    const uint32_t correlation_id,
    int32_t result_code) {
  return MockMessageHelper::message_helper_mock()->CreateNegativeResponse(
      connection_key, function_id, correlation_id, result_code);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateBlockedByPoliciesResponse(
    mobile_apis::FunctionID::eType function_id,
    mobile_apis::Result::eType result,
    const uint32_t correlation_id,
    uint32_t connection_key) {
  return MockMessageHelper::message_helper_mock()
      ->CreateBlockedByPoliciesResponse(
          function_id, result, correlation_id, connection_key);
}

const VehicleData& MessageHelper::vehicle_data() {
  return MockMessageHelper::message_helper_mock()->vehicle_data();
}

bool MessageHelper::SendStopAudioPathThru(ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendStopAudioPathThru(
      app_mngr);
}

std::string MessageHelper::StringifiedFunctionID(
    const mobile_apis::FunctionID::eType function_id) {
  return MockMessageHelper::message_helper_mock()->StringifiedFunctionID(
      function_id);
}

void MessageHelper::SendUIChangeRegistrationRequestToHMI(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()
      ->SendUIChangeRegistrationRequestToHMI(app, app_mngr);
}

bool MessageHelper::CreateHMIApplicationStruct(
    ApplicationConstSharedPtr app,
    const protocol_handler::SessionObserver& session_observer,
    const policy::PolicyHandlerInterface& policy_handler,
    smart_objects::SmartObject* output,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->CreateHMIApplicationStruct(
      app, session_observer, policy_handler, output, app_mngr);
}

void MessageHelper::SendOnAppUnregNotificationToHMI(
    ApplicationConstSharedPtr app,
    const bool is_unexpected_disconnect,
    ApplicationManager& app_mngr) {
  MockMessageHelper::message_helper_mock()->SendOnAppUnregNotificationToHMI(
      app, is_unexpected_disconnect, app_mngr);
}

void MessageHelper::SendLaunchApp(const uint32_t connection_key,
                                  const std::string& urlSchema,
                                  const std::string& packageName,
                                  ApplicationManager& app_man) {
  MockMessageHelper::message_helper_mock()->SendLaunchApp(
      connection_key, urlSchema, packageName, app_man);
}

bool MessageHelper::SendUnsubscribedWayPoints(ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendUnsubscribedWayPoints(
      app_mngr);
}

void MessageHelper::SendQueryApps(const uint32_t connection_key,
                                  ApplicationManager& app_man) {
  MockMessageHelper::message_helper_mock()->SendQueryApps(connection_key,
                                                          app_man);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateAppVrHelp(
    ApplicationConstSharedPtr app) {
  return MockMessageHelper::message_helper_mock()->CreateAppVrHelp(app);
}

mobile_apis::Result::eType MessageHelper::VerifyImageVrHelpItems(
    smart_objects::SmartObject& message,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->VerifyImageVrHelpItems(
      message, app, app_mngr);
}

const uint32_t MessageHelper::GetPriorityCode(const std::string& priority) {
  return MockMessageHelper::message_helper_mock()->GetPriorityCode(priority);
}

void MessageHelper::SendTTSGlobalProperties(ApplicationSharedPtr app,
                                            const bool default_help_prompt,
                                            ApplicationManager& app_man) {
  MockMessageHelper::message_helper_mock()->SendTTSGlobalProperties(
      app, default_help_prompt, app_man);
}

bool MessageHelper::PrintSmartObject(const smart_objects::SmartObject& object) {
  return MockMessageHelper::message_helper_mock()->PrintSmartObject(object);
}

std::string MessageHelper::SmartObjectToString(
    const smart_objects::SmartObject& object) {
  return MockMessageHelper::message_helper_mock()->SmartObjectToString(object);
}

void MessageHelper::SendSetAppIcon(const uint32_t app_id,
                                   const std::string& icon_path,
                                   ApplicationManager& application_manager) {
  MockMessageHelper::message_helper_mock()->SendSetAppIcon(
      app_id, icon_path, application_manager);
}

std::string MessageHelper::StringifiedHMILevel(
    const mobile_apis::HMILevel::eType hmi_level) {
  return MockMessageHelper::message_helper_mock()->StringifiedHMILevel(
      hmi_level);
}

std::string MessageHelper::GetDeviceMacAddressForHandle(
    const uint32_t device_handle, const ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->GetDeviceMacAddressForHandle(
      device_handle, app_mngr);
}

void MessageHelper::SendDeleteCommandRequest(smart_objects::SmartObject* cmd,
                                             ApplicationSharedPtr application,
                                             ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendDeleteCommandRequest(
      cmd, application, app_mngr);
}

void MessageHelper::SendDeleteSubmenuRequest(smart_objects::SmartObject* cmd,
                                             ApplicationSharedPtr application,
                                             ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendDeleteSubmenuRequest(
      cmd, application, app_mngr);
}

void MessageHelper::SendDeleteChoiceSetRequest(smart_objects::SmartObject* cmd,
                                               ApplicationSharedPtr application,
                                               ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendDeleteChoiceSetRequest(
      cmd, application, app_mngr);
}

void MessageHelper::SendResetPropertiesRequest(ApplicationSharedPtr application,
                                               ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendResetPropertiesRequest(
      application, app_mngr);
}

void MessageHelper::SendUnsubscribeButtonNotification(
    mobile_apis::ButtonName::eType button,
    ApplicationSharedPtr application,
    ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()
      ->SendUnsubscribeButtonNotification(button, application, app_mngr);
}

void MessageHelper::SendUnsubscribeIVIRequest(int32_t ivi_id,
                                              ApplicationSharedPtr application,
                                              ApplicationManager& app_mngr) {
  return MockMessageHelper::message_helper_mock()->SendUnsubscribeIVIRequest(
      ivi_id, application, app_mngr);
}

}  // namespace application_manager
