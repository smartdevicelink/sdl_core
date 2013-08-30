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

#include "application_manager/commands/mobile/unsubscribe_vehicle_data_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace commands {

namespace str = strings;

UnsubscribeVehicleDataRequest::UnsubscribeVehicleDataRequest(
  const MessageSharedPtr& message): CommandRequestImpl(message) {
}

UnsubscribeVehicleDataRequest::~UnsubscribeVehicleDataRequest() {
}

void UnsubscribeVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "UnsubscribeVehicleDataRequest::Run");

  int app_id = (*message_)[strings::params][strings::connection_key];
  Application* app =
    ApplicationManagerImpl::instance()->
    application(app_id);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  // counter for items to subscribe
  int items_to_unsubscribe = 0;
  // counter for subscribed items by application
  int unsubscribed_items = 0;
  // response params
  namespace NsSmart = NsSmartDeviceLink::NsSmartObjects;
  NsSmart::SmartObject response_params;

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[str::msg_params].keyExists(it->first) &&
        true == (*message_)[str::msg_params][it->first].asBool()) {
      ++items_to_unsubscribe;
      response_params[it->first][strings::data_type] = it->second;

      if (app->UnsubscribeFromIVI(static_cast<unsigned int>(it->second))) {
        ++unsubscribed_items;
        response_params[it->first][strings::result_code] =
          mobile_apis::VehicleDataResultCode::VDRC_SUCCESS;
      } else {
        response_params[it->first][strings::result_code] =
          mobile_apis::VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
      }
    }
  }

  if (0 == items_to_unsubscribe) {
    SendResponse(false, mobile_apis::Result::VEHICLE_DATA_NOT_AVAILABLE,
                 "Provided VehicleData is empty", &response_params);
  } else if (unsubscribed_items == items_to_unsubscribe) {
    SendResponse(true, mobile_apis::Result::SUCCESS,
                 "Unsubscribed on provided VehicleData", &response_params);
  } else if (0 == unsubscribed_items) {
    SendResponse(false, mobile_apis::Result::IGNORED,
                 "Was not subscribed on any VehicleData", &response_params);
  } else if (unsubscribed_items < items_to_unsubscribe) {
    SendResponse(false, mobile_apis::Result::WARNINGS,
                 "Was subscribed not to all VehicleData", &response_params);
  } else {
    LOG4CXX_ERROR(logger_, "Unknown command sequence!");
    return;
  }
}

}  // namespace commands

}  // namespace application_manager
