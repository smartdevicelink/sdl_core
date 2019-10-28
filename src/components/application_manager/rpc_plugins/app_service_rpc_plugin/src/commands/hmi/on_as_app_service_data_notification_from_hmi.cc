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

#include "app_service_rpc_plugin/commands/hmi/on_as_app_service_data_notification_from_hmi.h"

#include "application_manager/app_service_manager.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

OnASAppServiceDataNotificationFromHMI::OnASAppServiceDataNotificationFromHMI(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : NotificationFromHMI(message,
                          application_manager,
                          rpc_service,
                          hmi_capabilities,
                          policy_handler) {}

OnASAppServiceDataNotificationFromHMI::
    ~OnASAppServiceDataNotificationFromHMI() {}

void OnASAppServiceDataNotificationFromHMI::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Received an OnAppServiceData from HMI");

  std::string service_id =
      (*message_)[strings::msg_params][strings::service_data]
                 [strings::service_id]
                     .asString();
  AppService* service =
      application_manager_.GetAppServiceManager().FindServiceByID(service_id);
  if (!service) {
    LOG4CXX_ERROR(
        logger_, "No published services exist with service ID: " << service_id);
    return;
  } else if (service->mobile_service) {
    LOG4CXX_ERROR(logger_, "Service was not published by the HMI");
    return;
  } else if (!service->record[strings::service_active].asBool()) {
    LOG4CXX_ERROR(logger_, "Service is not active");
    return;
  }

  std::string service_type =
      (*message_)[strings::msg_params][strings::service_data]
                 [strings::service_type]
                     .asString();
  std::string published_service_type =
      service->record[strings::service_manifest][strings::service_type]
          .asString();
  if (published_service_type != service_type) {
    LOG4CXX_ERROR(logger_,
                  "Service type mismatch, expected "
                      << service_type
                      << ", but service was published with type "
                      << published_service_type);
    return;
  }

  SendNotificationToConsumers(
      mobile_apis::FunctionID::eType::OnAppServiceDataID);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin
