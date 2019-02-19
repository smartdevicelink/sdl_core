/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "app_service_rpc_plugin/app_service_mobile_command_factory.h"

#include "application_manager/message.h"
#include "interfaces/MOBILE_API.h"

#include "app_service_rpc_plugin/commands/mobile/on_app_service_data_notification.h"
#include "app_service_rpc_plugin/commands/mobile/on_app_service_data_notification_from_mobile.h"
#include "app_service_rpc_plugin/commands/mobile/publish_app_service_request.h"
#include "app_service_rpc_plugin/commands/mobile/publish_app_service_response.h"
#include "app_service_rpc_plugin/commands/mobile/get_app_service_data_request.h"
#include "app_service_rpc_plugin/commands/mobile/get_app_service_data_response.h"
#include "app_service_rpc_plugin/commands/mobile/get_app_service_data_request_to_mobile.h"
#include "app_service_rpc_plugin/commands/mobile/get_app_service_data_response_from_mobile.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceRpcPlugin")

namespace app_service_rpc_plugin {
namespace strings = app_mngr::strings;

AppServiceMobileCommandFactory::AppServiceMobileCommandFactory(
    application_manager::ApplicationManager& application_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : application_manager_(application_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {
  LOG4CXX_AUTO_TRACE(logger_);
}

app_mngr::CommandSharedPtr AppServiceMobileCommandFactory::CreateCommand(
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

  return buildCommandCreator(function_id, message_type, source).create(message);
}

bool AppServiceMobileCommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const app_mngr::commands::Command::CommandSource source) const {
  UNUSED(source);
  return buildCommandCreator(function_id,
                             mobile_apis::messageType::INVALID_ENUM,
                             source).CanBeCreated();
}

app_mngr::CommandCreator& AppServiceMobileCommandFactory::buildCommandCreator(
    const int32_t function_id,
    const int32_t message_type,
    const app_mngr::commands::Command::CommandSource source) const {
  auto factory = app_mngr::CommandCreatorFactory(
      application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);

  switch (function_id) {
    case mobile_apis::FunctionID::PublishAppServiceID:
      return mobile_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::PublishAppServiceRequest>()
                 : factory.GetCreator<commands::PublishAppServiceResponse>();
    case mobile_apis::FunctionID::OnAppServiceDataID:
      return app_mngr::commands::Command::CommandSource::SOURCE_MOBILE == source
                 ? factory.GetCreator<
                       commands::OnAppServiceDataNotificationFromMobile>()
                 : factory.GetCreator<commands::OnAppServiceDataNotification>();
    case mobile_apis::FunctionID::GetAppServiceDataID:
      if (app_mngr::commands::Command::CommandSource::SOURCE_MOBILE == source) {
        return mobile_apis::messageType::request == message_type
                   ? factory.GetCreator<commands::GetAppServiceDataRequest>()
                   : factory.GetCreator<
                         commands::GetAppServiceDataResponseFromMobile>();
      } else if (app_mngr::commands::Command::CommandSource::SOURCE_SDL ==
                 source) {
        return mobile_apis::messageType::request == message_type
                   ? factory.GetCreator<
                         commands::GetAppServiceDataRequestToMobile>()
                   : factory.GetCreator<commands::GetAppServiceDataResponse>();
      }
    default:
      LOG4CXX_WARN(logger_, "Unsupported function_id: " << function_id);
      return factory.GetCreator<app_mngr::InvalidCommand>();
  }
}
}  // namespace vehicle_info_plugin
