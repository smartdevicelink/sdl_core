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
#ifndef SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_UPDATE_STATUS_MANAGER_H_
#define SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_UPDATE_STATUS_MANAGER_H_

#include "gmock/gmock.h"

#include "policy/update_status_manager.h"

namespace policy {

class MockUpdateStatusManager : public UpdateStatusManager {
 public:
  MOCK_METHOD1(set_listener,
      void(PolicyListener* listener));
  MOCK_METHOD1(OnUpdateSentOut,
      void(uint32_t update_timeout));
  MOCK_METHOD0(OnUpdateTimeoutOccurs,
      void());
  MOCK_METHOD0(OnValidUpdateReceived,
      void());
  MOCK_METHOD0(OnWrongUpdateReceived,
      void());
  MOCK_METHOD1(OnResetDefaultPT,
      void(bool is_update_required));
  MOCK_METHOD0(OnResetRetrySequence,
      void());
  MOCK_METHOD0(OnNewApplicationAdded,
      void());
  MOCK_METHOD1(OnPolicyInit,
      void(bool is_update_required));
  MOCK_METHOD0(GetUpdateStatus,
      PolicyTableStatus());
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_UPDATE_STATUS_MANAGER_H_
