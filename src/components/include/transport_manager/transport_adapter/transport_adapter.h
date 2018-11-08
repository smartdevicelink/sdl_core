/*
 * \file transport_adapter.h
 * \brief TransportAdapter class header file.
 *
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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_H_

#include <string>
#include <vector>
#include <list>
#include <map>

#include "transport_manager/transport_adapter/device.h"
#include "transport_manager/common.h"
#include "transport_manager/error.h"
#include "protocol/common.h"

namespace transport_manager {

class TMTelemetryObserver;

namespace transport_adapter {

class TransportAdapterListener;

/**
 * @brief The DeviceType enum defines types based on available transport
 * adapters
 */
enum DeviceType {
  AOA,
  BLUETOOTH,
  IOS_BT,
  IOS_USB,
  TCP,
  CLOUD_WEBSOCKET,
  IOS_USB_HOST_MODE,
  IOS_USB_DEVICE_MODE,
  IOS_CARPLAY_WIRELESS,  // running on iAP over Carplay wireless transport
  UNKNOWN
};

enum HybridAppPreference { // todo find correct place for this enum defintion.
  MOBILE,
  CLOUD, 
  BOTH
};

struct CloudAppProperties {
  std::string endpoint;
  std::string certificate;
  bool enabled;
  std::string auth_token;
  DeviceType cloud_transport_type;
  HybridAppPreference hybrid_app_preference;
};

typedef std::map<DeviceType, std::string> DeviceTypes;

/**
 * @brief Type definition of container(map) that holds device unique
 *identifier(key value) and smart pointer to the device(mapped value).
 **/
typedef std::map<DeviceUID, DeviceSptr> DeviceMap;
/**
 * @brief Type definition for container(list) that holds pointers to device
 * adapter listeners
 */
typedef std::list<TransportAdapterListener*> TransportAdapterListenerList;

/**
 * @brief Type definition for transport's configuration information
 */
typedef std::map<std::string, std::string> TransportConfig;

/**
 * @brief Type definition of container indexed by app id that contains connection information for all cloud apps.
 */
typedef std::map<std::string, CloudAppProperties> CloudAppTransportConfig;

/**
 * @brief TransportConfig keys
 */
extern const char* tc_enabled;
extern const char* tc_tcp_port;
extern const char* tc_tcp_ip_address;

class TransportAdapter {
 public:
  /**
   * @enum Available types of errors.
   */
  enum Error { OK, FAIL, NOT_SUPPORTED, ALREADY_EXISTS, BAD_STATE, BAD_PARAM };

 public:
  /**
   * @brief Destructor.
   */
  virtual ~TransportAdapter() {}

  /**
   * @brief
   */
  virtual DeviceType GetDeviceType() const = 0;

  /**
   * @brief Allows to obtain connection type used by device.
   * @return connection type.
   */
  virtual ConnectionType GetConnectionType() const = 0;

  /* TODO
   virtual Error LoadState(TransportAdapterState* state) = 0;
   virtual void SaveState(TransportAdapterState* state) = 0;
   */

  /**
   * @brief Check initialization.
   *
   * @return true if initialized.
   * @return false if not initialized.
   */
  virtual bool IsInitialised() const = 0;

  /**
   * @brief Run device adapter.
   *
   * Called from transport manager to start device adapter.
   *
   * @return Error information about possible reason of failure.
   **/
  virtual Error Init() = 0;

  /**
   * @brief Stops device adapter
   * Called from transport manager to stop device adapter
   */
  virtual void Terminate() = 0;

  /**
   * @brief Add listener to the container(list) of device adapter listeners.
   *
   * @param listener Pointer to the device adapter listener.
   */
  virtual void AddListener(TransportAdapterListener* listener) = 0;

  /**
   * @brief Notify that device scanner is available.
   *
   * @return true - available, false - not available.
   */
  virtual bool IsSearchDevicesSupported() const = 0;

  /**
   * @brief Start scanning for new devices.
   *
   * List of new devices will be supplied in onDeviceListUpdated callback.
   *
   * @return Error information about possible reason of failure.
   **/
  virtual Error SearchDevices() = 0;

  /**
   * @brief Notify that server connection factory is available.
   *
   * @return true - available, false - not available.
   */
  virtual bool IsServerOriginatedConnectSupported() const = 0;

