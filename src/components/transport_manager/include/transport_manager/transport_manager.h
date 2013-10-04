/**
 * \file transport_manager.h
 * \brief TransportManager class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_H_

#include "protocol_handler/protocol_handler.h"//YK: temp solution until B1.0 release
#include "transport_manager/common.h"
#include "transport_manager/info.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"
#include "transport_manager/transport_manager_listener.h"

namespace transport_manager {
/**
 * @brief Interface of transport manager.
 **/
class TransportManager {
 public:
  /**
   * @brief Destructor.
   **/
  virtual ~TransportManager(void) {}

  /**
   * @brief Start scanning for new devices.
   *
   * @return Code error.
   **/
  virtual int SearchDevices(void) = 0;

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param device_id Handle of device to connect to.
   *
   * @return Code error.
   **/
  virtual int ConnectDevice(const DeviceHandle &device_id) = 0;

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param device_id Handle of device to Disconnect from.
   *
   * @return Code error.
   **/
  virtual int DisconnectDevice(const DeviceHandle &device_id) = 0;

  /**
   * @brief Disconnect from applications connected on device by connection unique identifier.
   *
   * @param connection Connection unique identifier.
   *
   * @return Code error.
   **/
  virtual int Disconnect(const ConnectionUID &connection_id) = 0;

  /**
   * @brief Post new message in queue for massages destined to device.
   *
   * @param message Smart pointer to the raw massage.
   *
   * @return Code error.
   **/
  virtual int SendMessageToDevice(const RawMessageSptr message) = 0;

  /**
   * @brief Post event in the event queue.
   *
   * @param event Current event information.
   *
   * @return Code error.
   **/
  virtual int ReceiveEventFromDevice(const TransportAdapterEvent &event) = 0;

  /**
   * @brief Add device adapter to the container of device adapters.
   *
   * @param transport_adapter Smart pointer to the device adapter.
   *
   * @return Code error.
   **/
  virtual int AddTransportAdapter(transport_adapter::TransportAdapterSptr transport_adapter) = 0;

  /**
   * @brief Post listener to the container of transport manager listeners.
   *
   * @param listener Pointer to the transport manager listener.
   *
   * @return Code error.
   **/
  virtual int AddEventListener(TransportManagerListener *listener) = 0;

  /**
   * @brief Delete listener from the container of transport manager listeners.
   *
   * @param listener Pointer to the transport manager listener.
   *
   * @return Code error.
   **/
  virtual int RemoveEventListener(TransportManagerListener *listener) = 0;

  /**
   * @brief Remove device from the container that hold devices.
   *
   * @param device Handle of device.
   *
   * @return Code error.
   **/
  virtual int RemoveDevice(const DeviceHandle &device_handle) = 0;

  /**
   * @brief Turns on or off visibility of SDL to mobile devices
   * when visibility is ON (on_off = true) mobile devices are able to connect
   * otherwise ((on_off = false)) SDL is not visible from outside
   *
   * @return Code error.
   */
  virtual int Visibility(const bool &on_off) const = 0;

  /**
   * @brief Establish protocom handler.
   *
   * @param ph Pointer to the handler of protocol.
   */
  virtual void SetProtocolHandler(protocol_handler::ProtocolHandler *ph) = 0;//YK: temp solution until B1.0 release

};
}

#endif
