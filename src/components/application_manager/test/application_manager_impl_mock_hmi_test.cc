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
#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "application_manager/application.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "utils/custom_string.h"
#include "utils/make_shared.h"
#include "encryption/hashing.h"

#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_resumption_data.h"
#include "application_manager/mock_hmi_command_factory.h"
#include "application_manager/mock_request.h"
#include "connection_handler/mock_connection_handler.h"
#include "policy/mock_policy_settings.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol_handler/mock_session_observer.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace am = application_manager;

using testing::_;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::SetArgPointee;

using namespace application_manager;

namespace {
const uint32_t kApplicationId = 0u;
const std::string kDirectoryName = "./test_storage";
const uint32_t kTimeout = 10000u;
}  // namespace

/**
 * @brief The ApplicationManagerImplMockHmiTest class uses mocked HMI factory
 * which calls its static method and real (non-mocked) MessageHelper.
 * Class is separated from ApplicationManagerImplTest since it requires real
 * HMI factory and mocked MessageHelper. So it is not possible to combine
 * this code together.
 */
class ApplicationManagerImplMockHmiTest : public ::testing::Test {
 public:
  ApplicationManagerImplMockHmiTest()
      : mock_storage_(
            ::std::make_shared<NiceMock<resumption_test::MockResumptionData> >(
                mock_app_mngr_)) {}

 protected:
  void SetUp() OVERRIDE {
    const uint8_t expected_tread_pool_size = 2u;
    const uint8_t stop_streaming_timeout = 1u;

    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(expected_tread_pool_size));
    ON_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(true));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(stop_streaming_timeout));
    ON_CALL(mock_application_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(kTimeout));

    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer_));

    app_manager_impl_.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings_, mock_policy_settings_));

    app_manager_impl_->set_connection_handler(&mock_connection_handler_);
    app_manager_impl_->resume_controller().set_resumption_storage(
        mock_storage_);
  }

  void SetCommonExpectationOnAppReconnection(
      const connection_handler::DeviceHandle new_device_id,
      const uint32_t new_application_id,
      const std::string& mac_address) {
    EXPECT_CALL(
        mock_session_observer_,
        GetDataOnSessionKey(new_application_id,
                            _,
                            _,
                            testing::An<connection_handler::DeviceHandle*>()))
        .WillOnce(DoAll(SetArgPointee<3u>(new_device_id), Return(0)));

    const std::string connection_type = "MyConnectionType";
    EXPECT_CALL(
        mock_session_observer_,
        GetDataOnDeviceID(
            ::testing::Matcher<connection_handler::DeviceHandle>(new_device_id),
            _,
            _,
            _,
            _))
        .WillOnce(DoAll(SetArgPointee<3u>(mac_address),
                        SetArgPointee<4u>(connection_type),
                        Return(0)));
  }

  std::shared_ptr<NiceMock<resumption_test::MockResumptionData> >
      mock_storage_;
  application_manager_test::MockApplicationManager mock_app_mngr_;
  NiceMock<policy_handler_test::MockPolicySettings> mock_policy_settings_;
  NiceMock<connection_handler_test::MockConnectionHandler>
      mock_connection_handler_;
  NiceMock<protocol_handler_test::MockSessionObserver> mock_session_observer_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  std::unique_ptr<am::ApplicationManagerImpl> app_manager_impl_;
};

