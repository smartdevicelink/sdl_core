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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_FUNCTION_IDS_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_FUNCTION_IDS_H_
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace functional_modules {

enum RCFunctionID {
  // Remote SDL functions ids
  BUTTON_PRESS = mobile_apis::FunctionID::ButtonPressID,
  GET_INTERIOR_VEHICLE_DATA = mobile_apis::FunctionID::GetInteriorVehicleDataID,
  SET_INTERIOR_VEHICLE_DATA = mobile_apis::FunctionID::SetInteriorVehicleDataID,
  ON_INTERIOR_VEHICLE_DATA = mobile_apis::FunctionID::OnInteriorVehicleDataID,
  ON_REMOTE_CONTROL_SETTINGS = hmi_apis::FunctionID::RC_OnRemoteControlSettings,
};

namespace hmi_api {
const char get_interior_vehicle_data[] = "RC.GetInteriorVehicleData";
const char set_interior_vehicle_data[] = "RC.SetInteriorVehicleData";
const char on_interior_vehicle_data[] = "RC.OnInteriorVehicleData";
const char button_press[] = "Buttons.ButtonPress";
const char on_remote_control_settings[] = "RC.OnRemoteControlSettings";

const char get_user_consent[] = "RC.GetInteriorVehicleDataConsent";
const char on_app_deactivated[] = "BasicCommunication.OnAppDeactivated";
const char sdl_activate_app[] = "SDL.ActivateApp";
}

}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_FUNCTION_IDS_H_
