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

#include "app_service_rpc_plugin/commands/hmi/as_get_app_service_data_request_from_hmi.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/application_impl.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"

#include "application_manager/message_helper.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

ASGetAppServiceDataRequestFromHMI::ASGetAppServiceDataRequestFromHMI(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromHMI(message,
                     application_manager,
                     rpc_service,
                     hmi_capabilities,
                     policy_handler) {}

ASGetAppServiceDataRequestFromHMI::~ASGetAppServiceDataRequestFromHMI() {}

void ASGetAppServiceDataRequestFromHMI::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::string service_type =
      (*message_)[strings::msg_params][strings::service_type].asString();

  LOG4CXX_DEBUG(logger_, "Get Service Type: " << service_type);

  SendProviderRequest(mobile_apis::FunctionID::GetAppServiceDataID,
                      hmi_apis::FunctionID::AppService_GetAppServiceData,
                      &(*message_),
                      true);
}

void ASGetAppServiceDataRequestFromHMI::GetWeatherImagePaths(
    smart_objects::SmartObject& data, ApplicationSharedPtr app) {
  if (data[strings::location].keyExists(strings::location_image)) {
    MessageHelper::VerifyImage(data[strings::location][strings::location_image],
                               app,
                               application_manager_);
  }

  if (data.keyExists(strings::current_forecast) &&
      data[strings::current_forecast].keyExists(strings::weather_icon)) {
    MessageHelper::VerifyImage(
        data[strings::current_forecast][strings::weather_icon],
        app,
        application_manager_);
  }

  if (data.keyExists(strings::minute_forecast)) {
    smart_objects::SmartObject& minute_forecast =
        data[strings::minute_forecast];
    for (size_t i = 0; i < minute_forecast.length(); i++) {
      if (minute_forecast[i].keyExists(strings::weather_icon)) {
        MessageHelper::VerifyImage(minute_forecast[i][strings::weather_icon],
                                   app,
                                   application_manager_);
      }
    }
  }

  if (data.keyExists(strings::hourly_forecast)) {
    smart_objects::SmartObject& hourly_forecast =
        data[strings::hourly_forecast];
    for (size_t i = 0; i < hourly_forecast.length(); i++) {
      if (hourly_forecast[i].keyExists(strings::weather_icon)) {
        MessageHelper::VerifyImage(hourly_forecast[i][strings::weather_icon],
                                   app,
                                   application_manager_);
      }
    }
  }

  if (data.keyExists(strings::multiday_forecast)) {
    smart_objects::SmartObject& multiday_forecast =
        data[strings::multiday_forecast];
    for (size_t i = 0; i < multiday_forecast.length(); i++) {
      if (multiday_forecast[i].keyExists(strings::weather_icon)) {
        MessageHelper::VerifyImage(multiday_forecast[i][strings::weather_icon],
                                   app,
                                   application_manager_);
      }
    }
  }
}

void ASGetAppServiceDataRequestFromHMI::GetNavigationImagePaths(
    smart_objects::SmartObject& data, ApplicationSharedPtr app) {
  if (data.keyExists(strings::origin) &&
      data[strings::origin].keyExists(strings::location_image)) {
    MessageHelper::VerifyImage(data[strings::origin][strings::location_image],
                               app,
                               application_manager_);
  }

  if (data.keyExists(strings::destination) &&
      data[strings::destination].keyExists(strings::location_image)) {
    MessageHelper::VerifyImage(
        data[strings::destination][strings::location_image],
        app,
        application_manager_);
  }

  if (data.keyExists(strings::instructions)) {
    smart_objects::SmartObject& instructions = data[strings::instructions];
    for (size_t i = 0; i < instructions.length(); i++) {
      if (instructions[i].keyExists(strings::image)) {
        MessageHelper::VerifyImage(
            instructions[i][strings::image], app, application_manager_);
      }

      if (instructions[i].keyExists(strings::location_details) &&
          instructions[i][strings::location_details].keyExists(
              strings::location_image)) {
        MessageHelper::VerifyImage(
            instructions[i][strings::location_details][strings::location_image],
            app,
            application_manager_);
      }
    }
  }
}

void ASGetAppServiceDataRequestFromHMI::GetMediaImagePaths(
    smart_objects::SmartObject& data, ApplicationSharedPtr app) {
  if (data.keyExists(strings::media_image)) {
    MessageHelper::VerifyImage(
        data[strings::media_image], app, application_manager_);
  }
}

