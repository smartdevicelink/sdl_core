/**
 * \file transport_manager.h
 * \brief Class transport_manager header.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER

#include "transport_manager/common.h"
#include "transport_manager/device_adapter.h"
#include "transport_manager/device_adapter_event.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/transport_manager_listener.h"

namespace transport_manager {
/**
 * @brief Interface of transport manager.
 * @interface TransportManager
 **/
class TransportManager {
 public:
  /**
   * @brief Destructor.
   **/
  virtual ~TransportManager(void);

  /**
   * @brief Start scanning for new devices.
   *
   *
   * @see @ref components_transportmanager_client_device_management
   **/
  virtual int searchDevices(void) = 0;

  /**
   * @brief Start scanning for new devices.
   *
   *
   * @see @ref components_transportmanager_client_device_management
   **/
  virtual void init(void) = 0;

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param DeviceHandle Handle of device to connect to.
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void connectDevice(const DeviceHandle &device_id,
                             const ApplicationHandle &app_id) = 0;

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param DeviceHandle Handle of device to disconnect from.
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void disconnectDevice(const DeviceHandle &device_id) = 0;

  virtual void disconnect(const ConnectionId &connection) = 0;
  /**
   * @brief post new mesage into TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int sendMessageToDevice(
      const RawMessageSptr message) = 0;

  /**
   * @brief receive event from device
   *
   * @param new event
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void receiveEventFromDevice(const DeviceAdapterEvent &event) = 0;

  /**
   * @brief add new device adapter
   *
   * @param device adapter
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void addDeviceAdapter(device_adapter::DeviceAdapter *device_adapter) = 0;

  /**
   * @brief register event listener that would be called when something happened in TM
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void addEventListener(TransportManagerListener *listener) = 0;

  /**
   * @brief unregister event listener
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void removeEventListener(TransportManagerListener *listener) = 0;
  /**
   * @brief remove device from internal storages
   *
   * @param event device id
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void removeDevice(const DeviceHandle &device) = 0;
};
}

#endif