TEST_F(ApplicationManagerImplMockHmiTest,
       PostponeCommandsDuringSwitching_ExpectSendingOnSwitchingEnd) {
  const uint32_t application_id = 1u;
  const std::string policy_app_id = "p_app_id";
  const std::string mac_address = "MA:CA:DD:RE:SS";
  const connection_handler::DeviceHandle device_id = 1u;
  const custom_str::CustomString app_name("");

  std::shared_ptr<ApplicationImpl> app_impl = std::make_shared<ApplicationImpl>(
      application_id,
      policy_app_id,
      encryption::MakeHash(mac_address),
      device_id,
      app_name,
      std::shared_ptr<usage_statistics::StatisticsManager>(
          new usage_statistics_test::MockStatisticsManager()),
      *app_manager_impl_);

  app_manager_impl_->AddMockApplication(app_impl);

  const connection_handler::Device bt(
      device_id, "BT_device", mac_address, "BLUETOOTH");

  const connection_handler::Device usb(
      device_id + 1, "USB_device", "USB_serial", "USB_IOS");

  MockHMICommandFactory* mock_hmi_factory =
      MockHMICommandFactory::mock_hmi_command_factory();

  // Skip sending notification on device switching as it is not the goal here
  EXPECT_CALL(*mock_hmi_factory, CreateCommand(_, _))
      .WillOnce(Return(std::shared_ptr<commands::Command>()));

  app_manager_impl_->OnDeviceSwitchingStart(bt, usb);

  const uint32_t connection_key = 1u;
  const uint32_t correlation_id_1 = 1u;
  const uint32_t correlation_id_2 = 2u;
  const uint32_t correlation_id_3 = 3u;
  std::shared_ptr<NiceMock<MockRequest> > cmd_1 =
      std::make_shared<NiceMock<MockRequest> >(connection_key,
                                                correlation_id_1);
  std::shared_ptr<NiceMock<MockRequest> > cmd_2 =
      std::make_shared<NiceMock<MockRequest> >(connection_key,
                                                correlation_id_2);
  std::shared_ptr<NiceMock<MockRequest> > cmd_3 =
      std::make_shared<NiceMock<MockRequest> >(connection_key,
                                                correlation_id_3);

  EXPECT_CALL(*mock_hmi_factory, CreateCommand(_, _))
      .WillOnce(Return(cmd_1))
      .WillOnce(Return(cmd_2))
      .WillOnce(Return(cmd_3));

  commands::MessageSharedPtr hmi_msg_1 =
      std::make_shared<smart_objects::SmartObject>();
  commands::MessageSharedPtr hmi_msg_2 =
      std::make_shared<smart_objects::SmartObject>();
  commands::MessageSharedPtr hmi_msg_3 =
      std::make_shared<smart_objects::SmartObject>();

  (*hmi_msg_1)[strings::msg_params][strings::app_id] =
      (*hmi_msg_2)[strings::msg_params][strings::app_id] =
          (*hmi_msg_3)[strings::msg_params][strings::app_id] = application_id;

  EXPECT_CALL(*cmd_1, Init()).Times(0);
  EXPECT_CALL(*cmd_2, Init()).Times(0);
  EXPECT_CALL(*cmd_3, Init()).Times(0);

  // Act
  app_manager_impl_->ManageHMICommand(hmi_msg_1);
  app_manager_impl_->ManageHMICommand(hmi_msg_2);
  app_manager_impl_->ManageHMICommand(hmi_msg_3);

  EXPECT_CALL(*mock_hmi_factory, CreateCommand(_, _))
      .WillOnce(Return(cmd_1))
      .WillOnce(Return(cmd_2))
      .WillOnce(Return(cmd_3));

  EXPECT_CALL(*cmd_1, Init()).WillOnce(Return(true));
  EXPECT_CALL(*cmd_1, Run());
  EXPECT_CALL(*cmd_2, Init()).WillOnce(Return(true));
  EXPECT_CALL(*cmd_2, Run());
  EXPECT_CALL(*cmd_3, Init()).WillOnce(Return(true));
  EXPECT_CALL(*cmd_3, Run());

  const connection_handler::DeviceHandle new_device_id = 2;
  const uint32_t new_application_id = 2;
  SetCommonExpectationOnAppReconnection(
      new_device_id, new_application_id, mac_address);

  app_manager_impl_->ProcessReconnection(app_impl, new_application_id);
  app_manager_impl_->OnApplicationSwitched(app_impl);

  Mock::VerifyAndClearExpectations(&mock_hmi_factory);
}

}  // application_manager_test
}  // namespace components
}  // namespace test
