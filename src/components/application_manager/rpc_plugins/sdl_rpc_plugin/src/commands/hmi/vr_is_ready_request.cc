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

#include "sdl_rpc_plugin/commands/hmi/vr_is_ready_request.h"
#include "application_manager/rpc_service.h"

namespace application_manager {

namespace commands {

VRIsReadyRequest::VRIsReadyRequest(const MessageSharedPtr& message,
                                   ApplicationManager& application_manager)
    : RequestToHMI(message, application_manager)
    , EventObserver(application_manager.event_dispatcher()) {}

VRIsReadyRequest::~VRIsReadyRequest() {}

void VRIsReadyRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  subscribe_on_event(hmi_apis::FunctionID::VR_IsReady, correlation_id());
  SendRequest();
}

void VRIsReadyRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::VR_IsReady: {
      LOG4CXX_DEBUG(logger_, "Received VR_IsReady event");
      unsubscribe_from_event(hmi_apis::FunctionID::VR_IsReady);
      const bool is_available = ChangeInterfaceState(
          application_manager_, message, HmiInterfaces::HMI_INTERFACE_VR);

      HMICapabilities& hmi_capabilities =
          application_manager_.hmi_capabilities();
      hmi_capabilities.set_is_vr_cooperating(is_available);
      if (!CheckAvailabilityHMIInterfaces(application_manager_,
                                          HmiInterfaces::HMI_INTERFACE_VR)) {
        LOG4CXX_INFO(logger_,
                     "HmiInterfaces::HMI_INTERFACE_VR isn't available");
        return;
      }
      SendMessageToHMI();
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

void VRIsReadyRequest::onTimeOut() {
  // Note(dtrunov): According to new requirment APPLINK-27956
  SendMessageToHMI();
}

void VRIsReadyRequest::SendMessageToHMI() {
  utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetLanguage,
                                        application_manager_));
  HMICapabilities& hmi_capabilities = application_manager_.hmi_capabilities();
  hmi_capabilities.set_handle_response_for(*get_language);
  application_manager_.GetRPCService().ManageHMICommand(get_language);
  utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VR_GetSupportedLanguages,
          application_manager_));
  application_manager_.GetRPCService().ManageHMICommand(get_all_languages);
  utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VR_GetCapabilities, application_manager_));
  application_manager_.GetRPCService().ManageHMICommand(get_capabilities);
}

}  // namespace commands

}  // namespace application_manager
