/**
 * \file DeviceAdapterImpl.hpp
 * \brief DeviceAdapterImpl class header.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_

#include <queue>
#include <set>
#include <memory>

#include "utils/logger.h"

#include "device_adapter.h"

namespace transport_manager {

class DeviceAdapterListener;
class DeviceHandleGenerator;

namespace device_adapter {

/**
 * @brief Internal class describing device.
 **/
class Device {
 public:
  /**
   * @brief Constructor.
   *
   * @param Name User-friendly device name.
   **/
  Device(const char* name);

  /**
   * @brief Destructor.
   **/
  virtual ~Device();

  /**
   * @brief Compare devices.
   *
   * This method checks whether two SDevice structures
   * refer to the same device.
   *
   * @param OtherDevice Device to compare with.
   *
   * @return true if devices are equal, false otherwise.
   **/
  virtual bool isSameAs(const Device* other_device) const = 0;

  const std::string& unique_device_id() const {
    return unique_device_id_;
  }

  const std::string& name() const {
    return name_;
  }

 protected:

  void set_unique_device_id(const std::string& unique_device_id) {
    unique_device_id_ = unique_device_id;
  }

 private:
  /**
   * @brief Device user-friendly name.
   **/
  std::string name_;

  /**
   * @brief Unique device identifier across all devices.
   **/
  std::string unique_device_id_;
};

/**
 * @brief Application connection.
 **/
class Connection {
 public:
  Connection();
  /**
   * @brief Destructor.
   *
   * Clears map of frames to send.
   **/
  virtual ~Connection();

  virtual Error sendData(RawMessageSptr message) = 0;

  virtual Error disconnect() = 0;
};

typedef utils::SharedPtr<Connection> ConnectionSptr;
typedef utils::SharedPtr<Device> DeviceSptr;
typedef std::vector<DeviceSptr> DeviceVector;

class DeviceAdapterController {
 public:

  virtual ~DeviceAdapterController() {
  }

  virtual DeviceHandleGenerator* getDeviceHandleGenerator() = 0;

  virtual void addDevice(DeviceSptr device) = 0;
  virtual void searchDeviceDone(const DeviceVector& devices) = 0;
  virtual void searchDeviceFailed(const SearchDeviceError& error) = 0;
  virtual DeviceSptr findDevice(DeviceHandle device_handle) const = 0;

  virtual void clientConnected(DeviceHandle, ApplicationHandle) = 0;
  virtual void connectionCreated(ConnectionSptr connection,
                                 const SessionID session_id,
                                 const DeviceHandle device_handle,
                                 const ApplicationHandle app_handle) = 0;
  virtual void connectDone(const SessionID session_id) = 0;
  virtual void connectFailed(const SessionID session_id, const ConnectError& error) = 0;
  virtual void connectionFinished(const SessionID session_id) = 0;
  virtual void disconnectDone(const SessionID session_id) = 0;
  virtual void dataReceiveDone(const SessionID session_id,
                               RawMessageSptr message) = 0;
  virtual void dataReceiveFailed(const SessionID session_id,
                                 const DataReceiveError&) = 0;
  virtual void dataSendDone(const SessionID session_id,
                            RawMessageSptr message) = 0;
  virtual void dataSendFailed(const SessionID session_id,
                              RawMessageSptr message, const DataSendError&) = 0;
};

class DeviceScanner {
 public:
  virtual Error init() = 0;
  virtual Error scan() = 0;
  virtual ~DeviceScanner() {
  }
};

class ServerConnectionFactory {
 public:
  virtual Error init() = 0;
  virtual Error createConnection(DeviceHandle device_handle, ApplicationHandle app_handle,
                                 SessionID session_id) = 0;
  virtual ~ServerConnectionFactory() {
  }
};

class ClientConnectionListener {
 public:
  virtual Error init() = 0;
  virtual ~ClientConnectionListener() {
  }
};

/*
 * @brief Base class for @link components_transportmanager_internal_design_device_adapters device adapters @endlink.
 **/
class DeviceAdapterImpl : public DeviceAdapter, public DeviceAdapterController {
 protected:
  /**
   * @brief Constructor.
   *
   **/
  DeviceAdapterImpl();

