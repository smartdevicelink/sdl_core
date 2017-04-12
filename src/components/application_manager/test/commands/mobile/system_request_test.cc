/*
 * Copyright (c) 2017, Ford Motor Company
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
#include <set>

#include "gtest/gtest.h"

#include "application_manager/commands/mobile/system_request.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace system_request {

namespace am = application_manager;
using am::commands::SystemRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockHmiInterfaces;
using am::event_engine::Event;
using policy_test::MockPolicyHandlerInterface;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;

typedef SharedPtr<SystemRequest> SystemRequestPtr;

namespace {
const uint32_t kConnectionKey = 2u;
const std::string kAppPolicyId = "fake-app-id";
const uint32_t kDefaultTimeout = 1000u;
const std::string kAppFolderName = "app_folder";
const std::string kAppStorageFolder = ".";
const std::string kSystemFilesPath = ".";
const std::string kIVSU = "IVSU";
const std::string kSYNC = "SYNC";
}  // namespace

class SystemRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SystemRequestTest() : mock_app_(CreateMockApp()) {}

 protected:
  MessageSharedPtr CreateIVSUMessage() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::msg_params][am::strings::request_type] =
        mobile_apis::RequestType::HTTP;
    (*msg)[am::strings::msg_params][am::strings::file_name] = kIVSU;
    return msg;
  }

  void PreConditions() {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));

    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kAppPolicyId));
    ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(kAppFolderName));

    ON_CALL(app_mngr_settings_, system_files_path())
        .WillByDefault(ReturnRef(kSystemFilesPath));
    ON_CALL(app_mngr_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));

    ON_CALL(mock_policy_handler_, IsRequestTypeAllowed(kAppPolicyId, _))
        .WillByDefault(Return(true));
  }

  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    EXPECT_CALL(
        app_mngr_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandOrigin::ORIGIN_SDL));
  }

  void ExpectGenericErrorResult(const std::string& raw_binary_data) {
    MessageSharedPtr msg = CreateIVSUMessage();
    const std::vector<uint8_t> binary_data(raw_binary_data.begin(),
                                           raw_binary_data.end());

    (*msg)[am::strings::params][am::strings::binary_data] = binary_data;
    (*msg)[am::strings::msg_params][am::strings::request_type] =
        mobile_apis::RequestType::QUERY_APPS;
    SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

    PreConditions();

    EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, 0))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));

    MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
    const mobile_apis::Result::eType received_result =
        static_cast<mobile_apis::Result::eType>(
            (*result_msg)[am::strings::msg_params][am::strings::result_code]
                .asInt());
    EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, received_result);
  }

  MockAppPtr mock_app_;
  MockPolicyHandlerInterface mock_policy_handler_;
};

TEST_F(SystemRequestTest, Run_HTTP_FileName_IVSU_REJECTED) {
  PreConditions();
  MessageSharedPtr msg = CreateIVSUMessage();

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::REJECTED);

  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  command->Run();
}

TEST_F(SystemRequestTest, OnEvent_InvalidFunctionId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  event.set_smart_object(*event_msg);
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>());
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  command->on_event(event);
}

TEST_F(SystemRequestTest, OnEvent_InvalidApplication_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::BasicCommunication_SystemRequest);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;
  event.set_smart_object(*event_msg);

  MessageSharedPtr msg = CreateIVSUMessage();
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command->on_event(event);
}

TEST_F(SystemRequestTest, OnEvent_SendResponse_SUCCESS) {
  Event event(hmi_apis::FunctionID::BasicCommunication_SystemRequest);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::msg_params] = 0;
  event.set_smart_object(*event_msg);
  const std::string dir_src_path = kAppStorageFolder + "/" + kAppFolderName;
  const std::string file_src_path = dir_src_path + "/1" + kSYNC;
  ASSERT_EQ(dir_src_path, file_system::CreateDirectory(dir_src_path));
  ASSERT_TRUE(file_system::CreateFile(file_src_path));
  std::vector<uint8_t> data;
  data.push_back(1u);
  file_system::WriteBinaryFile(file_src_path, data);

  PreConditions();
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  utils::SharedPtr<am::AppFile> file = utils::MakeShared<am::AppFile>();
  file->is_download_complete = true;
  EXPECT_CALL(*mock_app_, GetFile(_)).WillOnce(Return(file.get()));

  command->Run();

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());

  EXPECT_EQ(mobile_apis::Result::SUCCESS, received_result);
  ASSERT_TRUE(file_system::DeleteFile(dir_src_path));
}

TEST_F(SystemRequestTest, Run_InvalidApplication_APPLICATION_NOT_REGISTERED) {
  MessageSharedPtr msg = CreateIVSUMessage();
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED, received_result);
}

TEST_F(SystemRequestTest, Run_RequestTypeNotAllowed_DISALLOWED) {
  MessageSharedPtr msg = CreateIVSUMessage();
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();
  EXPECT_CALL(mock_policy_handler_, IsRequestTypeAllowed(kAppPolicyId, _))
      .WillOnce(Return(false));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::DISALLOWED, received_result);
}

TEST_F(SystemRequestTest, Run_FileNameHasForbiddenSymbol_INVALID_DATA) {
  MessageSharedPtr msg = CreateIVSUMessage();
  (*msg)[am::strings::msg_params][am::strings::file_name] = "/";
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, received_result);
}

TEST_F(SystemRequestTest, Run_CantSaveBinaryData_GENERIC_ERROR) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::params][am::strings::binary_data] = 1u;
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();
  const std::string file_name = "2" + kSYNC;
  EXPECT_CALL(app_mngr_, SaveBinary(_, _, file_name, 0))
      .WillOnce(Return(mobile_apis::Result::INVALID_ENUM));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, received_result);
}

TEST_F(SystemRequestTest, Run_ReceiveMessageFromSdk_SUCCESS) {
  MessageSharedPtr msg = CreateIVSUMessage();
  (*msg)[am::strings::params][am::strings::binary_data] = 1u;
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();
  std::vector<uint8_t> binary_data =
      (*msg)[am::strings::params][am::strings::binary_data].asBinary();
  EXPECT_CALL(app_mngr_, SaveBinary(binary_data, _, _, 0))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_policy_handler_, ReceiveMessageFromSDK(kIVSU, _))
      .WillOnce(Return(true));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::SUCCESS, received_result);
}

TEST_F(SystemRequestTest, Run_BinaryDataInvalidJson_UNSUCCESS) {
  MessageSharedPtr msg = CreateIVSUMessage();
  (*msg)[am::strings::params][am::strings::binary_data] = 1u;
  (*msg)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::QUERY_APPS;
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();
  std::vector<uint8_t> binary_data =
      (*msg)[am::strings::params][am::strings::binary_data].asBinary();
  EXPECT_CALL(app_mngr_, SaveBinary(binary_data, _, _, 0))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  command->Run();
}

TEST_F(SystemRequestTest, Run_InvalidQueryAppData_GENERIC_ERROR) {
  MessageSharedPtr msg = CreateIVSUMessage();
  const std::vector<uint8_t> binary_data(
      {'{', '\"', 'a', 'p', 'p', '\"', ':', '2', '}'});

  (*msg)[am::strings::params][am::strings::binary_data] = binary_data;

  (*msg)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::QUERY_APPS;
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();

  EXPECT_CALL(app_mngr_, SaveBinary(binary_data, _, _, 0))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, received_result);
}

TEST_F(SystemRequestTest, Run_ResponseIsNotArrayInBinaryData_GENERIC_ERROR) {
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\":"
      "{"
      "\"_id\": \"53f75cfb891ec700002d3592\""
      "}"
      "};";
  ExpectGenericErrorResult(raw_binary_data);
}

TEST_F(SystemRequestTest,
       Run_MandatoryParamPackageNameNotExists_GENERIC_ERROR) {
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\": ["
      "{"
      "\"_id\": \"45275cfb891ec700002d3845\","
      "\"__v\": 0,"
      "\"android\": {"
      "\"appHmiType\": \"MEDIA\","
      "\"category\": \"MusicAndAudio\","
      "\"isMediaApplication\": true,"
      "\"playStoreUrl\": "
      "\"http://play.google.com/store/apps/details?id=com.crappy.fake\","
      "\"sdlMaxVersion\": \"2.0\","
      "\"sdlMinVersion\": \"1.0\""
      "},"
      "\"appId\": \"9675308\","
      "\"development\": false,"
      "\"iconUrl\": \"http://i.imgur.com/R3a11YFAk3.png\","
      "\"name\": \"SPTAlice\""
      "}"
      "]"
      "};";
  ExpectGenericErrorResult(raw_binary_data);
}

TEST_F(SystemRequestTest, Run_MandatoryParamUrlSchemaNotExists_GENERIC_ERROR) {
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\": ["
      "{"
      "\"appId\": \"853426\","
      "\"ios\": {"
      "\"appHmiType\": \"MEDIA\""
      "}"
      "}"
      "]"
      "};";
  ExpectGenericErrorResult(raw_binary_data);
}

TEST_F(SystemRequestTest, Run_MandatoryParamIosAndroidNotExists_GENERIC_ERROR) {
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\": ["
      "{"
      "\"_id\": \"53f75cfb891ec700002d3592\""
      "}"
      "]"
      "};";
  ExpectGenericErrorResult(raw_binary_data);
}

TEST_F(SystemRequestTest, Run_MandatoryParamNameNotExists_GENERIC_ERROR) {
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\": ["
      "{"
      "\"appId\": \"853426\","
      "\"ios\": {"
      "\"urlScheme\": \"awesomemusicapp://\""
      "}"
      "},"
      "{"
      "\"android\": {"
      "\"packageName\": \"com.ford.syncV4.SPTAlice\""
      "},"
      "\"appId\": \"9675308\","
      "\"iconUrl\": \"http://i.imgur.com/R3a11YFAk3.png\""
      "}"
      "]"
      "};";
  ExpectGenericErrorResult(raw_binary_data);
}

TEST_F(SystemRequestTest, Run_MandatoryParamAppIdNotExists_GENERIC_ERROR) {
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\": ["
      "{"
      "\"ios\": {"
      "\"urlScheme\": \"awesomemusicapp://\""
      "},"
      "\"name\": \"Awesome Music App\""
      "},"
      "{"
      "\"android\": {"
      "\"packageName\": \"com.ford.syncV4.SPTAlice\""
      "},"
      "\"name\": \"SPTAlice\""
      "}"
      "]"
      "};";
  ExpectGenericErrorResult(raw_binary_data);
}

TEST_F(SystemRequestTest, Run_ValidateQueryAppData_SUCCESS) {
  MessageSharedPtr msg = CreateIVSUMessage();
  const std::string raw_binary_data =
      "{ \"status\": \"200 ok\","
      "\"responseType\": \"array\","
      "\"response\": ["
      "{"
      "\"_id\": \"53f75cfb891ec700002d3592\","
      "\"__v\": 0,"
      "\"appId\": \"853426\","
      "\"development\": false,"
      "\"iconUrl\": \"http://i.imgur.com/S0FAk3.png\","
      "\"ios\": {"
      "\"appHmiType\": \"MEDIA\","
      "\"category\": \"Music\","
      "\"isMediaApplication\": true,"
      "\"itunesUrl\": "
      "\"http://itunes.apple.com/app/awesome-music-app/id324384482?mt=8\","
      "\"sdlMaxVersion\": \"3.0\","
      "\"sdlMinVersion\": \"1.0\","
      "\"urlScheme\": \"awesomemusicapp://\","
      "\"languages\": ["
      "{"
      "\"default\": {"
      "\"ttsName\": \"Default Music\","
      "\"vrSynonyms\": ["
      "\"Default App1\","
      "\"Default Awesomeness1\"]"
      "}"
      "},"
      "{"
      "\"EN-US\": {"
      "\"ttsName\": \"Awesome Music\","
      "\"vrSynonyms\": ["
      "\"Awesome App1\","
      "\"Music Awesomeness1\"]"
      "}"
      "}"
      "]"
      "},"
      "\"name\": \"Awesome Music App\""
      "},"
      "{"
      "\"_id\": \"45275cfb891ec700002d3845\","
      "\"__v\": 0,"
      "\"android\": {"
      "\"appHmiType\": \"MEDIA\","
      "\"category\": \"MusicAndAudio\","
      "\"isMediaApplication\": true,"
      "\"packageName\": \"com.ford.syncV4.SPTAlice\","
      "\"playStoreUrl\": "
      "\"http://play.google.com/store/apps/details?id=com.crappy.fake\","
      "\"sdlMaxVersion\": \"2.0\","
      "\"sdlMinVersion\": \"1.0\","
      "\"languages\": ["
      "{"
      "\"default\": {"
      "\"ttsName\": \"Default Music\","
      "\"vrSynonyms\": ["
      "\"Default App\","
      "\"Default Awesomeness\"]"
      "}"
      "},"
      "{"
      "\"EN-US\": {"
      "\"ttsName\": \"Awesome Music\","
      "\"vrSynonyms\": ["
      "\"Awesome App\","
      "\"Music Awesomeness\"]"
      "}"
      "}"
      "]"
      "},"
      "\"appId\": \"9675308\","
      "\"development\": false,"
      "\"iconUrl\": \"http://i.imgur.com/R3a11YFAk3.png\","
      "\"name\": \"SPTAlice\""
      "}"
      "]"
      "};";
  const std::vector<uint8_t> binary_data(raw_binary_data.begin(),
                                         raw_binary_data.end());

  (*msg)[am::strings::params][am::strings::binary_data] = binary_data;
  (*msg)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::QUERY_APPS;
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));

  PreConditions();

  EXPECT_CALL(app_mngr_, application_by_policy_id(_))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, 0))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::SUCCESS, received_result);
}

}  // namespace system_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
