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
#include <string>
#include <set>
#include <sstream>
#include <fstream>
#include "policy/policy_manager_impl.h"
#include "mock_policy_listener.h"

using ::testing::_;
using ::policy::PolicyManagerImpl;
using ::policy::BinaryMessage;
using ::policy::MockPolicyListener;

namespace test {
namespace components {
namespace policy {

class PolicyManagerImplTest : public ::testing::Test {
 protected:
  static const std::string kNameFile;
  static const int kNumberGroups = 100;
  static const int kNumberFuncs = 200;
  static const int kNumberApps = 100;
  static const int kNumberAppGroups = 50;
  static PolicyManagerImpl* manager;

  static void SetUpTestCase() {
    std::ofstream ofs;
    ofs.open(kNameFile.c_str(), std::ofstream::out);
    CreateTable(ofs);
    ofs.close();

    manager = new PolicyManagerImpl();
    ASSERT_TRUE(manager->InitPT(kNameFile));
  }

  static void TearDownTestCase() {
    delete manager;
    remove(kNameFile.c_str());
    remove("policy.sqlite");
  }

  static void CreateTable(std::ofstream& ofs);
  static void CreateGroups(std::ofstream& ofs);
  static void CreateFuncs(std::ofstream& ofs);
  static void CreateApps(std::ofstream& ofs);
  static void CreateAppGroups(std::ofstream& ofs);
};

const std::string PolicyManagerImplTest::kNameFile = "pt.json";
PolicyManagerImpl* PolicyManagerImplTest::manager = 0;

void PolicyManagerImplTest::CreateGroups(std::ofstream& ofs) {
  std::stringstream ss;
  std::string number;
  for (int i = 0; i < kNumberGroups - 1; ++i) {
    ss << i << std::endl;
    ss >> number;
    ofs << "\"Group-" << number << "\":{ \"rpcs\":{";
    CreateFuncs(ofs);
    ofs << "} },";
  }
  ss << kNumberGroups - 1 << std::endl;
  ss >> number;
  ofs << "\"Group-" << number << "\":{ \"rpcs\":{";
  CreateFuncs(ofs);
  ofs << "} }";
}

void PolicyManagerImplTest::CreateFuncs(std::ofstream& ofs) {
  std::string func = "{"
      "\"hmi_levels\":["
      "\"BACKGROUND\","
      "\"FULL\","
      "\"LIMITED\""
      "],"
      "\"parameters\":["
      "\"gps\","
      "\"speed\","
      "\"enginetorque\","
      "\"externaltemperature\","
      "\"fuellevel\","
      "\"fuellevel_state\","
      "\"headlampstatus\","
      "\"instantfuelconsumption\","
      "\"odometer\","
      "\"tirepressure\","
      "\"wiperstatus\","
      "\"vin\","
      "\"accpedalposition\","
      "\"beltstatus\","
      "\"driverbraking\","
      "\"prndl\","
      "\"rpm\","
      "\"steeringwheelangle\""
      "]"
      "}";

  std::stringstream ss;
  std::string number;
  for (int i = 0; i < kNumberFuncs - 1; ++i) {
    ss << i << std::endl;
    ss >> number;
    ofs << "\"Func-" << number << "\":" << func << ",";
  }
  ss << kNumberFuncs - 1 << std::endl;
  ss >> number;
  ofs << "\"Func-" << number << "\":" + func;
}

void PolicyManagerImplTest::CreateApps(std::ofstream& ofs) {
  ofs << "\"default\":{"
      "\"groups\":["
      "\"Group-1\""
      "]"
      "},";

  std::stringstream ss;
  std::string number;
  for (int i = 0; i < kNumberApps - 1; ++i) {
    ss << i << std::endl;
    ss >> number;
    ofs << "\"" << number << "\": { \"groups\": ";
    CreateAppGroups(ofs);
    ofs << "},";
  }
  ss << kNumberApps - 1 << std::endl;
  ss >> number;
  ofs << "\"" << number << "\": { \"groups\": ";
  CreateAppGroups(ofs);
  ofs << "}";
}

void PolicyManagerImplTest::CreateAppGroups(std::ofstream& ofs) {
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
  ofs << "]";
}

void PolicyManagerImplTest::CreateTable(std::ofstream& ofs) {
  ofs << "{"
      "\"policy_table\":{"
      "\"module_config\":{"
      "\"preloaded_pt\":true,"
      "\"endpoints\":{"
      "\"default\": {"
      "\"default\":["
      "\"http://sdl.net/api\""
      "]"
      "}"
      "},"
      "\"exchange_after_x_ignition_cycles\": 40,"
      "\"exchange_after_x_kilometers\" : 2,"
      "\"exchange_after_x_days\" : 23,"
      "\"timeout_after_x_seconds\" : 20,"
      "\"seconds_between_retries\" : [10, 7, 5, 3, 1]"
      "},"
      "\"consumer_friendly_messages\":{"
      "\"version\":\"001.001.001\","
      "\"messages\":{} },"
      "\"functional_groupings\":{";

  CreateGroups(ofs);

  ofs << "}, \"app_policies\":{";

  CreateApps(ofs);

  ofs << "}";
}

TEST_F(PolicyManagerImplTest, StressTestOneCheck) {
  MockPolicyListener mock_listener;

  EXPECT_CALL(mock_listener, OnCurrentDeviceIdUpdateRequired("2")).Times(1);

  manager->set_listener(&mock_listener);
  ::policy::CheckPermissionResult output = manager->CheckPermissions("2",
                                                                     "FULL",
                                                                     "Func-1");
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest, StressTestNoPermission) {
  MockPolicyListener mock_listener;

  EXPECT_CALL(mock_listener, OnCurrentDeviceIdUpdateRequired("150")).Times(1);

  manager->set_listener(&mock_listener);
  ::policy::CheckPermissionResult output = manager->CheckPermissions(
      "150", "FULL", "Func-400");
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest, StressTestFewChecks) {
  MockPolicyListener mock_listener;

  EXPECT_CALL(mock_listener, OnCurrentDeviceIdUpdateRequired(_)).Times(100);
  manager->set_listener(&mock_listener);

  const int kNumberOfCheckings = 100;
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

    ::policy::CheckPermissionResult output = manager->CheckPermissions(
        app_number, "FULL", "Func-" + func_number);
    EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  }
}

}  // namespace policy
}  // namespace components
}  // namespace test

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
