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
#include "policies/policy_manager.h"
#include "policies/policy_table.h"
#include "policies/policy_configuration.h"

namespace test {
namespace components {
namespace policies {
namespace policies_draft_test {
  
  namespace pn = NsSmartDeviceLink::policies;
  namespace so_ns = NsSmartDeviceLink::NsSmartObjects;
 
  class PolicyManagerTest: public pn::PolicyManager {
  public:
    PolicyManagerTest(const pn::PolicyConfiguration& policy_config):
      PolicyManager(policy_config) {
	Init();
      }
    pn::PolicyTable* getPolicyTable() {
      return PolicyManager::getPolicyTable();
    }
  };
 
  TEST(policies_test, test_policies_no_json_file_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.setPTFileName("missing_file_for_sure.json");
    PolicyManagerTest policy_manager(policy_config);
    ASSERT_TRUE(0 == policy_manager.getPolicyTable());
  }

  TEST(policies_test, test_policies_bad_json_file_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.setPTFileName("bad.json");
    PolicyManagerTest policy_manager(policy_config);
    
    ASSERT_EQ(pn::PTValidationResult::VALIDATION_FAILED_BAD_JSON, 
	      policy_manager.getPolicyTable()->Validate());
  }
  
  TEST(policies_test, test_policies_no_schema_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.setPTFileName("SDLPolicyTable_basic.json");
    PolicyManagerTest policy_manager(policy_config);
    
    ASSERT_EQ(pn::PTValidationResult::VALIDATION_FAILED_NO_SCHEMA, 
	      policy_manager.getPolicyTable()->Validate());
  }
  
  TEST(policies_test, test_policies_json_validate_test) {
    pn::PolicyConfiguration policy_config;
    policy_config.setPTFileName("SDLPolicyTable_basic.json");
    PolicyManagerTest policy_manager(policy_config);
    
    //TODO: set schema
    
    ASSERT_TRUE(pn::PTValidationResult::VALIDATION_OK ==
      policy_manager.getPolicyTable()->Validate());
    
    so_ns::SmartObject initial_obj(
      policy_manager.getPolicyTable()->AsSmartObject());
    
    policy_config.setPTFileName("Stored.json");
    policy_manager.StorePolicyTable();
    
    PolicyManagerTest policy_manager2(policy_config);
    
    //TODO: set schema
    
    ASSERT_TRUE(pn::PTValidationResult::VALIDATION_OK ==
      policy_manager2.getPolicyTable()->Validate());
    
    so_ns::SmartObject stored_obj(
      policy_manager2.getPolicyTable()->AsSmartObject());
      
    //TODO: compare smart objects.
    
    //TODO: implement files comapre???
  }
  
  
} // namespace policies_draft_test
} // namespace policies
} // namespace components
} // namespace test

int main(int argc, char **argv) {
    log4cxx::PropertyConfigurator::configure("log4cxx.properties");  
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
