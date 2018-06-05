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

#include "gtest/gtest.h"
#include "application_manager/application_manager.h"
#include "application_manager/hmi_language_handler.h"
#include "application_manager/state_controller.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_application.h"
#include "application_manager/smart_object_keys.h"
#include "test/resumption/mock_last_state.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "utils/lock.h"

namespace test {
namespace components {
namespace hmi_language_handler {

namespace am = ::application_manager;

using am::event_engine::Event;
using am::ApplicationSet;
using ::utils::SharedPtr;

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::_;

typedef NiceMock<
    ::test::components::application_manager_test::MockApplicationManager>
    MockApplicationManager;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;
typedef NiceMock< ::test::components::event_engine_test::MockEventDispatcher>
    MockEventDispatcher;
typedef NiceMock<application_manager_test::MockApplication> MockApp;

typedef SharedPtr<MockApp> ApplicationSharedPtr;
typedef am::HMILanguageHandler::Apps Apps;

namespace {
const uint32_t kDefaultAppsSize = 0u;
}  // namespace

class HmiLanguageHandlerTest : public ::testing::Test {
 public:
  HmiLanguageHandlerTest() {
    EXPECT_CALL(app_manager_, event_dispatcher())
        .WillOnce(ReturnRef(event_dispatcher_));
    hmi_language_handler_ =
        ::utils::MakeShared<am::HMILanguageHandler>(app_manager_);
  }

  void InitHMIActiveLanguages(hmi_apis::Common_Language::eType ui_language,
                              hmi_apis::Common_Language::eType vr_language,
                              hmi_apis::Common_Language::eType tts_language) {
    EXPECT_CALL(hmi_capabilities_, active_ui_language())
        .WillRepeatedly(Return(ui_language));
    EXPECT_CALL(hmi_capabilities_, active_vr_language())
        .WillRepeatedly(Return(vr_language));
    EXPECT_CALL(hmi_capabilities_, active_tts_language())
        .WillRepeatedly(Return(tts_language));
  }

  void InitHMICapabilitiesLanguages(
      hmi_apis::Common_Language::eType ui_language,
      hmi_apis::Common_Language::eType vr_language,
      hmi_apis::Common_Language::eType tts_language) {
    InitHMIActiveLanguages(ui_language, vr_language, tts_language);
    hmi_language_handler_->set_default_capabilities_languages(
        ui_language, vr_language, tts_language);
  }

  ApplicationSharedPtr CreateMockApp(const uint32_t app_id,
                                     bool expect_call = false) const {
    ApplicationSharedPtr app = ::utils::MakeShared<MockApp>();
    if (expect_call) {
      EXPECT_CALL(*app, app_id()).WillRepeatedly(Return(app_id));
    } else {
      ON_CALL(*app, app_id()).WillByDefault(Return(app_id));
    }
    return app;
  }

  void AddMockAppsToApplicationSet(ApplicationSet& app_set,
                                   const uint32_t start_app_id,
                                   uint32_t count,
                                   bool expect_call = false) {
    while (count-- > 0) {
      app_set.insert(CreateMockApp(start_app_id + count, expect_call));
    }
  }

