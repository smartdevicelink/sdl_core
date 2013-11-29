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
#include "interfaces/MOBILE_API_schema.h"

namespace test {
namespace components {
namespace policies {
namespace policy_manager_generic_test {

using ::policies::CheckPermissionResult;
using ::NsSmartDeviceLink::NsSmartObjects::SmartObject;
namespace Strings = NsSmartDeviceLink::NsJSONHandler::strings;
namespace HMILevel = mobile_apis::HMILevel;
namespace func_ids = mobile_apis::FunctionID;
namespace msg_type = mobile_apis::messageType;


class PolicyManagerTest : public ::policies::PolicyManagerImpl {
  public:
    PolicyManagerTest() :
      PolicyManagerImpl() {}

    virtual ~PolicyManagerTest() {
    };
};

class TestWP1Integration: public ::testing::Test {
  private:
    ::policies::PolicyConfiguration policy_config_;

  protected:
    ::policies::PolicyManager *policy_manager_;

    TestWP1Integration()
      : Test()
      , policy_config_()
      , policy_manager_(0) {}

    virtual void SetUp() {
      policy_config_.set_pt_file_name("wp1_policy_table.json");
      policy_manager_ = new PolicyManagerTest();

      ::policies::InitResult init_result =
        policy_manager_->Init(policy_config_);

      ASSERT_EQ(::policies::INIT_OK, init_result);
    }

    virtual void TearDown() {
      delete policy_manager_;
      policy_manager_ = 0;
    }

    // ------------------------------------------------------------------------

    // String version. Pretend that the schema has already been unapplied.
    void AssertRPC(const std::string& rpc,
                   HMILevel::eType hmi_level,
                   bool allowed) {
      SmartObject rpc_obj;

      rpc_obj[Strings::S_PARAMS][Strings::S_FUNCTION_ID] = rpc;

      CheckPermissionResult result =
        policy_manager_->CheckPermission(1, rpc_obj, hmi_level);

      if (allowed) {
        ASSERT_EQ(::policies::PERMISSION_ALLOWED, result.result) <<
          "Failed at rpc: " << rpc;
      } else {
        ASSERT_EQ(::policies::PERMISSION_DISALLOWED, result.result) <<
          "Failed at rpc: " << rpc;
      }
      ASSERT_EQ(::policies::Priority::PRIORITY_NORMAL, result.priority);
    }

    // ------------------------------------------------------------------------

