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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMANDS_TEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMANDS_TEST_H_

#include <stdint.h>
#include "gtest/gtest.h"

#include "application_manager/commands/command.h"
#include "smart_objects/smart_object.h"

#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/test/include/application_manager/mock_application.h"
#include "application_manager/test/include/application_manager/mock_hmi_interface.h"
#include "application_manager/test/include/application_manager/mock_message_helper.h"
#include "test/application_manager/mock_application_manager_settings.h"
namespace test {
namespace components {
namespace commands_test {

namespace am = ::application_manager;
namespace strings = am::strings;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using am::commands::MessageSharedPtr;
using ::smart_objects::SmartObject;
using ::test::components::application_manager_test::MockApplication;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using ::test::components::event_engine_test::MockEventDispatcher;

// Depending on the value type will be selected
template <const bool kIf, class ThenT, class ElseT>
struct TypeIf {
  typedef ThenT Result;
};
template <class ThenT, class ElseT>
struct TypeIf<false, ThenT, ElseT> {
  typedef ElseT Result;
};

template <class MockT, bool kIsNice>
struct IsNiceMock : public TypeIf<kIsNice, ::testing::NiceMock<MockT>, MockT> {
};

#define MOCK(Type) typename IsNiceMock<Type, kMocksAreNice>::Result

// If `kIsNice` is `true` then all used mock types
// will be wrapped by a `NiceMock`

enum CommandsTestMocks { kNotNice = 0, kIsNice };

template <const CommandsTestMocks kIsNice = CommandsTestMocks::kNotNice>
class CommandsTest : public ::testing::Test {
 public:
  enum { kMocksAreNice = kIsNice };

  typedef NiceMock<MockApplicationManagerSettings> MockAppManagerSettings;
  typedef NiceMock<application_manager_test::MockRPCService> MockRPCService;
  typedef typename TypeIf<kIsNice,
                          NiceMock<MockApplicationManager>,
                          MockApplicationManager>::Result MockAppManager;
  typedef typename TypeIf<kIsNice, NiceMock<MockApplication>, MockApplication>::
      Result MockApp;
  typedef std::shared_ptr<MockApp> MockAppPtr;

  virtual ~CommandsTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  static MessageSharedPtr CreateMessage(
      const smart_objects::SmartType type = smart_objects::SmartType_Null) {
    return std::make_shared<SmartObject>(type);
  }

  static MockAppPtr CreateMockApp() {
    return std::make_shared<MockApp>();
  }

  template <class Command>
  std::shared_ptr<Command> CreateCommand(const uint32_t timeout,
                                         MessageSharedPtr& msg) {
    InitCommand(timeout);
    return std::make_shared<Command>((msg ? msg : msg = CreateMessage()),
                                     app_mngr_,
                                     mock_rpc_service_,
                                     mock_hmi_capabilities_,
                                     mock_policy_handler_);
  }

  template <class Command>
  std::shared_ptr<Command> CreateCommand(MessageSharedPtr& msg) {
    return CreateCommand<Command>(kDefaultTimeout_, msg);
  }

  template <class Command>
  std::shared_ptr<Command> CreateCommand(
      const uint32_t timeout = kDefaultTimeout_) {
    InitCommand(timeout);
    MessageSharedPtr msg = CreateMessage();
    return std::make_shared<Command>(msg,
                                     app_mngr_,
                                     mock_rpc_service_,
                                     mock_hmi_capabilities_,
                                     mock_policy_handler_);
  }

  void InitEventDispatcher() {
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
  }

