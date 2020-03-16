#include "hmi/tts_is_ready_request.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "utils/helpers.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace tts_is_ready_request {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using sdl_rpc_plugin::commands::TTSIsReadyRequest;

typedef std::shared_ptr<TTSIsReadyRequest> TTSIsReadyRequestPtr;

class TTSIsReadyRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  TTSIsReadyRequestTest() : command_(CreateCommand<TTSIsReadyRequest>()) {}

  void SetUpExpectations(bool is_vr_cooperating_available,
                         bool is_send_message_to_hmi,
                         bool is_message_contains_param,
                         am::HmiInterfaces::InterfaceState state) {
    if (is_send_message_to_hmi) {
      ExpectSendMessagesToHMI();
    }
    EXPECT_CALL(mock_hmi_capabilities_,
                set_is_tts_cooperating(is_vr_cooperating_available));

    if (is_message_contains_param) {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillRepeatedly(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(
          mock_hmi_interfaces_,
          SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS, state));
    } else {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillOnce(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(mock_hmi_interfaces_, SetInterfaceState(_, _)).Times(0);
    }
    EXPECT_CALL(mock_hmi_interfaces_,
                GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
        .WillOnce(Return(state));
  }

  void ExpectSendMessagesToHMI() {
    smart_objects::SmartObjectSPtr language(
        std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Map));
    EXPECT_CALL(mock_message_helper_,
                CreateModuleInfoSO(hmi_apis::FunctionID::TTS_GetLanguage, _))
        .WillOnce(Return(language));
    EXPECT_CALL(mock_hmi_capabilities_, set_handle_response_for(*language));
    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(language, _));

    smart_objects::SmartObjectSPtr support_language(
        std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Map));
    EXPECT_CALL(
        mock_message_helper_,
        CreateModuleInfoSO(hmi_apis::FunctionID::TTS_GetSupportedLanguages, _))
        .WillOnce(Return(support_language));
    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(support_language, _));

    smart_objects::SmartObjectSPtr capabilities(
        std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Map));
    EXPECT_CALL(
        mock_message_helper_,
        CreateModuleInfoSO(hmi_apis::FunctionID::TTS_GetCapabilities, _))
        .WillOnce(Return(capabilities));
    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(capabilities, _));
  }

  void PrepareEvent(bool is_message_contain_param,
                    Event& event,
                    bool is_vr_cooperating_available = false) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    if (is_message_contain_param) {
      (*msg)[am::strings::msg_params][am::strings::available] =
          is_vr_cooperating_available;
    }
    event.set_smart_object(*msg);
  }

  void InterfacesUpdateExpectations(
      const std::set<hmi_apis::FunctionID::eType>& interfaces_to_update) {
    EXPECT_CALL(mock_hmi_capabilities_, GetDefaultInitializedCapabilities())
        .WillRepeatedly(Return(interfaces_to_update));
  }

  TTSIsReadyRequestPtr command_;
};

MATCHER_P(HMIFunctionIDIs, function_id, "") {
  const auto msg_function_id = static_cast<hmi_apis::FunctionID::eType>(
      (*arg)[am::strings::params][am::strings::function_id].asInt());

  return msg_function_id == function_id;
}

TEST_F(TTSIsReadyRequestTest,
       OnEvent_NoKeyAvailableInMessage_HmiInterfacesIgnored_CacheIsAbsent) {
  const bool is_tts_cooperating_available = false;
  const bool is_send_message_to_hmi = true;
  const bool is_message_contain_param = false;
  Event event(hmi_apis::FunctionID::TTS_IsReady);
  PrepareEvent(is_message_contain_param, event);
  std::set<hmi_apis::FunctionID::eType> interfaces_to_update{
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities};
  InterfacesUpdateExpectations(interfaces_to_update);
  SetUpExpectations(is_tts_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_RESPONSE);

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->on_event(event);
}

TEST_F(TTSIsReadyRequestTest,
       OnEven_KeyAvailableEqualToFalse_StateNotAvailable_CacheIsAbsent) {
  const bool is_tts_cooperating_available = false;
  const bool is_send_message_to_hmi = false;
  const bool is_message_contain_param = true;
  Event event(hmi_apis::FunctionID::TTS_IsReady);
  PrepareEvent(is_message_contain_param, event);
  SetUpExpectations(is_tts_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE);

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->on_event(event);
}