    void AssertRPC(func_ids::eType rpc_id,
                   msg_type::eType type,
                   HMILevel::eType hmi_level,
                   bool allowed) {
      static mobile_apis::MOBILE_API mobile_factory;
      SmartObject rpc_obj;

      rpc_obj[Strings::S_PARAMS][Strings::S_FUNCTION_ID] = rpc_id;
      rpc_obj[Strings::S_PARAMS][Strings::S_MESSAGE_TYPE] = type;

      mobile_factory.attachSchema(rpc_obj);

      CheckPermissionResult result =
        policy_manager_->CheckPermission(1, rpc_obj, hmi_level);

      if (allowed) {
        ASSERT_EQ(::policies::PERMISSION_ALLOWED, result.result) <<
          "Failed at rpc: " << rpc_id;
      } else {
        ASSERT_EQ(::policies::PERMISSION_DISALLOWED, result.result) <<
          "Failed at rpc: " << rpc_id;
      }
      ASSERT_EQ(::policies::Priority::PRIORITY_NORMAL, result.priority);
    }
};

// ----------------------------------------------------------------------------

TEST_F(TestWP1Integration, test_straight_forward_str_ok) {
  AssertRPC("GetDTCsID", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("ReadDIDID", HMILevel::HMI_FULL, true);
  AssertRPC("AlertManeuverID", HMILevel::HMI_LIMITED, true);
  AssertRPC("ChangeRegistrationID", HMILevel::HMI_FULL, true);
  AssertRPC("ChangeRegistrationID", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("ChangeRegistrationID", HMILevel::HMI_NONE, true);
  AssertRPC("ChangeRegistrationID", HMILevel::HMI_LIMITED, true);
  AssertRPC("PutFileID", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("ListFilesID", HMILevel::HMI_BACKGROUND, true);
  AssertRPC("OnSyncPDataID", HMILevel::HMI_BACKGROUND, true);
}

// ----------------------------------------------------------------------------

TEST_F(TestWP1Integration, test_straight_forward_str_deny) {
  AssertRPC("SpeakID", HMILevel::HMI_BACKGROUND, false);
  AssertRPC("DeleteSubMenuID", HMILevel::HMI_NONE, false);
  AssertRPC("OnButtonEventID", HMILevel::HMI_NONE, false);
  AssertRPC("UnknownRPCID", HMILevel::HMI_FULL, false);
  AssertRPC("OnCommandID", HMILevel::HMI_BACKGROUND, false);
  AssertRPC("DeleteSubMenuID", HMILevel::HMI_NONE, false);
  AssertRPC("AlertID", HMILevel::HMI_BACKGROUND, false);
  AssertRPC("DeleteInteractionChoiceSetID", HMILevel::HMI_NONE, false);
  AssertRPC("DeleteCommandID", HMILevel::HMI_NONE, false);
  AssertRPC("GetVehicleDataID", HMILevel::HMI_NONE, false);
}

// ----------------------------------------------------------------------------

TEST_F(TestWP1Integration, test_straight_forward_ok) {
  AssertRPC(func_ids::GetDTCsID, msg_type::request,
            HMILevel::HMI_BACKGROUND, true);

  AssertRPC(func_ids::GetDTCsID, msg_type::response,
            HMILevel::HMI_BACKGROUND, true);

  AssertRPC(func_ids::ReadDIDID, msg_type::request,
            HMILevel::HMI_FULL, true);

  AssertRPC(func_ids::AlertManeuverID, msg_type::response,
            HMILevel::HMI_LIMITED, true);

  AssertRPC(func_ids::ChangeRegistrationID, msg_type::request,
            HMILevel::HMI_FULL, true);

  AssertRPC(func_ids::ChangeRegistrationID, msg_type::response,
            HMILevel::HMI_BACKGROUND, true);

  AssertRPC(func_ids::ChangeRegistrationID, msg_type::request,
            HMILevel::HMI_NONE, true);

  AssertRPC(func_ids::ChangeRegistrationID, msg_type::response,
            HMILevel::HMI_LIMITED, true);

  AssertRPC(func_ids::PutFileID, msg_type::request,
            HMILevel::HMI_BACKGROUND, true);

  AssertRPC(func_ids::ListFilesID, msg_type::request,
            HMILevel::HMI_BACKGROUND, true);

  AssertRPC(func_ids::OnSyncPDataID, msg_type::notification,
            HMILevel::HMI_BACKGROUND, true);
}

// ----------------------------------------------------------------------------

TEST_F(TestWP1Integration, test_straight_forward_deny) {
  AssertRPC(func_ids::SpeakID, msg_type::request,
            HMILevel::HMI_BACKGROUND, false);

  AssertRPC(func_ids::DeleteSubMenuID, msg_type::request,
            HMILevel::HMI_NONE, false);

  AssertRPC(func_ids::OnButtonEventID, msg_type::notification,
            HMILevel::HMI_NONE, false);

  AssertRPC(static_cast<func_ids::eType>(37483), msg_type::request,
            HMILevel::HMI_FULL, false);          // unknown rpc

  AssertRPC(func_ids::OnCommandID, msg_type::request,
            HMILevel::HMI_BACKGROUND, false);

  AssertRPC(func_ids::DeleteSubMenuID, msg_type::response,
            HMILevel::HMI_NONE, false);

  AssertRPC(func_ids::AlertID, msg_type::request,
            HMILevel::HMI_BACKGROUND, false);

  AssertRPC(func_ids::DeleteInteractionChoiceSetID, msg_type::response,
            HMILevel::HMI_NONE, false);

  AssertRPC(func_ids::DeleteCommandID, msg_type::request,
            HMILevel::HMI_NONE, false);

  AssertRPC(func_ids::GetVehicleDataID, msg_type::request,
            HMILevel::HMI_NONE, false);
}

}  // namespace policy_manager_generic_test
}  // namespace policies
}  // namespace components
}  // namespace test

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

