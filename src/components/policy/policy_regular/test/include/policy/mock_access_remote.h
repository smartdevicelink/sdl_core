/*
 * Copyright (c) 2015, Ford Motor Company
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
#ifndef SRC_COMPONENTS_POLICY_TEST_INCLUDE_MOCK_ACCESS_REMOTE_H_
#define SRC_COMPONENTS_POLICY_TEST_INCLUDE_MOCK_ACCESS_REMOTE_H_

#include "gmock/gmock.h"
#include "policy/access_remote.h"

namespace test {
namespace components {
namespace access_remote_test {

class MockSubject : public policy::Subject {
 public:
};

class MockObject : public policy::Object {
 public:
};

class MockAccessRemote : public policy::AccessRemote {
 public:
  MOCK_METHOD0(Init, void());
  MOCK_METHOD0(Enable, void());
  MOCK_METHOD0(Disable, void());
  MOCK_CONST_METHOD0(IsEnabled, bool());
  MOCK_CONST_METHOD1(IsPrimaryDevice, bool(const policy::PTString& dev_id));
  MOCK_METHOD1(SetPrimaryDevice, void(const policy::PTString& dev_id));
  MOCK_CONST_METHOD0(PrimaryDevice, policy::PTString());
  MOCK_CONST_METHOD2(IsPassengerZone,
                     bool(const policy::SeatLocation& seat,
                          const policy::SeatLocation& zone));
  MOCK_METHOD2(Allow,
               void(const policy::Subject& who, const policy::Object& what));
  MOCK_METHOD2(Deny,
               void(const policy::Subject& who, const policy::Object& what));
  MOCK_METHOD1(Reset, void(const policy::Subject& who));
  MOCK_METHOD1(Reset, void(const policy::Object& what));
  MOCK_CONST_METHOD2(Check,
                     policy::TypeAccess(const policy::Subject& who,
                                        const policy::Object& what));
  MOCK_CONST_METHOD3(
      FindGroup,
      policy::PTString(const policy::Subject& who,
                       const policy::PTString& rpc,
                       const policy::RemoteControlParams& params));
  MOCK_METHOD2(SetDefaultHmiTypes,
               void(const policy::Subject& who,
                    const std::vector<int>& hmi_types));
  MOCK_METHOD1(GetGroups,
               const policy_table::Strings&(const policy::Subject& who));
  MOCK_METHOD3(GetPermissionsForApp,
               bool(const std::string& device_id,
                    const std::string& app_id,
                    policy::FunctionalIdType& group_types));
  MOCK_CONST_METHOD2(CheckModuleType,
                     bool(const policy::PTString& app_id,
                          policy_table::ModuleType module));
  MOCK_CONST_METHOD3(
      CheckParameters,
      policy::TypeAccess(const policy::Object& what,
                         const std::string& rpc,
                         const policy::RemoteControlParams& params));
  MOCK_METHOD1(IsAppReverse, bool(const policy::Subject& who));
  MOCK_METHOD0(Reset, void());
  MOCK_CONST_METHOD1(GetDeviceZone,
                     const policy::SeatLocation*(const std::string& device_id));
  MOCK_METHOD2(SetDeviceZone,
               void(const std::string& device_id,
                    const policy::SeatLocation& zone));
  MOCK_METHOD2(GetModuleTypes,
               bool(const std::string& application_id,
                    std::vector<std::string>* modules));
};

}  // namespace access_remote_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_POLICY_TEST_INCLUDE_MOCK_ACCESS_REMOTE_H_
