/*
 * Copyright (c) 2019, Ford Motor Company
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

#include "app_service_rpc_plugin/app_service_app_extension.h"
#include <memory>
#include "app_service_rpc_plugin/app_service_rpc_plugin.h"
#include "application_manager/mock_application.h"
#include "gtest/gtest.h"

namespace {
const std::string kAppServiceType1 = "AppServiceType1";
const std::string kAppServiceType2 = "AppServiceType2";
const std::string kResumptionDataKey = "kResumptionDataKey";
const std::string kAppServiceInfoKey = "appService";
}  // namespace

namespace test {
namespace components {
namespace app_service_plugin_test {

using test::components::application_manager_test::MockApplication;
using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::ReturnRef;
using ::testing::SaveArg;

using namespace app_service_rpc_plugin;
namespace strings = application_manager::strings;
namespace plugins = application_manager::plugin_manager;

class AppServiceAppExtensionTest : public ::testing::Test {
 public:
  AppServiceAppExtensionTest() : mock_app_(new NiceMock<MockApplication>()) {}

 protected:
  void SetUp() OVERRIDE {
    app_service_app_extension_.reset(
        new AppServiceAppExtension(app_service_plugin_, *mock_app_));
  }

  void TearDown() OVERRIDE {
    app_service_app_extension_.reset();
  }

  app_service_rpc_plugin::AppServiceRpcPlugin app_service_plugin_;
  std::unique_ptr<MockApplication> mock_app_;
  std::unique_ptr<AppServiceAppExtension> app_service_app_extension_;
};

TEST_F(AppServiceAppExtensionTest, SubscribeToAppService_SUCCESS) {
  EXPECT_TRUE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType1));
  const auto& subs = app_service_app_extension_->Subscriptions();
  EXPECT_EQ(1u, subs.size());
  EXPECT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));
}

TEST_F(AppServiceAppExtensionTest,
       SubscribeToAppService_SubscribeOneAppServiceType_Twice_FAIL) {
  ASSERT_TRUE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType1));
  ASSERT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));

  EXPECT_FALSE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType1));
  EXPECT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));
  const auto& subs = app_service_app_extension_->Subscriptions();
  EXPECT_EQ(1u, subs.size());
}

TEST_F(
    AppServiceAppExtensionTest,
    UnsubscribeFromAppService_AppServiceType1Unsubscribed_AppServiceType2Remains_SUCCESS) {
  // Subscribe
  ASSERT_TRUE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType1));
  ASSERT_TRUE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType2));
  auto subs = app_service_app_extension_->Subscriptions();
  ASSERT_EQ(2u, subs.size());
  ASSERT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));
  ASSERT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType2));

  // Unsubscribe
  EXPECT_TRUE(
      app_service_app_extension_->UnsubscribeFromAppService(kAppServiceType1));
  EXPECT_FALSE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));
  EXPECT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType2));
  subs = app_service_app_extension_->Subscriptions();
  EXPECT_EQ(1u, subs.size());
}

TEST_F(AppServiceAppExtensionTest,
       UnsubscribeFromAppService_UnsubscribeNotSubscribedAppServiceType_FAIL) {
  ASSERT_TRUE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType1));
  ASSERT_EQ(1u, app_service_app_extension_->Subscriptions().size());
  ASSERT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));

  EXPECT_FALSE(
      app_service_app_extension_->UnsubscribeFromAppService(kAppServiceType2));
  EXPECT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));
  EXPECT_EQ(1u, app_service_app_extension_->Subscriptions().size());
}

TEST_F(AppServiceAppExtensionTest,
       UnsubscribeFromAppService_UnsubscribeAll_SUCCESS) {
  auto app_service_types = {kAppServiceType1, kAppServiceType2};

  for (const auto& app_service_type : app_service_types) {
    ASSERT_TRUE(
        app_service_app_extension_->SubscribeToAppService(app_service_type));
    ASSERT_TRUE(
        app_service_app_extension_->IsSubscribedToAppService(app_service_type));
  }
  ASSERT_EQ(2u, app_service_app_extension_->Subscriptions().size());

  app_service_app_extension_->UnsubscribeFromAppService();

  for (const auto& app_service_type : app_service_types) {
    EXPECT_FALSE(
        app_service_app_extension_->IsSubscribedToAppService(app_service_type));
  }
  EXPECT_EQ(0u, app_service_app_extension_->Subscriptions().size());
}

TEST_F(AppServiceAppExtensionTest, SaveResumptionData_SUCCESS) {
  ASSERT_TRUE(
      app_service_app_extension_->SubscribeToAppService(kAppServiceType1));
  ASSERT_EQ(1u, app_service_app_extension_->Subscriptions().size());
  ASSERT_TRUE(
      app_service_app_extension_->IsSubscribedToAppService(kAppServiceType1));

  smart_objects::SmartObject resumption_data;
  resumption_data[kResumptionDataKey] = "some resumption data";

  app_service_app_extension_->SaveResumptionData(resumption_data);

  EXPECT_TRUE(resumption_data.keyExists(kResumptionDataKey));
  EXPECT_TRUE(resumption_data.keyExists(kAppServiceInfoKey));
  EXPECT_EQ(kAppServiceType1,
            resumption_data[kAppServiceInfoKey][0].asString());
}

TEST_F(AppServiceAppExtensionTest, ProcessResumption_SUCCESS) {
  app_service_app_extension_->UnsubscribeFromAppService();
  ASSERT_EQ(0u, app_service_app_extension_->Subscriptions().size());

  smart_objects::SmartObject app_service_data =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  app_service_data.asArray()->push_back(
      smart_objects::SmartObject(kAppServiceType1));
  app_service_data.asArray()->push_back(
      smart_objects::SmartObject(kAppServiceType2));

  smart_objects::SmartObject resumption_data;
  resumption_data[kAppServiceInfoKey] = app_service_data;

  resumption::Subscriber subscriber;
  app_service_app_extension_->ProcessResumption(resumption_data, subscriber);

  for (const auto& app_service_type : {kAppServiceType1, kAppServiceType2}) {
    EXPECT_TRUE(
        app_service_app_extension_->IsSubscribedToAppService(app_service_type));
  }
  EXPECT_EQ(2u, app_service_app_extension_->Subscriptions().size());
}

}  // namespace app_service_plugin_test
}  // namespace components
}  // namespace test
