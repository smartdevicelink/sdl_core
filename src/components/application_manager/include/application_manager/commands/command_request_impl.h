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
#include "v4_protocol_v2_0_revT.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class CSmartObject;
}
}

namespace application_manager {

namespace commands {

/*
 * @brief Enum for HMI TextFieldName type
 */
enum TextFieldName {
  MAIN_FILED1              = 0,
  MAIN_FILED2              = 1,
  MAIN_FILED3              = 2,
  MAIN_FILED4              = 3,
  STATUS_BAR               = 4,
  MEDIA_CLOCK              = 5,
  MEDIA_TRACK              = 6,
  ALERT_TEXT1              = 7,
  ALERT_TEXT2              = 8,
  ALERT_TEXT3              = 9,
  SCROLLABLE_MSG_BODY      = 10,
  INITIAL_INTERACTION_TEXT = 11,
  NAVI_TEXT1               = 12,
  NAVI_TEXT2               = 13,
  ETA                      = 14,
  TOTAL_DISTANCE           = 15,
  NAVI_TEXT                = 16,
  AUDIO_DISPLAY_TEXT1      = 17,
  AUDIO_DISPLAY_TEXT2      = 18,
  SLIDER_HADER             = 19,
  SLIDER_FOOTEER           = 20
};


class CommandRequestImpl : public CommandImpl {
 public:
  explicit CommandRequestImpl(const MessageSharedPtr& message);
  virtual ~CommandRequestImpl();
  virtual bool Init();
  virtual bool CleanUp();
  virtual void Run();
  void SendResponse(const bool success,
      const NsSmartDeviceLinkRPC::V2::Result::eType& errorCode);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
