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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_GET_DEVICE_LIST_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_GET_DEVICE_LIST_REQUEST_H_

#include "application_manager/commands/hmi/request_to_hmi.h"
#include "application_manager/event_engine/event_observer.h"
#include "utils/lock.h"
#include "utils/conditional_variable.h"

namespace application_manager {

namespace commands {

/**
 * @brief UpdateDeviceListRequest command class
 **/
class UpdateDeviceListRequest : public RequestToHMI,
                                public event_engine::EventObserver {
 public:
  /**
   * @brief UpdateDeviceListRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit UpdateDeviceListRequest(const MessageSharedPtr& message);

  /**
   * @brief UpdateDeviceListRequest class destructor
   **/
  ~UpdateDeviceListRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   * Need to observe OnHMIReady event, to send UpdateDeviceListRequest
   * when HMI will be ready
   * @param event The received event
   */
  virtual void on_event(const event_engine::Event& event);

 /**
  * @brief Need to stop execution StopMethod if HMI did not started
  */
  virtual bool CleanUp();

 private:
  sync_primitives::Lock                            wait_hmi_lock;
  sync_primitives::ConditionalVariable             termination_condition_;

  DISALLOW_COPY_AND_ASSIGN(UpdateDeviceListRequest);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_GET_DEVICE_LIST_REQUEST_H_
