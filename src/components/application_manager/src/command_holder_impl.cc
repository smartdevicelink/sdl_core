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
#include "application_manager/commands/command.h"

namespace application_manager {
CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

CommandHolderImpl::CommandHolderImpl(ApplicationManager& app_manager)
    : app_manager_(app_manager) {}

void CommandHolderImpl::Suspend(
    ApplicationSharedPtr application,
    CommandType type,
    utils::SharedPtr<smart_objects::SmartObject> command) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);
  LOG4CXX_DEBUG(logger_,
                "Suspending command(s) for application: "
                    << application->policy_app_id());
  sync_primitives::AutoLock lock(commands_lock_);

  if (CommandType::kHmiCommand == type) {
    app_hmi_commands_[application].push_back(command);
    LOG4CXX_DEBUG(logger_,
                  "Suspended HMI command(s): " << app_hmi_commands_.size());
  } else {
    app_mobile_commands_[application].push_back(command);
    LOG4CXX_DEBUG(logger_,
                  "Suspended mobile command(s): " << app_hmi_commands_.size());
  }
}

void CommandHolderImpl::Resume(ApplicationSharedPtr application,
                               CommandType type) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);
  LOG4CXX_DEBUG(
      logger_,
      "Resuming command(s) for application: " << application->policy_app_id());
  if (CommandType::kHmiCommand == type) {
    ResumeHmiCommand(application);
  } else {
    ResumeMobileCommand(application);
  }
}

void CommandHolderImpl::Clear(ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);
  LOG4CXX_DEBUG(
      logger_,
      "Clearing command(s) for application: " << application->policy_app_id());
  sync_primitives::AutoLock lock(commands_lock_);
  auto app_hmi_commands = app_hmi_commands_.find(application);
  if (app_hmi_commands_.end() != app_hmi_commands) {
    LOG4CXX_DEBUG(
        logger_,
        "Clearing HMI command(s): " << app_hmi_commands->second.size());
    app_hmi_commands_.erase(app_hmi_commands);
  }

  auto app_mobile_commands = app_mobile_commands_.find(application);
  if (app_mobile_commands_.end() != app_mobile_commands) {
    LOG4CXX_DEBUG(
        logger_,
        "Clearing mobile command(s): " << app_mobile_commands->second.size());
    app_mobile_commands_.erase(app_mobile_commands);
  }
}

void CommandHolderImpl::ResumeHmiCommand(ApplicationSharedPtr application) {
  DCHECK_OR_RETURN_VOID(application);
  sync_primitives::AutoLock lock(commands_lock_);
  auto app_commands = app_hmi_commands_.find(application);
  if (app_hmi_commands_.end() == app_commands) {
    return;
  }

  LOG4CXX_DEBUG(logger_,
                "Resuming HMI command(s): " << app_hmi_commands_.size());

  for (auto cmd : app_commands->second) {
    (*cmd)[strings::msg_params][strings::app_id] = application->hmi_app_id();
    app_manager_.GetRPCService().ManageHMICommand(cmd);
  }

  app_hmi_commands_.erase(app_commands);
}

void CommandHolderImpl::ResumeMobileCommand(ApplicationSharedPtr application) {
  DCHECK_OR_RETURN_VOID(application);
  sync_primitives::AutoLock lock(commands_lock_);
  auto app_commands = app_mobile_commands_.find(application);
  if (app_mobile_commands_.end() == app_commands) {
    return;
  }

  LOG4CXX_DEBUG(logger_,
                "Resuming mobile command(s): " << app_mobile_commands_.size());

  for (auto cmd : app_commands->second) {
    (*cmd)[strings::params][strings::connection_key] = application->app_id();
    app_manager_.GetRPCService().ManageMobileCommand(
        cmd, commands::Command::CommandSource::SOURCE_MOBILE);
  }

  app_mobile_commands_.erase(app_commands);
}
}  // application_manager
