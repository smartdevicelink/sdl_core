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

#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "can_cooperation/mobile_command_factory.h"
#include "functional_module/function_ids.h"
/*
#include "can_cooperation/commands/start_scan_request.h"
#include "can_cooperation/commands/stop_scan_request.h"
#include "can_cooperation/commands/tune_radio_request.h"
#include "can_cooperation/commands/tune_up_request.h"
#include "can_cooperation/commands/tune_down_request.h"
#include "can_cooperation/commands/climate_control_on_request.h"
#include "can_cooperation/commands/get_seat_control_request.h"
*/
#include "can_cooperation/commands/button_press_request.h"
#include "can_cooperation/commands/get_interior_vehicle_data_capabilities_request.h"
#include "can_cooperation/commands/get_interior_vehicle_data_request.h"
#include "can_cooperation/commands/set_interior_vehicle_data_request.h"
/*
#include "can_cooperation/commands/on_preset_changed_notification.h"
#include "can_cooperation/commands/on_radio_details_notification.h"
*/
#include "can_cooperation/commands/on_interior_vehicle_data_notification.h"

namespace can_cooperation {

using functional_modules::MobileFunctionID;

utils::SharedPtr<commands::Command> MobileCommandFactory::CreateCommand(
    const application_manager::MessagePtr& msg,
    CANModuleInterface& can_module) {
  switch (msg->function_id()) {
    case MobileFunctionID::GET_INTERIOR_VEHICLE_DATA_CAPABILITIES: {
      return utils::MakeShared<
          commands::GetInteriorVehicleDataCapabiliesRequest>(msg, can_module);
      break;
    }
    case MobileFunctionID::GET_INTERIOR_VEHICLE_DATA: {
      return utils::MakeShared<commands::GetInteriorVehicleDataRequest>(
          msg, can_module);
      break;
    }
    case MobileFunctionID::SET_INTERIOR_VEHICLE_DATA: {
      return utils::MakeShared<commands::SetInteriorVehicleDataRequest>(
          msg, can_module);
      break;
    }
    case MobileFunctionID::BUTTON_PRESS: {
      return utils::MakeShared<commands::ButtonPressRequest>(msg, can_module);
      break;
    }
    case MobileFunctionID::ON_INTERIOR_VEHICLE_DATA: {
      return utils::MakeShared<commands::OnInteriorVehicleDataNotification>(
          msg, can_module);
      break;
    }
    /*  case MobileFunctionID::START_SCAN: {
        return utils::MakeShared<commands::StartScanRequest>(msg);
        break;
      }
      case MobileFunctionID::STOP_SCAN: {
        return utils::MakeShared< commands::StopScanRequest>(msg);
        break;
      }
      case MobileFunctionID::TUNE_RADIO: {
        return utils::MakeShared<Request>(msg);
        break;
      }
      case MobileFunctionID::TUNE_UP: {
        return utils::MakeShared< commands::TuneUpRequest>(msg));
        break;
      }
      case MobileFunctionID::TUNE_DOWN: {
        return utils::MakeShared (commands::TuneDownRequest(msg);
        break;
      }
      case MobileFunctionID::GET_SEAT_CONTROL: {
        return utils::MakeShared< commands::GetSeatControlRequest>(msg);
        break;
      }
      case MobileFunctionID::CLIMATE_CONTROL_ON: {
        return utils::MakeShared<commands::ClimateControlOnRequest>(msg);
        break;
      }*/
    /* case MobileFunctionID::ON_RADIO_DETAILS: {
       return utils::MakeShared< commands::OnRadioDetailsNotification>(msg);
       break;
     }
     case MobileFunctionID::ON_PRESETS_CHANGED: {
       return utils::MakeShared< commands::OnPresetChangedNotification>(msg);
       break;
     }*/
    default: {
      utils::SharedPtr<commands::Command> invalid_command;
      return invalid_command;
    }
  }
}

}  // namespace can_cooperation
