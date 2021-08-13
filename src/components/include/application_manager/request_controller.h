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

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_H_
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_H_

#include "application_manager/commands/command.h"

#include <memory>

namespace application_manager {
namespace request_controller {

typedef std::shared_ptr<commands::Command> RequestPtr;
/**
 * @brief RequestController class is used to control currently active mobile
 * requests.
 */
class RequestController {
 public:
  /**
   * @brief Result code for addRequest
   */
  enum class TResult {
    SUCCESS = 0,
    TOO_MANY_REQUESTS,
    TOO_MANY_PENDING_REQUESTS,
    NONE_HMI_LEVEL_MANY_REQUESTS,
    INVALID_DATA
  };

  /**
   * @brief Thread pool state
   */
  enum class TPoolState {
    UNDEFINED = 0,
    STARTED,
    STOPPED,
  };

  /**
   * @brief Class destructor
   */
  virtual ~RequestController() {}

  /**
   * @brief Stop request controller internal activities
   */
  virtual void Stop() = 0;

  /**
   * @brief Initialize thread pool
   */
  virtual void InitializeThreadpool() = 0;

  /**
   * @brief Destroy thread pool
   */
  virtual void DestroyThreadpool() = 0;

  /**
   * @brief Check if max request amount wasn't exceed and adds request to queue.
   * @param request Active request to mobile
   * @param hmi_level Current application hmi_level
   * @return Result code
   */
  virtual TResult AddMobileRequest(
      const RequestPtr request,
      const mobile_apis::HMILevel::eType& hmi_level) = 0;

  /**
   * @brief Store HMI request until response or timeout won't remove it
   * @param request Active request to hmi
   * @return Result code
   */
  virtual TResult AddHMIRequest(const RequestPtr request) = 0;

  /**
   * @brief Add notification to collection
   * @param ptr Reference to shared pointer that point on hmi notification
   */
  virtual void AddNotification(const RequestPtr ptr) = 0;

  /**
   * @brief Removes request from queue
   * @param correlation_id Active request correlation ID,
   * @param connection_key Active request connection key (0 for HMI requests)
   * @param function_id Active request function id
   * @param force_terminate if true, request controller will terminate
   * even if not allowed by request
   */
  virtual void TerminateRequest(const uint32_t correlation_id,
                                const uint32_t connection_key,
                                const int32_t function_id,
                                const bool force_terminate = false) = 0;

  /**
   * @brief Removes request from queue
   * @param mobile_correlation_id Active mobile request correlation ID
   * @param connection_key Active request connection key (0 for HMI requests)
   * @param function_id Active request function ID
   */
  virtual void OnMobileResponse(const uint32_t mobile_correlation_id,
                                const uint32_t connection_key,
                                const int32_t function_id) = 0;

  /**
   * @brief Removes request from queue
   * @param mobile_correlation_id Active mobile request correlation ID
   * @param function_id Active request function ID
   */
  virtual void OnHMIResponse(const uint32_t correlation_id,
                             const int32_t function_id) = 0;

  /**
   * @ Add notification to collection
   * @param notification Pointer that points to hmi notification
   */
  virtual void RemoveNotification(const commands::Command* notification) = 0;

  /**
   * @brief Removes all requests from queue for specified application
   * @param app_id Mobile application ID (app_id)
   */
  virtual void TerminateAppRequests(const uint32_t app_id) = 0;

  /**
   * @brief Terminates all requests from HMI
   */
  virtual void TerminateAllHMIRequests() = 0;

  /**
   * @brief Terminates all requests from Mobile
   */
  virtual void TerminateAllMobileRequests() = 0;

  /**
   * @brief Updates request timeout
   * @param app_id Connection key of application
   * @param mobile_correlation_id Correlation ID of the mobile request
   * @param new_timeout_value New timeout to be set in milliseconds
   */
  virtual void UpdateRequestTimeout(const uint32_t app_id,
                                    const uint32_t mobile_correlation_id,
                                    const uint32_t new_timeout) = 0;
  /**
   * @brief Function Should be called when Low Voltage is occured
   */
  virtual void OnLowVoltage() = 0;

  /**
   * @brief Function Should be called when WakeUp occures after Low Voltage
   */
  virtual void OnWakeUp() = 0;

  /**
   * @return true if low voltage state is active
   */
  virtual bool IsLowVoltage() = 0;
};

}  // namespace request_controller
}  // namespace application_manager

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_H_
