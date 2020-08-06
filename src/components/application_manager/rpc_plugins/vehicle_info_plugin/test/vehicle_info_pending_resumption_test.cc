/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "gtest/gtest.h"
#include "test/application_manager/mock_application_manager.h"
#include "application_manager/test/include/application_manager/mock_event_dispatcher.h"
#include "application_manager/test/include/application_manager/mock_event_observer.h"
#include "application_manager/test/include/application_manager/mock_application.h"
#include "application_manager/test/include/application_manager/mock_message_helper.h"
//#include "application_manager/mock_rpc_service.h"
#include "vehicle_info_plugin/vehicle_info_pending_resumption_handler.h"

namespace vehicle_info_plugin_test {
using namespace vehicle_info_plugin;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;

using application_manager::MockMessageHelper;
typedef NiceMock<::test::components::event_engine_test::MockEventDispatcher> MockEventDispatcher;
typedef NiceMock<::test::components::event_engine_test::MockEventObserver> MockEventObserver;
typedef NiceMock<::test::components::application_manager_test::MockApplicationManager> MockApplicationManager;
typedef NiceMock<::test::components::application_manager_test::MockApplication> MockApplication;
typedef NiceMock<test::components::application_manager_test::MockRPCService> MockRPCService;
typedef std::shared_ptr<MockApplication> MockAppPtr;
typedef std::shared_ptr<vehicle_info_plugin::VehicleInfoAppExtension>
    VehicleInfoAppExtensionPtr;
using hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData;

  class SubscribeCatcher {
   public:
     MOCK_METHOD2(subscribe, void(const int32_t,
                  const resumption::ResumptionRequest));

   };

class VehicleInfoPendingResumptionHandlerTest : public ::testing::Test {
// using namespace vehicle_info_plugin;
 public:
  VehicleInfoPendingResumptionHandlerTest()
      : mock_message_helper_(
            *application_manager::MockMessageHelper::message_helper_mock())

  {}

  void SetUp() OVERRIDE {
    ON_CALL(app_manager_mock_, event_dispatcher()).WillByDefault(ReturnRef(event_dispatcher_mock_));
    ON_CALL(app_manager_mock_, GetRPCService()).WillByDefault(ReturnRef(mock_rpc_service_));
    resumption_handler_.reset(
          new vehicle_info_plugin::VehicleInfoPendingResumptionHandler(app_manager_mock_));
  }


  MockAppPtr CreateApp(uint32_t app_id) {
    auto mock_app = std::make_shared<MockApplication>();

    ON_CALL(app_manager_mock_, application(app_id)).WillByDefault(Return(mock_app));
    ON_CALL(*mock_app, app_id()).WillByDefault(Return(app_id));
    return mock_app;
  }


  VehicleInfoAppExtensionPtr CreateExtension(MockApplication& app) {
    auto ext_ptr = std::make_shared<VehicleInfoAppExtension>(
          plugin_,
          app
          );
    ON_CALL(app, QueryInterface(VehicleInfoAppExtension::VehicleInfoAppExtensionUID)).
        WillByDefault(Return(ext_ptr));
    return ext_ptr;
  }

  resumption::Subscriber& get_subscriber() {
    static resumption::Subscriber subscriber =
        [this](const int32_t corr_id,
        const resumption::ResumptionRequest resumption_request) {
        this->subscribe_catcher_.subscribe(corr_id,resumption_request);
    };
    return subscriber;
  }


  SubscribeCatcher subscribe_catcher_;
  MockMessageHelper& mock_message_helper_;
  MockApplicationManager app_manager_mock_;
  MockEventDispatcher event_dispatcher_mock_;
  MockRPCService mock_rpc_service_;
  vehicle_info_plugin::VehicleInfoPlugin plugin_;

