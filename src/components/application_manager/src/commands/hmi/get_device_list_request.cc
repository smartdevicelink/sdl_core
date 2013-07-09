/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/hmi/get_device_list_request.h"
#include "application_manager/application_manager_impl.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

GetDeviceListRequest::GetDeviceListRequest(
    const MessageSharedPtr& message): RequestFromHMI(message) {
}

GetDeviceListRequest::~GetDeviceListRequest() {
}

void GetDeviceListRequest::Run() {
  LOG4CXX_INFO(logger_, "GetDeviceListRequest::Run");

  (*message_)[strings::params][strings::message_type] = MessageType::kResponse;

  const std::set<connection_handler::Device>& devices =
      ApplicationManagerImpl::instance()->device_list();

  int index = 0;

  if (devices.empty())  {
    (*message_)[strings::msg_params][hmi_response::code] =
          hmi_apis::Common_Result::eType::NO_DEVICES_CONNECTED;
  } else {
    (*message_)[strings::msg_params][hmi_response::code] =
          hmi_apis::Common_Result::eType::SUCCESS;

    for (std::set<connection_handler::Device>::iterator it = devices.begin();
        devices.end() != it; ++it) {
      (*message_)[strings::msg_params]
                  [strings::device_list]
                   [index][strings::name] = (*it).user_friendly_name();
      (*message_)[strings::msg_params]
                  [strings::device_list]
                   [index][strings::id] = (*it).device_handle();
      ++index;
    }
  }

  SendResponseToHMI();
}

}  // namespace commands

}  // namespace application_manager

