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

#include "application_manager/commands/hmi/on_exit_all_applications_notification.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "application_manager/application_manager.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

OnExitAllApplicationsNotification::OnExitAllApplicationsNotification(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : NotificationFromHMI(message, application_manager) {}

OnExitAllApplicationsNotification::~OnExitAllApplicationsNotification() {}

void OnExitAllApplicationsNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const hmi_apis::Common_ApplicationsCloseReason::eType reason =
      static_cast<hmi_apis::Common_ApplicationsCloseReason::eType>(
          (*message_)[strings::msg_params][hmi_request::reason].asInt());
  LOG4CXX_DEBUG(logger_, "Reason " << reason);

  mobile_api::AppInterfaceUnregisteredReason::eType mob_reason =
      mobile_api::AppInterfaceUnregisteredReason::INVALID_ENUM;

  switch (reason) {
    case hmi_apis::Common_ApplicationsCloseReason::IGNITION_OFF: {
      mob_reason = mobile_api::AppInterfaceUnregisteredReason::IGNITION_OFF;
      break;
    }
    case hmi_apis::Common_ApplicationsCloseReason::MASTER_RESET: {
      mob_reason = mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET;
      break;
    }
    case hmi_apis::Common_ApplicationsCloseReason::FACTORY_DEFAULTS: {
      mob_reason = mobile_api::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS;
      break;
    }
    case hmi_apis::Common_ApplicationsCloseReason::SUSPEND: {
      SendOnSDLPersistenceComplete();
      return;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown Application close reason" << reason);
      return;
    }
  }

  application_manager_.SetUnregisterAllApplicationsReason(mob_reason);

  if (mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET == mob_reason ||
      mobile_api::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS ==
          mob_reason) {
    application_manager_.HeadUnitReset(mob_reason);
  }
  kill(getpid(), SIGINT);
}

void OnExitAllApplicationsNotification::SendOnSDLPersistenceComplete() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;
  (*message)[strings::params][strings::correlation_id] =
      application_manager_.GetNextHMICorrelationID();

  application_manager_.ManageHMICommand(message);
}

}  // namespace commands
}  // namespace application_manager
