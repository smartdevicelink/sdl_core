/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_TEST_INCLUDE_MOCK_RESOURCE_ALLOCATION_MANAGER_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_TEST_INCLUDE_MOCK_RESOURCE_ALLOCATION_MANAGER_H_

#include "gmock/gmock.h"
#include "application_manager/rpc_plugins/rc_rpc_plugin/include/rc_rpc_plugin/resource_allocation_manager.h"

namespace rc_rpc_plugin_test {

class MockResourceAllocationManager
    : public rc_rpc_plugin::ResourceAllocationManager {
 public:
  MOCK_METHOD2(AcquireResource,
               rc_rpc_plugin::AcquireResult::eType(
                   const std::string& module_type, const uint32_t app_id));
  MOCK_METHOD2(ForceAcquireResource,
               void(const std::string& module_type, const uint32_t app_id));
  MOCK_METHOD2(OnDriverDisallowed,
               void(const std::string& module_type, const uint32_t app_id));
  MOCK_METHOD2(OnApplicationEvent,
               void(application_manager::plugin_manager::ApplicationEvent event,
                    application_manager::ApplicationSharedPtr application));
  MOCK_METHOD1(OnPolicyEvent,
               void(application_manager::plugin_manager::PolicyEvent event));
  MOCK_METHOD1(SetAccessMode,
               void(const hmi_apis::Common_RCAccessMode::eType access_mode));
  MOCK_CONST_METHOD0(GetAccessMode, hmi_apis::Common_RCAccessMode::eType());
  MOCK_METHOD3(SetResourceState,
               void(const std::string& module_type,
                    const uint32_t app_id,
                    const rc_rpc_plugin::ResourceState::eType state));
  MOCK_CONST_METHOD1(IsResourceFree, bool(const std::string& module_type));
  MOCK_METHOD0(ResetAllAllocations, void());
};

}  // namespace rc_rpc_plugin_test

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_TEST_INCLUDE_MOCK_RESOURCE_ALLOCATION_MANAGER_H_