  void InitNegativeResponse() {
    MessageSharedPtr timeout_response =
        CommandsTest<kIsNice>::CreateMessage(smart_objects::SmartType_Map);
    (*timeout_response)[am::strings::msg_params][am::strings::result_code] =
        am::mobile_api::Result::GENERIC_ERROR;
    (*timeout_response)[am::strings::msg_params][am::strings::success] = false;

    ON_CALL(mock_message_helper_,
            CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
        .WillByDefault(Return(timeout_response));
  }

  enum { kDefaultTimeout_ = 100, kDefaultTimeoutCompensation_ = 10 };

  MockAppManager app_mngr_;
  MockRPCService mock_rpc_service_;
  testing::NiceMock<application_manager_test::MockHMICapabilities>
      mock_hmi_capabilities_;
  testing::NiceMock<policy_test::MockPolicyHandlerInterface>
      mock_policy_handler_;
  MockAppManagerSettings app_mngr_settings_;
  MockEventDispatcher event_dispatcher_;
  MOCK(am::MockHmiInterfaces) mock_hmi_interfaces_;
  am::MockMessageHelper& mock_message_helper_;

 protected:
  virtual void InitCommand(const uint32_t& timeout) {
    this->InitCommand(timeout, kDefaultTimeoutCompensation_);
  }

  virtual void InitCommand(const uint32_t timeout,
                           const uint32_t compensation) {
    timeout_ = timeout;
    timeout_compensation_ = compensation;
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, default_timeout())
        .WillByDefault(ReturnRef(timeout_));
    ON_CALL(app_mngr_settings_, default_timeout_compensation())
        .WillByDefault(ReturnRef(timeout_compensation_));
  }

  CommandsTest()
      : mock_message_helper_(*am::MockMessageHelper::message_helper_mock())
      , timeout_(0)
      , timeout_compensation_(0) {
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_SDL));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    InitHMIToMobileResultConverter();
  }

  void FillVideoStreamingCapability(
      smart_objects::SmartObject& video_streaming_capability) {
    video_streaming_capability[strings::preferred_resolution] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    video_streaming_capability[strings::preferred_resolution]
                              [strings::resolution_width] = 800;
    video_streaming_capability[strings::preferred_resolution]
                              [strings::resolution_height] = 354;
    video_streaming_capability[strings::max_bitrate] = 10000;
    video_streaming_capability[strings::supported_formats] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    video_streaming_capability[strings::supported_formats][0] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    video_streaming_capability[strings::supported_formats][0]
                              [strings::protocol] =
                                  hmi_apis::Common_VideoStreamingProtocol::RAW;
    video_streaming_capability[strings::supported_formats][0][strings::codec] =
        hmi_apis::Common_VideoStreamingCodec::H264;
    video_streaming_capability[strings::haptic_spatial_data_supported] = true;
    video_streaming_capability[strings::diagonal_screen_size] = 7.47;
    video_streaming_capability[strings::pixel_per_inch] = 117.f;
    video_streaming_capability[strings::scale] = 1.f;
  }

