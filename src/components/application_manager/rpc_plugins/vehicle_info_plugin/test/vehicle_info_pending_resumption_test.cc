/*
 * Copyright (c) 2020, Ford Motor Company
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
 * POSSIBILITY OF SUCH DAMAGE. */

#include <boost/range/algorithm/set_algorithm.hpp>
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_event_observer.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_resume_ctrl.h"
#include "application_manager/mock_resumption_data_processor.h"
#include "application_manager/mock_rpc_service.h"
#include "gtest/gtest.h"
#include "vehicle_info_plugin/mock_custom_vehicle_data_manager.h"
#include "vehicle_info_plugin/vehicle_info_pending_resumption_handler.h"
#include "vehicle_info_plugin/vehicle_info_plugin.h"

namespace vehicle_info_plugin_test {
using namespace vehicle_info_plugin;

using ::testing::_;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;

using application_manager::MockMessageHelper;
using test::components::resumption_test::MockResumeCtrl;
using test::components::resumption_test::MockResumptionDataProcessor;
typedef NiceMock< ::test::components::event_engine_test::MockEventDispatcher>
    MockEventDispatcher;
typedef NiceMock< ::test::components::event_engine_test::MockEventObserver>
    MockEventObserver;
typedef NiceMock<
    ::test::components::application_manager_test::MockApplicationManager>
    MockApplicationManager;
typedef NiceMock< ::test::components::application_manager_test::MockApplication>
    MockApplication;
typedef NiceMock<test::components::application_manager_test::MockRPCService>
    MockRPCService;
typedef std::shared_ptr<MockApplication> MockAppPtr;
typedef std::shared_ptr<vehicle_info_plugin::VehicleInfoAppExtension>
    VehicleInfoAppExtensionPtr;
using hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData;

smart_objects::SmartObjectSPtr CreateVDRequest(const uint32_t corr_id) {
  using namespace application_manager;
  smart_objects::SmartObjectSPtr request =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *request;
  const auto message_type_request = 0;
  object[strings::params][strings::message_type] = message_type_request;
  object[strings::params][strings::function_id] =
      static_cast<int>(VehicleInfo_SubscribeVehicleData);

  object[strings::params][strings::correlation_id] = corr_id;
  object[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  return request;
}

mobile_apis::VehicleDataType::eType ToVDType(const std::string& vd) {
  if ("gps" == vd) {
    return mobile_apis::VehicleDataType::VEHICLEDATA_GPS;
  }
  if ("speed" == vd) {
    return mobile_apis::VehicleDataType::VEHICLEDATA_SPEED;
  }
  return mobile_apis::VehicleDataType::VEHICLEDATA_OEM_CUSTOM_DATA;
}

smart_objects::SmartObject CreateVDError(const uint32_t correlation_id,
                                         const int32_t result_code,
                                         const std::string& info) {
  namespace strings = application_manager::strings;
  namespace hmi_response = application_manager::hmi_response;
  smart_objects::SmartObject message(smart_objects::SmartType_Map);

  message[strings::params][strings::function_id] =
      VehicleInfo_SubscribeVehicleData;
  const auto hmi_protocol_type = 1;
  message[strings::params][strings::protocol_type] = hmi_protocol_type;
  message[strings::params][strings::correlation_id] = correlation_id;

  const auto message_type_response = 1;
  message[strings::params][strings::message_type] = message_type_response;
  message[strings::params][hmi_response::code] = result_code;
  message[strings::params][strings::error_msg] = info;

  return message;
}

smart_objects::SmartObjectSPtr CreateHMIResponseMessage(
    const hmi_apis::Common_Result::eType common_result,
    uint32_t correlation_id) {
  namespace strings = application_manager::strings;
  namespace hmi_response = application_manager::hmi_response;
  smart_objects::SmartObject params(smart_objects::SmartType_Map);
  params[strings::function_id] = VehicleInfo_SubscribeVehicleData;
  params[strings::message_type] = application_manager::MessageType::kResponse;
  params[strings::correlation_id] = correlation_id;
  const auto hmi_protocol_type = 1;
  params[strings::protocol_type] = hmi_protocol_type;
  params[hmi_response::code] = common_result;

  smart_objects::SmartObjectSPtr response =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  auto& message = *response;
  message[strings::params] = params;
  message[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  return response;
}

typedef std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
    VDResponseMap;
smart_objects::SmartObject CreateVDResponse(
    const hmi_apis::Common_Result::eType common_result,
    const VDResponseMap& subscriptions_result,
    uint32_t correleation_id) {
  namespace strings = application_manager::strings;
  namespace hmi_response = application_manager::hmi_response;

  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  for (const auto& subscription : subscriptions_result) {
    smart_objects::SmartObject subscription_result(
        smart_objects::SmartType_Map);
    subscription_result[strings::data_type] = ToVDType(subscription.first);
    subscription_result[strings::result_code] = subscription.second;
    msg_params[subscription.first] = subscription_result;
  }
  auto response = CreateHMIResponseMessage(common_result, correleation_id);
  (*response)[strings::msg_params] = msg_params;
  return *response;
}

enum class ContainsPolicy { Strict, Nice };

bool CheckThatMessageContainsVD(
    const smart_objects::SmartObject& message,
    const std::set<std::string>& vehicle_data,
    ContainsPolicy policy = ContainsPolicy::Strict) {
  using namespace application_manager;
  auto& msg_params = message[strings::msg_params];
  auto keys = msg_params.enumerate();
  std::set<std::string> missed;
  boost::set_difference(
      vehicle_data, keys, std::inserter(missed, missed.end()));
  if (missed.size() > 0) {
    return false;
  }
  if (ContainsPolicy::Strict == policy) {
    std::set<std::string> redundant;
    boost::set_difference(
        keys, vehicle_data, std::inserter(redundant, redundant.end()));
    if (redundant.size() > 0) {
      return false;
    }
  }
  return true;
}

/**
 * @brief EventCheck check that event contains apropriate vehicle data,
 * check that is matched correlation id,
 * check that function id is correct
 */
MATCHER_P2(EventCheck, expected_corr_id, vehicle_data, "") {
  namespace strings = application_manager::strings;
  const auto& response_message = arg.smart_object();
  const bool vehicle_data_ok =
      CheckThatMessageContainsVD(response_message, vehicle_data);
  const auto cid =
      response_message[strings::params][strings::correlation_id].asInt();
  const bool cid_ok = (cid == expected_corr_id);
  const auto fid =
      response_message[strings::params][strings::function_id].asInt();
  const bool fid_ok = (fid == VehicleInfo_SubscribeVehicleData);
  return fid_ok && cid_ok && vehicle_data_ok;
}

// Replace correlation id in the message shared pointer with argument specified
// in template
ACTION_TEMPLATE(ReplaceCorrelationId,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(pointer)) {
  namespace strings = application_manager::strings;
  (**pointer)[strings::params][strings::correlation_id] =
      ::std::tr1::get<k>(args);
}

class VehicleInfoPendingResumptionHandlerTest : public ::testing::Test {
  // using namespace vehicle_info_plugin;
 public:
  VehicleInfoPendingResumptionHandlerTest()
      : mock_message_helper_(
            *application_manager::MockMessageHelper::message_helper_mock())
      , applications_lock_(std::make_shared<sync_primitives::Lock>())
      , applications_(application_set_, applications_lock_) {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void SetUp() OVERRIDE {
    ON_CALL(app_manager_mock_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_mock_));
    ON_CALL(app_manager_mock_, GetRPCService())
        .WillByDefault(ReturnRef(mock_rpc_service_));
    ON_CALL(app_manager_mock_, resume_controller())
        .WillByDefault(ReturnRef(resume_ctrl_mock_));
    ON_CALL(resume_ctrl_mock_, resumption_data_processor())
        .WillByDefault(ReturnRef(resumption_data_processor_mock_));
    EXPECT_CALL(app_manager_mock_, applications())
        .WillRepeatedly(Return(applications_));

    resumption_handler_.reset(
        new vehicle_info_plugin::VehicleInfoPendingResumptionHandler(
            app_manager_mock_, custom_vehicle_data_manager_mock_));
    MessageHelperResponseCreateExpectation();
  }

  ~VehicleInfoPendingResumptionHandlerTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    Mock::VerifyAndClearExpectations(&app_manager_mock_);
  }

  void MessageHelperResponseCreateExpectation() {
    const int default_corr_id = 0;
    static auto response = CreateHMIResponseMessage(
        hmi_apis::Common_Result::SUCCESS, default_corr_id);

    ON_CALL(mock_message_helper_,
            CreateResponseMessageFromHmi(VehicleInfo_SubscribeVehicleData,
                                         _,
                                         hmi_apis::Common_Result::SUCCESS))
        .WillByDefault(
            DoAll(ReplaceCorrelationId<1>(&response), Return(response)));
  }

  MockAppPtr CreateApp(uint32_t app_id) {
    auto mock_app = std::make_shared<MockApplication>();

    ON_CALL(app_manager_mock_, application(app_id))
        .WillByDefault(Return(mock_app));
    ON_CALL(*mock_app, app_id()).WillByDefault(Return(app_id));
    return mock_app;
  }

  VehicleInfoAppExtensionPtr CreateExtension(MockApplication& app) {
    auto ext_ptr = std::make_shared<VehicleInfoAppExtension>(plugin_, app);
    ON_CALL(app,
            QueryInterface(VehicleInfoAppExtension::VehicleInfoAppExtensionUID))
        .WillByDefault(Return(ext_ptr));
    return ext_ptr;
  }

  MockMessageHelper& mock_message_helper_;
  MockApplicationManager app_manager_mock_;
  NiceMock<MockResumeCtrl> resume_ctrl_mock_;
  MockResumptionDataProcessor resumption_data_processor_mock_;
  MockEventDispatcher event_dispatcher_mock_;
  MockRPCService mock_rpc_service_;
  NiceMock<MockCustomVehicleDataManager> custom_vehicle_data_manager_mock_;
  vehicle_info_plugin::VehicleInfoPlugin plugin_;
  application_manager::ApplicationSet application_set_;
  std::shared_ptr<sync_primitives::Lock> applications_lock_;
  DataAccessor<application_manager::ApplicationSet> applications_;

  std::unique_ptr<vehicle_info_plugin::VehicleInfoPendingResumptionHandler>
      resumption_handler_;
};

TEST_F(VehicleInfoPendingResumptionHandlerTest, NoSubscriptionNoAction) {
  auto mock_app = CreateApp(1);
  auto ext_ptr = CreateExtension(*mock_app);

  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .Times(0);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext_ptr, *mock_app);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataSuccess) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  auto request = CreateVDRequest(1);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  resumption::ResumptionRequest resumption_request;
  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .WillOnce(SaveArg<1>(&resumption_request));

