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
  /**
   * @brief Set value that describe event type.
   *
   * @param type Type of the event.
   */
  virtual void set_event_type(int type) = 0;

  /**
   * @brief Set application unique identifier field.
   *
   * @param id Application unique identifier.
   */
  virtual void set_application_id(const ApplicationHandle id) = 0;

  /**
   * @brief Set device adapter field.
   *
   * @param transport_adapter Smart pointer to the device adapter.
   */
  virtual void set_transport_adapter(
      transport_adapter::TransportAdapter *transport_adapter) = 0;

  /**
   * @brief Set pointer to the data.
   *
   * @param message Smart pointer to the raw message.
   */
  virtual void set_data(RawMessagePtr message) = 0;

  /**
   * @brief Set field that responsible for the pointer to error.
   *
   * @param error Error class that contains details of this error situation.
   */
  virtual void set_error(BaseError *error) = 0;

  /**
   * @brief Set device handle field.
   *
   * @param device_handle Device unique identifier.
   */
  virtual void set_device_handle(const DeviceUID &device_handle) = 0;

  /**
   * @brief Return device unique identifier value.
   *
   * @return Device unique identifier.
   */
  virtual const DeviceUID &device_uid() const = 0;

  /**
   * @brief Return value that describe event type.
   *
   * @return Value that describe event type.
   */
  virtual int event_type() const = 0;

  /**
   * @brief Return handle of application value.
   *
   * @return Handle of application.
   */
  virtual ApplicationHandle application_id() const = 0;

  /**
   * @brief Return smart pointer to the device adapter.
   *
   * @return Transport adapter.
   */
  virtual transport_adapter::TransportAdapter *transport_adapter() const = 0;

  /**
   * @brief Return smart pointer to the raw message.
   *
   * @return Smart pointer to the raw message.
   */
  virtual RawMessagePtr data() const = 0;

  /**
   * @brief Return pointer to the class that contain details of error.
   *
   * @return Pointer to the class that contain details of error.
   */
  virtual BaseError *event_error() const = 0;
  /**
   * @brief Dectructor.
   */
  virtual ~TransportAdapterEvent() {}
};
typedef  utils::SharedPtr<TransportAdapterEvent> TransportAdapterEventPtr;
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_EVENT_H_
