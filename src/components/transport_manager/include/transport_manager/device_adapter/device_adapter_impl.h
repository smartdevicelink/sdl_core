/**
 * \file DeviceAdapterImpl.hpp
 * \brief DeviceAdapterImpl class header
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_DEVICE_ADAPTER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_DEVICE_ADAPTER_IMPL_H_

#include <queue>
#include <set>
#include <memory>
#include <string>

#include "utils/logger.h"

#include "transport_manager/device_adapter/device_adapter.h"
#include "transport_manager/device_adapter/device_adapter_controller.h"
#include "transport_manager/device_adapter/connection.h"

/**
 * @brief transport_manager namespace
 */
namespace transport_manager {

/**
 * @brief device_adapter namespace, part of the transport manager namespace
 */
namespace device_adapter {

class DeviceAdapterListener;
class DeviceScanner;
class ServerConnectionFactory;
class ClientConnectionListener;

/*
 * @brief Base class for @link components_transportmanager_internal_design_device_adapters device adapters @endlink.
 **/
class DeviceAdapterImpl : public DeviceAdapter, public DeviceAdapterController {
 protected:
  /**
   * @brief Constructor.
   *
   **/
  DeviceAdapterImpl(DeviceScanner* device_scanner,
                    ServerConnectionFactory* server_connection_factory,
                    ClientConnectionListener* client_connection_listener);

  /**
   * @brief Destructor.
   **/
  virtual ~DeviceAdapterImpl();

  virtual bool isInitialised() const;

 public:
  /**
   * @brief Run device adapter.
   *
   * Called from transport manager to start device adapter.
   *
   * @return Error information about possible reason of initialization failure.
   **/
  virtual DeviceAdapter::Error init();

  /**
   * @brief Add listener to the container(list) of device adapter listeners.
   *
   * @param listener pointer to the device adapter listener
   */
  virtual void addListener(DeviceAdapterListener *listener);

  /**
   * @brief Remove listener from the container(list) of device dapter listeners.
   *
   * @param listener pointer to the device adapter listener
   */
  virtual void removeListener(DeviceAdapterListener *listener);

  /**
   * @brief Start scanning for new devices.
   *
   * List of new devices will be supplied in onDeviceListUpdated callback.
   *
   * @return Error information about possible reason of searching devices failure
   **/
  virtual DeviceAdapter::Error searchDevices();

  /**
   * @brief Connect to the specified application discovered on device.
   *
   * @param device_handle Handle of device to connect to.
   * @param app_handle Handle of application to connect to.
   *
   * @return Error information about possible reason of connection to the device failure
   **/
  virtual DeviceAdapter::Error connect(const DeviceUID& device_handle,
                                       const ApplicationHandle& app_handle);

  /**
   * @brief Disconnect from specified session.
   *
   * @param device_handle handle of device to disconnect from.
   * @param app_handle  handle of application.
   *
   * @return Error information about possible reason of disconnection from the device failure
   **/
  virtual DeviceAdapter::Error disconnect(const DeviceUID& device_handle,
                                          const ApplicationHandle& app_handle);

  /**
   * @brief Disconnect from all sessions on specified device.
   *
   * @param device_handle Device handle to disconnect.
   *
   * @return Error information about possible reason of disconnecting from device failure.
   **/
  virtual DeviceAdapter::Error disconnectDevice(const DeviceUID& device_handle);

  /**
   * @brief Send frame.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param data smart pointer to the raw message.
   *
   * @return Error information about possible reason of sending data failure
   **/
  virtual DeviceAdapter::Error sendData(const DeviceUID& device_handle,
                                        const ApplicationHandle& app_handle,
                                        const RawMessageSptr data);

  /**
   * @brief Start client listener.
   *
   * @return Error information about possible reason of starting client listener failure.
   */
  virtual DeviceAdapter::Error startClientListening();

  /**
   * @brief Stop client listener.
   *
   * @return Error information about possible reason of stopping client listener failure.
   */
  virtual DeviceAdapter::Error stopClientListening();

  /**
   * @brief Notify that device scanner is available.
   *
   * @return true - available, false - not available.
   */
  virtual bool isSearchDevicesSupported() const;

  /**
   * @brief Notify that server connection factory is available.
   *
   * @return true - available, false - not available.
   */
  virtual bool isServerOriginatedConnectSupported() const;

  /**
   * @brief Notify that listener of client connection is available.
   *
   * @return true - available, false - not available.
   */
  virtual bool isClientOriginatedConnectSupported() const;

  /**
   * @brief Create container(vector) of device unique identifiers.
   *
   * @return container(vector) of device unique identifiers.
   */
  virtual DeviceList getDeviceList() const;

  /**
   * @brief Get container(vector) of application unique identifiers that available at specified device.
   *
   * @param device_handle device unique identifier.
   *
   * @return container(vector) that holds application unique identifiers.
   */
  virtual ApplicationList getApplicationList(
      const DeviceUID& device_handle) const;