  void InitHMIToMobileResultConverter() {
    namespace MobileResult = mobile_apis::Result;
    namespace HMIResult = hmi_apis::Common_Result;
    auto link_hmi_to_mob_result = [this](HMIResult::eType hmi_result,
                                         MobileResult::eType mobile_result) {
      ON_CALL(mock_message_helper_, HMIToMobileResult(hmi_result))
          .WillByDefault(Return(mobile_result));
    };
    link_hmi_to_mob_result(HMIResult::INVALID_ENUM, MobileResult::INVALID_ENUM);
    link_hmi_to_mob_result(HMIResult::SUCCESS, MobileResult::SUCCESS);
    link_hmi_to_mob_result(HMIResult::UNSUPPORTED_REQUEST,
                           MobileResult::UNSUPPORTED_REQUEST);
    link_hmi_to_mob_result(HMIResult::UNSUPPORTED_RESOURCE,
                           MobileResult::UNSUPPORTED_RESOURCE);
    link_hmi_to_mob_result(HMIResult::DISALLOWED, MobileResult::DISALLOWED);
    link_hmi_to_mob_result(HMIResult::REJECTED, MobileResult::REJECTED);
    link_hmi_to_mob_result(HMIResult::ABORTED, MobileResult::ABORTED);
    link_hmi_to_mob_result(HMIResult::IGNORED, MobileResult::IGNORED);
    link_hmi_to_mob_result(HMIResult::RETRY, MobileResult::RETRY);
    link_hmi_to_mob_result(HMIResult::IN_USE, MobileResult::IN_USE);
    link_hmi_to_mob_result(HMIResult::TIMED_OUT, MobileResult::TIMED_OUT);
    link_hmi_to_mob_result(HMIResult::INVALID_DATA, MobileResult::INVALID_DATA);
    link_hmi_to_mob_result(HMIResult::CHAR_LIMIT_EXCEEDED,
                           MobileResult::CHAR_LIMIT_EXCEEDED);
    link_hmi_to_mob_result(HMIResult::INVALID_ID, MobileResult::INVALID_ID);
    link_hmi_to_mob_result(HMIResult::DUPLICATE_NAME,
                           MobileResult::DUPLICATE_NAME);
    link_hmi_to_mob_result(HMIResult::APPLICATION_NOT_REGISTERED,
                           MobileResult::APPLICATION_NOT_REGISTERED);
    link_hmi_to_mob_result(HMIResult::WRONG_LANGUAGE,
                           MobileResult::WRONG_LANGUAGE);
    link_hmi_to_mob_result(HMIResult::OUT_OF_MEMORY,
                           MobileResult::OUT_OF_MEMORY);
    link_hmi_to_mob_result(HMIResult::TOO_MANY_PENDING_REQUESTS,
                           MobileResult::TOO_MANY_PENDING_REQUESTS);
    link_hmi_to_mob_result(HMIResult::WARNINGS, MobileResult::WARNINGS);
    link_hmi_to_mob_result(HMIResult::GENERIC_ERROR,
                           MobileResult::GENERIC_ERROR);
    link_hmi_to_mob_result(HMIResult::USER_DISALLOWED,
                           MobileResult::USER_DISALLOWED);
    link_hmi_to_mob_result(HMIResult::TRUNCATED_DATA,
                           MobileResult::TRUNCATED_DATA);
    link_hmi_to_mob_result(HMIResult::SAVED, MobileResult::SAVED);
    link_hmi_to_mob_result(HMIResult::DATA_NOT_AVAILABLE,
                           MobileResult::DATA_NOT_AVAILABLE);
    link_hmi_to_mob_result(HMIResult::READ_ONLY, MobileResult::READ_ONLY);
  }

 private:
  uint32_t timeout_;
  uint32_t timeout_compensation_;
};

MATCHER_P(MobileResultCodeIs, result_code, "") {
  return result_code == static_cast<mobile_apis::Result::eType>(
                            (*arg)[application_manager::strings::msg_params]
                                  [application_manager::strings::result_code]
                                      .asInt());
}

MATCHER_P(HMIResultCodeIs, result_code, "") {
  return result_code == static_cast<hmi_apis::FunctionID::eType>(
                            (*arg)[application_manager::strings::params]
                                  [application_manager::strings::function_id]
                                      .asInt());
}

MATCHER_P3(
    HMIMessageParametersAre, correlation_id, function_id, result_code, "") {
  using namespace application_manager;

  const bool corr_ids_eq =
      correlation_id ==
      (*arg)[strings::params][strings::correlation_id].asInt();
  const bool func_ids_eq =
      (*arg)[strings::params][strings::function_id].asInt() == function_id;
  const bool res_codes_eq =
      (*arg)[strings::params][hmi_response::code].asInt() == result_code;

  return corr_ids_eq && func_ids_eq && res_codes_eq;
}

MATCHER_P3(MobileResponseIs, result_code, result_info, result_success, "") {
  mobile_apis::Result::eType code = static_cast<mobile_apis::Result::eType>(
      (*arg)[am::strings::msg_params][am::strings::result_code].asInt());
  std::string info =
      (*arg)[am::strings::msg_params][am::strings::info].asString();
  bool success = (*arg)[am::strings::msg_params][am::strings::success].asBool();
  return result_code == code && result_info == info &&
         result_success == success;
}

}  // namespace commands_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMANDS_TEST_H_