  smart_objects::SmartObjectSPtr message_to_hmi;
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);
  EXPECT_EQ(resumption_request.request_id.function_id,
            VehicleInfo_SubscribeVehicleData);
  EXPECT_TRUE(
      CheckThatMessageContainsVD(resumption_request.message, {"gps", "speed"}));
  EXPECT_TRUE(CheckThatMessageContainsVD(*message_to_hmi, {"gps", "speed"}));
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseSuccess) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  resumption::ResumptionRequest resumption_request;
  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .WillOnce(SaveArg<1>(&resumption_request));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(1);

  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      subscriptions_result = {
          {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
          {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
      };

  auto response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);

  std::set<std::string> expected_data_in_event = {"gps", "speed"};
  const auto subscribed_correlation_id =
      resumption_request.request_id.correlation_id;
  EXPECT_CALL(event_dispatcher_mock_,
              raise_event(EventCheck(subscribed_correlation_id,
                                     expected_data_in_event)));

  resumption_handler_->HandleOnEvent(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseOneFailed) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  resumption::ResumptionRequest resumption_request;
  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .WillOnce(SaveArg<1>(&resumption_request));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      subscriptions_result = {
          {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
          {"speed",
           hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED},
      };
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);

  auto response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  std::set<std::string> expected_data_in_event = {"gps", "speed"};
  const auto subscribed_correlation_id =
      resumption_request.request_id.correlation_id;

  EXPECT_CALL(event_dispatcher_mock_,
              raise_event(EventCheck(subscribed_correlation_id,
                                     expected_data_in_event)));

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->HandleOnEvent(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseAllFailed) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  resumption::ResumptionRequest resumption_request;
  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .WillOnce(SaveArg<1>(&resumption_request));
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      subscriptions_result = {
          {"gps",
           hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED},
          {"speed",
           hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED},
      };

  auto response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->HandleOnEvent(event);
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseOveralResultFailed) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  resumption::ResumptionRequest resumption_request;
  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .WillOnce(SaveArg<1>(&resumption_request));

  EXPECT_CALL(event_dispatcher_mock_, raise_event(_));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(1);

  auto response =
      CreateVDError(corr_id, hmi_apis::Common_Result::ABORTED, "error message");
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->HandleOnEvent(event);
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest, TwoAppsOneSharedDataSuccess) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto mock_app2 = CreateApp(2);
  application_set_.insert(mock_app2);

  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);
  ext->AddPendingSubscription("gps");
  ext2->AddPendingSubscription("gps");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .Times(2);

  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(2);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      subscriptions_result = {
          {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
      };

  auto response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext2, *mock_app2);
  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->HandleOnEvent(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
  EXPECT_EQ(ext2->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       TwoAppsMultipleSharedDataSuccessResumption) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto mock_app2 = CreateApp(2);
  application_set_.insert(mock_app2);

  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");
  ext2->AddPendingSubscription("gps");
  ext2->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .Times(2);

  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(2);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      subscriptions_result = {
          {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
          {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
      };

  auto response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext2, *mock_app2);
  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->HandleOnEvent(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
  EXPECT_EQ(ext2->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       TwoAppsOneSharedDataFailRetryforSecondApp) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto mock_app2 = CreateApp(2);
  application_set_.insert(mock_app2);

  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);
  ext->AddPendingSubscription("gps");
  ext2->AddPendingSubscription("gps");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData, _))
      .WillByDefault(Return(request));

  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .Times(2);

  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(2);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(2);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      subscriptions_result = {
          {"gps",
           hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE},
      };

  auto response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext, *mock_app);
  resumption_handler_->HandleResumptionSubscriptionRequest(*ext2, *mock_app2);
  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->HandleOnEvent(event);

  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>
      second_subscriptions_result = {
          {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
      };
  auto second_response = CreateVDResponse(
      hmi_apis::Common_Result::SUCCESS, second_subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event second_event(
      VehicleInfo_SubscribeVehicleData);
  second_event.set_smart_object(second_response);

  resumption_handler_->HandleOnEvent(second_event);
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_EQ(ext->PendingSubscriptions().GetData().size(), 0u);
  EXPECT_EQ(ext2->PendingSubscriptions().GetData().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       TwoAppsOneSharedDataAlreadySubscribedByFirstAppNoRetryforSecondApp) {
  auto mock_app = CreateApp(1);
  application_set_.insert(mock_app);

  auto mock_app2 = CreateApp(2);
  application_set_.insert(mock_app2);

  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);

  ext->subscribeToVehicleInfo("gps");
  ext2->AddPendingSubscription("gps");

  EXPECT_CALL(resumption_data_processor_mock_, SubscribeToResponse(_, _))
      .Times(0);
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);

  resumption_handler_->HandleResumptionSubscriptionRequest(*ext2, *mock_app2);

  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_EQ(ext2->PendingSubscriptions().GetData().size(), 0u);
}
}  // namespace vehicle_info_plugin_test
