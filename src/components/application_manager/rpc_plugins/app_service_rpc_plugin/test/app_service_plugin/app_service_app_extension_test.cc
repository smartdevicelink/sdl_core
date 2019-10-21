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
  AppServiceAppExtensionTest()
      : mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , app_service_app_extension_(
            std::make_shared<app_service_rpc_plugin::AppServiceAppExtension>(
                app_service_plugin_, *mock_app_)) {
    ON_CALL(*mock_app_, AddExtension(_)).WillByDefault(Return(true));
    ON_CALL(*mock_app_, QueryInterface(_))
        .WillByDefault(Return(app_service_app_extension_));
  }

 protected:
  app_service_rpc_plugin::AppServiceRpcPlugin app_service_plugin_;
  std::shared_ptr<MockApplication> mock_app_;
  std::shared_ptr<AppServiceAppExtension> app_service_app_extension_;
};

TEST_F(AppServiceAppExtensionTest, SubscribeToAppService_SUCCESS) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);
  EXPECT_TRUE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType1));
  const auto& subs = app_service_app_extension->Subscriptions();
  EXPECT_EQ(1u, subs.size());
  EXPECT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));
}

TEST_F(AppServiceAppExtensionTest,
       SubscribeToAppService_SubscribeOneAppServiceType_Twice_FAIL) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);
  ASSERT_TRUE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType1));
  ASSERT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));

  EXPECT_FALSE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType1));
  EXPECT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));
  const auto& subs = app_service_app_extension->Subscriptions();
  EXPECT_EQ(1u, subs.size());
}

TEST_F(
    AppServiceAppExtensionTest,
    UnsubscribeFromAppService_AppServiceType1Unsubscribed_AppServiceType2Remains_SUCCESS) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);
  // Subscribe
  ASSERT_TRUE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType1));
  ASSERT_TRUE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType2));
  auto subs = app_service_app_extension->Subscriptions();
  ASSERT_EQ(2u, subs.size());
  ASSERT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));
  ASSERT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType2));

  // Unsubscribe
  EXPECT_TRUE(
      app_service_app_extension->UnsubscribeFromAppService(kAppServiceType1));
  EXPECT_FALSE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));
  EXPECT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType2));
  subs = app_service_app_extension->Subscriptions();
  EXPECT_EQ(1u, subs.size());
}

TEST_F(AppServiceAppExtensionTest,
       UnsubscribeFromAppService_UnsubscribeNotSubscribedAppServiceType_FAIL) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);

  ASSERT_TRUE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType1));
  ASSERT_EQ(1u, app_service_app_extension->Subscriptions().size());
  ASSERT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));

  EXPECT_FALSE(
      app_service_app_extension->UnsubscribeFromAppService(kAppServiceType2));
  EXPECT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));
  EXPECT_EQ(1u, app_service_app_extension->Subscriptions().size());
}

TEST_F(AppServiceAppExtensionTest,
       UnsubscribeFromAppService_UnsubscribeAll_SUCCESS) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);

  for (const auto& app_service_type : {kAppServiceType1, kAppServiceType2}) {
    ASSERT_TRUE(
        app_service_app_extension->SubscribeToAppService(app_service_type));
    ASSERT_TRUE(
        app_service_app_extension->IsSubscribedToAppService(app_service_type));
  }
  ASSERT_EQ(2u, app_service_app_extension->Subscriptions().size());

  app_service_app_extension->UnsubscribeFromAppService();

  for (const auto& app_service_type : {kAppServiceType1, kAppServiceType2}) {
    EXPECT_FALSE(
        app_service_app_extension->IsSubscribedToAppService(app_service_type));
  }
  EXPECT_EQ(0u, app_service_app_extension->Subscriptions().size());
}

TEST_F(AppServiceAppExtensionTest, SaveResumptionData_SUCCESS) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);

  ASSERT_TRUE(
      app_service_app_extension->SubscribeToAppService(kAppServiceType1));
  ASSERT_EQ(1u, app_service_app_extension->Subscriptions().size());
  ASSERT_TRUE(
      app_service_app_extension->IsSubscribedToAppService(kAppServiceType1));

  smart_objects::SmartObject resumption_data;
  resumption_data[kResumptionDataKey] = "some resumption data";

  app_service_app_extension->SaveResumptionData(resumption_data);

  EXPECT_TRUE(resumption_data.keyExists(kResumptionDataKey));
  EXPECT_TRUE(resumption_data.keyExists(kAppServiceInfoKey));
  EXPECT_EQ(kAppServiceType1,
            resumption_data[kAppServiceInfoKey][0].asString());
}

TEST_F(AppServiceAppExtensionTest, ProcessResumption_SUCCESS) {
  auto app_service_app_extension =
      std::make_shared<AppServiceAppExtension>(app_service_plugin_, *mock_app_);
  app_service_app_extension->UnsubscribeFromAppService();
  ASSERT_EQ(0u, app_service_app_extension->Subscriptions().size());

  smart_objects::SmartObject resumption_data;
  resumption_data[kAppServiceInfoKey] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  resumption_data[kAppServiceInfoKey].asArray()->push_back(
      smart_objects::SmartObject(kAppServiceType1));
  resumption_data[kAppServiceInfoKey].asArray()->push_back(
      smart_objects::SmartObject(kAppServiceType2));

  app_service_app_extension->ProcessResumption(resumption_data);

  for (const auto& app_service_type : {kAppServiceType1, kAppServiceType2}) {
    EXPECT_TRUE(
        app_service_app_extension->IsSubscribedToAppService(app_service_type));
  }
  EXPECT_EQ(2u, app_service_app_extension->Subscriptions().size());
  EXPECT_EQ(kAppServiceType1,
            *app_service_app_extension->Subscriptions().begin());
}
}  // namespace app_service_plugin_test
