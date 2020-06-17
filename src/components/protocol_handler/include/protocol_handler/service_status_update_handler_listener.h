/*
 * Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_STATUS_UPDATE_HANDLER_LISTENER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_STATUS_UPDATE_HANDLER_LISTENER_H_

#include "interfaces/HMI_API.h"
#include "protocol_handler/protocol_handler.h"
#include "transport_manager/transport_manager.h"
#include "utils/optional.h"

namespace protocol_handler {
/**
 * @brief Converts service type enum value from protocol_handler to hmi_apis.
 * @param service_type protocol_handler enum value.
 */
hmi_apis::Common_ServiceType::eType GetHMIServiceType(
    protocol_handler::ServiceType service_type);

/**
 * @brief ServiceStatusUpdateHandlerListener provides callbacks interface with
 * a purpose to notify HMI on successful or failed state updates of different
 * services
 */
class ServiceStatusUpdateHandlerListener {
 public:
  /**
   * @brief ProcessServiceStatusUpdate callback that is invoked in case of
   * service
   * status update
   * @param connection_key - connection key
   * @param service_type enum value containing type of service.
   * @param service_event enum value containing event that occured during
   * service start.
   * @param service_update_reason enum value containing reason why service_event
   * occured.
   */
  virtual void ProcessServiceStatusUpdate(
      const uint32_t connection_key,
      hmi_apis::Common_ServiceType::eType service_type,
      hmi_apis::Common_ServiceEvent::eType service_event,
      utils::Optional<hmi_apis::Common_ServiceStatusUpdateReason::eType>
          service_update_reason) = 0;
};

}  // namespace protocol_handler

#endif
