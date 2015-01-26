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
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {

OnVehicleDataNotification::OnVehicleDataNotification(
  const MessageSharedPtr& message)
  : CommandNotificationImpl(message) {
}

OnVehicleDataNotification::~OnVehicleDataNotification() {
}

void OnVehicleDataNotification::Run() {
  LOG4CXX_INFO(logger_, "OnVehicleDataNotification::Run");

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[strings::msg_params].keyExists(it->first)) {
      const std::vector<utils::SharedPtr<Application>>& applications =
            ApplicationManagerImpl::instance()->IviInfoUpdated(it->second,
                (*message_)[strings::msg_params][it->first].asInt());

      std::vector<utils::SharedPtr<Application>>::const_iterator it = applications.begin();
      for (; applications.end() != it; ++it) {
        utils::SharedPtr<Application> app = *it;
        if (!app) {
          LOG4CXX_ERROR_EXT(logger_, "NULL pointer");
          continue;
        }

        LOG4CXX_INFO(
          logger_,
          "Send OnVehicleData PRNDL notification to " << app->name()
          << " application id " << app->app_id());

        (*message_)[strings::params][strings::connection_key] = app->app_id();

        SendNotification();
      }

      return;
    }
  }
}

}  // namespace commands

}  // namespace application_manager
