/*
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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_EVENT_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_EVENT_H_

#include "transport_manager/common.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "protocol/common.h"

namespace transport_manager {

class TransportAdapterEvent {
 public:
  TransportAdapterEvent() {}
  /**
   * @brief Constructor.
   *
   * @param type Event type.
   * @param transport_adapter Transport adapter
   * @param device_handle Handle of device.
   * @param application_id Handle of application.
   * @param data Smart pointer to the raw message.
   * @param error Error class that contains details of this error situation.
   */
  TransportAdapterEvent(int type,
                        transport_adapter::TransportAdapter* adapter,
                        const DeviceUID& device_handle,
                        const ApplicationHandle& application_id,
                        ::protocol_handler::RawMessagePtr data, BaseErrorPtr error)
    : event_type(type),
      application_id(application_id),
      device_uid(device_handle),
      transport_adapter(adapter),
      event_data(data),
      event_error(error) {
  }
  /**
   * @brief Value that describe event type.
   */
  int event_type;
  /**
   * @brief Handle of application
   */
  ApplicationHandle application_id;
  /**
   * @brief  Device unique identifier.
   */
  DeviceUID device_uid;
  /**
   * @brief Transport adapter.
   */
  transport_adapter::TransportAdapter* transport_adapter;
  /**
   * @brief Smart pointer to the raw message
   */
  ::protocol_handler::RawMessagePtr event_data;
  /**
   * @brief Pointer to the class that contain details of error.
   */
  BaseErrorPtr event_error;
};
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_EVENT_H_
