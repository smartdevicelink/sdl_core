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

#include "application_manager/commands/mobile/reset_global_properties_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_impl.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

ResetGlobalPropertiesRequest::ResetGlobalPropertiesRequest(
  const MessageSharedPtr& message): CommandRequestImpl(message) {
}

ResetGlobalPropertiesRequest::~ResetGlobalPropertiesRequest() {
}

void ResetGlobalPropertiesRequest::Run() {
  LOG4CXX_INFO(logger_, "ResetGlobalPropertiesRequest::Run");

  int app_id = (*message_)[strings::params][strings::connection_key];
  Application* app = ApplicationManagerImpl::instance()->application(app_id);

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int correlation_id =
    (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
    (*message_)[strings::params][strings::connection_key];

  size_t obj_length =
    (*message_)[strings::msg_params][strings::properties].length();

  bool helpt_promt = false;
  bool timeout_promt = false;
  bool vr_help_title = false;
  bool vr_help_items = false;

  for (size_t i = 0; i < obj_length; ++i) {
    switch ((*message_)[strings::msg_params][strings::properties][i].asInt()) {
      case GlobalProperty::HELPPROMT: {
        helpt_promt = ResetHelpPromt(app);
        break;
      }
      case GlobalProperty::TIMEOUTPROMT: {
        timeout_promt = ResetTimeoutPromt(app);
        break;
      }
      case GlobalProperty::VRHELPTITLE: {
        vr_help_title = ResetVrHelpTitle(app);
        break;
      }
      case GlobalProperty::VRHELPITEMS: {
        vr_help_items = ResetVrHelpItems(app);
        break;
      }
      default: {
        LOG4CXX_ERROR(logger_, "Unknown global property 0x%02X value" <<
            (*message_)[strings::msg_params][strings::properties][i].asInt());
        break;
      }
    }
  }

  app->set_reset_global_properties_active(true);

  unsigned int chaining_counter = 0;
  if (vr_help_title || vr_help_items) {
    ++chaining_counter;
  }

  if (timeout_promt || helpt_promt) {
    ++chaining_counter;
  }

  if (vr_help_title || vr_help_items) {

    smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

    if (vr_help_title) {
      msg_params[strings::vr_help_title] = (*app->vr_help_title());
    }

    if (vr_help_items) {
      msg_params[strings::vr_help] = (*app->vr_help());
    }

    msg_params[strings::app_id] = app->app_id();

    CreateHMIRequest(hmi_apis::FunctionID::UI_SetGlobalProperties,
                     msg_params, true, chaining_counter);
  }

  if (timeout_promt || helpt_promt) {
    // create ui request
    smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

    if (helpt_promt) {
      msg_params[strings::help_prompt] = (*app->help_promt());
    }

    if (timeout_promt) {
      msg_params[strings::timeout_prompt] = (*app->timeout_promt());
    }

    msg_params[strings::app_id] = app->app_id();

    // check if only tts request should be sent
    if (1 == chaining_counter) {
      CreateHMIRequest(hmi_apis::FunctionID::TTS_SetGlobalProperties,
                           msg_params, true, chaining_counter);
    } else {
      CreateHMIRequest(hmi_apis::FunctionID::TTS_SetGlobalProperties,
                     msg_params, true);
    }
  }
}

bool ResetGlobalPropertiesRequest::ResetHelpPromt(Application* const app) {
  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "Null pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  const std::vector<std::string>& help_promt =
      profile::Profile::instance()->help_promt();

  smart_objects::SmartObject so_help_promt =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  for (unsigned int i = 0; i < help_promt.size(); ++i) {
    smart_objects::SmartObject helpPrompt =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    helpPrompt[strings::text] =  help_promt[i];
    so_help_promt[i] = helpPrompt;
  }

  app->set_help_prompt(so_help_promt);

  return true;
}

bool ResetGlobalPropertiesRequest::ResetTimeoutPromt(Application* const app) {
  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "Null pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  const std::vector<std::string>& time_out_promt =
      profile::Profile::instance()->time_out_promt();

  smart_objects::SmartObject so_time_out_promt =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  for (unsigned int i = 0; i < time_out_promt.size(); ++i) {
    smart_objects::SmartObject timeoutPrompt =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    timeoutPrompt[strings::text] = time_out_promt[i];
    so_time_out_promt[i] = timeoutPrompt;
  }

  app->set_timeout_prompt(so_time_out_promt);

  return true;
}

bool ResetGlobalPropertiesRequest::ResetVrHelpTitle(Application* const app) {
  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "Null pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  smart_objects::SmartObject help_title(app->name());
  app->set_vr_help_title(help_title);

  return true;
}

bool ResetGlobalPropertiesRequest::ResetVrHelpItems(Application* const app) {
  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "Null pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  const CommandsMap& cmdMap = app->commands_map();
  smart_objects::SmartObject vr_help_items;

  int index = 0;
  CommandsMap::const_iterator command_it = cmdMap.begin();

  for (; cmdMap.end() != command_it; ++command_it) {
    if (false == (*command_it->second).keyExists(strings::vr_commands)) {
      LOG4CXX_ERROR(logger_, "VR synonyms are empty");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return false;
    }
    // use only first
    vr_help_items[index][strings::position] = index;
    vr_help_items[index++][strings::text] = (*command_it->second)[strings::vr_commands][0];

  }

  app->set_vr_help(vr_help_items);

  return true;
}

}  // namespace commands

}  // namespace application_manager
