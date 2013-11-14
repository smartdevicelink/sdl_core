/**
 * @file generic_policy_manager_test.cc
 * @brief PolicyManager generic test.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "policies/policy_manager_impl.h"
#include "policies/policy_configuration.h"
#include "smart_objects/smart_object.h"
#include "formatters/CSmartFactory.hpp"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace policies {
namespace policy_manager_generic_test {

using ::NsSmartDeviceLink::policies::PolicyConfiguration;
using ::NsSmartDeviceLink::policies::PolicyManagerImpl;
using ::NsSmartDeviceLink::policies::CheckPermissionResult;
using ::NsSmartDeviceLink::NsSmartObjects::SmartObject;
namespace InitResult = NsSmartDeviceLink::policies::InitResult;
namespace PermissionResult = NsSmartDeviceLink::policies::PermissionResult;
namespace Priority = NsSmartDeviceLink::policies::Priority;
namespace ns_str = NsSmartDeviceLink::NsJSONHandler::strings;


TEST(policy_manager_generic_test, test_straight_forward) {
  PolicyConfiguration config;
  config.setPTFileName("SDLPolicyTable_basic.json");

  PolicyManagerImpl policy_manager(config);

  InitResult::eType init_result = policy_manager.Init();
  ASSERT_EQ(InitResult::INIT_OK, init_result);

  SmartObject rpc;

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "GenericResponse";
  CheckPermissionResult result =
    policy_manager.checkPermission(123456,              // existing app
                                   rpc,
                                   mobile_apis::HMILevel::HMI_FULL);

  ASSERT_EQ(PermissionResult::PERMISSION_OK_ALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_NORMAL, result.priority);

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "Speak";
  result = policy_manager.checkPermission(1,            // non existing app
                                          rpc,
                                          mobile_apis::HMILevel::HMI_FULL);

  ASSERT_EQ(PermissionResult::PERMISSION_OK_ALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_EMERGENCY, result.priority);

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "Alert";
  result = policy_manager.checkPermission(789,            // existing app
                                          rpc,
                                          mobile_apis::HMILevel::HMI_BACKGROUND);

  ASSERT_EQ(PermissionResult::PERMISSION_OK_ALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_EMERGENCY, result.priority);
}

// ----------------------------------------------------------------------------

TEST(policy_manager_generic_test, test_straight_forward_deny) {
  PolicyConfiguration config;
  config.setPTFileName("SDLPolicyTable_basic.json");

  PolicyManagerImpl policy_manager(config);

  InitResult::eType init_result = policy_manager.Init();
  ASSERT_EQ(InitResult::INIT_OK, init_result);

  SmartObject rpc;

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "Alert";
  CheckPermissionResult result =
    policy_manager.checkPermission(123456,              // existing app
                                   rpc,
                                   mobile_apis::HMILevel::HMI_BACKGROUND);

  ASSERT_EQ(PermissionResult::PERMISSION_NOK_DISALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_NORMAL, result.priority); 

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "Alert";
  result = policy_manager.checkPermission(123456,              // existing app
                                          rpc,
                                          mobile_apis::HMILevel::HMI_FULL);

  ASSERT_EQ(PermissionResult::PERMISSION_OK_ALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_NORMAL, result.priority);

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "GetVehicleData";
  result = policy_manager.checkPermission(234234,          // non existing app
                                          rpc,
                                          mobile_apis::HMILevel::HMI_FULL);

  ASSERT_EQ(PermissionResult::PERMISSION_NOK_DISALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_NONE, result.priority);

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "UnknownRPC";
  result = policy_manager.checkPermission(234234,          // non existing app
                                          rpc,
                                          mobile_apis::HMILevel::HMI_FULL);

  ASSERT_EQ(PermissionResult::PERMISSION_NOK_DISALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_NONE, result.priority);

  rpc[ns_str::S_PARAMS][ns_str::S_FUNCTION_ID] = "SubscribeVehicleData";
  result = policy_manager.checkPermission(123456,              // existing app
                                          rpc,
                                          mobile_apis::HMILevel::HMI_NONE);

  ASSERT_EQ(PermissionResult::PERMISSION_NOK_DISALLOWED, result.result);
  // TODO(YS): Fix enum handling
  //ASSERT_EQ(Priority::PRIORITY_NORMAL, result.priority);
}

}  // namespace policy_manager_generic_test
}  // namespace policies
}  // namespace components
}  // namespace test

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

