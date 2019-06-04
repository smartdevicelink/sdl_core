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

#include "vehicle_info_plugin/vehicle_info_mobile_command_factory.h"

#include "application_manager/message.h"
#include "interfaces/MOBILE_API.h"
#include "vehicle_info_plugin/vehicle_info_command_params.h"

#include "vehicle_info_plugin/custom_vehicle_data_manager.h"

#include "vehicle_info_plugin/commands/mobile/diagnostic_message_request.h"
#include "vehicle_info_plugin/commands/mobile/diagnostic_message_response.h"
#include "vehicle_info_plugin/commands/mobile/get_dtcs_request.h"
#include "vehicle_info_plugin/commands/mobile/get_dtcs_response.h"
#include "vehicle_info_plugin/commands/mobile/get_vehicle_data_request.h"
#include "vehicle_info_plugin/commands/mobile/get_vehicle_data_response.h"
#include "vehicle_info_plugin/commands/mobile/on_vehicle_data_notification.h"
#include "vehicle_info_plugin/commands/mobile/read_did_request.h"
#include "vehicle_info_plugin/commands/mobile/read_did_response.h"
#include "vehicle_info_plugin/commands/mobile/subscribe_vehicle_data_request.h"
#include "vehicle_info_plugin/commands/mobile/subscribe_vehicle_data_response.h"
#include "vehicle_info_plugin/commands/mobile/unsubscribe_vehicle_data_request.h"
#include "vehicle_info_plugin/commands/mobile/unsubscribe_vehicle_data_response.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPlugin")

namespace vehicle_info_plugin {
namespace strings = app_mngr::strings;

template <typename VehicleInfoCommandType>
class VehicleInfoCommandCreator : public application_manager::CommandCreator {
 public:
  VehicleInfoCommandCreator(const VehicleInfoCommandParams& params)
      : params_(params) {}

 private:
  bool CanBeCreated() const override {
    return true;
  }

  application_manager::CommandSharedPtr create(
      const application_manager::commands::MessageSharedPtr& message)
      const override {
    application_manager::CommandSharedPtr command(
        new VehicleInfoCommandType(message, params_));
    return command;
  }

  VehicleInfoCommandParams params_;
};

struct VehicleInfoInvalidCommand {};

template <>
class VehicleInfoCommandCreator<VehicleInfoInvalidCommand>
    : public application_manager::CommandCreator {
 public:
  VehicleInfoCommandCreator(const VehicleInfoCommandParams& params) {
    UNUSED(params);
  }

 private:
  bool CanBeCreated() const override {
    return false;
  }

  application_manager::CommandSharedPtr create(
      const application_manager::commands::MessageSharedPtr& message)
      const override {
    UNUSED(message);
    return application_manager::CommandSharedPtr();
  }
};

struct VehicleInfoCommandCreatorFactory {
  VehicleInfoCommandCreatorFactory(const VehicleInfoCommandParams& params)
      : params_(params) {}

  template <typename VehicleInfoCommandType>
  application_manager::CommandCreator& GetCreator() {
    LOG4CXX_AUTO_TRACE(logger_);
    static VehicleInfoCommandCreator<VehicleInfoCommandType> res(params_);
    return res;
  }
  const VehicleInfoCommandParams params_;
};

VehicleInfoMobileCommandFactory::VehicleInfoMobileCommandFactory(
    application_manager::ApplicationManager& application_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    CustomVehicleDataManager& custom_vehicle_data_manager)
    : application_manager_(application_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler)
    , custom_vehicle_data_manager_(custom_vehicle_data_manager) {
  LOG4CXX_AUTO_TRACE(logger_);
}

app_mngr::CommandSharedPtr VehicleInfoMobileCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  UNUSED(source);

  const mobile_apis::FunctionID::eType function_id =
      static_cast<mobile_apis::FunctionID::eType>(
          (*message)[strings::params][strings::function_id].asInt());

  const mobile_apis::messageType::eType message_type =
      static_cast<mobile_apis::messageType::eType>(
          (*message)[strings::params][strings::message_type].asInt());

  auto message_type_str = "request";
  if (mobile_apis::messageType::response == message_type) {
    message_type_str = "response";
  } else if (mobile_apis::messageType::notification == message_type) {
    message_type_str = "notification";
  }