  std::unique_ptr<
  vehicle_info_plugin::VehicleInfoPendingResumptionHandler> resumption_handler_;
};


smart_objects::SmartObjectSPtr CreateVDRequest(const uint32_t corr_id) {
  using namespace application_manager;
  smart_objects::SmartObjectSPtr request=
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *request;
  object[strings::params][strings::message_type] = static_cast<int>(0); // request
  object[strings::params][strings::function_id] = static_cast<int>(VehicleInfo_SubscribeVehicleData);

  object[strings::params][strings::correlation_id] =
      corr_id;
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

typedef std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType> VDResponseMap;
smart_objects::SmartObject CreateVDResponse(const hmi_apis::Common_Result::eType common_result,
    const VDResponseMap&  subscriptions_result,
    uint32_t correleation_id
                                            ) {
  namespace strings = application_manager::strings;
  namespace hmi_response = application_manager::hmi_response;
  smart_objects::SmartObject message(smart_objects::SmartType_Map);
  smart_objects::SmartObject params(smart_objects::SmartType_Map);
  params[strings::function_id] = VehicleInfo_SubscribeVehicleData;
  params[strings::message_type] = 1;  // MessageType::kResponse;
  params[strings::correlation_id] = correleation_id;
  params[strings::protocol_type] = 1;  // HMI protocol type
  params[hmi_response::code] = common_result;
  message[strings::params] = params;
  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  for (const auto& subscription : subscriptions_result) {
    smart_objects::SmartObject subscription_result(smart_objects::SmartType_Map);
    subscription_result[strings::data_type] = ToVDType(subscription.first);
    subscription_result[strings::result_code] = subscription.second;
    msg_params[subscription.first] = subscription_result;
  }
  message[strings::msg_params] = msg_params;
  return message;
}


TEST_F(VehicleInfoPendingResumptionHandlerTest,
       NoSubscriptionNoAction) {


  auto mock_app = CreateApp(1);
  auto ext_ptr = CreateExtension(*mock_app);


  EXPECT_CALL(subscribe_catcher_, subscribe(_,_)).Times(0);
  resumption::Subscriber& subscribe = get_subscriber();
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext_ptr,
        subscribe,
        *mock_app
        );
}


TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataSuccess) {
  auto mock_app = CreateApp(1);
  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  auto request = CreateVDRequest(1);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));


  EXPECT_CALL(subscribe_catcher_, subscribe(_,_));
  // TODO subscription data
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_));
  // TODO check hmi request
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );

}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseSuccess) {
  auto mock_app = CreateApp(1);
  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_));
  // TODO save cid and fid of the subscription
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(1);
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_));

  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
    {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseOneFailed) {
  auto mock_app = CreateApp(1);
  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_));
  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
    {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
}


TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseAllFailed) {
  auto mock_app = CreateApp(1);
  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_));
  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED},
    {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       OneAppSeveralVehicleDataResponseOveralResultFailed) {
  auto mock_app = CreateApp(1);
  auto ext = CreateExtension(*mock_app);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_));
  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
    {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::ABORTED, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );

  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       TwoAppsOneSharedDataSuccess) {
  auto mock_app = CreateApp(1);
  auto mock_app2 = CreateApp(2);
  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);
  ext->AddPendingSubscription("gps");
  ext2->AddPendingSubscription("gps");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_)).Times(2);
  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(2);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext2,
        get_subscriber(),
        *mock_app2
        );
  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
  EXPECT_EQ(ext2->PendingSubscriptions().size(), 0u);
}


TEST_F(VehicleInfoPendingResumptionHandlerTest,
       TwoAppsMultipleSharedDataSuccessResumption) {
  auto mock_app = CreateApp(1);
  auto mock_app2 = CreateApp(2);
  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);
  ext->AddPendingSubscription("gps");
  ext->AddPendingSubscription("speed");
  ext2->AddPendingSubscription("gps");
  ext2->AddPendingSubscription("speed");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_)).Times(2);
  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(2);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(1);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
    {"speed", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext2,
        get_subscriber(),
        *mock_app2
        );
  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext->isSubscribedToVehicleInfo("speed"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("speed"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
  EXPECT_EQ(ext2->PendingSubscriptions().size(), 0u);
}

TEST_F(VehicleInfoPendingResumptionHandlerTest,
       TwoAppsOneSharedDataFailRetryforSecondApp) {
  auto mock_app = CreateApp(1);
  auto mock_app2 = CreateApp(2);
  auto ext = CreateExtension(*mock_app);
  auto ext2 = CreateExtension(*mock_app2);
  ext->AddPendingSubscription("gps");
  ext2->AddPendingSubscription("gps");

  uint32_t corr_id = 345;
  auto request = CreateVDRequest(corr_id);
  ON_CALL(mock_message_helper_,
          CreateModuleInfoSO(VehicleInfo_SubscribeVehicleData,
                             _)
          ).WillByDefault(Return(request));

  EXPECT_CALL(subscribe_catcher_, subscribe(_,_)).Times(2);
  // TODO save cid and fid of the subscription
  EXPECT_CALL(event_dispatcher_mock_, raise_event(_)).Times(2);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_,_)).Times(2);
  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE},
  };

  auto response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event event(
      VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(response);
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext,
        get_subscriber(),
        *mock_app
        );
  resumption_handler_->HandleResumptionSubscriptionRequest(
        *ext2,
        get_subscriber(),
        *mock_app2
        );
  // TODO check that raized the same fid and cid as subscribed
  resumption_handler_->on_event(event);

  const std::map<std::string, hmi_apis::Common_VehicleDataResultCode::eType>  second_subscriptions_result=
  {
    {"gps", hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS},
  };
  auto second_response = CreateVDResponse(hmi_apis::Common_Result::SUCCESS, second_subscriptions_result, corr_id);
  // TODO use error message type instead of result code
  application_manager::event_engine::Event second_event(
      VehicleInfo_SubscribeVehicleData);
  second_event.set_smart_object(second_response);

  resumption_handler_->on_event(second_event);
  EXPECT_FALSE(ext->isSubscribedToVehicleInfo("gps"));
  EXPECT_TRUE(ext2->isSubscribedToVehicleInfo("gps"));
  EXPECT_EQ(ext->PendingSubscriptions().size(), 0u);
  EXPECT_EQ(ext2->PendingSubscriptions().size(), 0u);
}



}  // namespace vehicle_info_plugin_test
