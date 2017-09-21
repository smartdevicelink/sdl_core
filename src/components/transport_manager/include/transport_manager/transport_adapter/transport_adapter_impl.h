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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_IMPL_H_

#include <queue>
#include <set>
#include <map>
#include <memory>
#include <string>

#include "utils/lock.h"
#include "utils/rwlock.h"

#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/transport_adapter/connection.h"
#include "transport_manager/transport_manager_settings.h"
#include "resumption/last_state.h"

#ifdef TELEMETRY_MONITOR
#include "transport_manager/telemetry_observer.h"
#endif  // TELEMETRY_MONITOR

namespace transport_manager {

namespace transport_adapter {

class TransportAdapterListener;
class DeviceScanner;
class ServerConnectionFactory;
class ClientConnectionListener;

/*
 * @brief Implementation of device adapter class.
 **/
class TransportAdapterImpl : public TransportAdapter,
                             public TransportAdapterController {
 protected:
  /**
   * @brief Constructor.
   *
   * @param device_scanner Pointer to device scanner.
   * @param server_connection_factory Pointer to the factory that create
   *connections from server.
   * @param client_connection_listener Pointer to the listener of client
   *connection.
   **/
  TransportAdapterImpl(DeviceScanner* device_scanner,
                       ServerConnectionFactory* server_connection_factory,
                       ClientConnectionListener* client_connection_listener,
                       resumption::LastState& last_state,
                       const TransportManagerSettings& settings);

  /**
   * @brief Destructor.
   **/
  virtual ~TransportAdapterImpl();

  /**
   * @brief Check initialization.
   *
   * @return true if initialized.
   * @return false if not initialized.
   */
  bool IsInitialised() const OVERRIDE;

 public:
  /**
   * @brief Initiate device adapter.
   *
   * Called from transport manager to start device adapter.
   *
   * @return Error information about possible reason of failure.
   **/
  TransportAdapter::Error Init() OVERRIDE;

  /**
   * @brief Stops device adapter
   * Called from transport manager to stop device adapter
   */
  void Terminate() OVERRIDE;

  /**
   * @brief Add listener to the container(list) of device adapter listeners.
   *
   * @param listener Pointer to the device adapter listener.
   */
  void AddListener(TransportAdapterListener* listener) OVERRIDE;

  /**
   * @brief Start scanning for new devices.
   *
   * List of new devices will be supplied in onDeviceListUpdated callback.
   *
   * @return Error information about possible reason of search devices failure.
   **/
  TransportAdapter::Error SearchDevices() OVERRIDE;

  /**
   * @brief Connect to the specified application discovered on device.
   *
   * @param device_handle Device unique identifier to connect to.
   * @param app_handle Handle of application to connect to.
   *
   * @return Error information about possible reason of failure.
   **/
  TransportAdapter::Error Connect(const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Connect to all applications discovered on device
   * @param device_handle Handle of device
   * @return Error information about connecting applications on device
   */
  TransportAdapter::Error ConnectDevice(
      const DeviceUID& device_handle) OVERRIDE;

  /**
   * @brief Disconnect from specified session.
   *
   * @param devcie_handle Device unique identifier.
   * @param app_handle Handle of application.
   *
   * @return Error information about possible reason of disconnecting failure.
   **/
  TransportAdapter::Error Disconnect(
      const DeviceUID& device_handle,
      const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Disconnect from all sessions on specified device.
   *
   * @param device_handle Device handle to Disconnect.
   *
   * @return Error information about possible reason of disconnecting from
   *specified device failure.
   **/
  TransportAdapter::Error DisconnectDevice(
      const DeviceUID& device_handle) OVERRIDE;

  /**
   * @brief Send frame of data.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param data Smart pointer to the raw message.
   *
   * @return Error information about possible reason of sending data failure
   **/
  TransportAdapter::Error SendData(
      const DeviceUID& device_handle,
      const ApplicationHandle& app_handle,
      const ::protocol_handler::RawMessagePtr data) OVERRIDE;

  /**
   * @brief Start client listener.
   *
   * @return Error information about possible reason of failure.
   */
  TransportAdapter::Error StartClientListening() OVERRIDE;

  /**
   * @brief Stop client listener.
   *
   * @return Error information about possible reason of failure.
   */
  TransportAdapter::Error StopClientListening() OVERRIDE;

  /**
   * @brief Notify that device scanner is available.
   *
   * @return True - available, false - not available.
   */
  bool IsSearchDevicesSupported() const OVERRIDE;

  /**
   * @brief Notify that server connection factory is available.
   *
   * @return True - available, false - not available.
   */
  bool IsServerOriginatedConnectSupported() const OVERRIDE;

  /**
   * @brief Notify that listener of client connection is available.
   *
   * @return True - available, false - not available.
   */
  bool IsClientOriginatedConnectSupported() const OVERRIDE;

  /**
   * @brief Create container(vector) of device unique identifiers.
   *
   * @return Container(vector) of device unique identifiers.
   */
  DeviceList GetDeviceList() const OVERRIDE;

  /**
   * @brief Get container(vector) of application unique identifiers that
   *available at specified device.
   *
   * @param device_handle Device unique identifier.
   *
   * @return Container(vector) that holds application unique identifiers.
   */
  ApplicationList GetApplicationList(
      const DeviceUID& device_handle) const OVERRIDE;

  /**
   * @brief Find device in the internal container(map).
   *
   * @param device_handle Device unique identifier.
   *
   * @return Smart pointer to device.
   */
  DeviceSptr FindDevice(const DeviceUID& device_handle) const OVERRIDE;

  /**
   * @brief Search for device in container of devices, if it is not there - adds
   *it.
   *
   * @param devices Container(vector) of smart pointers to devices.
   */
  void SearchDeviceDone(const DeviceVector& devices) OVERRIDE;

  void ApplicationListUpdated(const DeviceUID& device_handle) OVERRIDE;

  void FindNewApplicationsRequest() OVERRIDE;

  /**
   * @brief Launch OnSearchDeviceFailed event in device adapter listener.
   *
   * @param error Error class that contains details of this error situation.
   */
  void SearchDeviceFailed(const SearchDeviceError& error) OVERRIDE;

  /**
   * @brief Add device to the container(map), if container doesn't hold it yet.
   *
   * @param device Smart pointer to the device.
   *
   * @return Smart pointer to the device.
   */
  DeviceSptr AddDevice(DeviceSptr device) OVERRIDE;

  /**
   * @brief Create connection and fill its parameters.
   *
   * @param connection  Smart pointer to the connection.
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  void ConnectionCreated(ConnectionSPtr connection,
                         const DeviceUID& device_handle,
                         const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Set state of specified connection - FINALISING.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  void ConnectionFinished(const DeviceUID& device_handle,
                          const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Set specified connection state to FINALISING and launch
   *OnUnexpectedDisconnect event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param error Error class that contains details of this error situation.
   */
  void ConnectionAborted(const DeviceUID& device_handle,
                         const ApplicationHandle& app_handle,
                         const CommunicationError& error) OVERRIDE;

  /**
   * @brief Set state of specified connection - ESTABLISHED and launch
   *OnConnectDone event in device adapter listener.
   *
   * @param devcie_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  void ConnectDone(const DeviceUID& device_handle,
                   const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Delete connection from the container of connections and launch
   *OnConnectFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  void ConnectFailed(const DeviceUID& device_handle,
                     const ApplicationHandle& app_handle,
                     const ConnectError& error) OVERRIDE;

  /**
   * @brief Remove marked as FINALISING connection from accounting.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  void RemoveFinalizedConnection(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Remove specified device and all its connections
   * @param device_handle Device unique identifier.
   * @param error Error class that contains details of this error situation.
   */
  void DeviceDisconnected(const DeviceUID& device_handle,
                          const DisconnectDeviceError& error) OVERRIDE;

  /**
   * @brief Delete specified connection from the container(map) of connections
   *and launch event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  void DisconnectDone(const DeviceUID& device_handle,
                      const ApplicationHandle& app_handle) OVERRIDE;

  /**
   * @brief Launch OnDataReceiveDone event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to the raw message.
   */
  void DataReceiveDone(const DeviceUID& device_handle,
                       const ApplicationHandle& app_handle,
                       ::protocol_handler::RawMessagePtr message) OVERRIDE;

  /**
   * @brief Launch OnDataReceiveFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param error Class that contains details of this error situation.
   */
  void DataReceiveFailed(const DeviceUID& device_handle,
                         const ApplicationHandle& app_handle,
                         const DataReceiveError& error) OVERRIDE;

  /**
   * @brief Launch OnDataSendDone event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to raw message.
   */
  void DataSendDone(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    ::protocol_handler::RawMessagePtr message) OVERRIDE;

  /**
   * @brief Launch OnDataSendFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to raw message.
   * @param error Class that contains details of this error situation.
   */
  void DataSendFailed(const DeviceUID& device_handle,
                      const ApplicationHandle& app_handle,
                      ::protocol_handler::RawMessagePtr message,
                      const DataSendError& error) OVERRIDE;

  /**
   * @brief Return name of device.
   *
   * @param device_id Device unique identifier.
   *
   * @return String with name.
   */
  std::string DeviceName(const DeviceUID& device_id) const OVERRIDE;

  /**
   * @brief StopDevice looks for specific device in devices list and calls
   * Stop() interface of that device
   * @param device_id unique device identifier that has to be stopped.
   */
  void StopDevice(const DeviceUID& device_id) const FINAL;

  /**
   * @brief DeviceSwitched is triggered for adapter to proceed with possible
   * further switching steps required on device side. E.g. to notify device
   * on end of switching so it can disconnect transport being switched from.
   * This is default implemenation does nothing. Must be overloaded if needed.
   * @param device_handle Device id to notify on event
   */
  void DeviceSwitched(const DeviceUID& device_handle) FINAL;

  /**
   * @brief Allows to obtain connection type used by device.
   * @return connection type.
   */
  std::string GetConnectionType() const OVERRIDE;

  /**
   * @brief RunAppOnDevice allows run specific application on the certain
   *device.
   *
   * @param device_handle device identifier to run application on.
   *
   * @param bundle_id application id also known as bundle id on some devices to
   *run.
   */
  void RunAppOnDevice(const DeviceUID& device_uid,
                      const std::string& bundle_id) OVERRIDE;

#ifdef TELEMETRY_MONITOR
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTelemetryObserver(TMTelemetryObserver* observer);

  /**
   * @brief Return Time metric observer
   *
   * @param return pointer to Time metric observer
   */
  TMTelemetryObserver* GetTelemetryObserver() OVERRIDE;
#endif  // TELEMETRY_MONITOR

 protected:
  /**
   * @brief Store adapter state where applicable
   */
  virtual void Store() const;

  /**
   * @brief Restore adapter state where applicable
   *
   * @return True on success false otherwise (or vacuous true)
   */
  virtual bool Restore();

  /**
   * @brief Returns true if \a device is to be connected automatically
   */
  virtual bool ToBeAutoConnected(DeviceSptr device) const;

  /**
   * @brief Returns true if \a device is to be disconnected automatically when
   * all applications will be closed
   */
  virtual bool ToBeAutoDisconnected(DeviceSptr device) const;

  /**
   * @brief Find connection that has state - ESTABLISHED.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   *
   * @return pointer to the connection.
   */
  virtual ConnectionSPtr FindEstablishedConnection(
      const DeviceUID& device_handle,
      const ApplicationHandle& app_handle) const;

  const TransportManagerSettings& get_settings() const {
    return settings_;
  }

 private:
  /**
   * @brief Connect to all applications discovered on device
   * @param device Pointer to device
   * @return Error information about connecting applications on device
   */
  TransportAdapter::Error ConnectDevice(DeviceSptr device);

  /**
   * @brief Remove specified device
   * @param device_handle Device unique identifier.
   */
  void RemoveDevice(const DeviceUID& device_handle);

  /**
   * Checks whether application is single active on device
   * @param device_uid
   * @param app_uid
   * @return true if this application is the single application on device
   */
  bool IsSingleApplication(const DeviceUID& device_uid,
                           const ApplicationHandle& app_uid);

  /**
   * @brief Listener for device adapter notifications.
   **/
  TransportAdapterListenerList listeners_;

  /**
   * @brief Flag variable that notify initialized device adapter or not.
   */
  bool initialised_;

  /**
   * @brief Structure that holds information about connection.
   */
  struct ConnectionInfo {
    ConnectionSPtr connection;
    DeviceUID device_id;
    ApplicationHandle app_handle;
    enum { NEW, ESTABLISHED, FINALISING } state;
  };

  /**
   * @brief Type definition of container(map) that holds pair<device unique
   * identifier, handle of application>(key value) and structure that holds
   * information
   * about connection(mapped value).
   */
  typedef std::map<std::pair<DeviceUID, ApplicationHandle>, ConnectionInfo>
      ConnectionMap;

  /**
   * @brief Map of device handle to device.
   *
   * This map contains all currently available devices.
   **/
  DeviceMap devices_;

  /**
   * @brief Mutex restricting access to device map.
   **/
  mutable sync_primitives::Lock devices_mutex_;

  /**
   * @brief Container(map) of connections.
   **/
  ConnectionMap connections_;

  /**
   * @brief Mutex restricting access to connections map.
   **/
  mutable sync_primitives::RWLock connections_lock_;

 protected:
#ifdef TELEMETRY_MONITOR
  /**
   * @brief Pointer to time metric observer
   */
  TMTelemetryObserver* metric_observer_;
#endif  // TELEMETRY_MONITOR

  resumption::LastState& last_state() const {
    return last_state_;
  }

  /**
   * @brief Pointer to the device scanner.
   */
  DeviceScanner* device_scanner_;

  /**
   * @brief Pointer to the factory if connections initiated from server.
   */
  ServerConnectionFactory* server_connection_factory_;

  /**
   * @brief Pointer to the factory of connections initiated from client.
   */
  ClientConnectionListener* client_connection_listener_;

  resumption::LastState& last_state_;
  const TransportManagerSettings& settings_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_IMPL_H_
