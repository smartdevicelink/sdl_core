/* Copyright (c) 2013, Ford Motor Company
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "mock_policy_listener.h"
#include "mock_pt_representation.h"
#include "mock_pt_ext_representation.h"
#include "policy/policy_manager_impl.h"
#include "json/value.h"

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;

using ::policy::PTRepresentation;
using ::policy::MockPolicyListener;
using ::policy::MockPTRepresentation;
using ::policy::MockPTExtRepresentation;
using ::policy::PolicyManagerImpl;
using ::policy::PolicyTable;
using ::policy::EndpointUrls;

namespace test {
namespace components {
namespace policy {

class PolicyManagerImplTest : public ::testing::Test {
  protected:
    static void SetUpTestCase() {
    }

    static void TearDownTestCase() {
    }
};

TEST_F(PolicyManagerImplTest, ExceededIgnitionCycles) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, IgnitionCyclesBeforeExchange()).Times(2).WillOnce(
    Return(5)).WillOnce(Return(0));
  EXPECT_CALL(
    mock_pt, IncrementIgnitionCycles()).Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_FALSE(manager->ExceededIgnitionCycles());
  manager->IncrementIgnitionCycles();
  EXPECT_TRUE(manager->ExceededIgnitionCycles());
}

TEST_F(PolicyManagerImplTest, ExceededDays) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, DaysBeforeExchange(_)).Times(2).WillOnce(Return(5))
  .WillOnce(Return(0));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_FALSE(manager->ExceededDays(5));
  EXPECT_TRUE(manager->ExceededDays(15));
}

TEST_F(PolicyManagerImplTest, ExceededKilometers) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, KilometersBeforeExchange(_)).Times(2).WillOnce(
    Return(50)).WillOnce(Return(0));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_FALSE(manager->ExceededKilometers(50));
  EXPECT_TRUE(manager->ExceededKilometers(150));
}

TEST_F(PolicyManagerImplTest, NextRetryTimeout) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;
  std::vector<int> seconds;
  seconds.push_back(50);
  seconds.push_back(100);
  seconds.push_back(200);

  EXPECT_CALL(mock_pt, TimeoutResponse()).Times(1).WillOnce(Return(60));
  EXPECT_CALL(mock_pt,
              SecondsBetweenRetries(_)).Times(1).WillOnce(
                DoAll(SetArgPointee<0>(seconds), Return(true)));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_EQ(50, manager->NextRetryTimeout());
  EXPECT_EQ(100, manager->NextRetryTimeout());
  EXPECT_EQ(200, manager->NextRetryTimeout());
  EXPECT_EQ(0, manager->NextRetryTimeout());
}

TEST_F(PolicyManagerImplTest, GetUpdateUrl) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;
  EndpointUrls urls_1234, urls_4321;
  urls_1234.push_back(::policy::EndpointData("http://ford.com/cloud/1"));
  urls_1234.push_back(::policy::EndpointData("http://ford.com/cloud/2"));
  urls_1234.push_back(::policy::EndpointData("http://ford.com/cloud/3"));
  urls_4321.push_back(::policy::EndpointData("http://panasonic.com/cloud/1"));
  urls_4321.push_back(::policy::EndpointData("http://panasonic.com/cloud/2"));
  urls_4321.push_back(::policy::EndpointData("http://panasonic.com/cloud/3"));

  EXPECT_CALL(mock_pt, GetUpdateUrls(7)).Times(4).WillRepeatedly(
    Return(urls_1234));
  EXPECT_CALL(mock_pt,
              GetUpdateUrls(4)).Times(2).WillRepeatedly(Return(urls_4321));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_EQ("http://ford.com/cloud/1", manager->GetUpdateUrl(7));
  EXPECT_EQ("http://ford.com/cloud/2", manager->GetUpdateUrl(7));
  EXPECT_EQ("http://ford.com/cloud/3", manager->GetUpdateUrl(7));
  EXPECT_EQ("http://panasonic.com/cloud/1", manager->GetUpdateUrl(4));
  EXPECT_EQ("http://ford.com/cloud/2", manager->GetUpdateUrl(7));
  EXPECT_EQ("http://panasonic.com/cloud/3", manager->GetUpdateUrl(4));
}

TEST_F(PolicyManagerImplTest, RefreshRetrySequence) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;
  std::vector<int> seconds, seconds_empty;
  seconds.push_back(50);
  seconds.push_back(100);
  seconds.push_back(200);

  EXPECT_CALL(mock_pt, TimeoutResponse()).Times(2).WillOnce(Return(0)).WillOnce(
    Return(60));
  EXPECT_CALL(mock_pt, SecondsBetweenRetries(_)).Times(2).WillOnce(
    DoAll(SetArgPointee<0>(seconds_empty), Return(true))).WillOnce(
      DoAll(SetArgPointee<0>(seconds), Return(true)));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->RefreshRetrySequence();
  EXPECT_EQ(60, manager->TimeoutExchange());
  EXPECT_EQ(50, manager->NextRetryTimeout());
  EXPECT_EQ(100, manager->NextRetryTimeout());
  EXPECT_EQ(200, manager->NextRetryTimeout());
}

#ifdef EXTENDED_POLICY
TEST_F(PolicyManagerImplTest, IncrementGlobalCounter) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, Increment("count_of_sync_reboots")).Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->Increment(usage_statistics::SYNC_REBOOTS);
}

TEST_F(PolicyManagerImplTest, IncrementAppCounter) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, Increment("12345", "count_of_user_selections")).Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->Increment("12345", usage_statistics::USER_SELECTIONS);
}

TEST_F(PolicyManagerImplTest, SetAppInfo) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, Set("12345", "app_registration_language_gui", "de-de")).
  Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->Set("12345", usage_statistics::LANGUAGE_GUI, "de-de");
}

TEST_F(PolicyManagerImplTest, AddAppStopwatch) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, Add("12345", "minutes_hmi_full", 30)).Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->Add("12345", usage_statistics::SECONDS_HMI_FULL, 30);
}
#endif  // EXTENDED_POLICY

TEST_F(PolicyManagerImplTest, ResetPT) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, Init()).WillOnce(Return(::policy::NONE))
  //.WillOnce(Return(::policy::EXISTS));
  //.WillOnce(Return(::policy::SUCCESS))
  .WillOnce(Return(::policy::FAIL));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_FALSE(manager->ResetPT("filename"));
  // TODO(AOleynik): Sometimes fails, check this
  //  EXPECT_TRUE(manager->ResetPT("filename"));
  //  EXPECT_TRUE(manager->ResetPT("filename"));
  EXPECT_FALSE(manager->ResetPT("filename"));
}

TEST_F(PolicyManagerImplTest, CheckPermissions) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;
  MockPolicyListener mock_listener;

  ::policy::CheckPermissionResult result;
  result.hmi_level_permitted = ::policy::kRpcAllowed;
  result.list_of_allowed_params = new std::vector< ::policy::PTString>();
  result.list_of_allowed_params->push_back("speed");
  result.list_of_allowed_params->push_back("gps");

#ifdef EXTENDED_POLICY
  EXPECT_CALL(mock_pt, CheckPermissions("pre_DataConsent", "FULL", "Alert")).WillOnce(
      Return(result));
#else  // EXTENDED_POLICY
  EXPECT_CALL(mock_pt, CheckPermissions("12345678", "FULL", "Alert")).WillOnce(
      Return(result));
#endif  // EXTENDED_POLICY
  EXPECT_CALL(mock_listener, OnCurrentDeviceIdUpdateRequired("12345678")).Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->set_listener(&mock_listener);
  ::policy::CheckPermissionResult out_result = manager->CheckPermissions(
      "12345678", "FULL", "Alert");
  EXPECT_EQ(::policy::kRpcAllowed, out_result.hmi_level_permitted);
  ASSERT_TRUE(out_result.list_of_allowed_params);
  ASSERT_EQ(2u, out_result.list_of_allowed_params->size());
  EXPECT_EQ("speed", (*out_result.list_of_allowed_params)[0]);
  EXPECT_EQ("gps", (*out_result.list_of_allowed_params)[1]);
}

TEST_F(PolicyManagerImplTest, DISABLED_LoadPT) {
  // TODO(KKolodiy): PolicyManagerImpl is hard for testing
  ::testing::NiceMock<MockPTRepresentation> mock_pt;
  MockPolicyListener mock_listener;

  Json::Value table(Json::objectValue);
  table["policy_table"] = Json::Value(Json::objectValue);

  Json::Value& policy_table = table["policy_table"];
  policy_table["module_meta"] = Json::Value(Json::objectValue);
  policy_table["module_config"] = Json::Value(Json::objectValue);
  policy_table["usage_and_error_counts"] = Json::Value(Json::objectValue);
  policy_table["device_data"] = Json::Value(Json::objectValue);
  policy_table["functional_groupings"] = Json::Value(Json::objectValue);
  policy_table["consumer_friendly_messages"] = Json::Value(Json::objectValue);
  policy_table["app_policies"] = Json::Value(Json::objectValue);
  policy_table["app_policies"]["1234"] = Json::Value(Json::objectValue);

  Json::Value& module_meta = policy_table["module_meta"];
  module_meta["ccpu_version"] = Json::Value("ccpu version");
  module_meta["language"] = Json::Value("ru");
  module_meta["wers_country_code"] = Json::Value("ru");
  module_meta["pt_exchanged_at_odometer_x"] = Json::Value(0);
  module_meta["pt_exchanged_x_days_after_epoch"] = Json::Value(0);
  module_meta["ignition_cycles_since_last_exchange"] = Json::Value(0);
  module_meta["vin"] = Json::Value("vin");

  Json::Value& module_config = policy_table["module_config"];
  module_config["preloaded_pt"] = Json::Value(true);
  module_config["exchange_after_x_ignition_cycles"] = Json::Value(10);
  module_config["exchange_after_x_kilometers"] = Json::Value(100);
  module_config["exchange_after_x_days"] = Json::Value(5);
  module_config["timeout_after_x_seconds"] = Json::Value(500);
  module_config["seconds_between_retries"] = Json::Value(Json::arrayValue);
  module_config["seconds_between_retries"][0] = Json::Value(10);
  module_config["seconds_between_retries"][1] = Json::Value(20);
  module_config["seconds_between_retries"][2] = Json::Value(30);
  module_config["endpoints"] = Json::Value(Json::objectValue);
  module_config["endpoints"]["0x00"] = Json::Value(Json::objectValue);
  module_config["endpoints"]["0x00"]["default"] = Json::Value(Json::arrayValue);
  module_config["endpoints"]["0x00"]["default"][0] = Json::Value(
      "http://ford.com/cloud/default");
  module_config["notifications_per_minute_by_priority"] = Json::Value(
      Json::objectValue);
  module_config["notifications_per_minute_by_priority"]["emergency"] =
      Json::Value(1);
  module_config["notifications_per_minute_by_priority"]["navigation"] =
      Json::Value(2);
  module_config["notifications_per_minute_by_priority"]["voiceCommunication"] =
      Json::Value(3);
  module_config["notifications_per_minute_by_priority"]["communication"] =
      Json::Value(4);
  module_config["notifications_per_minute_by_priority"]["normal"] = Json::Value(
      5);
  module_config["notifications_per_minute_by_priority"]["none"] = Json::Value(
      6);
  module_config["vehicle_make"] = Json::Value("MakeT");
  module_config["vehicle_model"] = Json::Value("ModelT");
  module_config["vehicle_year"] = Json::Value(2014);

  Json::Value& usage_and_error_counts = policy_table["usage_and_error_counts"];
  usage_and_error_counts["count_of_iap_buffer_full"] = Json::Value(0);
  usage_and_error_counts["count_sync_out_of_memory"] = Json::Value(0);
  usage_and_error_counts["count_of_sync_reboots"] = Json::Value(0);

  Json::Value& device_data = policy_table["device_data"];
  device_data["DEVICEHASH"] = Json::Value(Json::objectValue);
  device_data["DEVICEHASH"]["hardware"] = Json::Value("hardware");
  device_data["DEVICEHASH"]["firmware_rev"] = Json::Value("firmware_rev");
  device_data["DEVICEHASH"]["os"] = Json::Value("os");
  device_data["DEVICEHASH"]["os_version"] = Json::Value("os_version");
  device_data["DEVICEHASH"]["carrier"] = Json::Value("carrier");
  device_data["DEVICEHASH"]["max_number_rfcom_ports"] = Json::Value(10);

  Json::Value& functional_groupings = policy_table["functional_groupings"];
  functional_groupings["default"] = Json::Value(Json::objectValue);
  Json::Value& default_group = functional_groupings["default"];
  default_group["rpcs"] = Json::Value(Json::objectValue);
  default_group["rpcs"]["Update"] = Json::Value(Json::objectValue);
  default_group["rpcs"]["Update"]["hmi_levels"] = Json::Value(Json::arrayValue);
  default_group["rpcs"]["Update"]["hmi_levels"][0] = Json::Value("FULL");
  default_group["rpcs"]["Update"]["parameters"] = Json::Value(Json::arrayValue);
  default_group["rpcs"]["Update"]["parameters"][0] = Json::Value("speed");

  Json::Value& consumer_friendly_messages =
      policy_table["consumer_friendly_messages"];
  consumer_friendly_messages["version"] = Json::Value("1.2");

  Json::Value& app_policies = policy_table["app_policies"];
  app_policies["default"] = Json::Value(Json::objectValue);
  app_policies["default"]["memory_kb"] = Json::Value(50);
  app_policies["default"]["watchdog_timer_ms"] = Json::Value(100);
  app_policies["default"]["groups"] = Json::Value(Json::arrayValue);
  app_policies["default"]["groups"][0] = Json::Value("default");
  app_policies["default"]["priority"] = Json::Value("EMERGENCY");
  app_policies["default"]["default_hmi"] = Json::Value("FULL");
  app_policies["default"]["keep_context"] = Json::Value(true);
  app_policies["default"]["steal_focus"] = Json::Value(true);
  app_policies["default"]["certificate"] = Json::Value("sign");

  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(mock_pt, Save(_)).Times(1).WillOnce(Return(true));
  EXPECT_CALL(mock_listener, OnUpdateStatusChanged(_)).Times(1);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->set_listener(&mock_listener);

  EXPECT_TRUE(manager->LoadPT("file_pt_update.json", msg));
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate) {
  ::testing::NiceMock<MockPTRepresentation> mock_pt;
  MockPolicyListener mock_listener;

  ::utils::SharedPtr< ::policy_table::Table> p_table =
    new ::policy_table::Table();
  std::string json = p_table->ToJsonValue().toStyledString();
  ::policy::BinaryMessageSptr expect = new ::policy::BinaryMessage(json.begin(),
      json.end());

  EXPECT_CALL(mock_pt, GenerateSnapshot()).WillOnce(Return(p_table));
  EXPECT_CALL(mock_listener, OnUpdateStatusChanged(_)).Times(2);

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->set_listener(&mock_listener);
  ::policy::BinaryMessageSptr output = manager->RequestPTUpdate();
  EXPECT_EQ(*expect, *output);
}

#ifdef EXTENDED_POLICY
TEST_F(PolicyManagerImplTest, ResetUserConsent) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, ResetUserConsent()).WillOnce(Return(true)).WillOnce(
    Return(false));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  EXPECT_TRUE(manager->ResetUserConsent());
  EXPECT_FALSE(manager->ResetUserConsent());
}
#endif  // EXTENDED_POLICY

TEST_F(PolicyManagerImplTest, CheckAppPolicyState) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  // TODO(AOleynik): Implementation of method should be changed to avoid
  // using of snapshot
  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  //manager->CheckAppPolicyState("12345678");
}

TEST_F(PolicyManagerImplTest, GetPolicyTableStatus) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  // TODO(AOleynik): Test is not finished, to be continued
  //manager->GetPolicyTableStatus();
}

TEST_F(PolicyManagerImplTest, MarkUnpairedDevice) {
  ::testing::NiceMock<MockPTExtRepresentation> mock_pt;

  EXPECT_CALL(mock_pt, SetUnpairedDevice("12345")).WillOnce(Return(true));

  PolicyManagerImpl* manager = new PolicyManagerImpl();
  manager->ResetDefaultPT(::policy::PolicyTable(&mock_pt));
  manager->MarkUnpairedDevice("12345");
}


}  // namespace policy
}  // namespace components
}  // namespace test

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
