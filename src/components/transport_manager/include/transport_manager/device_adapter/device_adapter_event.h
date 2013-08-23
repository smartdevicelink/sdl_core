/**
 * \file device_adapter_event.h
 * \brief DeviceAdapterEvent class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_DEVICE_ADAPTER_EVENT_H
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_DEVICE_ADAPTER_EVENT_H

#include "transport_manager/common.h"
#include "transport_manager/device_adapter/device_adapter.h"

using transport_manager::device_adapter::DeviceAdapterSptr;

namespace transport_manager {

class DeviceAdapterEvent {
 public:
  /**
   * @brief Overloaded operator "==".
   *
   * @param Other reference to the event of device adapter.
   */
  bool operator ==(const DeviceAdapterEvent &other);

  /**
   * @brief Constructor.
   *
   * @param type Event type.
   * @param device_adapter Smart pointer to the device adapter.
   * @param device_handle Handle of device.
   * @param application_id Handle of application.
   * @param data Smart pointer to the raw message.
   * @param error Error class that contains details of this error situation.
   */
  DeviceAdapterEvent(int type,
                     DeviceAdapterSptr device_adapter,
                     const DeviceUID &device_handle,
                     const ApplicationHandle& application_id,
                     RawMessageSptr data,
                     BaseError *error);

  /**
   * @brief Destructor.
   */
  ~DeviceAdapterEvent();

  /**
   * @brief Set value that describe event type.
   *
   * @param type Type of the event.
   */
  void set_event_type(int type);

  /**
   * @brief Set connection unique identifier field.
   *
   * @param id Connection unique identifier.
   */
  void set_connection_uid(ConnectionUID id);

  /**
   * @brief Set device adapter field.
   *
   * @param device_adapter Smart pointer to the device adapter.
   */
  void set_device_adapter(DeviceAdapterSptr device_adapter);

  /**
   * @brief Set pointer to the data.
   *
   * @param data Smart pointer to the raw message.
   */
  void set_data(RawMessageSptr data);

  /**
   * @brief Set field that responsible for the pointer to error.
   *
   * @param error Error class that contains details of this error situation.
   */
  void set_error(BaseError *error);

  /**
   * @brief Set device handle field.
   *
   * @param device_handle Device unique identifier.
   */
  void set_device_handle(const DeviceUID &device_handle);

  /**
   * @brief Return device unique identifier value.
   *
   * @return Device unique identifier.
   */
  const DeviceUID &device_uid() const;

  /**
   * @brief Return value that describe event type.
   *
   * @return Value that describe event type.
   */
  int event_type(void) const;

  /**
   * @brief Return handle of application value.
   *
   * @return Handle of application.
   */
  ApplicationHandle application_id(void) const;

  /**
   * @brief Return smart pointer to the device adapter.
   *
   * @return Smart pointer to the device adapter.
   */
  DeviceAdapterSptr device_adapter(void) const;

  /**
   * @brief Return smart pointer to the raw message.
   *
   * @return Smart pointer to the raw message.
   */
  RawMessageSptr data(void) const;

  /**
   * @brief Return pointer to the class that contain details of error.
   *
   * @return Pointer to the class that contain details of error.
   */
  BaseError *event_error(void) const;

 private:

  DeviceUID device_uid_; /**< Device unique identifier. */
  int event_type_; /**< Value that describe event type. */
  ApplicationHandle application_id_; /**< Handle of application. */
  DeviceAdapterSptr device_adapter_; /**< Smart pointer to the device adapter. */
  RawMessageSptr event_data_; /**< Smart pointer to the raw message. */
  BaseError *event_error_; /** Pointer to the class that contain details of error */
};

}  // namespace

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_EVENT
