/* Copyright (c) 2014, Ford Motor Company
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
#include <fstream>
#include "config_profile/profile.h"
#include "mock_policy_listener.h"
#include "policy/policy_manager_impl.h"

using ::testing::_;
using ::policy::PolicyManagerImpl;
using ::policy::BinaryMessage;
using ::policy::MockPolicyListener;

namespace test {
namespace components {
namespace policy {

class PolicyManagerImplStressTest : public ::testing::Test {
 protected:
  static const std::string kNameFile;
  static const int kNumberFuncs = 999;
  static const int kNumberGroups = 254;
  static const int kNumberGroupFuncs = 49;
  static const int kNumberApps = 999;
  static const int kNumberAppGroups = 254;
  static PolicyManagerImpl* manager;
  static MockPolicyListener* mock_listener;

  static void SetUpTestCase();
  static void TearDownTestCase();
  static void CreateTable(std::ofstream& ofs);
  static void CreateOther(std::ofstream& ofs);
  static void CreateGroups(std::ofstream& ofs);
  static void CreateApps(std::ofstream& ofs);
  static void CreateFuncs(std::ofstream& ofs);
  static void CreateAppGroups(std::ofstream& ofs);
  static void CreateGroup(int i, std::ofstream& ofs);
  static void CreateFunction(int i, std::ofstream& ofs);
  static void CreateHmiLevels(std::ofstream& ofs);
  static void CreateParameters(std::ofstream& ofs);
};

const std::string PolicyManagerImplStressTest::kNameFile =
    "perfomance_test_pt.json";
PolicyManagerImpl* PolicyManagerImplStressTest::manager = 0;
MockPolicyListener* PolicyManagerImplStressTest::mock_listener = 0;

void PolicyManagerImplStressTest::SetUpTestCase() {
  std::ofstream ofs;
  ofs.open(kNameFile.c_str(), std::ofstream::out);
  CreateTable(ofs);
  ofs.close();

  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");

  manager = new PolicyManagerImpl();
  mock_listener = new MockPolicyListener();
  manager->set_listener(mock_listener);

  ASSERT_TRUE(manager->InitPT(kNameFile));
}

void PolicyManagerImplStressTest::TearDownTestCase() {
  delete manager;
  delete mock_listener;
  remove(kNameFile.c_str());
#ifndef __QNX__
  remove("policy.sqlite");
#endif  // __QNX__
}

void PolicyManagerImplStressTest::CreateHmiLevels(std::ofstream& ofs) {
  ofs << "\"hmi_levels\":[\"BACKGROUND\", \"FULL\", \"LIMITED\"]";
}

void PolicyManagerImplStressTest::CreateParameters(std::ofstream& ofs) {
  ofs << "\"parameters\":["
      "\"gps\","
      "\"speed\","
      "\"engineTorque\","
      "\"externalTemperature\","
      "\"fuelLevel\","
      "\"fuelLevel_State\","
      "\"headLampStatus\","
      "\"instantFuelConsumption\","
      "\"odometer\","
      "\"tirePressure\","
      "\"wiperStatus\","
      "\"vin\","
      "\"accPedalPosition\","
      "\"beltStatus\","
      "\"driverBraking\","
      "\"prndl\","
      "\"rpm\","
      "\"steeringWheelAngle\","
      "\"myKey\","
      "\"airbagStatus\","
      "\"bodyInformation\","
      "\"clusterModeStatus\","
      "\"deviceStatus\","
      "\"emergencyEvent\""
      "]";
}

void PolicyManagerImplStressTest::CreateFunction(int i, std::ofstream& ofs) {
  std::stringstream ss;
  std::string number;
  ss << i << std::endl;
  ss >> number;
  ofs << "\"Func-" << number << "\":{\n\t";
  CreateHmiLevels(ofs);
  ofs << ",\n";
  CreateParameters(ofs);
  ofs << "\n}";
}

void PolicyManagerImplStressTest::CreateGroup(int i, std::ofstream& ofs) {
  std::stringstream ss;
  std::string number;
  ss << i << std::endl;
  ss >> number;
  ofs << "\"Group-" << number << "\":{\n\t\"rpcs\":{\n";
  CreateFuncs(ofs);
  ofs << "}}";
}

void PolicyManagerImplStressTest::CreateGroups(std::ofstream& ofs) {
  ofs << "\"functional_groupings\":{\n";
  for (int i = 0; i < kNumberGroups - 1; ++i) {
    ofs << "\t";
    CreateGroup(i, ofs);
    ofs << ",\n";
  }
  ofs << "\t";
  CreateGroup(kNumberGroups - 1, ofs);
  ofs << "\n}\n";
}

void PolicyManagerImplStressTest::CreateFuncs(std::ofstream& ofs) {
  for (int i = 0; i < kNumberGroupFuncs - 1; ++i) {
    ofs << "\t";
    CreateFunction(rand() % kNumberFuncs, ofs);
    ofs << ",\n";
  }
  ofs << "\t";
  CreateFunction(0, ofs);
  ofs << "\n";
}

void PolicyManagerImplStressTest::CreateApps(std::ofstream& ofs) {
  ofs << "\"app_policies\":{";
  ofs << "\"default\":{\n";
  ofs << "\"keep_context\": true,\n"
      "\"steal_focus\": true,\n"
      "\"priority\": \"NORMAL\",\n"
      "\"default_hmi\": \"FULL\",\n";
  ofs << "\"groups\":["
      "\"Group-1\""
      "]"
      "},\n";

  std::stringstream ss;
  std::string number;
  for (int i = 0; i < kNumberApps - 1; ++i) {
    ss << i << std::endl;
    ss >> number;
    ofs << "\"" << number << "\" : {";
    ofs << "\n\"keep_context\": true,\n"
        "\"steal_focus\": true,\n"
        "\"priority\": \"NORMAL\",\n"
        "\"default_hmi\": \"FULL\",\n";

    ofs << "\"groups\": ";
    CreateAppGroups(ofs);
    ofs << "},\n";
  }
  ss << kNumberApps - 1 << std::endl;
  ss >> number;
  ofs << "\"" << number << "\" : {";
  ofs << "\n\"keep_context\": true,\n"
      "\"steal_focus\": true,\n"
      "\"priority\": \"NORMAL\",\n"
      "\"default_hmi\": \"FULL\",\n";

  ofs << "\"groups\": ";
  CreateAppGroups(ofs);
  ofs << "}}";
}

void PolicyManagerImplStressTest::CreateAppGroups(std::ofstream& ofs) {
  ofs << "[";

  std::stringstream ss;
  std::string number;
  std::set<int> app_groups;
  for (int i = 0; i < kNumberAppGroups; ++i) {
    app_groups.insert(rand() % kNumberGroups);
  }

  std::set<int>::const_iterator i = app_groups.begin();
  ss << *i << std::endl;
  ss >> number;
  ofs << "\"Group-" << number << "\"";
  ++i;
  for (; i != app_groups.end(); ++i) {
    ss << *i << std::endl;
    ss >> number;
    ofs << ",\"Group-" << number << "\"";
  }
  ofs << "]\n";
}

void PolicyManagerImplStressTest::CreateOther(std::ofstream& ofs) {
  ofs << "\"module_config\":{\n"
      "\"preloaded_pt\":true,\n"
      "\"endpoints\":{\n"
      "\"default\": {\n"
      "\"default\":[\"http://sdl.net/api\"]\n"
      "}},\n"
      "\"notifications_per_minute_by_priority\": {\n"
      "\"EMERGENCY\": 60,\n"
      "\"NAVIGATION\": 15,\n"
      "\"COMMUNICATION\": 6,\n"
      "\"NORMAL\": 4,\n"
      "\"NONE\": 0\n"
      "},\n"
      "\"exchange_after_x_ignition_cycles\": 40,\n"
      "\"exchange_after_x_kilometers\" : 2,\n"
      "\"exchange_after_x_days\" : 23,\n"
      "\"timeout_after_x_seconds\" : 20,\n"
      "\"seconds_between_retries\" : [10, 7, 5, 3, 1]\n"
      "},\n"

      "\"consumer_friendly_messages\":{\n"
      "\"version\":\"001.001.001\",\n"
      "\"messages\":{}\n"
      "}";
}

void PolicyManagerImplStressTest::CreateTable(std::ofstream& ofs) {
  ofs << "{\"policy_table\":{\n";
  CreateOther(ofs);
  ofs << ",\n";
  CreateGroups(ofs);
  ofs << ",\n";
  CreateApps(ofs);
  ofs << "\n}}";
}

TEST_F(PolicyManagerImplStressTest, OneCheck_AppAndFunctuionExisting_RpcAllowed) {
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
#ifdef SDL_REMOTE_CONTROL
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired("2"));
#endif  // SDL_REMOTE_CONTROL
  manager->CheckPermissions("2", "FULL", "Func-0", input_params, output);
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, NoApp_AppDoesNotExisted_RpcDissallowed) {
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
#ifdef SDL_REMOTE_CONTROL
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired("1500")).Times(0);
#endif  // SDL_REMOTE_CONTROL
  manager->CheckPermissions("1500", "FULL", "Func-88", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, NoFunc_FuncDoesNotExisted_RpcDissallowed) {
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
#ifdef SDL_REMOTE_CONTROL
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired("2"));
#endif  // SDL_REMOTE_CONTROL
  manager->CheckPermissions("2", "FULL", "Func-1500", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, NoHmi_HMIInLevelNone_RpcDissallowed) {
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
#ifdef SDL_REMOTE_CONTROL
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired("2"));
#endif  // SDL_REMOTE_CONTROL
  manager->CheckPermissions("2", "NONE", "Func-88", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, FewChecks_CheckSeveralFunctions) {
  const int kNumberOfCheckings = 100;
  std::stringstream ss;
  int app, func;
  std::string app_number, func_number;
#ifdef SDL_REMOTE_CONTROL
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired(_)).Times(
      kNumberOfCheckings);
#endif  // SDL_REMOTE_CONTROL
  for (int i = 0; i < kNumberOfCheckings; ++i) {
    app = rand() % kNumberApps;
    func = rand() % kNumberFuncs;
    ss << app << std::endl;
    ss >> app_number;
    ss << func << std::endl;
    ss >> func_number;

    ::policy::RPCParams input_params;
    ::policy::CheckPermissionResult output;
    manager->CheckPermissions(app_number, "FULL", "Func-" + func_number,
                              input_params, output);
    // This test always pass. It is needed only for measure performance.
  }
}

}  // namespace policy
}  // namespace components
}  // namespace test
