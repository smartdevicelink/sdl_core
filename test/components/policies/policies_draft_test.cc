/**
 * @file policy_draft_test.cc
 * @brief Policy table draft test source file.
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
#include "lib_messagebroker/md5.h"
#include "utils/file_system.h"
#include "policies/policy_manager_impl.h"
#include "policies/policy_table.h"
#include "policies/policy_configuration.h"

namespace test {
namespace components {
namespace policies {
namespace policies_draft_test {

  namespace pn = NsSmartDeviceLink::policies;
  namespace so_ns = NsSmartDeviceLink::NsSmartObjects;

  class PolicyManagerTest: public pn::PolicyManagerImpl {
  public:
    explicit PolicyManagerTest(const pn::PolicyConfiguration& policy_config):
      PolicyManagerImpl(policy_config) {
        init_result = Init();
      }
    pn::PolicyTable* getPolicyTable() {
      return PolicyManagerImpl::policy_table();
    }

    void StorePolicyTable() {
      PolicyManagerImpl::StorePolicyTable();
    }

    pn::InitResult::eType init_result;
  };

  class Policies_test: public ::testing::Test {
  protected:
    virtual void SetUp() {
      std::string buf;
      if (true == file_system::ReadFile("SDLPolicyTable_basic.json", buf)) {
        if (false == file_system::Write("PT4test.json",
             std::vector<unsigned char>(buf.begin(), buf.end()))) {
          FAIL() << "Failed to write \"PT4test.json\" file.";
        }
      } else {
        FAIL() << "Failed to read \"SDLPolicyTable_basic.json\" file.";
      }
    }
  };


  TEST_F(Policies_test, test_policies_modify_write_verify_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("PT4test.json");

    {
      PolicyManagerTest initial_PT(policy_config);
      so_ns::SmartObject& initial_obj(
        initial_PT.getPolicyTable()->AsSmartObject());

      ASSERT_TRUE(so_ns::SmartType_Null != initial_obj.getType());

      initial_obj["policy_table"]
                  ["module_config"]
                  ["endpoints"]
                  ["0x07"]
                  ["default"][0] = "ftp://127.0.0.1:1321";

      ASSERT_TRUE(initial_obj["policy_table"].erase("functional_groupings"));
      ASSERT_FALSE(initial_obj["policy_table"].erase("functional_groupings"));
    }

    {
      PolicyManagerTest modified_PT(policy_config);
      so_ns::SmartObject& modified_obj(
        modified_PT.getPolicyTable()->AsSmartObject());

      ASSERT_TRUE(so_ns::SmartType_Null != modified_obj.getType());
      ASSERT_EQ(modified_obj["policy_table"]
                            ["module_config"]
                            ["endpoints"]
                            ["0x07"]
                            ["default"][0].asString(),
                            "ftp://127.0.0.1:1321");
      ASSERT_FALSE(modified_obj["policy_table"].keyExists(
        "functional_groupings"));
    }
  }


  TEST_F(Policies_test, test_policies_read_write_compare_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("PT4test.json");

    std::string initial_json;
    std::string stored_json;

    {
      PolicyManagerTest policy_manager(policy_config);
      initial_json = policy_manager.getPolicyTable()->AsString();
    }

    {
      PolicyManagerTest policy_manager(policy_config);
      stored_json = policy_manager.getPolicyTable()->AsString();
    }

    ASSERT_EQ(md5(initial_json), md5(stored_json));
  }

  TEST_F(Policies_test, test_policies_no_PT_no_Preload_files_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("missing_file_for_sure.json");
    policy_config.set_preload_pt_file_name("missing_too.json");
    PolicyManagerTest policy_manager(policy_config);
    ASSERT_TRUE(NULL == policy_manager.getPolicyTable());
    ASSERT_EQ(pn::InitResult::INIT_FAILED_PRELOAD_NO_FILE,
              policy_manager.init_result);
  }

  TEST_F(Policies_test, test_policies_PT_bad_json_file_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("bad.json");
    policy_config.set_preload_pt_file_name("PT4test.json");
    PolicyManagerTest policy_manager(policy_config);

    ASSERT_EQ(pn::PTValidationResult::VALIDATION_FAILED_BAD_JSON,
              policy_manager.getPolicyTable()->Validate());
    ASSERT_EQ(pn::InitResult::INIT_OK, policy_manager.init_result);
  }

  TEST_F(Policies_test, test_policies_Preload_bad_json_file_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("missing_file.json");
    policy_config.set_preload_pt_file_name("bad.json");
    PolicyManagerTest policy_manager(policy_config);

    ASSERT_EQ(pn::PTValidationResult::VALIDATION_FAILED_BAD_JSON,
              policy_manager.getPolicyTable()->Validate());
    ASSERT_EQ(pn::InitResult::INIT_OK_PRELOAD, policy_manager.init_result);
  }

  TEST_F(Policies_test, test_policies_reInit_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("PT4test.json");
    policy_config.set_preload_pt_file_name("bad.json");
    PolicyManagerTest policy_manager(policy_config);

    ASSERT_EQ(pn::InitResult::INIT_OK, policy_manager.init_result);

    policy_config.set_pt_file_name("nofile");
    policy_config.set_preload_pt_file_name("nofile");

    policy_manager.Init();

    ASSERT_EQ(pn::InitResult::INIT_OK, policy_manager.init_result);
    ASSERT_FALSE(NULL == policy_manager.getPolicyTable());
  }

  TEST_F(Policies_test, test_policies_json_validate_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.set_pt_file_name("SDLPolicyTable_basic.json");
    PolicyManagerTest policy_manager(policy_config);

    ASSERT_TRUE(pn::PTValidationResult::VALIDATION_OK ==
      policy_manager.getPolicyTable()->Validate());

    policy_config.set_pt_file_name("Stored.json");
    policy_manager.StorePolicyTable();

    PolicyManagerTest policy_manager2(policy_config);

    ASSERT_TRUE(pn::PTValidationResult::VALIDATION_OK ==
      policy_manager2.getPolicyTable()->Validate());
  }


}  // namespace policies_draft_test
}  // namespace policies
}  // namespace components
}  // namespace test

int main(int argc, char **argv) {
    log4cxx::PropertyConfigurator::configure("log4cxx.properties");
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