  /**
   * @brief Connect to the specified application discovered on device.
   *
   * @param device_handle Handle of device to connect to.
   * @param app_handle Handle of application to connect to.
   *
   * @return Error information about possible reason of failure.
   **/
  virtual Error Connect(const DeviceUID& device_handle,
                        const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Connect to all applications discovered on device
   * @param device_handle Handle of device
   * @return Error information about connecting applications on device
   */
  virtual Error ConnectDevice(const DeviceUID& device_handle) = 0;

  /**
   * @brief RunAppOnDevice allows to run specific application on the certain
   *device.
   *
   * @param device_handle device identifier to run application on.
   *
   * @param app_id application id also known as bundle id on some devices to
   *run.
   */
  virtual void RunAppOnDevice(const std::string& device_uid,
                              const std::string& bundle_id) = 0;

  /**
   * @brief Notify that listener of client connection is available.
   *
   * @return true - available, false - not available.
   */
  virtual bool IsClientOriginatedConnectSupported() const = 0;

  /**
   * @brief Start client listener.
   *
   * @return Error information about possible reason of failure.
   */
  virtual Error StartClientListening() = 0;

  /**
   * @brief Stop client listener.
   *
   * @return Error information about possible reason of failure.
   */
  virtual Error StopClientListening() = 0;

  /**
   * @brief Remove marked as FINALISING connection from accounting.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void RemoveFinalizedConnection(
      const DeviceUID& device_handle, const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Disconnect from specified session.
   *
   * @param device_handle Handle of device to Disconnect from.
   * @param app_handle Handle of application.
   *
   * @return Error information about possible reason of disconnection from the
   *device failure.
   **/
  virtual Error Disconnect(const DeviceUID& device_handle,
                           const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Disconnect from all sessions on specified device.
   *
   * @param device_handle Device handle to Disconnect.
   *
   * @return Error information about possible reason of disconnecting from
   *device failure
   **/
  virtual Error DisconnectDevice(const DeviceUID& device_handle) = 0;

  /**
   * @brief Send frame.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param data Smart pointer to the raw message.
   *
   * @return Error information about possible reason of sending data failure.
   **/
  virtual Error SendData(const DeviceUID& device_handle,
                         const ApplicationHandle& app_handle,
                         const protocol_handler::RawMessagePtr data) = 0;

  /**
   * @brief Create container(vector) of device unique identifiers.
   *
   * @return container(vector) of device unique identifiers.
   */
  virtual DeviceList GetDeviceList() const = 0;

  /**
   * @brief Get container(vector) of application unique identifiers that
   *available at specified device.
   *
   * @param device_handle Device unique identifier.
   *
   * @return Container(vector) that holds application unique identifiers.
   */
  virtual ApplicationList GetApplicationList(
      const DeviceUID& device_handle) const = 0;

  /**
   * @brief Return name of device.
   *
   * @param device_id device unique identifier.
   *
   * @return string.
   */
  virtual std::string DeviceName(const DeviceUID& device_id) const = 0;

  /**
   * @brief StopDevice Stop all activity on device without removing it from
   * devices list
   * @param device_id unique device identifier that has to be stopped.
   */
  virtual void StopDevice(const DeviceUID& device_id) const = 0;

  /**
   * @brief DoTransportSwitch notifies listeners of transport adapter events
   * that transport switching is requested by system
   */
  virtual void DoTransportSwitch() const = 0;

  /**
   * @brief DeviceSwitched is triggered for adapter to proceed with possible
   * further switching steps required on device side. E.g. to notify device
   * on end of switching so it can disconnect transport being switched from.
   * @param device_handle Device id to notify on event
   */
  virtual void DeviceSwitched(const DeviceUID& device_handle) = 0;

  virtual SwitchableDevices GetSwitchableDevices() const = 0;

  /**
   * @brief Returns the transport's configuration information
   */
  virtual TransportConfig GetTransportConfiguration() const = 0;

#ifdef TELEMETRY_MONITOR
  /**
   * @brief Return Time metric observer
   *
   * @param return pointer to Time metric observer
   */
  virtual TMTelemetryObserver* GetTelemetryObserver() = 0;
#endif  // TELEMETRY_MONITOR
};
}  // namespace transport_adapter
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_H_
