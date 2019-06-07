/*
 Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_STATUS_UPDATE_HANDLER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_STATUS_UPDATE_HANDLER_H_

#include "protocol_handler/service_status_update_handler_listener.h"

namespace protocol_handler {

/**
 * @brief  ServiceUpdateFailureReason helper enum containing reasons for
 * service
 * status to be updated
 */
enum class ServiceStatus {
  INVALID_ENUM = -1,
  SERVICE_RECEIVED,
  SERVICE_ACCEPTED,
  SERVICE_START_FAILED,
  PTU_FAILED,
  CERT_INVALID,
  INVALID_TIME,
  PROTECTION_ENFORCED,
  PROTECTION_DISABLED
};

/**
 * @brief ServiceStatusUpdateHandler class is used to notify listeners about
 * occured events during service start
 */
class ServiceStatusUpdateHandler {
 public:
  /**
   * @brief ServiceStatusUpdateHandler class constructor
   * @param listener pointer to ServiceStatusUpdateHandlerListener instance
   */
  ServiceStatusUpdateHandler(ServiceStatusUpdateHandlerListener* listener)
      : listener_(listener) {}

  /**
   * @brief OnServiceUpdate callback that is invoked in case of
   * service status update needed
   * @param connection_key - connection key
   * @param service_type enum value containing type of service.
   * @param service_status enum value containing status of service.
   * received
   */
  void OnServiceUpdate(const uint32_t connection_key,
                       const protocol_handler::ServiceType service_type,
                       const ServiceStatus service_status);

 private:
  ServiceStatusUpdateHandlerListener* listener_;
};

}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_STATUS_UPDATE_HANDLER_H_
