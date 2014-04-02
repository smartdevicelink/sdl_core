/**
 * Copyright (c) 2013, Ford Motor Company
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
#include "rpc/application_manager_impl_test.h"
#include "application_manager/application_manager_impl.h"

namespace test {

ApplicationManagerImplTest::ApplicationManagerImplTest()
    : app_(NULL) {
  app_ = am::ApplicationManagerImpl::instance();
  //app_ = new application_manager::ApplicationManagerImpl;
  app_->application_list_;
}

ApplicationManagerImplTest::~ApplicationManagerImplTest() {
}

const std::map<int32_t, am::ApplicationSharedPtr>&
ApplicationManagerImplTest::GetApplications() {
  return app_->applications_;
}

const std::set<am::ApplicationSharedPtr>&
ApplicationManagerImplTest::GetApplicationList() {
  return app_->application_list_;
}

const std::list<am::CommandSharedPtr>&
ApplicationManagerImplTest::GetNotificationList() {
  return app_->notification_list_;
}

const std::map<const int32_t, const uint32_t>&
ApplicationManagerImplTest::GetAppIDList() {
  return app_->appID_list_;
}

bool ApplicationManagerImplTest::GetAudioPassThruActive() {
  return app_->audio_pass_thru_active_;
}

bool ApplicationManagerImplTest::GetIsDistractingDriver() {
  return app_->is_distracting_driver_;
}

bool ApplicationManagerImplTest::GetIsVrSessionStrated() {
  return app_->is_vr_session_strated_;
}

bool ApplicationManagerImplTest::GetHMICooperating() {
  return app_->hmi_cooperating_;
}

bool ApplicationManagerImplTest::GetIsAllAppsAllowed() {
  return app_->is_all_apps_allowed_;
}

const media_manager::MediaManager*
ApplicationManagerImplTest::GetMediaManager() {
  return app_->media_manager_;
}

const hmi_message_handler::HMIMessageHandler*
ApplicationManagerImplTest::GetHmiHandler() {
  return app_->hmi_handler_;
}

const connection_handler::ConnectionHandler*
ApplicationManagerImplTest::GetConnectionHandler() {
  return app_->connection_handler_;
}

const protocol_handler::ProtocolHandler*
ApplicationManagerImplTest::GetProtocolHandler() {
  return app_->protocol_handler_;
}

const policies::PolicyManager* ApplicationManagerImplTest::GetPolicyManager() {
  return app_->policy_manager_;
}

const am::HMICapabilities& ApplicationManagerImplTest::GetHmiCapabilities() {
  return app_->hmi_capabilities_;
}

const am::policies_manager::PoliciesManager&
ApplicationManagerImplTest::GetPoliciesManager() {
  return app_->policies_manager_;
}

const hmi_apis::HMI_API* ApplicationManagerImplTest::GetHmiSoFactory() {
  return app_->hmi_so_factory_;
}

const mobile_apis::MOBILE_API*
ApplicationManagerImplTest::GetMobileSoFactory() {
  return app_->mobile_so_factory_;
}
/*
uint32_t ApplicationManagerImplTest::GetCorelationId() {
  return app_->corelation_id_;
}

uint32_t ApplicationManagerImplTest::GetMaxCorelationId() {
  return app_->max_corelation_id_;
}
*/
am::mobile_api::AppInterfaceUnregisteredReason::eType
ApplicationManagerImplTest::GetUnregisterReason() {
  return app_->unregister_reason_;
}

const am::impl::FromMobileQueue&
ApplicationManagerImplTest::GetMessagesFromMobile() {
  return app_->messages_from_mobile_;
}

const am::impl::ToMobileQueue&
ApplicationManagerImplTest::GetMessagesToMobile() {
  return app_->messages_to_mobile_;
}

const am::impl::FromHmiQueue& ApplicationManagerImplTest::GetMessagesFromHmi() {
  return app_->messages_from_hmi_;
}

const am::impl::ToHmiQueue& ApplicationManagerImplTest::GetMessagesToHmi() {
  return app_->messages_to_hmi_;
}

void RegistrSO(utils::SharedPtr<smart::SmartObject> AppRegRequest) {
  (*AppRegRequest)[jsn::S_PARAMS][am::strings::function_id] =
  mobile_apis::FunctionID::RegisterAppInterfaceID;
  (*AppRegRequest)[jsn::S_PARAMS][am::strings::message_type] =
  mobile_apis::messageType::request;

  (*AppRegRequest)[jsn::S_PARAMS][am::strings::connection_key] = 65546;

  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::app_name] =
  "SyncProxyTester";
  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::available] = true;
  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::language_desired] =
  hmi_apis::Common_Language::EN_US;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
                   [am::strings::hmi_display_language_desired] =
                       hmi_apis::Common_Language::EN_US;

  (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65537";
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
  [am::strings::sync_msg_version]["majorVersion"] = 2;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
  [am::strings::sync_msg_version]["minorVersion"] = 2;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
  [am::strings::is_media_application] = true;
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
  [am::strings::app_hmi_type][0] = "NAVIGATION";
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
  [am::strings::ngn_media_screen_app_name] = "SyncP";
  (*AppRegRequest)[jsn::S_MSG_PARAMS]
  [am::strings::vr_synonyms][0] = "VR SyncProxyTester";
}