  ::sync_primitives::Lock app_set_lock_;
  MockApplicationManager app_manager_;
  MockHMICapabilities hmi_capabilities_;
  MockEventDispatcher event_dispatcher_;
  SharedPtr<am::HMILanguageHandler> hmi_language_handler_;
  resumption_test::MockLastState last_state_;
};

TEST_F(HmiLanguageHandlerTest, OnEvent_AllLanguageIsReceivedAndSame_SUCCESS) {
  // Repeatedly add events to set `is_*_language_received_` flags up

  Event ui_event(hmi_apis::FunctionID::UI_GetLanguage);
  hmi_language_handler_->on_event(ui_event);

  Event vr_event(hmi_apis::FunctionID::VR_GetLanguage);
  hmi_language_handler_->on_event(vr_event);

  // After last flag gets up, `VerifyWithPersistedLanguages`
  // method been called to and then will call `hmi_capabilities`
  ON_CALL(app_manager_, hmi_capabilities())
      .WillByDefault(ReturnRef(hmi_capabilities_));

  // Set up `active_*_language` and
  //`persisted_ui_language_` to be the same
  Json::Value dictionary = Json::Value();
  ON_CALL(last_state_, get_dictionary()).WillByDefault(ReturnRef(dictionary));
  hmi_language_handler_->Init(&last_state_);
  InitHMICapabilitiesLanguages(hmi_apis::Common_Language::eType::EN_US,
                               hmi_apis::Common_Language::eType::EN_US,
                               hmi_apis::Common_Language::eType::EN_US);
  // Then `active_*_language` and
  //`persisted_ui_language_` will be compared.
  // So if they same app_manager_'s method `applications`
  // will never be called.
  EXPECT_CALL(app_manager_, applications()).Times(0);
  EXPECT_CALL(app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_manager_, UnregisterApplication(_, _, _, _)).Times(0);
  Event tts_event(hmi_apis::FunctionID::TTS_GetLanguage);
  hmi_language_handler_->on_event(tts_event);
}

TEST_F(HmiLanguageHandlerTest, OnEvent_AllReceivedLanguagesMismatch_SUCCESS) {
  Event ui_event(hmi_apis::FunctionID::UI_GetLanguage);
  hmi_language_handler_->on_event(ui_event);
  Event vr_event(hmi_apis::FunctionID::VR_GetLanguage);
  hmi_language_handler_->on_event(vr_event);

  ON_CALL(app_manager_, hmi_capabilities())
      .WillByDefault(ReturnRef(hmi_capabilities_));

  // Set up `active_*_language` and
  //`persisted_ui_language_` to be different
  hmi_language_handler_->set_default_capabilities_languages(
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US);

  InitHMIActiveLanguages(hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU);

  ApplicationSet app_set;
  const uint32_t kAppCount = 5u;
  // Adding to app set `kAppCount` apps with app_id form 0 to `kAppCount`.
  AddMockAppsToApplicationSet(app_set, 0u, kAppCount);
  DataAccessor<ApplicationSet> data_accessor(app_set, app_set_lock_);

  // Because `active_*_language` and
  //`persisted_ui_language_` are different,
  // the `applications` will be called and
  // app data will checked by `CheckApplication` method
  ON_CALL(app_manager_, applications()).WillByDefault(Return(data_accessor));
  EXPECT_CALL(app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_manager_, UnregisterApplication(_, _, _, _)).Times(0);

  Event tts_event(hmi_apis::FunctionID::TTS_GetLanguage);
  hmi_language_handler_->on_event(tts_event);
}

TEST_F(HmiLanguageHandlerTest, OnEvent_AllReceivedLanguagesMismatch_UNSUCCESS) {
  Event ui_event(hmi_apis::FunctionID::UI_GetLanguage);
  hmi_language_handler_->on_event(ui_event);
  Event vr_event(hmi_apis::FunctionID::VR_GetLanguage);
  hmi_language_handler_->on_event(vr_event);

  ON_CALL(app_manager_, hmi_capabilities())
      .WillByDefault(ReturnRef(hmi_capabilities_));

  // Set up `active_*_language` and
  //`persisted_ui_language_` to be different
  hmi_language_handler_->set_default_capabilities_languages(
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US);

  InitHMIActiveLanguages(hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU);

  ApplicationSet app_set;
  DataAccessor<ApplicationSet> data_accessor(app_set, app_set_lock_);

  // Send empty application set.
  ON_CALL(app_manager_, applications()).WillByDefault(Return(data_accessor));

  Event tts_event(hmi_apis::FunctionID::TTS_GetLanguage);
  hmi_language_handler_->on_event(tts_event);
}

TEST_F(HmiLanguageHandlerTest,
       SetHandleResponseRor_InvalidGetLanguage_UNSUCCESS) {
  // Sending requests with invalid data
  //`add_observer` method will be never called
  const hmi_apis::FunctionID::eType kFunctionId =
      hmi_apis::FunctionID::INVALID_ENUM;
  const uint32_t kCorrelationId = 0u;

  smart_objects::SmartObject request;

  EXPECT_CALL(event_dispatcher_, add_observer(_, _, _)).Times(0);
  hmi_language_handler_->set_handle_response_for(request);

  request[am::strings::params] = 0u;

  EXPECT_CALL(event_dispatcher_, add_observer(_, _, _)).Times(0);
  hmi_language_handler_->set_handle_response_for(request);

  request[am::strings::params][am::strings::function_id] = kFunctionId;

  EXPECT_CALL(event_dispatcher_, add_observer(_, _, _)).Times(0);
  hmi_language_handler_->set_handle_response_for(request);

  request[am::strings::params][am::strings::correlation_id] = kCorrelationId;

  EXPECT_CALL(event_dispatcher_, add_observer(_, _, _)).Times(0);
  hmi_language_handler_->set_handle_response_for(request);
}

TEST_F(HmiLanguageHandlerTest,
       SetHandleResponseRor_RequestsWithoutNeededKeys_SUCCESS) {
  // Sending requests with valid data
  //`add_observer` method will be called
  const hmi_apis::FunctionID::eType kFunctionId =
      hmi_apis::FunctionID::UI_GetLanguage;
  const uint32_t kCorrelationId = 0u;

  smart_objects::SmartObject request;
  request[am::strings::params][am::strings::function_id] = kFunctionId;
  request[am::strings::params][am::strings::correlation_id] = kCorrelationId;

  EXPECT_CALL(event_dispatcher_, add_observer(kFunctionId, kCorrelationId, _));
  hmi_language_handler_->set_handle_response_for(request);
}

TEST_F(HmiLanguageHandlerTest,
       HandleWrongLanguageApp_UnregisteredAppId_SUCCESS) {
  smart_objects::SmartObject msg;
  msg[am::strings::params][am::strings::app_id] = 5u;

  Event event(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
  event.set_smart_object(msg);

  EXPECT_CALL(app_manager_, hmi_capabilities()).Times(0);
  hmi_language_handler_->on_event(event);

  EXPECT_CALL(app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(hmi_capabilities_));

  // Set up `active_*_language` and
  //`persisted_ui_language_` to be different
  hmi_language_handler_->set_default_capabilities_languages(
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US);

  InitHMIActiveLanguages(hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU);

  // Needed to call of `ManageMobileCommand` method
  ON_CALL(*am::MockMessageHelper::message_helper_mock(),
          GetOnAppInterfaceUnregisteredNotificationToMobile(_, _))
      .WillByDefault(Return(::utils::MakeShared<smart_objects::SmartObject>()));

  // Wait for `ManageMobileCommand` call twice.
  // First time in `SendOnLanguageChangeToMobile`
  // method, second time in `HandleWrongLanguageApp`.
  EXPECT_CALL(app_manager_, ManageMobileCommand(_, _)).Times(2);
  EXPECT_CALL(app_manager_, UnregisterApplication(_, _, _, _)).Times(1);
  hmi_language_handler_->on_event(event);
}

TEST_F(HmiLanguageHandlerTest, OnUnregisterApp_SUCCESS) {
  const uint32_t app_id = 5u;
  smart_objects::SmartObject msg;
  msg[am::strings::params][am::strings::app_id] = app_id;

  Event event(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
  event.set_smart_object(msg);

  EXPECT_CALL(app_manager_, hmi_capabilities()).Times(0);
  hmi_language_handler_->on_event(event);

  hmi_language_handler_->OnUnregisterApplication(app_id);

  // Set up `active_*_language` and
  //`persisted_ui_language_` to be different
  hmi_language_handler_->set_default_capabilities_languages(
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US,
      hmi_apis::Common_Language::eType::EN_US);

  InitHMIActiveLanguages(hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU,
                         hmi_apis::Common_Language::eType::RU_RU);
  EXPECT_CALL(app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_manager_, UnregisterApplication(_, _, _, _)).Times(0);
  hmi_language_handler_->on_event(event);
}
}  // namespace hmi_language_handler
}  // namespace components
}  // namespace test
