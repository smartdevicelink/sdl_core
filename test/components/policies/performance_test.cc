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

#include <stdlib.h>
#include <time.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "policies/policy_manager.h"
#include "policies/policy_manager_impl.h"
#include "policies/policy_configuration.h"
#include "utils/file_system.h"
#include "smart_objects/smart_object.h"
#include "formatters/CSmartFactory.hpp"
#include "interfaces/MOBILE_API.h"
#include "interfaces/MOBILE_API_schema.h"

namespace test {
namespace components {
namespace policies {
namespace policy_manager_generic_test {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace Strings = NsSmartDeviceLink::NsJSONHandler::strings;
namespace HMILevel = mobile_apis::HMILevel;
namespace func_ids = mobile_apis::FunctionID;
namespace msg_type = mobile_apis::messageType;


class TestPerformance: public ::testing::Test {
  private:
    ::policies::PolicyConfiguration policy_config_;

  protected:
    ::policies::PolicyManager *policy_manager_;

    TestPerformance()
      : Test()
      , policy_config_()
      , policy_manager_(0) {}

    virtual void SetUp() {
      policy_config_.set_pt_file_name("wp1_policy_table.json");
      policy_manager_ = ::policies::PolicyManagerImpl::instance();

      ::policies::InitResult init_result =
          policy_manager_->Init(policy_config_);
      ASSERT_EQ(::policies::InitResult::INIT_OK, init_result);

      fillRPCList(rpc_ids_, sizeof(rpc_ids_)/sizeof(rpc_ids_[0]));
    }

    virtual void TearDown() { }


    int16_t CreateRPCListWithData(smart_objects::SmartObject *&rpc_list) {
      int16_t num = sizeof(rpc_ids_)/sizeof(rpc_ids_[0]);
      rpc_list = new smart_objects::SmartObject[num];

      for (int16_t i = 0; i < num; i++) {
        smart_objects::SmartObject& rpc = rpc_list[i];
        rpc[Strings::S_PARAMS][Strings::S_FUNCTION_ID] = rpc_ids_[i];
        rpc[Strings::S_PARAMS][Strings::S_MESSAGE_TYPE] = msg_type::request;

        GetRPCWithData(rpc_list[i]);
      }

      return num;
    }


  private:
    mobile_apis::FunctionID::eType rpc_ids_[1000];

    //-----------------------------------------------------------------------
    void fillRPCList(mobile_apis::FunctionID::eType rpcs[], int size) {
    //-----------------------------------------------------------------------
      srand(time(NULL));
      unsigned int seed = 25;
      for (int i = 0; i < size; i++) {
        rpcs[i] =
            static_cast<mobile_apis::FunctionID::eType>(rand_r(&seed) % 40 + 1);
      }
    }

    //-----------------------------------------------------------------------
    smart_objects::SmartObject& GetRPCWithData(
        smart_objects::SmartObject& rpc_obj) {
    //-----------------------------------------------------------------------
      static mobile_apis::MOBILE_API mobile_factory;

      mobile_factory.attachSchema(rpc_obj);

      rpc_obj[Strings::S_MSG_PARAMS]["number1"] = 100500;
      rpc_obj[Strings::S_MSG_PARAMS]["number2"] = 100500.2354;
      rpc_obj[Strings::S_MSG_PARAMS]["number3"] = 0.0;

      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][0] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][1] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][2] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][3] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][4] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][5] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][6] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][7] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][8] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT
      rpc_obj[Strings::S_MSG_PARAMS]["arraystrings"][9] =
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // NOLINT

      return rpc_obj;
    }
};

TEST_F(TestPerformance, test_straight_forward_checkout) {
  timespec start_time;
  timespec end_time;
  int16_t count_allowed = 0;
  int16_t count_disallowed = 0;
  double perform_time = 2.3;

  // smart_objects::SmartObject& rpc_obj = GetRPCWithData();

  smart_objects::SmartObject* rpc_objects = 0;
  int rpc_num = CreateRPCListWithData(rpc_objects);

  clock_gettime(CLOCK_REALTIME, &start_time);

  for (int16_t i = 0 ; i < rpc_num; i++) {
    ::policies::CheckPermissionResult result =
        policy_manager_->CheckPermission(1, rpc_objects[i], HMILevel::HMI_FULL);
    if (result.result == ::policies::PERMISSION_ALLOWED) {
      count_allowed++;
    } else if (result.result == ::policies::PERMISSION_DISALLOWED) {
      count_disallowed++;
    }
  }

  clock_gettime(CLOCK_REALTIME, &end_time);

  // in ms
  perform_time = static_cast<double>(end_time.tv_sec - start_time.tv_sec)*1000
      + static_cast<double>(end_time.tv_nsec - start_time.tv_nsec) * 1e-6;

  std::cout << "rpc_num: " << rpc_num << ", overall perform time: " <<
    perform_time << "(ms)" <<  ", one call(avg): "  <<
    perform_time/static_cast<double>(rpc_num) << "(ms)" << std::endl;
  std::cout << "count_allowed: " << count_allowed  <<
    ", count_disallowed: " << count_disallowed << std::endl;

  if (rpc_objects != NULL) {
    delete[] rpc_objects;
  }
}



TEST_F(TestPerformance, test_checkpermission_huge_rpc) {
  std::string input_json;
  if (true ==
      file_system::ReadFile("CreateInteractionChoiceSet.json", input_json)) {
    // TODO(anyone)
  } else {
    // FAIL() << "Failed to read \"CreateInteractionChoiceSet.json\" file.";
  }
}


}  // namespace policy_manager_generic_test
}  // namespace policies
}  // namespace components
}  // namespace test

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

