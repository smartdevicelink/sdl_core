/*
 * Copyright (c) 2014, Ford Motor Company
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
#ifndef SRC_COMPONENTS_INCLUDE_TEST_POLICY_MOCK_POLICY_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_TEST_POLICY_MOCK_POLICY_SETTINGS_H_

#include "gmock/gmock.h"
#include <string>
#include "policy/policy_settings.h"

namespace test {
namespace components {
namespace policy_handler_test {

class MockPolicySettings
    : public ::policy::PolicySettings {
 public:
  MOCK_CONST_METHOD0(enable_policy, bool());
  MOCK_CONST_METHOD0(preloaded_pt_file, const std::string&());
  MOCK_CONST_METHOD0(app_storage_folder, const std::string&());
  MOCK_CONST_METHOD0(attempts_to_open_policy_db, uint16_t());
  MOCK_CONST_METHOD0(open_attempt_timeout_ms, uint16_t());
  MOCK_CONST_METHOD0(policies_snapshot_file_name, const std::string&());
  MOCK_CONST_METHOD0(system_files_path, const std::string&());
};

}  // namespace policy_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_POLICY_MOCK_POLICY_SETTINGS_H_
