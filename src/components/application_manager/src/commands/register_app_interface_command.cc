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

#include "application_manager/commands/register_app_interface_command.h"

#include "application_manager/commands/register_app_interface_response_command.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_conversion.h"

#include "mobile_message_handler/mobile_message_handler_impl.h"


namespace application_manager {

namespace commands {

void RegisterAppInterfaceCommand::Run() {
  if (ApplicationManagerImpl::GetInstance()->
      application((*message_)[strings::msg_params][strings::app_id])) {
    SendResponse(false,
             NsSmartDeviceLinkRPC::V2::Result::APPLICATION_REGISTERED_ALREADY);
  } else {
    ApplicationImpl* application_impl = new ApplicationImpl(
        (*message_)[strings::msg_params][strings::app_id]);

    application_impl->set_sync_msg_version(
        (*message_)[strings::msg_params][strings::sync_msg_version]);

    application_impl->set_name(
        (*message_)[strings::msg_params][strings::app_name]);

    application_impl->set_is_media_application(
        (*message_)[strings::msg_params][strings::is_media_application]);

    if ((*message_)[strings::msg_params].keyExists(strings::vr_synonyms)) {
      application_impl->set_vr_synonyms(
          (*message_)[strings::msg_params][strings::vr_synonyms]);
    }

    if ((*message_)[strings::msg_params].keyExists(
        strings::ngn_media_screen_app_name)) {
      application_impl->set_ngn_media_screen_name(
          (*message_)[strings::msg_params][strings::ngn_media_screen_app_name]);
    }

    application_impl->set_language(
        (*message_)[strings::msg_params][strings::language_desired]);
    application_impl->set_ui_language(
      (*message_)[strings::msg_params][strings::hmi_display_language_desired]);

    if ((*message_)[strings::msg_params].keyExists(strings::tts_name)) {
      application_impl->set_tts_name(
          (*message_)[strings::msg_params][strings::tts_name]);
    }

    if ((*message_)[strings::msg_params].keyExists(strings::app_type)) {
      application_impl->set_app_types(
          (*message_)[strings::msg_params][strings::app_type]);
    }

    bool register_application_result =
        ApplicationManagerImpl::GetInstance()->
        RegisterApplication(application_impl);

    if (!register_application_result) {
      SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::GENERIC_ERROR);
    } else {
      SendResponse(true, NsSmartDeviceLinkRPC::V2::Result::SUCCESS);
    }
  }
}

}  // namespace commands

}  // namespace application_manager
