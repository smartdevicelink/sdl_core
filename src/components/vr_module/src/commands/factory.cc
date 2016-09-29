/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "vr_module/commands/factory.h"

#include "utils/logger.h"

#include "vr_module/commands/async_command.h"
#include "vr_module/commands/activate_service.h"
#include "vr_module/commands/on_default_service_chosen.h"
#include "vr_module/commands/on_register_service.h"
#include "vr_module/commands/on_service_deactivated.h"
#include "vr_module/commands/on_unregister_service.h"
#include "vr_module/commands/process_data.h"
#include "vr_module/commands/support_service.h"
#include "vr_module/interface/hmi.pb.h"
#include "vr_module/interface/mobile.pb.h"

namespace vr_module {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

Factory::Factory(ServiceModule* module)
    : module_(module) {
}

CommandPtr Factory::Create(const vr_hmi_api::ServiceMessage& message) const {
  LOG4CXX_AUTO_TRACE(logger_);
  switch (message.rpc()) {
    case vr_hmi_api::SUPPORT_SERVICE:
      return CommandPtr(new AsyncCommand(new SupportService(message, module_)));
    case vr_hmi_api::ON_REGISTER:
      return CommandPtr(new OnRegisterService(message, module_));
    case vr_hmi_api::ON_DEFAULT_CHOSEN:
      return CommandPtr(new OnDefaultServiceChosen(message, module_));
    case vr_hmi_api::ON_DEACTIVATED:
      return CommandPtr(new OnServiceDeactivated(module_));
    case vr_hmi_api::ACTIVATE:
      return CommandPtr(new AsyncCommand(new ActivateService(message, module_)));
    case vr_hmi_api::PROCESS_DATA:
      return CommandPtr(new AsyncCommand(new ProcessData(message, module_)));
    case vr_hmi_api::ON_UNREGISTER:
      return CommandPtr(new OnUnregisterService(message, module_));
    default:
      return CommandPtr();
  }
  return CommandPtr();
}

CommandPtr Factory::Create(const vr_mobile_api::ServiceMessage& message) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return CommandPtr();
}

}  // namespace commands
}  // namespace vr_module
