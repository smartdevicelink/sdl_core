/*
 * Copyright (c) 2016, Ford Motor Company
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
#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "application_manager/commands/mobile/send_location_request.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "utils/shared_ptr.h"
#include "commands/command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace send_location_request {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::SendLocationRequest;
using application_manager::MockMessageHelper;
using test::components::application_manager_test::MockHMICapabilities;
using smart_objects::SmartObject;
using utils::SharedPtr;
using testing::_;
using testing::Return;
using testing::ReturnRef;
using ::testing::Mock;

namespace strings = application_manager::strings;
namespace hmi_response = application_manager::hmi_response;
namespace Common_TextFieldName = hmi_apis::Common_TextFieldName;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kFunctionID = 2u;
const uint32_t kAppID = 3u;
const double kLongitudeDegrees = 13.4;
const std::string kLocationDescription = "Location_Description";
const std::string kNewLine = "\\n";
const std::string kAddressLine = "Address_Line";
const std::string kPhoneNumber = "Phone_number";
const std::string kCorrectAddress = "Correct address";
const std::string kEmptyAddress = "";
const std::string kAddressWithWrongSyntax = "Wrong Syntax\\n";
}  // namespace

class SendLocationRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  class UnwrappedSendLocationRequest : public SendLocationRequest {
   public:
    UnwrappedSendLocationRequest(
        const MessageSharedPtr& message,
        application_manager::ApplicationManager& application_manager)
        : SendLocationRequest(message, application_manager) {}

    ::application_manager::CommandParametersPermissions&
    get_parameters_permissions() {
      return parameters_permissions_;
    }
  };

  typedef SharedPtr<UnwrappedSendLocationRequest> CommandSPrt;

  SendLocationRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    mock_app_ = CreateMockApp();
    disp_cap_ = utils::MakeShared<SmartObject>(smart_objects::SmartType_Map);
    message_ = CreateMessage();
    command_ = CreateCommand<UnwrappedSendLocationRequest>(message_);
    navi_caps_so = SmartObject(smart_objects::SmartType_Map);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void InitialSetup(MessageSharedPtr message_) {
    (*message_)[strings::params][strings::connection_key] = kConnectionKey;
    (*message_)[strings::params][strings::function_id] = kFunctionID;
    (*message_)[strings::msg_params] =
        SmartObject(smart_objects::SmartType_Map);
    (*message_)[strings::msg_params][strings::address] = kCorrectAddress;
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));

    AllowMandatoryFields();
  }

  void HMICapabilitiesSetupHelper() {
    EXPECT_CALL(app_mngr_, hmi_capabilities())
        .WillOnce(ReturnRef(mock_hmi_capabilities_));
    EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating())
        .WillOnce(Return(true));
    EXPECT_CALL(mock_hmi_capabilities_, is_navi_cooperating())
        .WillOnce(Return(true));
    navi_caps_so["sendLocationEnabled"] = true;
    EXPECT_CALL(mock_hmi_capabilities_, navigation_capability())
        .WillOnce(Return(&navi_caps_so));
  }

  void HMICapabilitiesSetupWithArguments(
      Common_TextFieldName::eType field_name) {
    HMICapabilitiesSetupHelper();
    (*disp_cap_)[hmi_response::text_fields] =
        SmartObject(smart_objects::SmartType_Array);
    (*disp_cap_)[hmi_response::text_fields][0] =
        SmartObject(smart_objects::SmartType_Map);
    (*disp_cap_)[hmi_response::text_fields][0][strings::name] = field_name;
    EXPECT_CALL(mock_hmi_capabilities_, display_capabilities())
        .Times(2)
        .WillRepeatedly(Return(disp_cap_.get()));
  }

  void FinishSetup() {
    EXPECT_CALL(*mock_app_, hmi_app_id()).WillOnce(Return(kAppID));
    EXPECT_CALL(app_mngr_,
                ManageHMICommand(HMIResultCodeIs(
                    hmi_apis::FunctionID::Navigation_SendLocation)));
  }

  void FinishSetupCancelled(mobile_apis::Result::eType result) {
    EXPECT_CALL(*mock_app_, hmi_app_id()).Times(0);
    EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
    EXPECT_CALL(app_mngr_, ManageMobileCommand(MobileResultCodeIs(result), _));
  }

  void AllowMandatoryFields() {
    application_manager::CommandParametersPermissions& permissions =
        command_->get_parameters_permissions();
    permissions.allowed_params.insert(strings::longitude_degrees);
    permissions.allowed_params.insert(strings::latitude_degrees);
  }

  MockAppPtr mock_app_;
  MockHMICapabilities mock_hmi_capabilities_;
  MockMessageHelper& mock_message_helper_;
  SharedPtr<SmartObject> disp_cap_;
  MessageSharedPtr message_;
  CommandSPrt command_;
  smart_objects::SmartObject navi_caps_so;
};

TEST_F(SendLocationRequestTest, Run_InvalidApp_Success) {
  (*message_)[strings::params][strings::connection_key] = kConnectionKey;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  FinishSetupCancelled(mobile_apis::Result::APPLICATION_NOT_REGISTERED);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_DeliveryMode_Success) {
  InitialSetup(message_);
  SmartObject msg_params = (*message_)[strings::msg_params];
  msg_params[strings::delivery_mode] = SmartObject();
  (*message_)[strings::msg_params] = msg_params;
  msg_params.erase(strings::delivery_mode);
  HMICapabilitiesSetupHelper();
  FinishSetup();
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationNameIsAllowed_Success) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::location_name] = "Location_Name";
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::locationName);
  FinishSetup();
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationNameIsNotAllowed_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::location_name] = "Location_Name";
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::INVALID_ENUM);
  FinishSetupCancelled(mobile_apis::Result::UNSUPPORTED_RESOURCE);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationNameWrongSyntax_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::location_name] = "Location_Name\\n";
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::locationName);
  FinishSetupCancelled(mobile_apis::Result::INVALID_DATA);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationDescriptionIsAllowed_Success) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::location_description] =
      kLocationDescription;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::locationDescription);
  FinishSetup();
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationDescriptionIsNotAllowed_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::location_description] =
      kLocationDescription;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::INVALID_ENUM);
  FinishSetupCancelled(mobile_apis::Result::UNSUPPORTED_RESOURCE);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationDescriptionWrongSyntax_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::location_description] =
      kLocationDescription + kNewLine;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::locationDescription);
  FinishSetupCancelled(mobile_apis::Result::INVALID_DATA);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_AddressLinesAreAllowed_Success) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::address_lines] =
      SmartObject(smart_objects::SmartType_Array);
  (*message_)[strings::msg_params][strings::address_lines][0] = kAddressLine;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::addressLines);
  FinishSetup();
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_AddressLinesAreNotAllowed_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::address_lines] =
      SmartObject(smart_objects::SmartType_Array);
  (*message_)[strings::msg_params][strings::address_lines][0] = kAddressLine;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::INVALID_ENUM);
  FinishSetupCancelled(mobile_apis::Result::UNSUPPORTED_RESOURCE);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_AddressLinesWrongSyntax_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::address_lines] =
      SmartObject(smart_objects::SmartType_Array);
  (*message_)[strings::msg_params][strings::address_lines][0] =
      kAddressLine + kNewLine;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::addressLines);
  FinishSetupCancelled(mobile_apis::Result::INVALID_DATA);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_PhoneNumberIsAllowed_Success) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::phone_number] = kPhoneNumber;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::phoneNumber);
  FinishSetup();
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_PhoneNumberIsNotAllowed_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::phone_number] = kPhoneNumber;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::INVALID_ENUM);
  FinishSetupCancelled(mobile_apis::Result::UNSUPPORTED_RESOURCE);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_PhoneNumberWrongSyntax_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::phone_number] =
      kPhoneNumber + kNewLine;
  HMICapabilitiesSetupWithArguments(Common_TextFieldName::phoneNumber);
  FinishSetupCancelled(mobile_apis::Result::INVALID_DATA);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_AddressesContainWrongSyntax_Cancelled) {
  InitialSetup(message_);
  HMICapabilitiesSetupHelper();
  (*message_)[strings::msg_params][strings::address] =
      SmartObject(smart_objects::SmartType_Array);
  (*message_)[strings::msg_params][strings::address]["Address 1"] =
      kCorrectAddress;
  (*message_)[strings::msg_params][strings::address]["Address 2"] =
      kAddressWithWrongSyntax;
  FinishSetupCancelled(mobile_apis::Result::INVALID_DATA);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationImageValid_Success) {
  InitialSetup(message_);
  HMICapabilitiesSetupHelper();
  (*message_)[strings::msg_params][strings::location_image] =
      SmartObject(smart_objects::SmartType_Map);
  (*message_)[strings::msg_params][strings::location_image][strings::value] =
      "1";
  EXPECT_CALL(
      mock_message_helper_,
      VerifyImage(
          (*message_)[strings::msg_params][strings::location_image], _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  FinishSetup();
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_LocationImageInvalid_Cancelled) {
  InitialSetup(message_);
  HMICapabilitiesSetupHelper();
  (*message_)[strings::msg_params][strings::location_image] =
      SmartObject(smart_objects::SmartType_Map);
  (*message_)[strings::msg_params][strings::location_image][strings::value] =
      "1";
  EXPECT_CALL(
      mock_message_helper_,
      VerifyImage(
          (*message_)[strings::msg_params][strings::location_image], _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  FinishSetupCancelled(mobile_apis::Result::ABORTED);
  command_->Run();
}

TEST_F(SendLocationRequestTest, Run_HMIUINotCoop_Cancelled) {
  InitialSetup(message_);
  (*message_)[strings::msg_params][strings::phone_number] = kPhoneNumber;
  (*disp_cap_)[hmi_response::text_fields] =
      SmartObject(smart_objects::SmartType_Array);
  (*disp_cap_)[hmi_response::text_fields][0] =
      SmartObject(smart_objects::SmartType_Map);
  (*disp_cap_)[hmi_response::text_fields][0][strings::name] =
      Common_TextFieldName::phoneNumber;
  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillOnce(Return(false));
  FinishSetupCancelled(mobile_apis::Result::UNSUPPORTED_RESOURCE);
  command_->Run();
}

TEST_F(SendLocationRequestTest, OnEvent_Success) {
  mobile_apis::Result::eType response_code = mobile_apis::Result::SUCCESS;
  (*message_)[strings::params][hmi_response::code] = response_code;
  (*message_)[strings::params][strings::connection_key] = kConnectionKey;

  Event event(hmi_apis::FunctionID::Navigation_SendLocation);
  event.set_smart_object(*message_);

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));

  command_->on_event(event);
}

TEST_F(SendLocationRequestTest, OnEvent_Cancelled) {
  (*message_)[strings::params][hmi_response::code] =
      mobile_apis::Result::SUCCESS;
  Event event(hmi_apis::FunctionID::Navigation_OnWayPointChange);
  event.set_smart_object(*message_);
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .Times(0);
  command_->on_event(event);
}

TEST_F(SendLocationRequestTest,
       Run_MandatoryParamsDisallowedByUser_UserDisallowed) {
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  application_manager::CommandParametersPermissions& permissions =
      command_->get_parameters_permissions();
  // 1st one allowed
  permissions.allowed_params.insert(strings::longitude_degrees);
  // 2nd one disallowed
  permissions.disallowed_params.insert(strings::latitude_degrees);

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::USER_DISALLOWED), _));

  command_->Run();
}

TEST_F(SendLocationRequestTest,
       Run_MandatoryParamsDisallowedByPolicy_Disallowed) {
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  application_manager::CommandParametersPermissions& permissions =
      command_->get_parameters_permissions();
  // 1st one allowed
  permissions.allowed_params.insert(strings::longitude_degrees);
  // 2nd one undefined
  permissions.undefined_params.insert(strings::latitude_degrees);

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DISALLOWED), _));

  command_->Run();
}

}  // namespace send_location_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