  UNUSED(message_type_str);
  LOG4CXX_DEBUG(logger_,
                "HMICommandFactory::CreateCommand function_id: "
                    << function_id << ", message type: " << message_type_str);

  return get_creator_factory(function_id, message_type, source).create(message);
}

bool VehicleInfoMobileCommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const app_mngr::commands::Command::CommandSource source) const {
  UNUSED(source);
  auto id = static_cast<mobile_apis::FunctionID::eType>(function_id);
  return get_command_creator(id, mobile_apis::messageType::INVALID_ENUM)
             .CanBeCreated() ||
         get_notification_creator(id).CanBeCreated();
}

app_mngr::CommandCreator& VehicleInfoMobileCommandFactory::get_command_creator(
    const mobile_apis::FunctionID::eType id,
    const mobile_apis::messageType::eType message_type) const {
  VehicleInfoCommandParams params = {application_manager_,
                                     rpc_service_,
                                     hmi_capabilities_,
                                     policy_handler_,
                                     custom_vehicle_data_manager_};
  auto factory = VehicleInfoCommandCreatorFactory(params);
  switch (id) {
    case mobile_apis::FunctionID::GetVehicleDataID: {
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::GetVehicleDataRequest>()
                 : factory.GetCreator<commands::GetVehicleDataResponse>();
    }
    case mobile_apis::FunctionID::SubscribeVehicleDataID: {
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::SubscribeVehicleDataRequest>()
                 : factory.GetCreator<commands::SubscribeVehicleDataResponse>();
    }
    case mobile_apis::FunctionID::UnsubscribeVehicleDataID: {
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UnsubscribeVehicleDataRequest>()
                 : factory
                       .GetCreator<commands::UnsubscribeVehicleDataResponse>();
    }
    case mobile_apis::FunctionID::ReadDIDID: {
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::ReadDIDRequest>()
                 : factory.GetCreator<commands::ReadDIDResponse>();
    }
    case mobile_apis::FunctionID::GetDTCsID: {
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::GetDTCsRequest>()
                 : factory.GetCreator<commands::GetDTCsResponse>();
    }
    case mobile_apis::FunctionID::DiagnosticMessageID: {
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::DiagnosticMessageRequest>()
                 : factory.GetCreator<commands::DiagnosticMessageResponse>();
    }
    default: {}
  }
  return factory.GetCreator<VehicleInfoInvalidCommand>();
}

app_mngr::CommandCreator&
VehicleInfoMobileCommandFactory::get_notification_creator(
    const mobile_apis::FunctionID::eType id) const {
  VehicleInfoCommandParams params = {application_manager_,
                                     rpc_service_,
                                     hmi_capabilities_,
                                     policy_handler_,
                                     custom_vehicle_data_manager_};
  auto factory = VehicleInfoCommandCreatorFactory(params);
  switch (id) {
    case mobile_apis::FunctionID::OnVehicleDataID: {
      return factory.GetCreator<commands::OnVehicleDataNotification>();
    }
    default: {}
  }
  return factory.GetCreator<VehicleInfoInvalidCommand>();
}

app_mngr::CommandCreator& VehicleInfoMobileCommandFactory::get_creator_factory(
    const mobile_apis::FunctionID::eType id,
    const mobile_apis::messageType::eType message_type,
    const app_mngr::commands::Command::CommandSource source) const {
  switch (message_type) {
    case mobile_apis::messageType::request: {
      if (app_mngr::commands::Command::CommandSource::SOURCE_MOBILE == source) {
        return get_command_creator(id, message_type);
      }
      break;
    }
    case mobile_apis::messageType::response: {
      if (app_mngr::commands::Command::CommandSource::SOURCE_SDL == source) {
        return get_command_creator(id, message_type);
      }
      break;
    }
    case mobile_apis::messageType::notification: {
      if (app_mngr::commands::Command::CommandSource::SOURCE_SDL == source) {
        return get_notification_creator(id);
      }
      break;
    }
    default: {}
  }
  VehicleInfoCommandParams params = {application_manager_,
                                     rpc_service_,
                                     hmi_capabilities_,
                                     policy_handler_,
                                     custom_vehicle_data_manager_};
  auto factory = VehicleInfoCommandCreatorFactory(params);
  return factory.GetCreator<VehicleInfoInvalidCommand>();
}
}  // namespace vehicle_info_plugin
