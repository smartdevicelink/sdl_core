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

#ifndef HMI_VI_GETBSCRIBE_VEHICLE_DATA_REQUEST_TEMPLATE_H_
#define HMI_VI_GETBSCRIBE_VEHICLE_DATA_REQUEST_TEMPLATE_H_

#include "application_manager/event_engine/event.h"
#include "application_manager/commands/hmi/request_to_hmi.h"

namespace application_manager {
namespace commands {

/**
 * @brief VIGetVehicleDataRequestTemplate command class
 *
 * Template class for sending 1 subscribe thin request
 **/
template<event_engine::Event::EventID eventID>
class VIGetVehicleDataRequestTemplate : public RequestToHMI {
 public:
  /**
   * @brief VIGetVehicleDataRequestTemplate class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit VIGetVehicleDataRequestTemplate(const MessageSharedPtr& message)
      : RequestToHMI(message) {
  }

  /**
   * @brief Execute command with sending DBus thin request to HMI
   **/
  virtual void Run() {
    LOG4CXX_INFO(logger_, "VIGetVehicleDataRequestTemplate::Run");
    SendRequest();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(VIGetVehicleDataRequestTemplate<eventID>);
};

}  // namespace commands
}  // namespace application_manager
#endif  // HMI_VI_GETBSCRIBE_VEHICLE_DATA_REQUEST_TEMPLATE_H_
