/**
 * @file wp1_integration_test.cc
 * @brief Policy Manager test for first work package functionality. 
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
#include "policies/policy_manager.h"
#include "policies/policy_manager_impl.h"
#include "policies/policy_configuration.h"
#include "smart_objects/smart_object.h"
#include "formatters/CSmartFactory.hpp"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace policies {
namespace policy_manager_generic_test {

using ::NsSmartDeviceLink::policies::CheckPermissionResult;
using ::NsSmartDeviceLink::NsSmartObjects::SmartObject;
namespace Policies = NsSmartDeviceLink::policies;
namespace PermissionResult = NsSmartDeviceLink::policies::PermissionResult;
namespace Strings = NsSmartDeviceLink::NsJSONHandler::strings;
namespace HMILevel = mobile_apis::HMILevel;


class TestWP1Integration: public ::testing::Test {
  private:
    Policies::PolicyConfiguration policy_config_;

  protected:
    Policies::PolicyManager *policy_manager_;

    TestWP1Integration()
      : Test()
      , policy_config_()
      , policy_manager_(0) {}

    virtual void SetUp() {
      policy_config_.set_pt_file_name("wp1_policy_table.json");
      policy_manager_ = new Policies::PolicyManagerImpl(policy_config_);

      Policies::InitResult::eType init_result = policy_manager_->Init();
      ASSERT_EQ(Policies::InitResult::INIT_OK, init_result);
    }

    virtual void TearDown() {
      delete policy_manager_;
      policy_manager_ = 0;
    }

    void AssertRPC(const std::string& rpc,
                   HMILevel::eType hmi_level,
                   bool allowed) {
      SmartObject rpc_obj;

      rpc_obj[Strings::S_PARAMS][Strings::S_FUNCTION_ID] = rpc;

      CheckPermissionResult result =
        policy_manager_->CheckPermission(1, rpc_obj, hmi_level);

      if (allowed) {
        ASSERT_EQ(PermissionResult::PERMISSION_OK_ALLOWED, result.result);
      } else {
        ASSERT_EQ(PermissionResult::PERMISSION_DISALLOWED, result.result);
      }
      ASSERT_EQ(Policies::Priority::PRIORITY_NORMAL, result.priority);
    }
};

TEST_F(TestWP1Integration, test_straight_forward_ok) {
  AssertRPC("GetDTCs", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("ReadDID", HMILevel::HMI_FULL, true);
  AssertRPC("AlertManeuver", HMILevel::HMI_LIMITED, true);
  AssertRPC("ChangeRegistration", HMILevel::HMI_FULL, true);
  AssertRPC("ChangeRegistration", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("ChangeRegistration", HMILevel::HMI_NONE, true);
  AssertRPC("ChangeRegistration", HMILevel::HMI_LIMITED, true);
  AssertRPC("PutFile", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("ListFiles", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("OnSyncPData", HMILevel::HMI_BACKGROUND, true);
}

// ----------------------------------------------------------------------------

TEST_F(TestWP1Integration, test_straight_forward_deny) {
  AssertRPC("Speak", HMILevel::HMI_BACKGROUND, false);
  AssertRPC("DeleteSubMenu", HMILevel::HMI_NONE, false);
  AssertRPC("OnButtonEvent", HMILevel::HMI_NONE, false);
  AssertRPC("UnknownRPC", HMILevel::HMI_FULL, false);
  AssertRPC("OnCommand", HMILevel::HMI_BACKGROUND, false);
  AssertRPC("DeleteSubMenu", HMILevel::HMI_NONE, false);
  AssertRPC("Alert", HMILevel::HMI_BACKGROUND, false);
  AssertRPC("DeleteInteractionChoiceSet", HMILevel::HMI_NONE, false);
  AssertRPC("DeleteCommand", HMILevel::HMI_NONE, false);
  AssertRPC("GetVehicleData", HMILevel::HMI_NONE, false);
}

}  // namespace policy_manager_generic_test
}  // namespace policies
}  // namespace components
}  // namespace test

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

