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

#ifndef SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_INTERFACE_H_
#define SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_INTERFACE_H_

#include "utils/shared_ptr.h"
#include "policy/policy_types.h"

namespace policy {

class PolicyListener;

class UpdateStatusManagerInterface {
 public:
  virtual ~UpdateStatusManagerInterface() {
  }
  /**
   * @brief Sets listener pointer
   * @param listener Pointer to policy listener implementation
   */
  virtual void set_listener(PolicyListener* listener) = 0;

  /**
   * @brief Update status hanlder for PTS sending out
   * @param update_timeout Timeout for waiting of incoming PTU
   */
  virtual void OnUpdateSentOut(uint32_t update_timeout) = 0;

  /**
   * @brief Update status handler for PTU waiting timeout
   */
  virtual void OnUpdateTimeoutOccurs() = 0;

  /**
   * @brief Update status handler for valid PTU receiving
   */
  virtual void OnValidUpdateReceived() = 0;

  /**
   * @brief Update status handler for wrong PTU receiving
   */
  virtual void OnWrongUpdateReceived() = 0;

  /**
   * @brief Update status handler for reset PT to default state
   * @param is_update_required Update necessity flag
   */
  virtual void OnResetDefaultPT(bool is_update_required) = 0;

  /**
   * @brief Update status handler for restarting retry sequence
   */
  virtual void OnResetRetrySequence() = 0;

  /**
   * @brief Update status handler on new application registering
   */
  virtual void OnNewApplicationAdded() = 0;

  /**
   * @brief Update status handler for policy initialization
   * @param is_update_required Update necessity flag
   */
  virtual void OnPolicyInit(bool is_update_required) = 0;

  /**
   * @brief Returns current policy update status
   * @return
   */
  virtual PolicyTableStatus GetUpdateStatus() = 0;
};

typedef utils::SharedPtr<UpdateStatusManagerInterface> UpdateStatusManagerInterfaceSPtr;

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_INTERFACE_H_