  /**
   * @brief Destructor.
   **/
  virtual ~DeviceAdapterImpl();

  /**
   * @brief Run device adapter.
   *
   * Called from transport manager to start device adapter.
   **/
  virtual Error init(DeviceAdapterListener* listener,
                     DeviceHandleGenerator* handle_generator,
                     Configuration* configuration);

  /**
   * @brief Start scanning for new devices.
   *
   * List of new devices will be supplied in onDeviceListUpdated callback.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
   **/
  virtual Error searchDevices();

  /**
   * @brief Connect to the specified application discovered on device.
   *
   * @param device_handle Handle of device to connect to.
   * @param app_handle Handle of application to connect to.
   * @param session_id Session id for future identification.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connecting_devices
   **/
  virtual Error connect(const DeviceHandle device_handle,
                        const ApplicationHandle app_handle,
                        const SessionID session_id);

  /**
   * @brief Disconnect from specified session.
   *
   * @param session_id Session identifier to disconnect.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_disconnecting_devices
   **/
  virtual Error disconnect(const SessionID session_id);

  /**
   * @brief Disconnect from all sessions on specified device.
   *
   * @param device_handle Device handle to disconnect.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_disconnecting_devices
   **/
  virtual Error disconnectDevice(const DeviceHandle device_handle);

  /**
   * @brief Send frame.
   *
   * @param ConnectionHandle Connection handle.
   * @param Data Frame payload data.
   * @param DataSize Size of data in bytes.
   * @param UserData Any user data.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_handling_communication
   **/
  virtual Error sendData(const SessionID session_id, const RawMessageSptr data);

  virtual bool isSearchDevicesSupported() const;
  virtual bool isServerOriginatedConnectSupported() const;
  virtual bool isClientOriginatedConnectSupported() const;

  virtual DeviceList getDeviceList() const;
  virtual DeviceSptr findDevice(DeviceHandle device_handle) const;

  virtual DeviceAdapterListener* getListener();
  virtual DeviceHandleGenerator* getDeviceHandleGenerator();

  virtual void searchDeviceDone(const DeviceVector& devices) = 0;
  virtual void searchDeviceFailed(const SearchDeviceError& error) = 0;
  virtual void addDevice(DeviceSptr device);

  virtual void connectionCreated(ConnectionSptr connection,
                                 const SessionID session_id,
                                 const DeviceHandle device_handle,
                                 const ApplicationHandle app_handle);
  virtual void connectDone(const SessionID session_id);
  virtual void connectFailed(const SessionID session_id, const ConnectError& error);
  virtual void disconnnectDone(const SessionID session_id);
  virtual void dataReceiveDone(const SessionID session_id,
                               RawMessageSptr message);
  virtual void dataReceiveFailed(const SessionID session_id,
                                 const DataReceiveError&);
  virtual void dataSendDone(const SessionID session_id, RawMessageSptr message);
  virtual void dataSendFailed(const SessionID session_id,
                              RawMessageSptr message, const DataSendError&);

  void setDeviceScanner(DeviceScanner* device_scanner);
  void setServerConnectionFactory(
      ServerConnectionFactory* server_connection_factory);
  void setClientConnectionListener(
      ClientConnectionListener* client_connection_listener);

 private:

  ConnectionSptr findEstablishedConnection(const SessionID session_id);

  /**
   * @brief Listener for device adapter notifications.
   **/
  DeviceAdapterListener* listener_;

  /**
   * @brief Handle generator implementation.
   **/
  DeviceHandleGenerator* handle_generator_;

  bool initialised_;

  /**
   * @brief Devices map.
   **/
  typedef std::map<DeviceHandle, DeviceSptr> DeviceMap;

  struct ConnectionInfo {
    ConnectionSptr connection;
    DeviceHandle device_handle;
    ApplicationHandle app_handle;
    enum {
      NEW,
      ESTABLISHED,
      FINALISING
    } state;
  };

  typedef std::map<SessionID, ConnectionInfo> ConnectionMap;

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

  DeviceScanner* device_scanner_;
  ServerConnectionFactory* server_connection_factory_;
  ClientConnectionListener* client_connection_listener_;
};

extern log4cxx::LoggerPtr logger_;

}  // namespace device_adapter

}  // namespace transport_manager

#endif // #ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_
