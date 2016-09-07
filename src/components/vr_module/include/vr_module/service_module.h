/*
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

#ifndef SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_SERVICE_MODULE_H_
#define SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_SERVICE_MODULE_H_

#include <stdint.h>
#include "vr_module/commands/timed_command.h"

namespace vr_hmi_api {
class ServiceMessage;
}  // namespace vr_hmi_api

namespace vr_mobile_api {
class ServiceMessage;
}  // namespace vr_mobile_api

namespace vr_module {
class ServiceModule {
 public:
  virtual ~ServiceModule() {
  }

  /**
   * Returns unique correlation ID for request to mobile
   * @return Unique correlation ID
   */
  virtual int32_t GetNextCorrelationID() = 0;

  /**
   * Registers request to HMI or Mobile side
   * @param correlation_id unique id of request
   */
  virtual void RegisterRequest(int32_t correlation_id,
                               commands::TimedCommand* command) = 0;

  /**
   * Unregisters request to HMI or Mobile side
   * @param correlation_id unique id of request
   */
  virtual void UnregisterRequest(int32_t correlation_id) = 0;

  /**
   * Sends message to HMI (Applink)
   * @param message is GPB message according with protocol
   * @return true if message was sent successful
   */
  virtual bool SendToHmi(const vr_hmi_api::ServiceMessage& message) = 0;

  /**
   * Sends message to Mobile side
   * @param message is GPB message according with protocol
   * @return true if message was sent successful
   */
  virtual bool SendToMobile(const vr_mobile_api::ServiceMessage& message) = 0;

  /**
   * Marks the application to use as service
   * @param app_id unique application ID
   */
  virtual void ActivateService(int32_t app_id) = 0;

  /**
   * Resets using the current application as service
   */
  virtual void DeactivateService() = 0;

  /**
   * Sets default service
   * @param app_id unique application ID
   */
  virtual void SetDefaultService(int32_t app_id) = 0;

  /**
   * Resets default service
   */
  virtual void ResetDefaultService() = 0;

  /**
   * Checks service is supported
   * @return true if it's supported
   */
  virtual bool IsSupported() const = 0;

  /**
   * Enables support of service
   */
  virtual void EnableSupport() = 0;

  /**
   * Disables support of service
   */
  virtual void DisableSupport() = 0;
};
}  // namespace vr_module

#endif  // SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_SERVICE_MODULE_H_
