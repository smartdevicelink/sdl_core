/* Copyright (c) 2013, Ford Motor Company
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

#include <fstream>

#include "gtest/gtest.h"

#include "json/reader.h"
#include "json/value.h"
#include "policy/policy_table/enums.h"
#include "policy/policy_table/types.h"
#include "rpc_base/gtest_support.h"
#include "rpc_base/rpc_base.h"

using rpc::Integer;
using rpc::policy_table_interface_base::Table;

namespace test {
namespace components {
namespace policy_test {

TEST(PolicyGeneratedCodeTest, TestValidPTPreloadJsonIsValid) {
  std::ifstream json_file("json/sdl_preloaded_pt.json");
  ASSERT_TRUE(json_file.is_open());
  Json::Value valid_table;
  Json::Reader reader;
  ASSERT_TRUE(reader.parse(json_file, valid_table));
  Table table(&valid_table);
  table.SetPolicyTableType(rpc::policy_table_interface_base::PT_PRELOADED);
  ASSERT_RPCTYPE_VALID(table);
}

TEST(PolicyGeneratedCodeTest, TestValidPTUpdateJsonIsValid) {
  std::ifstream json_file("json/valid_sdl_pt_update.json");
  ASSERT_TRUE(json_file.is_open());
  Json::Value valid_table;
  Json::Reader reader;
  ASSERT_TRUE(reader.parse(json_file, valid_table));
  Table table(&valid_table);
  table.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  ASSERT_RPCTYPE_VALID(table);
}

TEST(PolicyGeneratedCodeTest, IntegerLimitsTest) {
  const int32_t min_value = -5;
  const int32_t max_value = 5;

  Integer<int32_t, min_value, max_value> value;
  EXPECT_FALSE(value.is_valid());

  value = min_value;
  EXPECT_TRUE(value.is_valid());

  value = max_value;
  EXPECT_TRUE(value.is_valid());

  value = min_value + max_value;
  EXPECT_TRUE(value.is_valid());

  value = min_value - 1;
  EXPECT_FALSE(value.is_valid());

  value = max_value + 1;
  EXPECT_FALSE(value.is_valid());
}

TEST(PolicyGeneratedCodeTest, IntegerConstructionAndAssignmentTest) {
  const int32_t min_value = -5;
  const int32_t max_value = 5;

  const Json::Value json_null = Json::Value(Json::nullValue);
  Integer<int32_t, min_value, max_value> value_from_json_null(&json_null);
  EXPECT_FALSE(value_from_json_null.is_valid());

  const Json::Value json_string = Json::Value("string");
  Integer<int32_t, min_value, max_value> value_from_json_string(&json_string);
  EXPECT_FALSE(value_from_json_string.is_valid());

  const Json::Value json_float = Json::Value(10.5);
  Integer<int32_t, min_value, max_value> value_from_json_float(&json_float);
  EXPECT_FALSE(value_from_json_float.is_valid());

  const Json::Value json_int_in_range = Json::Value(4);
  Integer<int32_t, min_value, max_value> value_from_json_int_in_range(
      &json_int_in_range);
  EXPECT_TRUE(value_from_json_int_in_range.is_valid());

  const Json::Value json_int_out_of_range = Json::Value(9);
  Integer<int32_t, min_value, max_value> value_from_json_int_out_of_range(
      &json_int_out_of_range);
  EXPECT_FALSE(value_from_json_int_out_of_range.is_valid());

  const Json::Value json_zero = Json::Value(0);
  Integer<int32_t, min_value, max_value> value_from_json_zero(&json_zero);
  EXPECT_TRUE(value_from_json_zero.is_valid());

  Integer<int32_t, min_value, max_value> invalid_value(&json_string);
  EXPECT_FALSE(invalid_value.is_valid());
  Integer<int32_t, min_value, max_value> reassigned_value = invalid_value;
  EXPECT_FALSE(reassigned_value.is_valid());

  Integer<int32_t, min_value, max_value> another_invalid_value(&json_string);
  EXPECT_FALSE(another_invalid_value.is_valid());
  Integer<int32_t, min_value, max_value> valid_value(0);
  EXPECT_TRUE(valid_value.is_valid());
  valid_value = another_invalid_value;
  EXPECT_FALSE(valid_value.is_valid());
}

TEST(PolicyGeneratedCodeTest, TestConsentsGroup_Validation) {
  using namespace rpc::policy_table_interface_base;
  // Need to validate CCS consents container wrapped with Optional for case w/o
  // specific values assigned since it is of generic 'Map' type which requires
  // to be empty + initialized in order to be valid
  // Also Map type does not have specific validation on PT type

  rpc::Optional<ConsentGroups> consent_groups;

  EXPECT_TRUE(consent_groups.is_valid());

  consent_groups->insert(
      std::make_pair(std::string("Group1"), rpc::Boolean(true)));

  EXPECT_TRUE(consent_groups.is_valid());

  // Adds more than container maximum size
  for (size_t number = 0; number < 256; ++number) {
    std::stringstream name;
    name << "Group" << number;
    consent_groups->insert(std::make_pair(name.str(), rpc::Boolean(true)));
  }

  EXPECT_FALSE(consent_groups.is_valid());
}

TEST(PolicyGeneratedCodeTest,
     TestConsentRecords_ExternalConsents_PT_Validation) {
  using namespace rpc::policy_table_interface_base;
  ConsentRecords consent_records;

  // PT_SNAPSHOT does not care of consents since their type is 'optional'
  // PT_UPDATE and PT_SNAPSHOT have consents as 'omitted' so they must be absent
  consent_records.SetPolicyTableType(PT_UPDATE);
  EXPECT_TRUE(consent_records.is_valid());

  consent_records.SetPolicyTableType(PT_PRELOADED);
  EXPECT_TRUE(consent_records.is_valid());

  consent_records.SetPolicyTableType(PT_SNAPSHOT);
  EXPECT_TRUE(consent_records.is_valid());

  consent_records.external_consent_status_groups->insert(
      std::make_pair(std::string("Group1"), true));

  consent_records.external_consent_status_groups->insert(
      std::make_pair(std::string("Group2"), false));

  consent_records.SetPolicyTableType(PT_UPDATE);
  EXPECT_FALSE(consent_records.is_valid());

  consent_records.SetPolicyTableType(PT_PRELOADED);
  EXPECT_FALSE(consent_records.is_valid());

  consent_records.SetPolicyTableType(PT_SNAPSHOT);
  EXPECT_TRUE(consent_records.is_valid());
}

TEST(PolicyGeneratedCodeTest, TestCCSEntities_PT_Validation) {
  using namespace rpc::policy_table_interface_base;
  rpc::Optional<DisallowedByExternalConsentEntities>
      disallowed_by_external_consent_entities_on;

  // PT_SNAPSHOT, PT_UPDATE, PT_SNAPSHOT do not care of CCS entities since their
  // type is 'optional'
  disallowed_by_external_consent_entities_on.SetPolicyTableType(PT_UPDATE);
  EXPECT_TRUE(disallowed_by_external_consent_entities_on.is_valid());

  disallowed_by_external_consent_entities_on.SetPolicyTableType(PT_PRELOADED);
  EXPECT_TRUE(disallowed_by_external_consent_entities_on.is_valid());

  disallowed_by_external_consent_entities_on.SetPolicyTableType(PT_SNAPSHOT);
  EXPECT_TRUE(disallowed_by_external_consent_entities_on.is_valid());

  disallowed_by_external_consent_entities_on->push_back(
      ExternalConsentEntity(0, 0));
  disallowed_by_external_consent_entities_on->push_back(
      ExternalConsentEntity(1, 1));

  disallowed_by_external_consent_entities_on.SetPolicyTableType(PT_UPDATE);
  EXPECT_TRUE(disallowed_by_external_consent_entities_on.is_valid());

  disallowed_by_external_consent_entities_on.SetPolicyTableType(PT_PRELOADED);
  EXPECT_TRUE(disallowed_by_external_consent_entities_on.is_valid());

  disallowed_by_external_consent_entities_on.SetPolicyTableType(PT_SNAPSHOT);
  EXPECT_TRUE(disallowed_by_external_consent_entities_on.is_valid());
}

}  // namespace policy_test
}  // namespace components
}  // namespace test
