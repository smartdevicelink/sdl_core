/*
 * Copyright (c) 2019, Ford Motor Company
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

#include "application_manager/rpc_handler_impl.h"
#include <memory>
#include "application_manager/mock_application_manager.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace application_manager_test {

using ::test::components::application_manager_test::MockApplicationManager;
using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using namespace ::smart_objects;
using namespace application_manager::rpc_handler;

class RPCHandlerImplTest : public ::testing::Test {
 public:
  RPCHandlerImplTest()
      : rpc_handler_(new RPCHandlerImpl(
            mock_app_mngr_, hmi_so_factory_, mobile_so_factory_)) {}

 protected:
  std::unique_ptr<RPCHandlerImpl> rpc_handler_;
  hmi_apis::HMI_API hmi_so_factory_;
  mobile_apis::MOBILE_API mobile_so_factory_;
  NiceMock<MockApplicationManager> mock_app_mngr_;
};

TEST_F(RPCHandlerImplTest, GetMessageVersion_SUCCESS) {
  namespace json_str = ns_smart_device_link::ns_json_handler::strings;
  namespace app_str = application_manager::strings;

  std::vector<utils::SemanticVersion> test_versions = {
      utils::SemanticVersion("2.5.0"),
      utils::SemanticVersion("3.0.0"),
      utils::SemanticVersion("5.0.0"),
      utils::SemanticVersion("6.1.0")};

  std::vector<utils::SemanticVersion> expected_versions = {
      utils::base_rpc_version,
      utils::base_rpc_version,
      utils::SemanticVersion("5.0.0"),
      utils::SemanticVersion("6.1.0")};

  SmartObject message;
  message[json_str::S_MSG_PARAMS] = SmartObject(SmartType_Map);
  message[json_str::S_MSG_PARAMS][app_str::sync_msg_version] =
      SmartObject(SmartType_Map);

  for (size_t i = 0; i < test_versions.size(); ++i) {
    message[json_str::S_MSG_PARAMS][app_str::sync_msg_version]
           [app_str::major_version] = test_versions[i].major_version_;
    message[json_str::S_MSG_PARAMS][app_str::sync_msg_version]
           [app_str::minor_version] = test_versions[i].minor_version_;
    message[json_str::S_MSG_PARAMS][app_str::sync_msg_version]
           [app_str::patch_version] = test_versions[i].patch_version_;

    utils::SemanticVersion result_message_version;
    rpc_handler_->GetMessageVersion(message, result_message_version);
    EXPECT_EQ(expected_versions[i], result_message_version);
  }
}

TEST_F(RPCHandlerImplTest, GetMessageVersion_InvalidVersion) {
  namespace json_str = ns_smart_device_link::ns_json_handler::strings;
  namespace app_str = application_manager::strings;

  SmartObject message;
  message[json_str::S_MSG_PARAMS] = SmartObject(SmartType_Map);
  message[json_str::S_MSG_PARAMS][app_str::sync_msg_version] =
      SmartObject(SmartType_Map);
  message[json_str::S_MSG_PARAMS][app_str::sync_msg_version]
         [app_str::major_version] = 0;
  message[json_str::S_MSG_PARAMS][app_str::sync_msg_version]
         [app_str::minor_version] = 0;
  message[json_str::S_MSG_PARAMS][app_str::sync_msg_version]
         [app_str::patch_version] = 0;

  utils::SemanticVersion expected_version("0.0.0");

  utils::SemanticVersion result_message_version;
  rpc_handler_->GetMessageVersion(message, result_message_version);
  EXPECT_EQ(expected_version, result_message_version);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
