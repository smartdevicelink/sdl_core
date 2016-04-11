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
#include "application_manager/commands/hmi/tts_get_language_response.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/event_engine/event.h"

namespace application_manager {

namespace commands {

TTSGetLanguageResponse::TTSGetLanguageResponse(const MessageSharedPtr& message)
    : ResponseFromHMI(message) {
}

TTSGetLanguageResponse::~TTSGetLanguageResponse() {
}

void TTSGetLanguageResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace hmi_apis;

  Common_Language::eType language = Common_Language::INVALID_ENUM;

  if ((*message_).keyExists(strings::msg_params) &&
      (*message_)[strings::msg_params].keyExists(hmi_response::language)) {

    language = static_cast<Common_Language::eType>(
             (*message_)[strings::msg_params][hmi_response::language].asInt());
  }

  ApplicationManagerImpl::instance()->hmi_capabilities().
      set_active_tts_language(language);

  LOG4CXX_DEBUG(logger_, "Raising event for function_id "
                << function_id()
                << " and correlation_id " << correlation_id());
  event_engine::Event event(FunctionID::TTS_GetLanguage);
  event.set_smart_object(*message_);
  event.raise();
}

}  // namespace commands

}  // namespace application_manager
