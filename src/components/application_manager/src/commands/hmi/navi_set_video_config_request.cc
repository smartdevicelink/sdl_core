/*
 * Copyright (c) 2017 Xevo Inc.
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

#include "application_manager/commands/hmi/navi_set_video_config_request.h"
#include <string>
#include <vector>

namespace application_manager {

namespace commands {

NaviSetVideoConfigRequest::NaviSetVideoConfigRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : RequestToHMI(message, application_manager)
    , EventObserver(application_manager.event_dispatcher()) {}

NaviSetVideoConfigRequest::~NaviSetVideoConfigRequest() {}

void NaviSetVideoConfigRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!CheckAvailabilityHMIInterfaces(
          application_manager_, HmiInterfaces::HMI_INTERFACE_Navigation)) {
    LOG4CXX_WARN(logger_, "HMI interface Navigation is not supported");
    return;
  }

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "Application with hmi_app_id " << application_id()
                                                 << "does not exist");
    return;
  }

  subscribe_on_event(hmi_apis::FunctionID::Navigation_SetVideoConfig,
                     correlation_id());
  SendRequest();
}

void NaviSetVideoConfigRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "Application is not found, abort NaviSetVideoConfigRequest");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_SetVideoConfig: {
      const hmi_apis::Common_Result::eType code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      bool result = false;
      std::vector<std::string> rejected_params;

      if (code == hmi_apis::Common_Result::SUCCESS) {
        LOG4CXX_DEBUG(logger_, "Received SetVideoConfig success response");
        result = true;
      } else {
        LOG4CXX_DEBUG(logger_,
                      "Received SetVideoConfig failure response (" << event.id()
                                                                   << ")");
        result = false;
        if (message[strings::msg_params].keyExists("rejectedParams")) {
          const smart_objects::SmartArray* list =
              message[strings::msg_params]["rejectedParams"].asArray();
          if (list != NULL) {
            for (unsigned int i = 0; i < list->size(); i++) {
              const std::string& param = (*list)[i].asString();
              // Make sure that we actually sent the parameter in the request
              if ((*message_)[strings::msg_params].keyExists(strings::config) &&
                  (*message_)[strings::msg_params][strings::config].keyExists(
                      param)) {
                rejected_params.push_back(param);
              }
            }
          }
        }
      }
      application_manager_.OnStreamingConfigured(
          application_id(),
          protocol_handler::ServiceType::kMobileNav,
          result,
          rejected_params);
      break;
    }
    default:
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
  }
}

void NaviSetVideoConfigRequest::onTimeOut() {
  LOG4CXX_WARN(logger_, "Timed out while waiting for SetVideoConfig response");

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not found");
    return;
  }

  std::vector<std::string> empty;
  application_manager_.OnStreamingConfigured(
      application_id(),
      protocol_handler::ServiceType::kMobileNav,
      false,
      empty);

  application_manager_.TerminateRequest(
      connection_key(), correlation_id(), function_id());
}

}  // namespace commands

}  // namespace application_manager
