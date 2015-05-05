/*
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
#include "application_manager/state_context.h"
#include "application_manager/application_manager_impl.h"
#include "config_profile/profile.h"

namespace application_manager {


bool StateContext::is_navi_app(const uint32_t app_id) const {
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);
  DCHECK_OR_RETURN(app, false);
  return app ? app->is_navi() : false;
}

bool StateContext::is_meida_app(const uint32_t app_id) const {
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);
  return app ? app->is_media_application() : false;
}

bool StateContext::is_voice_comunication_app(const uint32_t app_id) const {
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);
  return app ? app->is_voice_communication_supported() : false;
}

bool StateContext::is_attenuated_supported() const{
  const HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();
  return hmi_capabilities.attenuated_supported() &&
         profile::Profile::instance()->is_mixing_audio_supported();
}

}