utils::SharedPtr<protocol_handler::RawMessage> ConvertSOToRawMess(
    utils::SharedPtr<smart::SmartObject> so) {
  application_manager::Message* mes = new application_manager::Message(
      protocol_handler::MessagePriority::kDefault);

  utils::SharedPtr<application_manager::Message> message_to_app(mes);
  std::string str;

  (*message_to_app).set_correlation_id(
      (*so)[jsn::S_PARAMS][am::strings::correlation_id].asInt());
  (*message_to_app).set_function_id(
      (*so)[jsn::S_PARAMS][am::strings::function_id].asInt());
  (*message_to_app).set_connection_key(
      (*so)[jsn::S_PARAMS][am::strings::connection_key].asInt());
  (*message_to_app).set_message_type(application_manager::kRequest);

  NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
      toString((*so), str);

  (*message_to_app).set_json_message(str);

  utils::SharedPtr<protocol_handler::RawMessage> raw_mes(
      am::MobileMessageHandler::HandleOutgoingMessageProtocolV2(
          message_to_app));

  return raw_mes;
}

void AddDevice(uint32_t value) {
  connection_handler::ConnectionHandlerImpl* connection_handle =
  connection_handler::ConnectionHandlerImpl::instance();

  transport_manager::DeviceHandle device_handler_test = value;
  std::string mac_address_test = "255.255.255.0";
  std::string name_test = "test_DeviceInfo";

  transport_manager::ConnectionUID connection_id_test = value;

  transport_manager::DeviceInfo device_info_test(
      device_handler_test,
      mac_address_test,
      name_test);

  connection_handle->OnDeviceAdded(device_info_test);

  connection_handle->OnConnectionEstablished(
      device_info_test,
      connection_id_test);
}

/*
 am::ApplicationSharedPtr RegistrApp() {
 connection_handler::ConnectionHandlerImpl* connection_handler_ =
 connection_handler::ConnectionHandlerImpl::instance();

 application_manager::ApplicationManagerImpl* app_manager_ =
 &app_mngr_impl_test_;

 app_manager_->set_connection_handler(connection_handler_);
 connection_handler_->set_connection_handler_observer(app_manager_);

 utils::SharedPtr<am::Application> appl(app_manager_->application(65537));

 if (appl.valid()) {
 return;
 }

 printf("\n\n\n Registration Application \n\n\n");

 transport_manager::DeviceHandle device_handler_test = 1;
 std::string mac_address_test = "255.255.255.0";
 std::string name_test = "test_DeviceInfo";

 transport_manager::ConnectionUID connection_id_test = 1;

 transport_manager::DeviceInfo device_info_test(
 device_handler_test,
 mac_address_test,
 name_test);

 // std::vector<transport_manager::DeviceInfo> device_list_test;
 // device_list_test.push_back(device_info_test);
 // connection_handler_->OnDeviceListUpdated(device_list_test);
 connection_handler_->OnDeviceAdded(device_info_test);

 connection_handler_->OnConnectionEstablished(
 device_info_test,
 connection_id_test);
 sleep(5);

 utils::SharedPtr<smart::SmartObject> AppRegRequest(new smart::SmartObject);

 // -------------------------------------------------------------------------
 (*AppRegRequest)[jsn::S_PARAMS][am::strings::function_id] =
 mobile_apis::FunctionID::RegisterAppInterfaceID;
 (*AppRegRequest)[jsn::S_PARAMS][am::strings::message_type] =
 mobile_apis::messageType::request;

 (*AppRegRequest)[jsn::S_PARAMS][am::strings::connection_key] = 65537;

 (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::app_name] =
 "SyncProxyTester";
 (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::available] = true;
 (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::language_desired] =
 hmi_apis::Common_Language::EN_US;
 (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::hmi_display_language_desired]
 = hmi_apis::Common_Language::EN_US;

 (*AppRegRequest)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65537";
 (*AppRegRequest)[jsn::S_MSG_PARAMS]
 [am::strings::sync_msg_version]["majorVersion"] = 2;
 (*AppRegRequest)[jsn::S_MSG_PARAMS]
 [am::strings::sync_msg_version]["minorVersion"] = 2;
 (*AppRegRequest)[jsn::S_MSG_PARAMS]
 [am::strings::is_media_application] = true;
 (*AppRegRequest)[jsn::S_MSG_PARAMS]
 [am::strings::app_hmi_type][0] = "NAVIGATION";
 (*AppRegRequest)[jsn::S_MSG_PARAMS]
 [am::strings::ngn_media_screen_app_name] = "SyncP";
 (*AppRegRequest)[jsn::S_MSG_PARAMS]
 [am::strings::vr_synonyms][0] = "VR SyncProxyTester";

 app_manager_->ManageMobileCommand(AppRegRequest);

 }
 */

}  // namespace test
