/* Copyright (c) 2016, Ford Motor Company
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
#include "mock_policy_listener.h"
#include "policy/policy_manager_impl.h"

using ::testing::_;
using ::policy::PolicyManagerImpl;
using ::policy::BinaryMessage;
using ::policy::MockPolicyListener;

namespace test {
namespace components {
namespace policy_test {

class PolicyManagerImplStressTest : public ::testing::Test {
 protected:
  static const std::string kNameFile;
  static const int kNumberGroups = 3;  // 10;
  static const int kNumberFuncs = 4;   // 100;
  static const int kNumberApps = 5;
  static const int kNumberAppGroups = 5;
  static PolicyManagerImpl* manager;
  static MockPolicyListener* mock_listener;

  static void SetUpTestCase();
  static void TearDownTestCase();
  static void CreateTable(std::ofstream& ofs);
  static void CreateGroups(std::ofstream& ofs);
  static void CreateFuncs(std::ofstream& ofs);
  static void CreateApps(std::ofstream& ofs);
  static void CreateAppGroups(std::ofstream& ofs);
};

const std::string PolicyManagerImplStressTest::kNameFile =
    "sdl_preloaded_pt.json";
PolicyManagerImpl* PolicyManagerImplStressTest::manager = 0;
MockPolicyListener* PolicyManagerImplStressTest::mock_listener = 0;

void PolicyManagerImplStressTest::SetUpTestCase() {
  std::ofstream ofs;
  ofs.open(kNameFile.c_str(), std::ofstream::out);
  CreateTable(ofs);
  ofs.close();

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

void PolicyManagerImplStressTest::CreateGroups(std::ofstream& ofs) {
  std::stringstream ss;
  std::string number;
  for (int i = 0; i < kNumberGroups - 1; ++i) {
    ss << i << std::endl;
    ss >> number;
    ofs << "\"Group-" << number << "\":{\n \t\"rpcs\":{\n";
    CreateFuncs(ofs);
    ofs << "} },\n";
  }
  ss << kNumberGroups - 1 << std::endl;
  ss >> number;
  ofs << "\"Group-" << number << "\":{\n \t\"rpcs\":{\n";
  CreateFuncs(ofs);
  ofs << "} }\n";
}

void PolicyManagerImplStressTest::CreateFuncs(std::ofstream& ofs) {
  std::string func =
      "{\n"
      "\t\t\"hmi_levels\":["
      "\"BACKGROUND\","
      "\"FULL\","
      "\"LIMITED\""
      "]"
      "}";

  std::stringstream ss;
  std::string number;
  for (int i = 0; i < kNumberFuncs - 1; ++i) {
    ss << i << std::endl;
    ss >> number;
    ofs << "\t\"Func-" << number << "\":" << func << ",\n";
  }
  ss << kNumberFuncs - 1 << std::endl;
  ss >> number;
  ofs << "\t\"Func-" << number << "\":" + func;
}

void PolicyManagerImplStressTest::CreateApps(std::ofstream& ofs) {
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
  ofs << "}\n";
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

void PolicyManagerImplStressTest::CreateTable(std::ofstream& ofs) {
  ofs << "{"
         "\"policy_table\":{\n"
         "\"module_config\":{\n"
         "\t\"preloaded_pt\":true,\n"
         "\t\"endpoints\":{\n"
         "\t\t\"default\": {\n"
         "\t\t\t\"default\":["
         "\"http://sdl.net/api\""
         "]\n"
         "\t\t}\n"
         "\t},\n"

         "\"notifications_per_minute_by_priority\": {\n"
         "\t\"EMERGENCY\": 60,\n"
         "\t\"NAVIGATION\": 15,\n"
         "\t\"COMMUNICATION\": 6,\n"
         "\t\"NORMAL\": 4,\n"
         "\t\"NONE\": 0\n"
         "},\n"

         "\"exchange_after_x_ignition_cycles\": 40,\n"
         "\"exchange_after_x_kilometers\" : 2,\n"
         "\"exchange_after_x_days\" : 23,\n"
         "\"timeout_after_x_seconds\" : 20,\n"
         "\"seconds_between_retries\" : [10, 7, 5, 3, 1]\n"
         "},"
         "\"consumer_friendly_messages\":{\n"
         "\t\"version\":\"001.001.001\",\n"
         "\t\"messages\":{} },\n"
         "\"functional_groupings\":{\n";

  CreateGroups(ofs);

  ofs << "}, \"app_policies\":{";

  CreateApps(ofs);

  ofs << "} } }";
}

TEST_F(PolicyManagerImplStressTest,
       OneCheck_AppAndFunctuionExisting_RpcAllowed) {
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired(_));
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  manager->CheckPermissions("2", "FULL", "Func-1", input_params, output);
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, NoApp_AppDoesNotExisted_RpcDissallowed) {
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired(_));
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  manager->CheckPermissions("150", "FULL", "Func-88", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, NoFunc_FuncDoesNotExisted_RpcDissallowed) {
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired(_)).Times(1);
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  manager->CheckPermissions("2", "FULL", "Func-400", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest, NoHmi_HMIInLevelNone_RpcDissallowed) {
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired(_));
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  manager->CheckPermissions("2", "NONE", "Func-88", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplStressTest,
       FewChecks_CheckSeveralFunctions_RpcAllowed) {
  EXPECT_CALL(*mock_listener, OnCurrentDeviceIdUpdateRequired(_))
      .Times(kNumberFuncs);
  const int kNumberOfCheckings = kNumberFuncs;  // 100;
  std::stringstream ss;
  int app, func;
  std::string app_number, func_number;
  for (int i = 0; i < kNumberOfCheckings; ++i) {
    app = rand() % kNumberApps;
    func = rand() % kNumberFuncs;
    ss << app << std::endl;
    ss >> app_number;
    ss << func << std::endl;
    ss >> func_number;

    ::policy::RPCParams input_params;
    ::policy::CheckPermissionResult output;
    manager->CheckPermissions(
        app_number, "FULL", "Func-" + func_number, input_params, output);
    EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  }
}

}  // namespace policy_test
}  // namespace components
}  // namespace test