TEST_F(TTSIsReadyRequestTest,
       OnEven_KeyAvailableEqualToTrue_StateAvailable_CacheIsAbsnet) {
  const bool is_tts_cooperating_available = true;
  const bool is_send_message_to_hmi = true;
  const bool is_message_contain_param = true;
  Event event(hmi_apis::FunctionID::TTS_IsReady);
  PrepareEvent(is_message_contain_param, event, is_tts_cooperating_available);
  std::set<hmi_apis::FunctionID::eType> interfaces_to_update{
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities};
  InterfacesUpdateExpectations(interfaces_to_update);
  SetUpExpectations(is_tts_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_AVAILABLE);

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->on_event(event);
}

TEST_F(TTSIsReadyRequestTest,
       OnEven_HMIDoestRespond_SendMessageToHMIByTimeout_CacheIsAbsent) {
  std::set<hmi_apis::FunctionID::eType> interfaces_to_update{
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities};
  InterfacesUpdateExpectations(interfaces_to_update);
  ExpectSendMessagesToHMI();

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->onTimeOut();
}

TEST_F(TTSIsReadyRequestTest,
       OnEvent_TTSLanguageIsAbsentInCache_SendTTSGetLanguageRequest) {
  std::set<hmi_apis::FunctionID::eType> interfaces_to_update{
      hmi_apis::FunctionID::TTS_GetLanguage};

  Event event(hmi_apis::FunctionID::TTS_IsReady);

  InterfacesUpdateExpectations(interfaces_to_update);

  smart_objects::SmartObjectSPtr language(
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map));
  (*language)[am::strings::params][am::strings::function_id] =
      hmi_apis::FunctionID::TTS_GetLanguage;
  EXPECT_CALL(mock_message_helper_,
              CreateModuleInfoSO(hmi_apis::FunctionID::TTS_GetLanguage, _))
      .WillOnce(Return(language));
  EXPECT_CALL(mock_hmi_capabilities_, set_handle_response_for(*language));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetLanguage), _));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetSupportedLanguages), _))
      .Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetCapabilities), _))
      .Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->on_event(event);
}

TEST_F(
    TTSIsReadyRequestTest,
    OnEvent_TTSGetSupportedLanguagesIsAbsentInCache_SendTTSGetSupportedLanguagesRequest) {
  std::set<hmi_apis::FunctionID::eType> interfaces_to_update{
      hmi_apis::FunctionID::TTS_GetSupportedLanguages};

  Event event(hmi_apis::FunctionID::TTS_IsReady);

  InterfacesUpdateExpectations(interfaces_to_update);
  smart_objects::SmartObjectSPtr all_languages(
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map));
  (*all_languages)[am::strings::params][am::strings::function_id] =
      hmi_apis::FunctionID::TTS_GetSupportedLanguages;
  EXPECT_CALL(
      mock_message_helper_,
      CreateModuleInfoSO(hmi_apis::FunctionID::TTS_GetSupportedLanguages, _))
      .WillOnce(Return(all_languages));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetSupportedLanguages), _));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetLanguage), _))
      .Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetCapabilities), _))
      .Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->on_event(event);
}

TEST_F(
    TTSIsReadyRequestTest,
    OnEvent_TTSGetCapabilitiesIsAbsentInCache_SendTTSGetCapabilitiesRequest) {
  std::set<hmi_apis::FunctionID::eType> interfaces_to_update{
      hmi_apis::FunctionID::TTS_GetCapabilities};

  Event event(hmi_apis::FunctionID::TTS_IsReady);

  InterfacesUpdateExpectations(interfaces_to_update);
  smart_objects::SmartObjectSPtr capabilities(
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map));
  (*capabilities)[am::strings::params][am::strings::function_id] =
      hmi_apis::FunctionID::TTS_GetCapabilities;
  EXPECT_CALL(mock_message_helper_,
              CreateModuleInfoSO(hmi_apis::FunctionID::TTS_GetCapabilities, _))
      .WillOnce(Return(capabilities));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetCapabilities), _));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetLanguage), _))
      .Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIFunctionIDIs(hmi_apis::FunctionID::TTS_GetSupportedLanguages), _))
      .Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
  command_->on_event(event);
}

}  // namespace tts_is_ready_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
