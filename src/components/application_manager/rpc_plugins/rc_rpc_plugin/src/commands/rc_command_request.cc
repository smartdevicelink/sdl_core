/*
 Copyright (c) 2018, Ford Motor Company
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

#include "rc_rpc_plugin/commands/rc_command_request.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "application_manager/message_helper.h"
#include "application_manager/hmi_interfaces.h"
#include "smart_objects/enum_schema_item.h"
#include "rc_rpc_plugin/interior_data_cache.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

namespace rc_rpc_plugin {
namespace commands {

RCCommandRequest::RCCommandRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager,
    InteriorDataCache& interior_data_cache)
    : application_manager::commands::CommandRequestImpl(message,
                                                        application_manager,
                                                        rpc_service,
                                                        hmi_capabilities,
                                                        policy_handle)
    , is_subscribed(false)
    , resource_allocation_manager_(resource_allocation_manager)
    , interior_data_cache_(interior_data_cache) {}

RCCommandRequest::~RCCommandRequest() {}

bool RCCommandRequest::IsInterfaceAvailable(
    const app_mngr::HmiInterfaces::InterfaceID interface) const {
  app_mngr::HmiInterfaces& hmi_interfaces =
      application_manager_.hmi_interfaces();
  const app_mngr::HmiInterfaces::InterfaceState state =
      hmi_interfaces.GetInterfaceState(interface);
  return app_mngr::HmiInterfaces::STATE_NOT_AVAILABLE != state;
}

void RCCommandRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();
  SetResourceState(module_type, ResourceState::FREE);
  SendResponse(
      false, mobile_apis::Result::GENERIC_ERROR, "Request timeout expired");
}

bool RCCommandRequest::CheckDriverConsent() {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  const std::string module_type = ModuleType();
  rc_rpc_plugin::TypeAccess access = CheckModule(module_type, app);

  if (rc_rpc_plugin::kAllowed == access) {
    set_auto_allowed(true);
    return true;
  }
  SendDisallowed(access);
  return false;
}

rc_rpc_plugin::TypeAccess RCCommandRequest::CheckModule(
    const std::string& module_type,
    application_manager::ApplicationSharedPtr app) {
  return policy_handler_.CheckModule(app->policy_app_id(), module_type)
             ? rc_rpc_plugin::TypeAccess::kAllowed
             : rc_rpc_plugin::TypeAccess::kDisallowed;
}

void RCCommandRequest::SendDisallowed(rc_rpc_plugin::TypeAccess access) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string info;
  if (rc_rpc_plugin::kDisallowed == access) {
    info = disallowed_info_.empty()
               ? "The RPC is disallowed by vehicle settings"
               : disallowed_info_;
  } else {
    return;
  }
  LOG4CXX_ERROR(logger_, info);
  SendResponse(false, mobile_apis::Result::DISALLOWED, info.c_str());
}

void RCCommandRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  if (!IsInterfaceAvailable(app_mngr::HmiInterfaces::HMI_INTERFACE_RC)) {
    LOG4CXX_WARN(logger_, "HMI interface RC is not available");
    SendResponse(false,
                 mobile_apis::Result::UNSUPPORTED_RESOURCE,
                 "Remote control is not supported by system");
    return;
  }
  LOG4CXX_TRACE(logger_, "RC interface is available!");
  if (!policy_handler_.CheckHMIType(
          app->policy_app_id(),
          mobile_apis::AppHMIType::eType::REMOTE_CONTROL,
          app->app_types())) {
    LOG4CXX_WARN(logger_, "Application has no remote control functions");
    SendResponse(false, mobile_apis::Result::DISALLOWED, "");
    return;
  }
  if (CheckDriverConsent()) {
    if (AcquireResources()) {
      Execute();  // run child's logic
    }
    // If resource is not aqcuired, AcquireResources method will either
    // send response to mobile or
    // send additional request to HMI to ask driver consent
  }
}

bool RCCommandRequest::AcquireResources() {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();

  if (!IsResourceFree(module_type)) {
    LOG4CXX_WARN(logger_, "Resource is busy.");
    SendResponse(false, mobile_apis::Result::IN_USE, "");
    return false;
  }

  AcquireResult::eType acquire_result = AcquireResource(message_);
  switch (acquire_result) {
    case AcquireResult::ALLOWED: {
      SetResourceState(module_type, ResourceState::BUSY);
      return true;
    }
    case AcquireResult::IN_USE: {
      SendResponse(false, mobile_apis::Result::IN_USE, "");
      return false;
    }
    case AcquireResult::ASK_DRIVER: {
      SendGetUserConsent(module_type);
      return false;
    }
    case AcquireResult::REJECTED: {
      SendResponse(false, mobile_apis::Result::REJECTED, "");
      return false;
    }
  }
  return false;
}

void RCCommandRequest::on_event(const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (event.id() == hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent) {
    ProcessAccessResponse(event);
  } else {
    const std::string module_type = ModuleType();
    SetResourceState(module_type, ResourceState::FREE);
  }
}

void RCCommandRequest::ProcessAccessResponse(
    const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  const std::string module_type = ModuleType();
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED, "");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();

  mobile_apis::Result::eType result_code =
      GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          message[app_mngr::strings::params][app_mngr::hmi_response::code]
              .asUInt()));

  const bool result =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS);

  bool is_allowed = false;
  if (result) {
    if (message[app_mngr::strings::msg_params].keyExists(
            message_params::kAllowed)) {
      is_allowed =
          message[app_mngr::strings::msg_params][message_params::kAllowed]
              .asBool();
    }
    if (is_allowed) {
      resource_allocation_manager_.ForceAcquireResource(module_type,
                                                        app->app_id());
      SetResourceState(module_type, ResourceState::BUSY);
      Execute();  // run child's logic
    } else {
      resource_allocation_manager_.OnDriverDisallowed(module_type,
                                                      app->app_id());
      SendResponse(
          false,
          mobile_apis::Result::REJECTED,
          "The resource is in use and the driver disallows this remote "
          "control RPC");
    }
  } else {
    std::string response_info;
    GetInfo(message, response_info);
    SendResponse(false, result_code, response_info.c_str());
  }
}

void RCCommandRequest::SendGetUserConsent(const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  DCHECK(app);
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[app_mngr::strings::app_id] = app->app_id();
  msg_params[message_params::kModuleType] = module_type;
  SendHMIRequest(hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent,
                 &msg_params,
                 true);
}
}
}
