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

#include "gtest/gtest.h"
#include <sstream>
#include "utils/macro.h"
#include "application_manager/app_launch/app_launch_ctrl_impl.h"
#include "application_manager/mock_app_launch_data.h"
#include "application_manager/mock_app_launch_settings.h"
#include "application_manager/mock_resume_ctrl.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "connection_handler/mock_connection_handler.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace app_launch_test {
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Truly;
using ::testing::NiceMock;
using ::testing::Invoke;
using ::testing::AtLeast;
using ::testing::InSequence;

namespace ch_test = test::components::connection_handler_test;
namespace am_test = test::components::application_manager_test;

const uint32_t MAX_TEST_DURATION = 1000;  // 1 second
typedef utils::SharedPtr<am_test::MockApplication> MockAppPtr;
class AppLaunchCtrlTest : public ::testing::Test {
 public:
  MockAppPtr AppFromAppData(const app_launch::ApplicationData& app_data) {
    utils::SharedPtr<NiceMock<am_test::MockApplication> > app =
        utils::MakeShared<NiceMock<am_test::MockApplication> >();

    ON_CALL(*app, mac_address()).WillByDefault(ReturnRef(app_data.device_mac_));
    ON_CALL(*app, bundle_id()).WillByDefault(ReturnRef(app_data.bundle_id_));
    ON_CALL(*app, policy_app_id())
        .WillByDefault(Return(app_data.mobile_app_id_));
    return app;
  }

  app_launch::ApplicationDataPtr AppDataFromApp(
      const am_test::MockApplication& app) {
    app_launch::ApplicationDataPtr app_data =
        utils::MakeShared<NiceMock<app_launch::ApplicationData> >(
            app.policy_app_id(), app.bundle_id(), app.mac_address());
    return app_data;
  }

  void SetUp() OVERRIDE {
    ON_CALL(settings_, app_launch_wait_time()).WillByDefault(Return(5));
    ON_CALL(settings_, app_launch_max_retry_attempt()).WillByDefault(Return(3));
    ON_CALL(settings_, app_launch_retry_wait_time()).WillByDefault(Return(15));
    ON_CALL(settings_, remove_bundle_id_attempts()).WillByDefault(Return(3));
    ON_CALL(settings_, max_number_of_ios_device()).WillByDefault(Return(10));
    ON_CALL(settings_, wait_time_between_apps()).WillByDefault(Return(4));
    ON_CALL(settings_, enable_app_launch_ios()).WillByDefault(Return(true));
    ON_CALL(settings_, resumption_delay_after_ign()).WillByDefault(Return(30));
    ON_CALL(resume_ctrl_mock_, LaunchTime()).WillByDefault(Return(0));

    ON_CALL(app_mngr_mock_, resume_controller())
        .WillByDefault(ReturnRef(resume_ctrl_mock_));
    ON_CALL(app_mngr_mock_, connection_handler())
        .WillByDefault(ReturnRef(connection_handler_mock_));
    app_launch_ctrl_.reset(new app_launch::AppLaunchCtrlImpl(
        app_launch_data_mock_, app_mngr_mock_, settings_));
    FillApplications();
  }

  typedef std::pair<MockAppPtr, app_launch::ApplicationDataPtr> AppAndAppData;
  std::vector<AppAndAppData> test_app_datas_;

  app_launch::ApplicationDataPtr GetTestAppData(size_t index) {
    return test_app_datas_[index].second;
  }

  MockAppPtr GetTestApp(size_t index) {
    return test_app_datas_[index].first;
  }

  const std::string DeviceMac(size_t device_number) {
    std::ostringstream ss;
    ss << "device_mac_" << device_number;
    return ss.str();
  }

  std::vector<AppAndAppData> DeviceApps(std::string device_mac) {
    std::vector<AppAndAppData> res;
    std::vector<AppAndAppData>::iterator it = test_app_datas_.begin();
    for (; it != test_app_datas_.end(); ++it) {
      const AppAndAppData& app_and_data = *it;
      if (app_and_data.second->device_mac_ == device_mac) {
        res.push_back(app_and_data);
      }
    }
    return res;
  }

  AppAndAppData GetAppAndAppData(const std::string& app_id,
                                 const std::string& bundle_id,
                                 const std::string& device_mac) {
    using app_launch::ApplicationData;
    AppAndAppData app;
    app.second =
        utils::MakeShared<ApplicationData>(app_id, bundle_id, device_mac);
    app.first = AppFromAppData(*app.second);
    return app;
  }

