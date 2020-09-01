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
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "rc_rpc_plugin/rc_app_extension.h"

#include <memory>
#include <string>

#include "application_manager/mock_application.h"
#include "gtest/gtest.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

namespace {
const uint32_t kRCAppExtensionId = 1ull;
}  // namespace

namespace test {
namespace components {
namespace rc_rpc_plugin_test {
namespace rc_app_extension_test {

using test::components::application_manager_test::MockApplication;
using ::testing::NiceMock;

class RcAppExtensionTest : public testing::Test {
 public:
  RcAppExtensionTest()
      : mock_app_(new NiceMock<MockApplication>())
      , rc_app_extension_(std::make_shared<rc_rpc_plugin::RCAppExtension>(
            kRCAppExtensionId, rc_plugin_, *mock_app_)) {}

 protected:
  std::unique_ptr<MockApplication> mock_app_;
  rc_rpc_plugin::RCRPCPlugin rc_plugin_;
  rc_rpc_plugin::RCAppExtensionPtr rc_app_extension_;
};

TEST_F(RcAppExtensionTest, SubscribeToInteriorVehicleData_AppDoesntUpdateHash) {
  const std::string module_type = "CLIMATE";
  const std::string module_id = "9cb963f3-c5e8-41cb-b001-19421cc16552";
  rc_rpc_plugin::ModuleUid module{module_type, module_id};

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  rc_app_extension_->SubscribeToInteriorVehicleData(module);
  EXPECT_TRUE(rc_app_extension_->IsSubscribedToInteriorVehicleData(module));

  auto subscription = rc_app_extension_->InteriorVehicleDataSubscriptions();
  EXPECT_EQ(1ull, subscription.size());
}

TEST_F(RcAppExtensionTest,
       UnsubscribeFromInteriorVehicleData_AppDoesntUpdateHash) {
  const std::string module_type = "CLIMATE";
  const std::string module_id = "9cb963f3-c5e8-41cb-b001-19421cc16552";
  rc_rpc_plugin::ModuleUid module{module_type, module_id};

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);
  rc_app_extension_->SubscribeToInteriorVehicleData(module);

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);
  rc_app_extension_->UnsubscribeFromInteriorVehicleData(module);

  EXPECT_FALSE(rc_app_extension_->IsSubscribedToInteriorVehicleData(module));

  auto subscription = rc_app_extension_->InteriorVehicleDataSubscriptions();
  EXPECT_TRUE(subscription.empty());
}

TEST_F(RcAppExtensionTest,
       UnsubscribeFromInteriorVehicleDataOfType_AppDoesntUpdateHash) {
  const std::string module1_type = "CLIMATE";
  const std::string module1_id = "9cb963f3-c5e8-41cb-b001-19421cc16552";
  rc_rpc_plugin::ModuleUid module1{module1_type, module1_id};

  const std::string module2_type = "RADIO";
  const std::string module2_id = "357a3918-9f35-4d86-a8b6-60cd4308d76f";
  rc_rpc_plugin::ModuleUid module2{module2_type, module2_id};

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);
  rc_app_extension_->SubscribeToInteriorVehicleData(module1);
  rc_app_extension_->SubscribeToInteriorVehicleData(module2);

  EXPECT_CALL(*mock_app_, UpdateHash());
  rc_app_extension_->UnsubscribeFromInteriorVehicleDataOfType(module1_type);

  EXPECT_FALSE(
      rc_app_extension_->IsSubscribedToInteriorVehicleDataOfType(module1_type));
  EXPECT_TRUE(
      rc_app_extension_->IsSubscribedToInteriorVehicleDataOfType(module2_type));

  auto subscription = rc_app_extension_->InteriorVehicleDataSubscriptions();
  EXPECT_EQ(1ull, subscription.size());
}

TEST_F(RcAppExtensionTest,
       UnsubscribeFromInteriorVehicleDataOfType_UNSUCCESS_AppDoesntUpdateHash) {
  const std::string module1_type = "CLIMATE";
  const std::string module_id = "9cb963f3-c5e8-41cb-b001-19421cc16552";
  rc_rpc_plugin::ModuleUid module{module1_type, module_id};

  const std::string module2_type = "RADIO";

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);
  rc_app_extension_->SubscribeToInteriorVehicleData(module);

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);
  rc_app_extension_->UnsubscribeFromInteriorVehicleDataOfType(module2_type);

  EXPECT_TRUE(rc_app_extension_->IsSubscribedToInteriorVehicleData(module));

  auto subscription = rc_app_extension_->InteriorVehicleDataSubscriptions();
  EXPECT_EQ(1ull, subscription.size());
}

TEST_F(RcAppExtensionTest, SaveResumptionData_SUCCESS) {
  using namespace rc_rpc_plugin;

  const std::string module1_type = "CLIMATE";
  const std::string module1_id = "9cb963f3-c5e8-41cb-b001-19421cc16552";
  ModuleUid module1{module1_type, module1_id};

  const std::string module2_type = "RADIO";
  const std::string module2_id = "357a3918-9f35-4d86-a8b6-60cd4308d76f";
  ModuleUid module2{module2_type, module2_id};

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);
  rc_app_extension_->SubscribeToInteriorVehicleData(module1);
  rc_app_extension_->SubscribeToInteriorVehicleData(module2);

  const auto subscriptions =
      rc_app_extension_->InteriorVehicleDataSubscriptions();

  auto subscription_so =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  rc_app_extension_->SaveResumptionData(subscription_so);

  const auto module_data =
      subscription_so[message_params::kModuleData].asArray();

  EXPECT_NE(nullptr, module_data);

  EXPECT_EQ(module_data->size(), subscriptions.size());

  auto module_info_so =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  module_info_so[message_params::kModuleType] = module1.first;
  module_info_so[message_params::kModuleId] = module1.second;

  auto found_module =
      std::find(module_data->begin(), module_data->end(), module_info_so);

  EXPECT_NE(found_module, module_data->end());

  module_info_so[message_params::kModuleType] = module2.first;
  module_info_so[message_params::kModuleId] = module2.second;

  found_module =
      std::find(module_data->begin(), module_data->end(), module_info_so);

  EXPECT_NE(found_module, module_data->end());
}

}  // namespace rc_app_extension_test
}  // namespace rc_rpc_plugin_test
}  // namespace components
}  // namespace test