  /**
   * @brief Find device in container(map)
   *
   * @param device_handle device unique identifier
   *
   * @return smart pointer to device
   */
  virtual DeviceSptr findDevice(const DeviceUID& device_handle) const;

  /**
   * @brief Search for specified device ????
   *
   * @param devices container(vector) of smart pointers to devices.
   */
  virtual void searchDeviceDone(const DeviceVector& devices);

  /**
   * @brief ????
   *
   * @param error error class that contains details of this error situation.
   */
  virtual void searchDeviceFailed(const SearchDeviceError& error);

  /**
   * @brief Add device in the container(map), if container doesn't hold it yet.
   *
   * @param device smart pointer to the device.
   */
  virtual DeviceSptr addDevice(DeviceSptr device);

  /**
   * @brief Create connection and fill its fields.
   *
   * @param connection smart pointer to the connection
   * @param divice_handle handle of device
   * @param app_handle handle of device
   */
  virtual void connectionCreated(ConnectionSptr connection,
                                 const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle);

  /**
   * @brief Make state of connection - FINILIZING
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   */
  virtual void connectionFinished(const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle);

  /**
   * @brief Check device adapter listeners container for aborted connections.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application
   * @param error error class that contains details of this error situation.
   */
  virtual void connectionAborted(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const CommunicationError& error);

  /**
   * @brief Make state of connection - ESTABLISHED and launch event for listener.
   *
   * @param device_handle handle of device
   * @param app_handle handle of application
   */
  virtual void connectDone(const DeviceUID& device_handle,
                           const ApplicationHandle& app_handle);

  /**
   * @brief Delete connection from the container of connections and launch event for device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param error error class that contains details of this error situation.
   */
  virtual void connectFailed(const DeviceUID& device_handle,
                             const ApplicationHandle& app_handle,
                             const ConnectError& error);

  /**
   * @brief Delete connection from the list of connections and launch event for device adapter listener.
   *
   * @param device_handle handle of device
   * @param app_handle handle of application
   */
  virtual void disconnectDone(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle);

  /**
   * @brief Launch onDataReceiveDone event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param message smart pointer to the raw message.
   */
  virtual void dataReceiveDone(const DeviceUID& device_handle,
                               const ApplicationHandle& app_handle,
                               RawMessageSptr message);

  /**
   * @brief Launch onDataReceiveFailed event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param error error class that contains details of this error situation.
   */
  virtual void dataReceiveFailed(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const DataReceiveError&);

  /**
   * @brief Launch onDataSendDone event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param message smart poiner to the raw message.
   */
  virtual void dataSendDone(const DeviceUID& device_handle,
                            const ApplicationHandle& app_handle,
                            RawMessageSptr message);

  /**
   * @brief Launch onDataSendFailed event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   */
  virtual void dataSendFailed(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle,
                              RawMessageSptr message, const DataSendError&);

  /**
   * @brief Return name of device.
   *
   * @param device_id device unique identifier.
   *
   * @return string std::string.
   */
  virtual std::string DeviceName(const DeviceUID &device_id) const;

 private:

  /**
   * @brief Find connection that have state - ESTABLISHED.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   *
   * @return ConnectionSptr smart pointer to the connection.
   */
  ConnectionSptr findEstablishedConnection(const DeviceUID& device_handle,
                                           const ApplicationHandle& app_handle);

  /**
   * @brief Listener for device adapter notifications.
   **/
  DeviceAdapterListenerList listeners_;

  bool initialised_;

  /**
   * @brief Devices map.
   **/
  typedef std::map<DeviceUID, DeviceSptr> DeviceMap;

  struct ConnectionInfo {
    ConnectionSptr connection;
    DeviceUID device_id;
    ApplicationHandle app_handle;
    enum {
      NEW,
      ESTABLISHED,
      FINALISING
    } state;
  };

  typedef std::map<std::pair<DeviceUID, ApplicationHandle>, ConnectionInfo> ConnectionMap;

  /**
   * @brief Map of device handle to device.
   *
   * This map contains all currently available bluetooth devices.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_devices_map
   **/
  DeviceMap devices_;

  /**
   * @brief Mutex restricting access to device map.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_devices_map
   **/
  mutable pthread_mutex_t devices_mutex_;

  /**
   * @brief Map of connections.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connections_map
   **/
  ConnectionMap connections_;

  /**
   * @brief Mutex restricting access to connections map.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connections_map
   **/
  mutable pthread_mutex_t connections_mutex_;

 protected:
  DeviceScanner* device_scanner_;
  ServerConnectionFactory* server_connection_factory_;
  ClientConnectionListener* client_connection_listener_;
};

extern log4cxx::LoggerPtr logger_;

}  // namespace device_adapter
}  // namespace transport_manager

#endif // #ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_