  void FillApplications() {
    test_app_datas_.push_back(
        GetAppAndAppData("mob_id1", "bundle_id1", DeviceMac(1)));
    test_app_datas_.push_back(
        GetAppAndAppData("mob_id2", "bundle_id2", DeviceMac(1)));
    test_app_datas_.push_back(
        GetAppAndAppData("mob_id3", "bundle_id3", DeviceMac(1)));
    test_app_datas_.push_back(
        GetAppAndAppData("mob_id4", "bundle_id4", DeviceMac(2)));
    test_app_datas_.push_back(
        GetAppAndAppData("mob_id5", "bundle_id5", DeviceMac(2)));
    test_app_datas_.push_back(
        GetAppAndAppData("mob_id6", "bundle_id6", DeviceMac(3)));
  }

  void ExpectRegisteration(const AppAndAppData& app_data);

  NiceMock<am_test::MockApplicationManager> app_mngr_mock_;
  NiceMock<app_launch_test::AppLaunchDataMock> app_launch_data_mock_;
  NiceMock<ch_test::MockConnectionHandler> connection_handler_mock_;
  NiceMock<resumprion_test::MockResumeCtrl> resume_ctrl_mock_;
  NiceMock<app_launch_test::MockAppLaunchSettings> settings_;
  std::auto_ptr<app_launch::AppLaunchCtrlImpl> app_launch_ctrl_;
};

app_launch::ApplicationData AppDataFromApp(
    application_manager::Application& app) {
  // TODO(AK) Use amc op device
  return app_launch::ApplicationData(
      app.policy_app_id(), app.bundle_id(), "Dummy Deevice Id");
}

struct AppDataComparator {
  const app_launch::ApplicationData& app_data_;

  AppDataComparator(const app_launch::ApplicationData& app_data)
      : app_data_(app_data) {}

  bool operator()(const app_launch::ApplicationData& app_data) const {
    return app_data_ == app_data;
  }
};

TEST_F(AppLaunchCtrlTest, AddRegisteredApplicationToDataStorage) {
  MockAppPtr app = GetTestApp(0);
  EXPECT_CALL(app_launch_data_mock_,
              AddApplicationData(Truly(AppDataComparator(*GetTestAppData(0)))));
  app_launch_ctrl_->OnAppRegistered(*app);
}

ACTION_P2(InvokeOnAppRegistered, app_launch_ctrl, app) {
  (app_launch_ctrl->*&app_launch::AppLaunchCtrlImpl::OnAppRegistered)(*app);
}

TEST_F(AppLaunchCtrlTest, StoredAppIsLaunchedAfterDeviceConnected) {
  std::vector<app_launch::ApplicationDataPtr> applications_on_device;
  app_launch::ApplicationDataPtr app_to_launch = GetTestAppData(0);
  MockAppPtr app = GetTestApp(0);

  applications_on_device.push_back(app_to_launch);
  EXPECT_CALL(app_launch_data_mock_,
              GetApplicationDataByDevice(app_to_launch->device_mac_))
      .WillOnce(Return(applications_on_device));
  EXPECT_CALL(
      connection_handler_mock_,
      RunAppOnDevice(app_to_launch->device_mac_, app_to_launch->bundle_id_))
      .Times(AtLeast(1))
      .WillOnce(InvokeOnAppRegistered(app_launch_ctrl_.get(), app.get()));
  app_launch_ctrl_->OnDeviceConnected(app_to_launch->device_mac_);
  const uint32_t wait_time =
      MAX_TEST_DURATION + settings_.app_launch_wait_time();
  testing::Mock::AsyncVerifyAndClearExpectations(wait_time);
}

TEST_F(AppLaunchCtrlTest, RelaunchAppIfNotRegisteredMultipleTimes) {
  std::vector<app_launch::ApplicationDataPtr> applications_on_device;
  app_launch::ApplicationDataPtr app_to_launch = GetTestAppData(0);
  applications_on_device.push_back(app_to_launch);
  EXPECT_CALL(app_launch_data_mock_,
              GetApplicationDataByDevice(app_to_launch->device_mac_))
      .WillOnce(Return(applications_on_device));

  EXPECT_CALL(
      connection_handler_mock_,
      RunAppOnDevice(app_to_launch->device_mac_, app_to_launch->bundle_id_))
      .Times(settings_.app_launch_max_retry_attempt());

  app_launch_ctrl_->OnDeviceConnected(app_to_launch->device_mac_);
  const uint32_t wait_time = MAX_TEST_DURATION +
                             settings_.app_launch_wait_time() +
                             settings_.app_launch_max_retry_attempt() *
                                 settings_.app_launch_retry_wait_time();
  testing::Mock::AsyncVerifyAndClearExpectations(wait_time);
}

