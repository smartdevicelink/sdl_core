/*
 Copyright (c) 2016, Ford Motor Company
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

#include "vr_module/vr_module.h"

#include "utils/logger.h"
#include "utils/scope_guard.h"
#include "vr_module/commands/factory.h"
#include "vr_module/event_engine/event_dispatcher.h"
#include "vr_module/hmi_event.h"
#include "vr_module/mobile_event.h"
//#include "protocol/common.h"
#include "functional_module/plugin_manager.h"

namespace vr_module {

using event_engine::EventDispatcher;

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

PLUGIN_FACTORY(VRModule)

VRModule::VRModule()
    : GenericModule(kModuleID),
      proxy_(this),
      supported_(false),
      active_service_(0),
      default_service_(0),
      messages_from_mobile_service_("IncomingFromMobileRemoteService", this) {
  factory_ = new commands::Factory(this);
  plugin_info_.name = "VRModulePlugin";
  plugin_info_.version = 1;
  plugin_info_.service_type = functional_modules::ServiceType::VR;
  SubscribeToRpcMessages();
}

VRModule::VRModule(Channel* channel)
    : GenericModule(kModuleID),
      proxy_(this, channel),
      supported_(false),
      active_service_(0),
      default_service_(0),
      messages_from_mobile_service_("IncomingFromMobileRemoteService", this) {
  factory_ = new commands::Factory(this);
  plugin_info_.name = "VRModulePlugin";
  plugin_info_.version = 1;
  plugin_info_.service_type = functional_modules::ServiceType::VR;
  SubscribeToRpcMessages();
}

VRModule::~VRModule() {
  delete factory_;
}

void VRModule::CheckSupport() {
  vr_hmi_api::ServiceMessage message;
  message.set_rpc(vr_hmi_api::SUPPORT_SERVICE);
  commands::CommandPtr command = factory_->Create(message);
  RunCommand(command);
}

void VRModule::OnReady() {
  CheckSupport();
}

void VRModule::OnReceived(const vr_hmi_api::ServiceMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (message.rpc_type() == vr_hmi_api::RESPONSE) {
    EmitEvent(message);
  } else {
    commands::CommandPtr command = factory_->Create(message);
    RunCommand(command);
  }
}

void VRModule::OnReceived(const vr_mobile_api::ServiceMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (message.rpc_type() == vr_mobile_api::RESPONSE) {
    EmitEvent(message);
  } else {
    commands::CommandPtr command = factory_->Create(message);
    RunCommand(command);
  }
}

void VRModule::EmitEvent(const vr_hmi_api::ServiceMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  HmiEvent event(message);
  EventDispatcher<vr_hmi_api::ServiceMessage,
      vr_hmi_api::RPCName>::instance()->raise_event(event);
}

void VRModule::EmitEvent(const vr_mobile_api::ServiceMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  MobileEvent event(message);
  EventDispatcher<vr_mobile_api::ServiceMessage,
      vr_mobile_api::RPCName>::instance()->raise_event(event);
}

bool VRModule::SendToMobile(
    const vr_mobile_api::ServiceMessage& message) const {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t size = message.ByteSize();

  uint8_t* data = new uint8_t[size];

  utils::ScopeGuard data_guard = utils::MakeGuard(
      utils::ArrayDeleter<uint8_t*>, data);
  UNUSED(data_guard)

  if (!message.SerializeToArray(data, size)) {
    return false;
  }

  protocol_handler::RawMessagePtr messageToMobile =
      new protocol_handler::RawMessage(active_service_,
                                       protocol_handler::PROTOCOL_VERSION_2,
                                       data, size,
                                       protocol_handler::ServiceType::kVr);

  // TODO(VS): GenericModule may contain pointer to PluginManager(can be fixed if
  //           PluginManager will be redesigned from singleton)
  functional_modules::PluginManager::instance()
      ->SendMessageToRemoteMobileService(messageToMobile);

  return true;
}

void VRModule::RunCommand(commands::CommandPtr command) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (command) {
    command->Execute();
  }
}

bool VRModule::SendToHmi(const vr_hmi_api::ServiceMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  return proxy_.Send(message);
}

void VRModule::SubscribeToRpcMessages() {
  // TODO(VSemenyuk): Subscribe to rpc messages here
}

const PluginInfo& VRModule::GetPluginInfo() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return plugin_info_;
}

ProcessResult VRModule::ProcessMessage(application_manager::MessagePtr msg) {
  DCHECK(msg);

  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message received.");
    return ProcessResult::FAILED;
  }

  LOG4CXX_DEBUG(logger_, "Mobile message: " << msg->json_message());

  return ProcessResult::CANNOT_PROCESS;
}

ProcessResult VRModule::ProcessHMIMessage(application_manager::MessagePtr msg) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(msg);
  LOG4CXX_DEBUG(logger_, "HMI message: " << msg->json_message());
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message received.");
    return ProcessResult::FAILED;
  }

  // TODO(VSemenyuk): here should be implemented logic of the message processing

  return ProcessResult::PROCESSED;
}

void VRModule::RemoveAppExtension(uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::ApplicationSharedPtr app = service()->GetApplication(
        app_id);

  if (app) {
    app->RemoveExtension(GetModuleID());
  }
}

void VRModule::OnDeviceRemoved(const connection_handler::DeviceHandle& device) {
  LOG4CXX_AUTO_TRACE(logger_);
  // TODO(VSemenyuk): here should be implemented logic of the device removing
}

void VRModule::RemoveAppExtensions() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<application_manager::ApplicationSharedPtr> applications =
    service()->GetApplications(GetModuleID());

  std::vector<application_manager::ApplicationSharedPtr>::iterator it =
    applications.begin();

  for (; it != applications.end(); ++it) {
    application_manager::ApplicationSharedPtr app = *it;
    if (app) {
      app->RemoveExtension(GetModuleID());
    }
  }
}

bool VRModule::IsAppForPlugin(application_manager::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::AppExtensionPtr app_extension = app->QueryInterface(
    GetModuleID());
  if (app_extension) {
    return true;
  }

  // TODO(VSemenyuk): here should be implemented logic of the checking

  return false;
}

void VRModule::OnAppHMILevelChanged(application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Application " << app->name()
        << " has changed hmi level to " << app->hmi_level());
  // TODO(VSemenyuk): here should be implemented the corresponding logic
}

void VRModule::RegisterRequest(int32_t correlation_id,
                               commands::TimedCommand* command) {
  LOG4CXX_AUTO_TRACE(logger_);
  request_controller_.AddRequest(correlation_id, command);
}

void VRModule::UnregisterRequest(int32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  request_controller_.DeleteRequest(correlation_id);
}

void VRModule::ActivateService(int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  active_service_ = app_id;
}

void VRModule::DeactivateService() {
  LOG4CXX_AUTO_TRACE(logger_);
  active_service_ = 0;
}

bool VRModule::HasActivatedService() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return active_service_ != 0;
}

void VRModule::SetDefaultService(int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  default_service_ = app_id;
}

void VRModule::ResetDefaultService() {
  LOG4CXX_AUTO_TRACE(logger_);
  default_service_ = 0;
}

bool VRModule::IsDefaultService(int32_t app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return default_service_ == app_id;
}

void VRModule::RegisterService(int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  vr_hmi_api::ServiceMessage message;
  message.set_rpc(vr_hmi_api::ON_REGISTER);
  vr_hmi_api::OnRegisterServiceNotification notification;
  notification.set_appid(app_id);
  std::string params;
  if (notification.SerializeToString(&params)) {
    message.set_params(params);
  }
  commands::CommandPtr command = factory_->Create(message);
  RunCommand(command);
}

void VRModule::Handle(protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);

  vr_mobile_api::ServiceMessage service_message;
  if (service_message.ParseFromArray(
      message->data(), message->payload_size())) {
    OnReceived(service_message);
  } else {
    LOG4CXX_ERROR(logger_,
                  "Wrong message received from remote mobile service.");
    // TODO(VS): INVALID_DATA response must be sent
  }
}

void VRModule::ProcessMessageFromRemoteMobileService(
    const protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  messages_from_mobile_service_.PostMessage(message);
}

bool VRModule::OnServiceStartedCallback(const uint32_t& connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!IsSupported()) {
    return false;
  }

  RegisterService(connection_key);

  return true;
}

void VRModule::OnServiceEndedCallback(const uint32_t& connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  if ((static_cast<int32_t>(connection_key)) == active_service_) {
    DeactivateService();
  }

  // TODO(VSemenyuk): here should be implemented reaction on service stopping(need to notify HMI)
}

void VRModule::Start() {
  proxy_.Start();
}

void VRModule::Stop() {
  proxy_.Stop();
}

}  //  namespace vr_module
