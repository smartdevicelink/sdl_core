/**
 * @file policy_table_test_fixture.h
 * @brief Policy table test helper class.
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

#ifndef TEST_COMPONENTS_POLICIES_SCHEMA_POLICY_TABLE_TEST_FIXTURE_H_
#define TEST_COMPONENTS_POLICIES_SCHEMA_POLICY_TABLE_TEST_FIXTURE_H_

#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "policies/policy_table_schema.h"
#include "utils/file_system.h"
#include "formatters/generic_json_formatter.h"
#include "smart_objects/smart_schema.h"

namespace test {
namespace components {
namespace policies {
namespace policy_table_schema_test {

using ::NsSmartDeviceLink::NsSmartObjects::SmartObject;
using ::NsSmartDeviceLink::NsJSONHandler::Formatters::GenericJsonFormatter;

class SchemaTest: public ::testing::Test {
 protected:
    void GetPolicyTable(const std::string &file_path, SmartObject &pt) {
      std::string pt_string;

      if (true == file_system::ReadFile(file_path, pt_string)) {
        if (false == GenericJsonFormatter::FromString(pt_string, pt)) {
          FAIL() << "Failed to make a smart object";
        }
      } else {
        FAIL() << "Failed to read a file";
      }
    }
};

}  // namespace policy_table_schema_test
}  // namespace policies
}  // namespace components
}  // namespace test

#endif  // TEST_COMPONENTS_POLICIES_SCHEMA_POLICY_TABLE_TEST_FIXTURE_H_

