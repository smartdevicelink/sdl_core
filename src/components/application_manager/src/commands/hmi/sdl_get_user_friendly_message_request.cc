/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/hmi/sdl_get_user_friendly_message_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {

namespace commands {

SDLGetUserFriendlyMessageRequest::SDLGetUserFriendlyMessageRequest(
    const MessageSharedPtr& message)
    : RequestFromHMI(message) {
}

SDLGetUserFriendlyMessageRequest::~SDLGetUserFriendlyMessageRequest() {
}

void SDLGetUserFriendlyMessageRequest::Run() {
  LOG4CXX_INFO(logger_, "SDLGetUserFriendlyMessageRequest::Run");
  const std::string messageCodes = "messageCodes";
  if (!(*message_)[strings::msg_params].keyExists(messageCodes)) {
    LOG4CXX_WARN(logger_,
                 "Mandatory parameter '"+messageCodes+"'' is missing");
    return;
  }
  smart_objects::SmartArray* msg =
      (*message_)[strings::msg_params][messageCodes].asArray();

  std::vector<std::string> msg_codes;

  smart_objects::SmartArray::const_iterator it = msg->begin();
  smart_objects::SmartArray::const_iterator it_end = msg->end();
  for (; it != it_end; ++it) {
    msg_codes.push_back((*it).asString());
  }

  std::string required_language;
  const std::string language = "language";
  if ((*message_)[strings::msg_params].keyExists(language)) {
    uint32_t lang_code = (*message_)[strings::msg_params][language].asUInt();
    required_language = CommonLanguageToString(
          static_cast<hmi_apis::Common_Language::eType>(lang_code));
  } else {
    hmi_apis::Common_Language::eType ui_language =
        application_manager::ApplicationManagerImpl::instance()
        ->hmi_capabilities().active_ui_language();

    required_language = CommonLanguageToString(ui_language);
  }

  policy::PolicyHandler::instance()->OnGetUserFriendlyMessage(
        msg_codes, required_language,
        (*message_)[strings::params][strings::correlation_id].asInt());
}

std::string SDLGetUserFriendlyMessageRequest::CommonLanguageToString(
    hmi_apis::Common_Language::eType language) {
  switch (language) {
  case hmi_apis::Common_Language::EN_US:
    return "en-us";
  case hmi_apis::Common_Language::ES_MX:
    return "es-mx";
  case hmi_apis::Common_Language::FR_CA:
    return "fr-ca";
  case hmi_apis::Common_Language::DE_DE:
    return "de-de";
  case hmi_apis::Common_Language::ES_ES:
    return "es-es";
  case hmi_apis::Common_Language::EN_GB:
    return "en-gb";
  case hmi_apis::Common_Language::RU_RU:
    return "ru-ru";
  case hmi_apis::Common_Language::TR_TR:
    return "tr-tr";
  case hmi_apis::Common_Language::PL_PL:
    return "pl-pl";
  case hmi_apis::Common_Language::FR_FR:
    return "fr-fr";
  case hmi_apis::Common_Language::IT_IT:
    return "it-it";
  case hmi_apis::Common_Language::SV_SE:
    return "sv-se";
  case hmi_apis::Common_Language::PT_PT:
    return "pt-pt";
  case hmi_apis::Common_Language::NL_NL:
    return "nl-nl";
  case hmi_apis::Common_Language::EN_AU:
    return "en-au";
  case hmi_apis::Common_Language::ZH_CN:
    return "zh-cn";
  case hmi_apis::Common_Language::ZH_TW:
    return "zh-tw";
  case hmi_apis::Common_Language::JA_JP:
    return "ja-jp";
  case hmi_apis::Common_Language::AR_SA:
    return "as-sa";
  case hmi_apis::Common_Language::KO_KR:
    return "ko-kr";
  case hmi_apis::Common_Language::PT_BR:
    return "pt-br";
  case hmi_apis::Common_Language::CS_CZ:
    return "cs-cz";
  case hmi_apis::Common_Language::DA_DK:
    return "da-dk";
  case hmi_apis::Common_Language::NO_NO:
    return "no-no";
  default:
    LOG4CXX_WARN(logger_, "Language is unknown.");
    return "";
  }
}

}  // namespace commands
}  // namespace application_manager