TEST_F(AppLaunchCtrlTest, LaunchMultipleApps) {
  std::vector<AppAndAppData> apps_and_data = DeviceApps(DeviceMac(1));
  std::vector<app_launch::ApplicationDataPtr> apps;
  for (std::vector<AppAndAppData>::iterator it = apps_and_data.begin();
       it != apps_and_data.end();
       ++it) {
    apps.push_back(it->second);
  }

  EXPECT_CALL(app_launch_data_mock_, GetApplicationDataByDevice(DeviceMac(1)))
      .WillOnce(Return(apps));

  // Expect multiple call
  for (std::vector<AppAndAppData>::iterator it = apps_and_data.begin();
       it != apps_and_data.end();
       ++it) {
    ExpectRegisteration(*it);
  }
  app_launch_ctrl_->OnDeviceConnected(DeviceMac(1));
  const uint32_t wait_time = MAX_TEST_DURATION +
                             settings_.app_launch_wait_time() +
                             apps.size() * settings_.wait_time_between_apps();
  testing::Mock::AsyncVerifyAndClearExpectations(wait_time);
}

TEST_F(AppLaunchCtrlTest, LaunchMultipleAppsNoRegister) {
  std::vector<AppAndAppData> apps_and_data = DeviceApps(DeviceMac(1));
  std::vector<app_launch::ApplicationDataPtr> apps;
  for (std::vector<AppAndAppData>::iterator it = apps_and_data.begin();
       it != apps_and_data.end();
       ++it) {
    apps.push_back(it->second);
  }

  EXPECT_CALL(app_launch_data_mock_, GetApplicationDataByDevice(DeviceMac(1)))
      .WillOnce(Return(apps));

  // Expect multiple call
  for (std::vector<AppAndAppData>::iterator it = apps_and_data.begin();
       it != apps_and_data.end();
       ++it) {
    const AppAndAppData& app_data = *it;
    EXPECT_CALL(connection_handler_mock_,
                RunAppOnDevice(app_data.second->device_mac_,
                               app_data.second->bundle_id_))
        .Times(settings_.app_launch_max_retry_attempt());
  }
  app_launch_ctrl_->OnDeviceConnected(DeviceMac(1));
  const uint32_t wait_time = MAX_TEST_DURATION +
                             settings_.app_launch_wait_time() +
                             apps.size() * settings_.wait_time_between_apps();
  testing::Mock::AsyncVerifyAndClearExpectations(wait_time);
}

TEST_F(AppLaunchCtrlTest, LaunchMultipleAppsInHMILevelOrder) {
  std::vector<AppAndAppData> apps_and_data = DeviceApps(DeviceMac(1));
  DCHECK(apps_and_data.size() == 3);
  std::vector<app_launch::ApplicationDataPtr> apps;

  {
    app_launch::ApplicationDataPtr app_data = apps_and_data[0].second;
    EXPECT_CALL(
        resume_ctrl_mock_,
        GetSavedAppHmiLevel(app_data->mobile_app_id_, app_data->device_mac_))
        .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
    apps.push_back(app_data);
  }

  {
    app_launch::ApplicationDataPtr app_data = apps_and_data[1].second;
    EXPECT_CALL(
        resume_ctrl_mock_,
        GetSavedAppHmiLevel(app_data->mobile_app_id_, app_data->device_mac_))
        .WillOnce(Return(mobile_apis::HMILevel::HMI_LIMITED));
    ;
    apps.push_back(app_data);
  }

  {
    app_launch::ApplicationDataPtr app_data = apps_and_data[2].second;
    EXPECT_CALL(
        resume_ctrl_mock_,
        GetSavedAppHmiLevel(app_data->mobile_app_id_, app_data->device_mac_))
        .WillOnce(Return(mobile_apis::HMILevel::HMI_BACKGROUND));
    ;
    apps.push_back(app_data);
  }

  EXPECT_CALL(app_launch_data_mock_, GetApplicationDataByDevice(DeviceMac(1)))
      .WillOnce(Return(apps));
  {
    InSequence s;

    ExpectRegisteration(apps_and_data[0]);
    ExpectRegisteration(apps_and_data[1]);
    ExpectRegisteration(apps_and_data[2]);
  }

  app_launch_ctrl_->OnDeviceConnected(DeviceMac(1));
  const uint32_t wait_time = MAX_TEST_DURATION +
                             settings_.app_launch_wait_time() +
                             apps.size() * settings_.wait_time_between_apps();
  testing::Mock::AsyncVerifyAndClearExpectations(wait_time);
}

void AppLaunchCtrlTest::ExpectRegisteration(
    const AppLaunchCtrlTest::AppAndAppData& app_data) {
  EXPECT_CALL(
      connection_handler_mock_,
      RunAppOnDevice(app_data.second->device_mac_, app_data.second->bundle_id_))
      .Times(AtLeast(1))
      .WillOnce(
          InvokeOnAppRegistered(app_launch_ctrl_.get(), app_data.first.get()));
}

}  // namespace app_launch_test
}  // namespace components
}  // namespace test
