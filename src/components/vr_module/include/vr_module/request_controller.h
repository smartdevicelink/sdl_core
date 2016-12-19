/**
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_REQUEST_CONTROLLER_H_
#define SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_REQUEST_CONTROLLER_H_

#include <map>

#include "vr_module/commands/timed_command.h"
#include "vr_module/vr_module_timer.h"

namespace vr_module {
namespace request_controller {

typedef uint32_t  correlation_id;

/**
 * @brief RequestController class is used to manage requests lifetime.
 */
class RequestController : public functional_modules::TimerObserver<TrackableMessage> {
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
   * @param correlation_id request correlation id
   * @param command pointer to request created in mobile factory
   */
  void AddRequest(const uint32_t& correlation_id,
                  commands::TimedCommandPtr request);

  /**
   * @brief Removes request
   * @param corellation_id request correlation id
   */
  void DeleteRequest(const uint32_t& correlation_id);

  void OnTimeoutTriggered(const TrackableMessage& expired);

  /**
   * @brief Update request timer from cur time
   * Use it if you want reset timet for some reason
   * @param corellation_id request correlation id
   */
  void ResetTimer(const uint32_t& correlation_id);

 private:
  typedef std::map<correlation_id, commands::TimedCommandPtr> RequestList;
  RequestList request_list_;
  functional_modules::ModuleTimer<TrackableMessage> timer_;
  DISALLOW_COPY_AND_ASSIGN(RequestController);
};

}  // namespace request_controller
}  // namespace vr_module

#endif  // SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_REQUEST_CONTROLLER_H_
