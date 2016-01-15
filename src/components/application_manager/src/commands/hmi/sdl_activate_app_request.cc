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

#include "application_manager/commands/hmi/sdl_activate_app_request.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

SDLActivateAppRequest::SDLActivateAppRequest(const MessageSharedPtr& message)
    : RequestFromHMI(message) {}

SDLActivateAppRequest::~SDLActivateAppRequest() {}

void SDLActivateAppRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace hmi_apis::FunctionID;
  using namespace hmi_apis::Common_Result;

  const uint32_t application_id = app_id();

  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(application_id);

  if (!app) {
    LOG4CXX_WARN(
        logger_,
        "Can't find application within regular apps: " << application_id);

    app = ApplicationManagerImpl::instance()->waiting_app(application_id);

    if (!app) {
      LOG4CXX_WARN(
          logger_,
          "Can't find application within waiting apps: " << application_id);
      return;
    }
  }

  if (!app->IsRegistered()) {
    DevicesApps devices_apps = FindAllAppOnParticularDevice(app->device());
    if (!devices_apps.first && devices_apps.second.empty()) {
      LOG4CXX_ERROR(logger_,
                    "Can't find regular foreground app with the same "
                    "connection id:"
                        << app->device());
      SendResponse(correlation_id(), SDL_ActivateApp, NO_APPS_REGISTERED);
      return;
    }
    if (devices_apps.first) {
      MessageHelper::SendLaunchApp(
          devices_apps.first->app_id(), app->SchemaUrl(), app->PackageName());
    } else {
      std::vector<ApplicationSharedPtr>::const_iterator it =
          devices_apps.second.begin();
      for (; it != devices_apps.second.end(); ++it) {
        MessageHelper::SendLaunchApp(
            (*it)->app_id(), app->SchemaUrl(), app->PackageName());
      }
    }
    subscribe_on_event(BasicCommunication_OnAppRegistered);
  } else {
    policy::PolicyHandler::instance()->OnActivateApp(application_id,
                                                     correlation_id());
  }
}

void SDLActivateAppRequest::onTimeOut() {
  using namespace hmi_apis::FunctionID;
  using namespace hmi_apis::Common_Result;
  using namespace application_manager;
  unsubscribe_from_event(BasicCommunication_OnAppRegistered);
  SendResponse(correlation_id(), SDL_ActivateApp, APPLICATION_NOT_REGISTERED);
}

void SDLActivateAppRequest::on_event(const event_engine::Event& event) {
  using namespace hmi_apis::FunctionID;
  if (event.id() != BasicCommunication_OnAppRegistered) {
    return;
  }
  unsubscribe_from_event(BasicCommunication_OnAppRegistered);

  // Have to use HMI app id from event, since HMI app id from original request
  // message will be changed after app, initially requested for launch via
  // SDL.ActivateApp, will be registered
  const uint32_t hmi_application_id = hmi_app_id(event.smart_object());

  ApplicationSharedPtr app =
      application_manager::ApplicationManagerImpl::instance()
          ->application_by_hmi_app(hmi_application_id);
  if (!app) {
    LOG4CXX_ERROR(
        logger_, "Application not found by HMI app id: " << hmi_application_id);
    return;
  }
  policy::PolicyHandler::instance()->OnActivateApp(app->app_id(),
                                                   correlation_id());
}

uint32_t SDLActivateAppRequest::app_id() const {
  if ((*message_).keyExists(strings::msg_params)) {
    if ((*message_)[strings::msg_params].keyExists(strings::app_id)) {
      return (*message_)[strings::msg_params][strings::app_id].asUInt();
    }
  }
  LOG4CXX_DEBUG(logger_, "app_id section is absent in the message.");
  return 0;
}

uint32_t SDLActivateAppRequest::hmi_app_id(
    const smart_objects::SmartObject& so) const {
  if (so.keyExists(strings::params)) {
    if (so[strings::msg_params].keyExists(strings::application)) {
      if (so[strings::msg_params][strings::application].keyExists(
              strings::app_id)) {
        return so[strings::msg_params][strings::application][strings::app_id]
            .asUInt();
      }
    }
  }
  LOG4CXX_DEBUG(logger_, "Can't find app_id section is absent in the message.");
  return 0;
}

DevicesApps SDLActivateAppRequest::FindAllAppOnParticularDevice(
    const connection_handler::DeviceHandle handle) {
  DevicesApps apps;

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const ApplicationSet app_list = accessor.applications();

  ApplicationSetIt it = app_list.begin();
  ApplicationSetIt it_end = app_list.end();

  for (; it != it_end; ++it) {
    if (handle == (*it)->device()) {
      if ((*it)->is_foreground()) {
        apps.first = *it;
      }
      apps.second.push_back(*it);
    }
  }
  return apps;
}

}  // namespace commands
}  // namespace application_manager
