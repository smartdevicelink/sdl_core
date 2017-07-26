/**
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REQUEST_CONTROLLER_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REQUEST_CONTROLLER_H_

#include <map>

#include "remote_control/commands/command.h"
#include "remote_control/rc_module_timer.h"
#include "functional_module/timer/timer_director.h"

namespace remote_control {

namespace commands {
class Command;
}

namespace request_controller {

typedef utils::SharedPtr<commands::Command> MobileRequestPtr;
typedef uint32_t correlation_id;

/**
 * @brief RequestController class is used to manage mobile requests lifetime.
 */
class RequestController
    : public functional_modules::TimerObserver<TrackableMessage> {
 public:
  /**
  * @brief Class constructor
  *
  */
  RequestController();

  /**
  * @brief Class destructor
  *
  */
  virtual ~RequestController();

  /**
   * @brief Adds pointer to request.
   * @param mobile_correlation_id mobile request correlation id
   * @param command pointer to request created in mobile factory
   */
  void AddRequest(const uint32_t mobile_correlation_id,
                  MobileRequestPtr request);

  /**
   * @brief Removes request
   * @param mobile_corellation_id mobile request correlation id
   */
  void DeleteRequest(const uint32_t& mobile_correlation_id);

  void OnTimeoutTriggered(const TrackableMessage& expired);

 private:
  DISALLOW_COPY_AND_ASSIGN(RequestController);

  std::map<correlation_id, MobileRequestPtr> mobile_request_list_;
  functional_modules::ModuleTimer<TrackableMessage> timer_;
  functional_modules::TimerDirector time_director_;
};

}  // namespace request_controller
}  // namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REQUEST_CONTROLLER_H_
