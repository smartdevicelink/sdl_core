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

#include "application_manager/postponed_activation_controller.h"

namespace application_manager {

SDL_CREATE_LOG_VARIABLE("StateControllerImpl")

PostponedActivationController::PostponedActivationController()
    : activate_app_list_lock_ptr_(std::make_shared<sync_primitives::Lock>()) {}

void PostponedActivationController::AddAppToActivate(uint32_t app_id,
                                                     uint32_t corr_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(activate_app_list_lock_ptr_);
  app_to_activate_.insert(std::pair<uint32_t, uint32_t>(app_id, corr_id));
}

uint32_t PostponedActivationController::GetPendingActivationCorrId(
    uint32_t app_id) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(activate_app_list_lock_ptr_);
  auto it = app_to_activate_.find(app_id);
  if (app_to_activate_.end() == it) {
    return 0;
  }
  return it->second;
}

void PostponedActivationController::RemoveAppToActivate(uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(activate_app_list_lock_ptr_);
  app_to_activate_.erase(app_id);
}

}  // namespace application_manager
