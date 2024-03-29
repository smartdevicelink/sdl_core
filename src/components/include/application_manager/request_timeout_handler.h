/*
 * Copyright (c) 2020, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_REQUEST_TIMEOUT_HANDLER_H_
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_REQUEST_TIMEOUT_HANDLER_H_

#include <cstdint>

namespace application_manager {
namespace request_controller {

/*
 * @brief Structure for active mobile request
 * that is waiting for a response from the HMI.
 * Used for OnResetTimeout logic.
 */
struct Request {
  Request(const uint32_t mob_correlation_id,
          const uint32_t connection_key,
          const uint32_t hmi_function_id)
      : mob_correlation_id_(mob_correlation_id)
      , connection_key_(connection_key)
      , hmi_function_id_(hmi_function_id) {}

  uint32_t mob_correlation_id_;
  uint32_t connection_key_;
  uint32_t hmi_function_id_;
};

/**
 * @brief The RequestTimeoutHandler class
 * handles OnResetTimeout notification from HMI,
 * reset timeout for mobile request
 */
class RequestTimeoutHandler {
 public:
  /**
   * @brief AddRequest adds request that waits for response
   * @param hmi_correlation_id hmi correlation id
   * @param request active mobile request
   */
  virtual void AddRequest(const uint32_t hmi_correlation_id,
                          const Request& request) = 0;
  /**
   * @brief RemoveRequest removes processed request
   * @param hmi_correlation_id hmi correlation id
   */
  virtual void RemoveRequest(const uint32_t hmi_correlation_id) = 0;

  virtual ~RequestTimeoutHandler() {}
};

}  // namespace request_controller
}  // namespace application_manager

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_REQUEST_TIMEOUT_HANDLER_H_
