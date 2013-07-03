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

#include <pthread.h>
#include <queue>
#include <time.h>

#include "utils/logger.h"

#include "device_adapter.h"

namespace transport_manager {

namespace device_adapter {

class DeviceAdapterListener;
class DeviceHandleGenerator;

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
  virtual bool isSameAs(const Device* other_device) const;

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

typedef utils::SharedPtr<Device> DeviceSptr;

/**
 * @brief Frame queue.
 **/
typedef std::queue<RawMessageSptr> FrameQueue;

/**
 * @brief Application connection.
 **/
class Connection {
 public:
  /**
   * @brief Constructor.
   *
   * @param device_handle Device handle.
   * @param app_handle Application handle.
   * @param session_id Session identifier.
   **/
  Connection(const DeviceHandle device_handle,
             const ApplicationHandle app_handle, const int session_id);

  /**
   * @brief Destructor.
   *
   * Clears map of frames to send.
   **/
  virtual ~Connection();

  DeviceHandle device_handle() const {
    return device_handle_;
  }

  ApplicationHandle application_handle() const {
    return app_handle_;
  }

  int session_id() const {
    return session_id_;
  }

  void pushFrame(RawMessageSptr frame);
  RawMessageSptr popFrame();

 private:
  /**
   * @brief Device handle.
   **/
  const DeviceHandle device_handle_;

  /**
   * @brief Application handle.
   **/
  const ApplicationHandle app_handle_;

  /**
   * @brief Session identifier.
   **/
  const SessionID session_id_;

  /**
   * @brief Frames that must be sent to remote device.
   **/
  FrameQueue frames_to_send_;

  mutable pthread_mutex_t frames_to_send_mutex_;
};

typedef utils::SharedPtr<Connection> ConnectionSptr;

class DeviceAdapterFunctional {
 public:
  virtual Error init() = 0;
  virtual void terminate() = 0;
  virtual ~DeviceAdapterFunctional() {
  }
};

typedef utils::SharedPtr<DeviceAdapterFunctional> DeviceAdapterFunctionalSptr

class DeviceScanner : virtual public DeviceAdapterFunctional {
 public:
  virtual void scan() = 0;
  virtual ~DeviceScanner() {
  }
};

typedef utils::SharedPtr<DeviceScanner> DeviceScannerSptr;

class DataTransmitter : virtual public DeviceAdapterFunctional {
 public:
  virtual void registerConnection(ConnectionSptr) = 0;
  virtual void unregisterConnection(ConnectionSptr) = 0;
  virtual void notifyDataAvailable(ConnectionSptr) = 0;
  virtual ~DataTransmitter() {
  }
};

typedef utils::SharedPtr<DataTransmitter> DataTransmitterSptr;

class ServerConnectionProcessor : virtual public DeviceAdapterFunctional {
 public:
  virtual void createConnection(DeviceHandle, ApplicationHandle, SessionID) = 0;
  virtual ~ServerConnectionProcessor() {
  }
};

typedef utils::SharedPtr<ServerConnectionProcessor> ServerConnectionProcessorSptr;

class ClientConnectionListener : virtual public DeviceAdapterFunctional {
 public:
  virtual void createConnection(DeviceHandle, ApplicationHandle, SessionID) = 0;
  virtual ~ClientConnectionListener() {
  }
};

typedef utils::SharedPtr<ClientConnectionListener> ClientConnectionListenerSptr;

class Disconnector : virtual public DeviceAdapterFunctional {
 public:
  virtual void disconnect(ConnectionSptr) = 0;
  virtual ~Disconnector() {
  }
};

typedef utils::SharedPtr<Disconnector> DisconnectorSptr;

class DeviceAdapterInternals {
 public:
  virtual ~DeviceAdapterInternals() {
  }

  virtual DeviceAdapterListener* getListener() = 0;
  virtual DeviceHandleGenerator* getDeviceHandleGenerator() = 0;

  virtual DeviceHandle addDevice(Device*) = 0;
  virtual std::vector<DeviceHandle> addDevices(const std::vector<Device>&) = 0;
  virtual bool isDeviceExist(Device*) = 0;
  virtual Device* findDevice(DeviceHandle) = 0;

  virtual void addConnection(Connection*) = 0;
  virtual void removeConnection(Connection*) = 0;
};

/*
 * @brief Base class for @link components_transportmanager_internal_design_device_adapters device adapters @endlink.
 **/
class DeviceAdapterImpl : public DeviceAdapter, public DeviceAdapterInternals {
 public:
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

 protected:

  void setDeviceScanner(DeviceScannerSptr device_scanner);
  void setDataTransmitter(DataTransmitterSptr data_transmitter);
  void setServerConnectionProcessor(ServerConnectionProcessorSptr server_connection_processor);
  void setClientConnectionListener(ClientConnectionListenerSptr client_connection_listener);
  void setDisconnector(DisconnectorSptr disconnector);

  /**
   * @brief Delete connection.
   *
   * Destroy connection object and remove it from map.
   *
   * @param connection Connection to delete.
   **/
  void deleteConnection(ConnectionSptr connection);

  /**
   * @brief Start connection.
   *
   * Start connection thread.
   *
   * @param connection Connection to start.
   *
   * @return true if connection thread has been started, false otherwise.
   **/
  Error startConnection(ConnectionSptr connection);

  /**
   * @brief Stop connection.
   *
   * This method only initiates connection termination. It returns immediately
   * without waiting for actual termination of the connection.
   *
   * @param connection Connection to stop.
   **/
  Error stopConnection(ConnectionSptr connection);

  virtual DeviceList getDeviceList() const;

  virtual DeviceAdapterListener* getListener();
  virtual DeviceHandleGenerator* getDeviceHandleGenerator();

  virtual DeviceHandle addDevice(DeviceSptr device);
  virtual std::vector<DeviceHandle> addDevices(const std::vector<Device>& devices);
  virtual bool isDeviceExist(Device* device);
  virtual Device* findDevice(DeviceHandle device_handle);

  virtual void addConnection(ConnectionSptr connection);
  virtual void removeConnection(ConnectionSptr connection);

  /**
   * @brief Logger.
   **/
  static log4cxx::LoggerPtr logger_;

 private:

  ConnectionSptr findConnection(const SessionID session_id);

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

  /**
   * @brief Connections map.
   **/
  typedef std::map<SessionID, ConnectionSptr> ConnectionMap;

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

  DeviceScannerSptr device_scanner_;
  DataTransmitterSptr data_transmitter_;
  ServerConnectionProcessorSptr server_connection_processor_;
  ClientConnectionListenerSptr client_connection_listener_;
  DisconnectorSptr disconnector_;

  typedef std::set<DeviceAdapterFunctionalSptr> DeviceAdapterFunctionals;
  DeviceAdapterFunctionals functionals_;
};

}  // namespace device_adapter

}  // namespace transport_manager

#endif // #ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_