bool ASGetAppServiceDataRequestFromHMI::ValidateResponse(
    smart_objects::SmartObject& message_params) {
  if (!message_params.keyExists(strings::service_data)) {
    LOG4CXX_DEBUG(
        logger_,
        "GASD response received without any service data, passing through");
    return true;
  }
  smart_objects::SmartObject& service_data =
      message_params[strings::service_data];
  std::string service_type = service_data[strings::service_type].asString();
  mobile_apis::AppServiceType::eType service_type_value;
  const std::string& service_id = service_data[strings::service_id].asString();
  auto service =
      application_manager_.GetAppServiceManager().FindServiceByID(service_id);
  if (!service) {
    LOG4CXX_ERROR(logger_,
                  "GASD response received with an unpublished service ID");
    SendErrorResponse(
        correlation_id(),
        hmi_apis::FunctionID::AppService_GetAppServiceData,
        hmi_apis::Common_Result::GENERIC_ERROR,
        "The provider responded with incorrect data",
        application_manager::commands::Command::SOURCE_SDL_TO_HMI);
    return false;
  }

  using namespace ns_smart_device_link::ns_smart_objects;
  if (service && service->mobile_service &&
      EnumConversionHelper<mobile_apis::AppServiceType::eType>::StringToEnum(
          service_type, &service_type_value)) {
    auto app = application_manager_.application(service->connection_key);
    if (!app) {
      LOG4CXX_ERROR(logger_,
                    "Failed to find service provider for GASD response");
      SendErrorResponse(
          correlation_id(),
          hmi_apis::FunctionID::AppService_GetAppServiceData,
          hmi_apis::Common_Result::GENERIC_ERROR,
          "The provider responded with incorrect data",
          application_manager::commands::Command::SOURCE_SDL_TO_HMI);
      return false;
    }

    if (service_type_value == mobile_apis::AppServiceType::WEATHER &&
        service_data.keyExists(strings::weather_service_data)) {
      GetWeatherImagePaths(service_data[strings::weather_service_data], app);
    }

    if (service_type_value == mobile_apis::AppServiceType::NAVIGATION &&
        service_data.keyExists(strings::navigation_service_data)) {
      GetNavigationImagePaths(service_data[strings::navigation_service_data],
                              app);
    }

    if (service_type_value == mobile_apis::AppServiceType::MEDIA &&
        service_data.keyExists(strings::media_service_data)) {
      GetMediaImagePaths(service_data[strings::media_service_data], app);
    }
  }
  return true;
}

void ASGetAppServiceDataRequestFromHMI::on_event(
    const event_engine::Event& event) {
  smart_objects::SmartObject event_message(event.smart_object());

  auto& msg_params = event_message[strings::msg_params];

  hmi_apis::Common_Result::eType result =
      static_cast<hmi_apis::Common_Result::eType>(
          event_message[strings::params][hmi_response::code].asInt());
  bool success =
      IsHMIResultSuccess(result, HmiInterfaces::HMI_INTERFACE_AppService);
  if (ValidateResponse(msg_params)) {
    SendResponse(success,
                 correlation_id(),
                 hmi_apis::FunctionID::AppService_GetAppServiceData,
                 result,
                 &msg_params,
                 application_manager::commands::Command::SOURCE_SDL_TO_HMI);
  }
}

void ASGetAppServiceDataRequestFromHMI::on_event(
    const event_engine::MobileEvent& event) {
  smart_objects::SmartObject event_message(event.smart_object());

  auto& msg_params = event_message[strings::msg_params];

  mobile_apis::Result::eType mobile_result =
      static_cast<mobile_apis::Result::eType>(
          msg_params[strings::result_code].asInt());
  hmi_apis::Common_Result::eType result =
      MessageHelper::MobileToHMIResult(mobile_result);
  bool success = IsMobileResultSuccess(mobile_result);

  if (ValidateResponse(msg_params)) {
    SendResponse(success,
                 correlation_id(),
                 hmi_apis::FunctionID::AppService_GetAppServiceData,
                 result,
                 &msg_params,
                 application_manager::commands::Command::SOURCE_SDL_TO_HMI);
  }
}

void ASGetAppServiceDataRequestFromHMI::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendErrorResponse(correlation_id(),
                    hmi_apis::FunctionID::AppService_GetAppServiceData,
                    hmi_apis::Common_Result::GENERIC_ERROR,
                    "The provider did not respond to the request",
                    application_manager::commands::Command::SOURCE_SDL_TO_HMI);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin
