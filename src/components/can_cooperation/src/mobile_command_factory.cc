/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "can_cooperation/mobile_command_factory.h"
#include "functional_module/function_ids.h"
#include "can_cooperation/commands/grant_access_request.h"
#include "can_cooperation/commands/cancel_access_request.h"
#include "can_cooperation/commands/start_scan_request.h"
#include "can_cooperation/commands/stop_scan_request.h"
#include "can_cooperation/commands/tune_radio_request.h"
#include "can_cooperation/commands/tune_up_request.h"
#include "can_cooperation/commands/tune_down_request.h"
#include "can_cooperation/commands/on_control_changed_notification.h"
#include "can_cooperation/commands/on_preset_changed_notification.h"
#include "can_cooperation/commands/on_radio_details_notification.h"


namespace can_cooperation {

using functional_modules::MobileFunctionID;

commands::Command* MobileCommandFactory::CreateCommand(
    const application_manager::MessagePtr& msg) {

  switch (msg->function_id()) {
    case MobileFunctionID::GRANT_ACCESS: {
      return new commands::GrantAccessRequest(msg);
      break;
    }
    case MobileFunctionID::CANCEL_ACCESS: {
      return new commands::CancelAccessRequest(msg);
      break;
    }
    case MobileFunctionID::START_SCAN: {
      return new commands::StartScanRequest(msg);
      break;
    }
    case MobileFunctionID::STOP_SCAN: {
      return new commands::StopScanRequest(msg);
      break;
    }
    case MobileFunctionID::TUNE_RADIO: {
      return new commands::TuneRadioRequest(msg);
      break;
    }
    case MobileFunctionID::TUNE_UP: {
      return new commands::TuneUpRequest(msg);
      break;
    }
    case MobileFunctionID::TUNE_DOWN: {
      return new commands::TuneDownRequest(msg);
      break;
    }
    case MobileFunctionID::ON_CONTROL_CHANGED: {
      return new commands::OnControlChangedNotification(msg);
      break;
    }
    case MobileFunctionID::ON_RADIO_DETAILS: {
      return new commands::OnRadioDetailsNotification(msg);
      break;
    }
    case MobileFunctionID::ON_PRESET_CHANGED: {
      return new commands::OnPresetChangedNotification(msg);
      break;
    }
    default: {
      return NULL;
    }
  }
}

}  // namespace can_cooperation;
