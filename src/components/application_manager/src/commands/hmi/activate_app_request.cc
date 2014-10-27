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

#include "application_manager/commands/hmi/activate_app_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"

namespace application_manager {

  namespace commands {

    ActivateAppRequest::ActivateAppRequest(
        const MessageSharedPtr& message): RequestToHMI(message) {
    }


    ActivateAppRequest::~ActivateAppRequest() {
      LOG4CXX_TRACE(logger_, "~ActivateAppRequest");
    }

    void ActivateAppRequest::Run() {
      LOG4CXX_TRACE(logger_, "enter " << correlation_id());
      uint32_t app_id = RequestToHMI::application_id();
      ApplicationManagerImpl::instance()->set_application_id(correlation_id(), app_id);
#ifdef ENABLE_LOG
      if ((*message_)[strings::msg_params].keyExists(
            strings::activate_app_hmi_level)) {
        mobile_apis::HMILevel::eType requested_hmi_level =
            static_cast<mobile_apis::HMILevel::eType>(
                   (*message_)[strings::msg_params][strings::activate_app_hmi_level].asInt());
        LOG4CXX_TRACE(logger_, "requested_hmi_level = "
                        << requested_hmi_level);
      }
#endif
      SendRequest();
      subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_ActivateApp,
                         correlation_id());
      LOG4CXX_TRACE(logger_, "exit");
    }


    void ActivateAppRequest::on_event(const event_engine::Event& event) {
      LOG4CXX_TRACE_ENTER(logger_);
      const smart_objects::SmartObject* response = &(event.smart_object());
      const hmi_apis::Common_Result::eType code =
          static_cast<hmi_apis::Common_Result::eType>(
            (*response)[strings::params][hmi_response::code].asInt());
      if (hmi_apis::Common_Result::SUCCESS != code) {
        LOG4CXX_ERROR(logger_, "Error ActivateApp result code " << code);
        return;
      }
      int32_t correlation_id = RequestToHMI::correlation_id();
      // Mobile id is converted to HMI id for HMI requests
      const uint32_t hmi_app_id = ApplicationManagerImpl::instance()->
                                  application_id(correlation_id);
      mobile_apis::HMILevel::eType requested_hmi_level = static_cast<mobile_apis::HMILevel::eType>(
          (*message_)[strings::msg_params][strings::activate_app_hmi_level].asInt());
      if (0 == hmi_app_id) {
        LOG4CXX_ERROR(logger_, "Error hmi_app_id = "<< hmi_app_id);
        return;
      }

      ApplicationSharedPtr application = ApplicationManagerImpl::instance()->
                                         application_by_hmi_app(hmi_app_id);
      if (!application.valid()) {
        LOG4CXX_ERROR(logger_, "Application can't be activated.");
        return;
      }
      if (mobile_apis::HMILevel::HMI_FULL == requested_hmi_level) {
        if (ApplicationManagerImpl::instance()->ActivateApplication(application)) {
          MessageHelper::SendHMIStatusNotification(*(application.get()));
        }
      }
    }
  }  // namespace commands

}  // namespace application_manager

