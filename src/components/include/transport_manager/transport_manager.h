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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_H_

#include "protocol/common.h"
#include "resumption/last_state_wrapper.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/transport_manager_listener.h"

namespace transport_manager {

class TransportAdapterEvent;

/**
 * @brief Interface of transport manager.
 **/
class TransportManager {
 public:
  /**
   * @brief Destructor.
   **/
  virtual ~TransportManager() {}

  DEPRECATED
  virtual int Init(resumption::LastState& last_state) = 0;

  /**
   * @brief Initialize transport manager.
   * @return Error code.
   */
  virtual int Init(resumption::LastStateWrapperPtr last_state_wrapper) = 0;

  /**
   * @brief Reinitializes transport manager
   * @return Error code
   */
  virtual int Reinit() = 0;

  /**
   * @brief Deinitializes all transport adapters and device instances
   */
  virtual void Deinit() = 0;

  /**
   * @brief Stops transport events processing handler threads
   */
  virtual void StopEventsProcessing() = 0;

  /**
   * @brief Resumes transport events processing handler threads
   */
  virtual void StartEventsProcessing() = 0;

  /**
   * @brief Start scanning for new devices.
   *
   * @return Code error.
   **/
  virtual int SearchDevices() = 0;

  virtual void AddCloudDevice(
      const transport_manager::transport_adapter::CloudAppProperties&
          cloud_properties) = 0;

  virtual void RemoveCloudDevice(const DeviceHandle device_id) = 0;

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param device_id Handle of device to connect to.
   *
   * @return Code error.
   **/
  virtual int ConnectDevice(const DeviceHandle device_id) = 0;

  /**
   * @brief Retrieves the connection status of a given device
   *
   * @param device_handle Handle of device to query
   *
   * @return The connection status of the given device
   */
  virtual ConnectionStatus GetConnectionStatus(
      const DeviceHandle& device_handle) const = 0;

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param device_id Handle of device to Disconnect from.
   *
   * @return Code error.
   **/
  virtual int DisconnectDevice(const DeviceHandle device_id) = 0;

  /**
   * @brief Disconnect from applications connected on device by connection
   *unique identifier.
   *
   * @param connection Connection unique identifier.
   *
   * @return Code error.
   **/
  virtual int Disconnect(const ConnectionUID connection_id) = 0;

  /**
   * @brief Disconnect and clear all unprocessed data.
   *
   * @param connection Connection unique identifier.
   */
  virtual int DisconnectForce(const ConnectionUID connection_id) = 0;

  /**
   * @brief Post new message in queue for massages destined to device.
   *
   * @param message Smart pointer to the raw massage.
   *
   * @return Code error.
   **/
  virtual int SendMessageToDevice(
      const protocol_handler::RawMessagePtr message) = 0;

  /**
   * @brief RunAppOnDevice allows run specific application on the certain
   *device.
   *
   * @param device_handle device identifier to run application on.
   *
   * @param bundle_id application id alsow known as bundle id on some devices to
   *run.
   */
  virtual void RunAppOnDevice(const DeviceHandle device_handle,
                              const std::string& bundle_id) = 0;

  /**
   * @brief Post event in the event queue.
   *
   * @param event Current event information.
   *
   * @return Code error.
   **/
  virtual int ReceiveEventFromDevice(const TransportAdapterEvent& event) = 0;

  /**
   * @brief Add transport adapter.
   *
   * @param transport_adapter Transport adapter
   *
   * @return Error code.
   **/
  virtual int AddTransportAdapter(
      transport_adapter::TransportAdapter* transport_adapter) = 0;

  /**
   * @brief Post listener to the container of transport manager listeners.
   *
   * @param listener Pointer to the transport manager listener.
   *
   * @return Code error.
   **/
  virtual int AddEventListener(TransportManagerListener* listener) = 0;

  /**
   * @brief Stop work finally. No new events guaranteed after method finish.
   *
   * @return Error code.
   **/
  virtual int Stop() = 0;

  /**
   * @brief Remove device from the container that hold devices.
   *
   * @param device Handle of device.
   *
   * @return Code error.
   **/
  virtual int RemoveDevice(const DeviceHandle device_handle) = 0;

  /**
   * @brief Performs specified action on connected clients
   * @param required_action is the action which should be performed for the
   * connected clients
   * @return error code
   */
  virtual int PerformActionOnClients(
      const TransportAction required_action) const = 0;

  /**
   * @brief Called when websocket server transport adapter is available.
   * Creates WebSocketDevice for WebEngine and add it to the device list
   */
  virtual void CreateWebEngineDevice() = 0;

  /**
   * @brief GetWebEngineDeviceInfo
   * @return device info for WebEngine device
   */
  virtual const DeviceInfo& GetWebEngineDeviceInfo() const = 0;
};
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_H_
