/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Copyright (c) 2017 Xevo Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "rc_rpc_plugin/commands/mobile/button_press_response.h"
#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/commands/hmi/rc_button_press_request.h"
#include "rc_rpc_plugin/commands/hmi/rc_button_press_response.h"
#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/commands/hmi/rc_on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/commands/hmi/rc_on_remote_control_settings_notification.h"
#include "rc_rpc_plugin/commands/hmi/rc_set_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/commands/hmi/rc_set_interior_vehicle_data_response.h"

#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

#include "rc_rpc_plugin/resource_allocation_manager.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")
namespace application_manager {
using rc_rpc_plugin::ResourceAllocationManager;

template <typename RCCommandType>
class RCCommandCreator : public ICommandCreator {
 public:
  RCCommandCreator(ResourceAllocationManager& resource_allocation_manager,
                   ApplicationManager& application_manager,
                   rpc_service::RPCService& rpc_service,
                   HMICapabilities& hmi_capabilities,
                   PolicyHandlerInterface& policy_handler)
      : resource_allocation_manager_(resource_allocation_manager)
      , application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler)
      , able_(true) {}

 private:
  bool isAble() const override;
  CommandSharedPtr create(
      const commands::MessageSharedPtr& message) const override {
    CommandSharedPtr command(new RCCommandType(resource_allocation_manager_,
                                               message,
                                               application_manager_,
                                               rpc_service_,
                                               hmi_capabilities_,
                                               policy_handler_));
    return command;
  }

  ResourceAllocationManager& resource_allocation_manager_;
  ApplicationManager& application_manager_;
  RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
  bool able_;
};

struct RCCommandCreatorFacotry {
  RCCommandCreatorFacotry(
      ResourceAllocationManager& resource_allocation_manager,
      ApplicationManager& application_manager,
      rpc_service::RPCService& rpc_service,
      HMICapabilities& hmi_capabilities,
      PolicyHandlerInterface& policy_handler)
      : resource_allocation_manager_(resource_allocation_manager)
      , application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler) {}

  template <typename RCCommandType>
  ICommandCreator& GetCreator() {
    static RCCommandCreator<RCCommandType> res(resource_allocation_manager_,
                                               application_manager_,
                                               rpc_service_,
                                               hmi_capabilities_,
                                               policy_handler_);
    return res;
  }
  ResourceAllocationManager& resource_allocation_manager_;
  ApplicationManager& application_manager_;
  RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
};
}

namespace rc_rpc_plugin {
using namespace application_manager;

RCCommandFactory::RCCommandFactory(
    app_mngr::ApplicationManager& app_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    ResourceAllocationManager& allocation_manager)
    : app_manager_(app_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler)
    , allocation_manager_(allocation_manager) {}

CommandSharedPtr RCCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  if (app_mngr::commands::Command::SOURCE_HMI == source) {
    mobile_apis::messageType::eType message_type =
        static_cast<mobile_apis::messageType::eType>(
            (*message)[strings::params][strings::message_type].asInt());

    mobile_apis::FunctionID::eType function_id =
        static_cast<mobile_apis::FunctionID::eType>(
            (*message)[strings::params][strings::function_id].asInt());

    return get_mobile_creator_factory(function_id, message_type)
        .create(message);
  } else {
    hmi_apis::messageType::eType message_type =
        static_cast<hmi_apis::messageType::eType>(
            (*message)[strings::params][strings::message_type].asInt());

    hmi_apis::FunctionID::eType function_id =
        static_cast<hmi_apis::FunctionID::eType>(
            (*message)[strings::params][strings::function_id].asInt());

    return get_hmi_creator_factory(function_id, message_type).create(message);
  }
}

bool RCCommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource message_source)
    const {
  using app_mngr::commands::Command;
  if (Command::SOURCE_HMI == message_source) {
    return get_mobile_creator_factory(
               static_cast<mobile_api::FunctionID::eType>(function_id),
               mobile_api::messageType::INVALID_ENUM).isAble();
  } else {
    return get_hmi_creator_factory(
               static_cast<hmi_apis::FunctionID::eType>(function_id),
               hmi_apis::messageType::INVALID_ENUM).isAble();
  }
}

ICommandCreator& RCCommandFactory::get_mobile_creator_factory(
    mobile_api::FunctionID::eType id,
    mobile_api::messageType::eType message_type) const {
  LOG4CXX_DEBUG(logger_, "CreateMobileCommand function_id: " << id);
  RCCommandCreatorFacotry rc_factory(allocation_manager_,
                                     app_manager_,
                                     rpc_service_,
                                     hmi_capabilities_,
                                     policy_handler_);
  CommandCreatorFacotry factory(
      app_manager_, rpc_service_, hmi_capabilities_, policy_handler_);

  switch (id) {
    case mobile_apis::FunctionID::ButtonPressID: {
      return mobile_api::messageType::request == message_type
                 ? rc_factory.GetCreator<commands::ButtonPressRequest>()
                 : factory.GetCreator<commands::ButtonPressResponse>();
    }
    case mobile_apis::FunctionID::GetInteriorVehicleDataID: {
      return mobile_api::messageType::request == message_type
                 ? rc_factory
                       .GetCreator<commands::GetInteriorVehicleDataRequest>()
                 : factory
                       .GetCreator<commands::GetInteriorVehicleDataResponse>();
    }
    case mobile_apis::FunctionID::SetInteriorVehicleDataID: {
      return mobile_api::messageType::request == message_type
                 ? rc_factory
                       .GetCreator<commands::SetInteriorVehicleDataRequest>()
                 : factory
                       .GetCreator<commands::SetInteriorVehicleDataResponse>();
    }
    case mobile_apis::FunctionID::OnInteriorVehicleDataID: {
      return factory.GetCreator<commands::OnInteriorVehicleDataNotification>();
    }
    default: { return factory.GetCreator<InvalidCommand>(); }
  }
}

ICommandCreator& RCCommandFactory::get_hmi_creator_factory(
    hmi_apis::FunctionID::eType id,
    hmi_apis::messageType::eType message_type) const {
  LOG4CXX_DEBUG(logger_, "CreateHMICommand function_id: " << id);
  CommandCreatorFacotry factory(
      app_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
  RCCommandCreatorFacotry rc_factory(allocation_manager_,
                                     app_manager_,
                                     rpc_service_,
                                     hmi_capabilities_,
                                     policy_handler_);

  switch (id) {
    case hmi_apis::FunctionID::Buttons_ButtonPress: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::RCButtonPressRequest>()
                 : factory.GetCreator<commands::RCButtonPressResponse>();
    }
    case hmi_apis::FunctionID::RC_GetInteriorVehicleData: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::RCGetInteriorVehicleDataRequest>()
                 : factory.GetCreator<
                       commands::RCGetInteriorVehicleDataResponse>();
    }
    case hmi_apis::FunctionID::RC_SetInteriorVehicleData: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::RCSetInteriorVehicleDataRequest>()
                 : factory.GetCreator<
                       commands::RCSetInteriorVehicleDataResponse>();
    }
    case hmi_apis::FunctionID::RC_OnInteriorVehicleData: {
      return factory
          .GetCreator<commands::RCOnInteriorVehicleDataNotification>();
    }
    case hmi_apis::FunctionID::RC_OnRemoteControlSettings: {
      return rc_factory
          .GetCreator<commands::RCOnRemoteControlSettingsNotification>();
    }
    default: { return factory.GetCreator<InvalidCommand>(); }
  }
}
}
