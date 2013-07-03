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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_

#include "application_manager/commands/command_impl.h"
#include "interfaces/v4_protocol_v2_0_revT.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class CSmartObject;
}
}

namespace application_manager {

namespace commands {

/*
 * @brief Typedef for HMI TextFieldName type
 */
typedef enum {
  MAIN_FILED1              = 0,
  MAIN_FILED2,
  MAIN_FILED3,
  MAIN_FILED4,
  STATUS_BAR,
  MEDIA_CLOCK,
  MEDIA_TRACK,
  ALERT_TEXT1,
  ALERT_TEXT2,
  ALERT_TEXT3,
  SCROLLABLE_MSG_BODY,
  INITIAL_INTERACTION_TEXT,
  NAVI_TEXT1,
  NAVI_TEXT2,
  ETA,
  TOTAL_DISTANCE,
  NAVI_TEXT,
  AUDIO_DISPLAY_TEXT1,
  AUDIO_DISPLAY_TEXT2,
  SLIDER_HADER,
  SLIDER_FOOTEER
} TextFieldName;

/*
 * @brief Typedef for the vehicle data types that can
 * be published and subscribed to
 */
typedef enum {
  GPS                      = 0,
  SPEED,
  RPM,
  FUELLEVEL,
  FUELLEVEL_STATE,
  FUELCONSUMPTION,
  EXTERNTEMP,
  VIN,
  PRNDL,
  TIREPRESSURE,
  ODOMETER,
  BELTSTATUS,
  BODYINFO,
  DEVICESTATUS,
  ECALLINFO,
  AIRBAGSTATUS,
  EMERGENCYEVENT,
  CLUSTERMODESTATUS,
  MYKEY,
  BRAKING,
  WIPERSTATUS,
  HEADLAMPSTATUS,
  BATTVOLTAGE,
  ENGINETORQUE,
  ACCPEDAL,
  STEERINGWHEEL
} VehicleDataType;

class CommandRequestImpl : public CommandImpl {
  public:
    explicit CommandRequestImpl(const MessageSharedPtr& message);
    virtual ~CommandRequestImpl();
    virtual bool Init();
    virtual bool CleanUp();
    virtual void Run();
    void SendResponse(const bool success,
                      const NsSmartDeviceLinkRPC::V2::Result::eType& result_code,
                      const char* info = NULL);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
