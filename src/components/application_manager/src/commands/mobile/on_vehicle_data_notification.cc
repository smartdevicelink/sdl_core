/*

 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/mobile/on_vehicle_data_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/v4_protocol_v2_0_revT.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

OnVehicleDataNotification::OnVehicleDataNotification(
    const MessageSharedPtr& message): CommandNotificationImpl(message) {
}

OnVehicleDataNotification::~OnVehicleDataNotification() {
}

void OnVehicleDataNotification::Run() {
  LOG4CXX_INFO(logger_, "OnVehicleDataNotification::Run ");

  if ((*message_)[strings::msg_params].keyExists(hmi_notification::prndl)) {
    const unsigned int prndl = static_cast<unsigned int>(
        (*message_)[strings::msg_params][hmi_notification::prndl].asInt());

    const std::vector<Application*>& applications =
        ApplicationManagerImpl::instance()->applications_by_ivi(prndl);

    std::vector<Application*>::const_iterator it = applications.begin();
    for (; applications.end() != it; ++it) {
      ApplicationImpl* app = static_cast<ApplicationImpl*>(*it);
      if (!app) {
          LOG4CXX_ERROR_EXT(logger_, "NULL pointer");
          continue;
      }

      LOG4CXX_INFO(logger_, "Send OnVehicleData PRNDL notification to "
                     << app->name() << " application id " << app->app_id());
      SendVehicleData(app);
    }
  }
}

void OnVehicleDataNotification::SendVehicleData(const ApplicationImpl* app) {
  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "SendVehicleData NULL pointer");
    return;
  }

  smart_objects::CSmartObject* on_vehicle_data =
      new smart_objects::CSmartObject();

  if (!on_vehicle_data) {
    LOG4CXX_ERROR_EXT(logger_, "SendVehicleData NULL pointer");
    return;
  }

  const int correlation_id =
    (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
    (*message_)[strings::params][strings::connection_key];

  (*on_vehicle_data)[strings::params][strings::message_type] =
    MessageType::kNotification;
  (*on_vehicle_data)[strings::params][strings::correlation_id] =
    correlation_id;

  (*on_vehicle_data)[strings::params][strings::app_id] =
    app->app_id();

  (*on_vehicle_data)[strings::params][strings::connection_key] =
    connection_key;
  (*on_vehicle_data)[strings::params][strings::function_id] =
    NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnVehicleDataID;
  (*on_vehicle_data)[strings::msg_params][strings::prndl] =
    (*message_)[strings::msg_params][hmi_notification::prndl];

  (*on_vehicle_data)[strings::msg_params][strings::success] = true;
  (*on_vehicle_data)[strings::msg_params][strings::result_code] =
    NsSmartDeviceLinkRPC::V2::Result::SUCCESS;

  message_.reset(on_vehicle_data);
  SendNotification();
}

}  // namespace commands

}  // namespace application_manager
