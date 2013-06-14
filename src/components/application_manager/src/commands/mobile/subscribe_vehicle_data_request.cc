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

#include <string>
#include "application_manager/commands/mobile/subscribe_vehicle_data_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/Result.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

namespace str = strings;

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

SubscribeVehicleDataRequest::SubscribeVehicleDataRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

SubscribeVehicleDataRequest::~SubscribeVehicleDataRequest() {
}

void SubscribeVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "SubscribeVehicleDataRequest::Run ");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[str::params][str::connection_key]));

  std::string info = "";
  bool result = false;
  NsSmartDeviceLinkRPC::V2::Result::eType result_code =
      NsSmartDeviceLinkRPC::V2::Result::INVALID_DATA;

  do {
    if (NULL == app) {
      LOG4CXX_ERROR_EXT(logger_, "APPLICATION_NOT_REGISTERED");
      result_code =
          NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED;
      break;
    }

    size_t length = (*message_)[str::msg_params][str::data_type].length();
    int item_count = 0;
    for (int i = 0; i < length; ++i) {
      if (app->SubscribeToIVI(static_cast<unsigned int>(
          (*message_)[str::msg_params][str::data_type][i].asInt()))) {
        ++item_count;
      }
    }

    if (item_count == length) {
      result = true;
      result_code = NsSmartDeviceLinkRPC::V2::Result::SUCCESS;
    } else if (0 == item_count) {
      result_code = NsSmartDeviceLinkRPC::V2::Result::REJECTED;
      info = "Already subscribed on all VehicleData";
    } else if (item_count < length) {
      result_code = NsSmartDeviceLinkRPC::V2::Result::WARNINGS;
      info = "Already subscribed on some VehicleData";
    } else {
      LOG4CXX_ERROR_EXT(logger_, "Unknown command sequence!");
      break;
    }
  } while (0);

  SendResponse(result, result_code, info.c_str());
}

}  // namespace commands

}  // namespace application_manager
