/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/transport_adapter/connection.h"

#ifdef TIME_TESTER
#include "transport_manager/time_metric_observer.h"
#endif  // TIME_TESTER

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
                       ClientConnectionListener* client_connection_listener);

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
  virtual bool IsInitialised() const;

 public:
  /**
   * @brief Initiate device adapter.
   *
   * Called from transport manager to start device adapter.
   *
   * @return Error information about possible reason of failure.
   **/
  virtual TransportAdapter::Error Init();

  /**
   * @brief Add listener to the container(list) of device adapter listeners.
   *
   * @param listener Pointer to the device adapter listener.
   */
  virtual void AddListener(TransportAdapterListener* listener);

  /**
   * @brief Start scanning for new devices.
   *
   * List of new devices will be supplied in onDeviceListUpdated callback.
   *
   * @return Error information about possible reason of search devices failure.
   **/
  virtual TransportAdapter::Error SearchDevices();

  /**
   * @brief Connect to the specified application discovered on device.
   *
   * @param device_handle Device unique identifier to connect to.
   * @param app_handle Handle of application to connect to.
   *
   * @return Error information about possible reason of failure.
   **/
  virtual TransportAdapter::Error Connect(const DeviceUID& device_handle,
                                          const ApplicationHandle& app_handle);

  /**
   * @brief Connect to all applications discovered on device
   * @param device_handle Handle of device
   * @return Error information about connecting applications on device
   */
  virtual TransportAdapter::Error ConnectDevice(const DeviceUID& device_handle);

  /**
   * @brief Disconnect from specified session.
   *
   * @param devcie_handle Device unique identifier.
   * @param app_handle Handle of application.
   *
   * @return Error information about possible reason of disconnecting failure.
   **/
  virtual TransportAdapter::Error Disconnect(
      const DeviceUID& device_handle, const ApplicationHandle& app_handle);

  /**
   * @brief Disconnect from all sessions on specified device.
   *
   * @param device_handle Device handle to Disconnect.
   *
   * @return Error information about possible reason of disconnecting from
   *specified device failure.
   **/
  virtual TransportAdapter::Error DisconnectDevice(
      const DeviceUID& device_handle);

  /**
   * @brief Send frame of data.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param data Smart pointer to the raw message.
   *
   * @return Error information about possible reason of sending data failure
   **/
  virtual TransportAdapter::Error SendData(const DeviceUID& device_handle,
                                           const ApplicationHandle& app_handle,
                                           const ::protocol_handler::RawMessagePtr data);

  /**
   * @brief Start client listener.
   *
   * @return Error information about possible reason of failure.
   */
  virtual TransportAdapter::Error StartClientListening();

  /**
   * @brief Stop client listener.
   *
   * @return Error information about possible reason of failure.
   */
  virtual TransportAdapter::Error StopClientListening();

  /**
   * @brief Notify that device scanner is available.
   *
   * @return True - available, false - not available.
   */
  virtual bool IsSearchDevicesSupported() const;

  /**
   * @brief Notify that server connection factory is available.
   *
   * @return True - available, false - not available.
   */
  virtual bool IsServerOriginatedConnectSupported() const;

  /**
   * @brief Notify that listener of client connection is available.
   *
   * @return True - available, false - not available.
   */
  virtual bool IsClientOriginatedConnectSupported() const;

  /**
   * @brief Create container(vector) of device unique identifiers.
   *
   * @return Container(vector) of device unique identifiers.
   */
  virtual DeviceList GetDeviceList() const;

  /**
   * @brief Get container(vector) of application unique identifiers that
   *available at specified device.
   *
   * @param device_handle Device unique identifier.
   *
   * @return Container(vector) that holds application unique identifiers.
   */
  virtual ApplicationList GetApplicationList(const DeviceUID& device_handle)
      const;

  /**
   * @brief Find device in the internal container(map).
   *
   * @param device_handle Device unique identifier.
   *
   * @return Smart pointer to device.
   */
  virtual DeviceSptr FindDevice(const DeviceUID& device_handle) const;

  /**
   * @brief Search for device in container of devices, if it is not there - adds
   *it.
   *
   * @param devices Container(vector) of smart pointers to devices.
   */
  virtual void SearchDeviceDone(const DeviceVector& devices);

  virtual void ApplicationListUpdated(const DeviceUID& device_handle);

  virtual void FindNewApplicationsRequest();

  /**
   * @brief Launch OnSearchDeviceFailed event in device adapter listener.
   *
   * @param error Error class that contains details of this error situation.
   */
  virtual void SearchDeviceFailed(const SearchDeviceError& error);

  /**
   * @brief Add device to the container(map), if container doesn't hold it yet.
   *
   * @param device Smart pointer to the device.
   *
   * @return Smart pointer to the device.
   */
  virtual DeviceSptr AddDevice(DeviceSptr device);

  /**
   * @brief Create connection and fill its parameters.
   *
   * @param connection  Smart pointer to the connection.
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectionCreated(Connection* connection,
                                 const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle);

  /**
   * @brief Set state of specified connection - FINILIZING.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectionFinished(const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle);

  /**
   * @brief Set specified connection state to FINILIZING and launch
   *OnUnexpectedDisconnect event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param error Error class that contains details of this error situation.
   */
  virtual void ConnectionAborted(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const CommunicationError& error);

  /**
   * @brief Set state of specified connection - ESTABLISHED and launch
   *OnConnectDone event in device adapter listener.
   *
   * @param devcie_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectDone(const DeviceUID& device_handle,
                           const ApplicationHandle& app_handle);

  /**
   * @brief Delete connection from the container of connections and launch
   *OnConnectFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectFailed(const DeviceUID& device_handle,
                             const ApplicationHandle& app_handle,
                             const ConnectError& error);

  /**
   * @brief Remove specified device and all its connections
   * @param device_handle Device unique identifier.
   * @param error Error class that contains details of this error situation.
   */
  virtual void DeviceDisconnected(const DeviceUID& device_handle,
                                  const DisconnectDeviceError& error);

  /**
   * @brief Delete specified connection from the container(map) of connections
   *and launch event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void DisconnectDone(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle);

  /**
   * @brief Launch OnDataReceiveDone event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to the raw message.
   */
  virtual void DataReceiveDone(const DeviceUID& device_handle,
                               const ApplicationHandle& app_handle,
                               ::protocol_handler::RawMessagePtr message);

  /**
   * @brief Launch OnDataReceiveFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param error Class that contains details of this error situation.
   */
  virtual void DataReceiveFailed(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const DataReceiveError& error);

  /**
   * @brief Launch OnDataSendDone event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to raw message.
   */
  virtual void DataSendDone(const DeviceUID& device_handle,
                            const ApplicationHandle& app_handle,
                            ::protocol_handler::RawMessagePtr message);

  /**
   * @brief Launch OnDataSendFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to raw message.
   * @param error Class that contains details of this error situation.
   */
  virtual void DataSendFailed(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle,
                              ::protocol_handler::RawMessagePtr message,
                              const DataSendError& error);

  /**
   * @brief Return name of device.
   *
   * @param device_id Device unique identifier.
   *
   * @return String with name.
   */
  virtual std::string DeviceName(const DeviceUID& device_id) const;

  /**
   * @brief Allows to obtain connection type used by device.
   * @return connection type.
   */
  virtual std::string GetConnectionType() const;

#ifdef TIME_TESTER
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTimeMetricObserver(TMMetricObserver* observer);

  /**
   * @brief Return Time metric observer
   *
   * @param return pointer to Time metric observer
   */
  virtual TMMetricObserver* GetTimeMetricObserver();
#endif  // TIME_TESTER


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
   * @brief Find connection that has state - ESTABLISHED.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   *
   * @return pointer to the connection.
   */
  Connection* FindEstablishedConnection(const DeviceUID& device_handle,
                                           const ApplicationHandle& app_handle) const;

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
    Connection* connection;
    DeviceUID device_id;
    ApplicationHandle app_handle;
    enum {
      NEW,
      ESTABLISHED,
      FINALISING
    } state;
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
  mutable pthread_mutex_t devices_mutex_;

  /**
   * @brief Container(map) of connections.
   **/
  ConnectionMap connections_;

  /**
   * @brief Mutex restricting access to connections map.
   **/
  mutable pthread_mutex_t connections_mutex_;

 protected:
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

#ifdef TIME_TESTER
  /**
   * @brief Pointer to time metric observer
   */
  TMMetricObserver* metric_observer_;
#endif  // TIME_TESTER
};
}  // namespace transport_adapter
}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_IMPL_H_
