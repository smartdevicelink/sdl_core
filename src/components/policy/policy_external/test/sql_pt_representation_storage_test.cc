/* Copyright (c) 2018, Ford Motor Company
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
#include "policy/sql_pt_representation.h"
#include "utils/file_system.h"
#include "policy/sql_wrapper.h"
#include "policy/mock_policy_settings.h"

#include <string>
#include <memory>

using testing::ReturnRef;
using testing::NiceMock;

namespace test {
namespace components {
namespace policy_test {

class SQLPTRepresentationStorageTest : public ::testing::Test {
 protected:
  const std::string kAppStorageFolder =
      "storage_SQLPTRepresentationStorageTest";

  std::shared_ptr<policy::SQLPTRepresentation> reps;
  std::shared_ptr<utils::dbms::SQLQuery> query_wrapper_;
  std::shared_ptr<policy_handler_test::MockPolicySettings> policy_settings_;

  void SetUp() {
    file_system::CreateDirectory(kAppStorageFolder);

    reps = std::make_shared<policy::SQLPTRepresentation>();
    ASSERT_TRUE(reps != NULL);

    policy_settings_ =
        std::make_shared<NiceMock<policy_handler_test::MockPolicySettings> >();
    ON_CALL(*policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    EXPECT_EQ(::policy::SUCCESS, reps->Init(policy_settings_.get()));

    query_wrapper_ = std::make_shared<utils::dbms::SQLQuery>(reps->db());
    ASSERT_TRUE(query_wrapper_ != NULL);
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(reps->Drop());
    EXPECT_TRUE(reps->Close());
    reps->RemoveDB();

    file_system::remove_directory_content(kAppStorageFolder);
    file_system::RemoveDirectory(kAppStorageFolder, true);
  }
};

TEST_F(
    SQLPTRepresentationStorageTest,
    CheckPermissionsAllowed_SetValuesInAppGroupRpcFunctionalGroup_GetEqualParamsInCheckPermissionResult) {
  // Arrange
  const std::string value_12345 = "12345";
  const std::string gps = "gps";
  const std::string speed = "speed";
  const std::string query =
      "INSERT OR REPLACE INTO `application` (`id`, `memory_kb`,"
      " `heart_beat_timeout_ms`) VALUES ('" +
      value_12345 +
      "', 5, 10); "
      "INSERT OR REPLACE INTO functional_group (`id`, `name`)"
      "  VALUES (1, 'Base-4'); "
      "INSERT OR REPLACE INTO `app_group` (`application_id`,"
      " `functional_group_id`) VALUES ('" +
      value_12345 +
      "', 1); "
      "INSERT OR REPLACE INTO `rpc` (`name`, `parameter`, `hmi_level_value`,"
      " `functional_group_id`) VALUES ('Update', '" +
      gps +
      "', 'FULL', 1); "
      "INSERT OR REPLACE INTO `rpc` (`name`, `parameter`, `hmi_level_value`,"
      " `functional_group_id`) VALUES ('Update', '" +
      speed + "', 'FULL', 1);";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query));

  // Act
  policy::CheckPermissionResult ret;
  reps->CheckPermissions(value_12345, "FULL", "Update", ret);

  // Assert
  EXPECT_TRUE(ret.hmi_level_permitted == ::policy::kRpcAllowed);
  ASSERT_EQ(2u, ret.list_of_allowed_params.size());
  EXPECT_TRUE(ret.list_of_allowed_params.end() !=
              ret.list_of_allowed_params.find(gps));
  EXPECT_TRUE(ret.list_of_allowed_params.end() !=
              ret.list_of_allowed_params.find(speed));
}

}  // namespace policy_test
}  // namespace components
}  // namespace test
