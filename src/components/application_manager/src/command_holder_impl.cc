/*
 * Copyright (c) 2017, Ford Motor Company
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

#include "application_manager/command_holder_impl.h"
#include "application_manager/application_manager.h"

namespace application_manager {
CommandHolderImpl::CommandHolderImpl(ApplicationManager& app_manager)
    : app_manager_(app_manager) {}

void CommandHolderImpl::Suspend(
    const std::string& policy_app_id,
    utils::SharedPtr<smart_objects::SmartObject> command) {
  sync_primitives::AutoLock lock(commands_lock_);
  app_commands_[policy_app_id].push_back(command);
}

void CommandHolderImpl::Resume(const std::string& policy_app_id) {
  sync_primitives::AutoLock lock(commands_lock_);
  auto app_commands = app_commands_.find(policy_app_id);
  if (app_commands_.end() == app_commands) {
    return;
  }
  for (auto cmd : app_commands->second) {
    app_manager_.ManageHMICommand(cmd);
  }

  app_commands_.erase(app_commands);
}

void CommandHolderImpl::Clear(const std::string& policy_app_id) {
  sync_primitives::AutoLock lock(commands_lock_);
  auto app_commands = app_commands_.find(policy_app_id);
  if (app_commands_.end() == app_commands) {
    return;
  }

  app_commands_.erase(app_commands);
}
}  // application_manager
