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

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <sys/stat.h>

#include "gtest/gtest.h"
#include "policy/driver_dbms.h"
#include "policy/sql_pt_representation.h"
#include "policy/policy_types.h"
#include "policy/mock_policy_settings.h"
#include "policy/policy_table/types.h"
#include "policy/policy_table/enums.h"
#include "json/writer.h"
#include "json/reader.h"
#include "rpc_base/rpc_base.h"
#include "utils/file_system.h"
#include "utils/sqlite_wrapper/sql_database.h"
#include "sqlite_wrapper/sql_query.h"

using testing::ReturnRef;
using testing::NiceMock;

namespace test {
namespace components {
namespace policy_test {

class SQLPTRepresentationStorageTest : public ::testing::Test {
 protected:
  const std::string kAppStorageFolder =
      "storage_SQLPTRepresentationStorageTest";

  std::shared_ptr<utils::dbms::SQLQuery> query_wrapper_;
  std::shared_ptr<policy::SQLPTRepresentation> reps;
  std::shared_ptr<NiceMock<policy_handler_test::MockPolicySettings> >
      policy_settings_;

  void SetUp() OVERRIDE {
    file_system::CreateDirectory(kAppStorageFolder);

    reps = std::make_shared<policy::SQLPTRepresentation>();
    ASSERT_TRUE(reps != NULL);

    policy_settings_ =
        std::make_shared<NiceMock<policy_handler_test::MockPolicySettings> >();
    ON_CALL(*policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    ASSERT_EQ(policy::SUCCESS, reps->Init(policy_settings_.get()));

    query_wrapper_ = std::make_shared<utils::dbms::SQLQuery>(reps->db());
    ASSERT_TRUE(query_wrapper_ != NULL);
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(reps->Drop());
    EXPECT_TRUE(reps->Close());

    file_system::remove_directory_content(kAppStorageFolder);
    file_system::RemoveDirectory(kAppStorageFolder, true);
  }
};

TEST_F(
    SQLPTRepresentationStorageTest,
    CheckPermissionsAllowedWithoutParameters_SetLimitedPermissions_ExpectEmptyListOfAllowedParams) {
  // Arrange
  const std::string value_12345 = "12345";
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
      "DELETE FROM `rpc`; "
      "INSERT OR REPLACE INTO `rpc` (`name`, `hmi_level_value`,"
      " `functional_group_id`) VALUES ('Update', 'LIMITED', 1);";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query));

  // Act
  policy::CheckPermissionResult ret;
  reps->CheckPermissions(value_12345, "LIMITED", "Update", ret);

  // Assert
  EXPECT_TRUE(ret.hmi_level_permitted == ::policy::kRpcAllowed);
  EXPECT_TRUE(ret.list_of_allowed_params.empty());
}

}  // namespace policy_test
}  // namespace components
}  // namespace test
