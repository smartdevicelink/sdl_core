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
#include "policy/sql_pt_ext_representation.h"
#include "policy/mock_policy_settings.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "sqlite_wrapper/sql_query.h"

#include <string>

using namespace ::policy;

using testing::ReturnRef;
using testing::NiceMock;

namespace test {
namespace components {
namespace policy_test {

class SQLPTExtRepresentationStorageTest : public ::testing::Test {
 public:
  SQLPTExtRepresentationStorageTest() : reps_(NULL) {}

 protected:
  const std::string kAppStorageFolder =
      "storage_SQLPTExtRepresentationStorageTest";

  std::shared_ptr<SQLPTExtRepresentation> reps_;
  std::shared_ptr<NiceMock<policy_handler_test::MockPolicySettings> >
      policy_settings_;
  std::shared_ptr<utils::dbms::SQLQuery> query_wrapper_;

  void SetUp() OVERRIDE {
    file_system::CreateDirectory(kAppStorageFolder);

    reps_ = std::make_shared<SQLPTExtRepresentation>();
    ASSERT_TRUE(reps_ != NULL);

    policy_settings_ =
        std::make_shared<NiceMock<policy_handler_test::MockPolicySettings> >();
    ON_CALL(*policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    ASSERT_EQ(SUCCESS, reps_->Init(policy_settings_.get()));

    query_wrapper_ = std::make_shared<utils::dbms::SQLQuery>(reps_->db());
    ASSERT_TRUE(query_wrapper_ != NULL);
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(reps_->Drop());
    EXPECT_TRUE(reps_->Close());

    file_system::remove_directory_content(kAppStorageFolder);
    file_system::RemoveDirectory(kAppStorageFolder, true);
  }
};

TEST_F(
    SQLPTExtRepresentationStorageTest,
    CanAppKeepContext_InsertKeepContext_ExpectValuesThatSetInKeepContextParams) {
  // Arrange
  const std::string query_delete = "DELETE FROM `application`; ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_delete));

  // Act
  const std::string value_12345 = "12345";
  const std::string query_insert_12345 =
      "INSERT INTO `application` (`id`, `memory_kb`,"
      " `heart_beat_timeout_ms`, `keep_context`) VALUES ('" +
      value_12345 + "', 5, 10, 1)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_12345));
  EXPECT_FALSE(reps_->CanAppKeepContext("0"));
  EXPECT_TRUE(reps_->CanAppKeepContext(value_12345));

  // Act
  const std::string value_123 = "123";
  const std::string query_insert_123 =
      "INSERT INTO `application` (`id`, `memory_kb`,"
      " `heart_beat_timeout_ms`, `keep_context`) VALUES ('" +
      value_123 + "', 10, 7, 0)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_123));
  EXPECT_FALSE(reps_->CanAppKeepContext(value_123));
}

}  // namespace policy_test
}  // namespace components
}  // namespace test
