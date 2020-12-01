/*
 * Copyright (c) 2020, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POSTPONED_ACTIVATION_CONTROLLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POSTPONED_ACTIVATION_CONTROLLER_H_

#include "application.h"

namespace application_manager {

/**
 * @brief AppToActivateSet is a map of application ids expected to be
 * activated after the registration is completed (default hmi level is assigned)
 * and correlation_ids of the SDLActivateApp requests
 */
typedef std::map<uint32_t, uint32_t> AppToActivate;

class PostponedActivationController {
 public:
  PostponedActivationController();

  /**
   * @brief AddAppToActivate adds app_id to app_to_activate_ map
   * @param app_id id of the app that should be activated
   * @param corr_id correlation_id of the SDLActivateApp request
   */
  void AddAppToActivate(uint32_t app_id, uint32_t corr_id);

  /**
   * @brief GetPendingActivationCorrId gets the pending
   * activation correlation id
   * @param app_id application id
   * @return correlation id of the SDLActivateApp requests
   */
  uint32_t GetPendingActivationCorrId(uint32_t app_id) const;

  /**
   * @brief RemoveAppToActivate removes app_id from app_to_activate_ map
   * @param app_id application id
   */
  void RemoveAppToActivate(uint32_t app_id);

 private:
  AppToActivate app_to_activate_;
  mutable std::shared_ptr<sync_primitives::Lock> activate_app_list_lock_ptr_;
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POSTPONED_ACTIVATION_CONTROLLER_H_
