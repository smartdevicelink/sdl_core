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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_CONTROL_EVENT_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_CONTROL_EVENT_H_

#include <string>

#include "application_manager/message.h"

#include "remote_control/event_engine/event.h"

namespace remote_control {

class RCPluginEvent
    : public rc_event_engine::Event<application_manager::MessagePtr,
                                    std::string> {
 public:
  /*
   * @brief Constructor with parameters
   *
   * @param id Event ID. (HMI or CAN function name)
   * @param message Message received in HMI or CAN response
   */
  RCPluginEvent(application_manager::MessagePtr& message,
                const std::string& id);

  /*
   * @brief Destructor
   */
  virtual ~RCPluginEvent();

  /*
   * @brief Retrieves event message request ID
   */
  virtual int32_t event_message_function_id() const;

  /*
   * @brief Retrieves event message correlation ID
   */
  virtual int32_t event_message_correlation_id() const;

  /*
   * @brief Retrieves event message response type
   */
  virtual int32_t event_message_type() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(RCPluginEvent);
};
}

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_CONTROL_EVENT_H_
