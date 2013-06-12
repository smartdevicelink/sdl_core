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

#include "application_manager/message_helper.h"
#include "application_manager/basic_command_factory.h"

namespace application_manager {

void MessageHelper::SendHMIStatusNotification(
          const ApplicationImpl& application_impl) {

  smart_objects::CSmartObject hmi_status_notification;


  hmi_status_notification[strings::params][strings::function_id] =
      mobile_api::FunctionID::eType::OnHMIStatusID;

  hmi_status_notification[strings::params][strings::connection_key] =
      application_impl.app_id();

  hmi_status_notification[strings::msg_params][strings::hmi_level] =
      application_impl.hmi_level();

 // hmi_status_notification[strings::msg_params]
   //                         [strings::audio_streaming_state] =
     //                           application_impl.audio_streaming_state();

  hmi_status_notification[strings::msg_params][strings::system_context] =
      application_impl.system_context();

  CommandSharedPtr command = BasicCommandFactory::CreateCommand(&hmi_status_notification);
  command->Init();
  //TODO (VS): run must return bool, so SendHMIStatusNotification must also return bool
  command->Run();
  command->CleanUp();
}

}
