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
  FrameQueue frames_to_send() {
    return frames_to_send_;
  }  // FIXME

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
typedef utils::SharedPtr<Device> DeviceSptr;
typedef std::vector<DeviceSptr> DeviceVector;

class DeviceAdapterController {
 public:

  virtual ~DeviceAdapterController() {
  }

  virtual DeviceAdapterListener* getListener() = 0;
  virtual DeviceHandleGenerator* getDeviceHandleGenerator() = 0;

  virtual void addDevice(DeviceSptr device) = 0;
  virtual void setDevices(const DeviceVector&devices) = 0;

  virtual void addConnection(ConnectionSptr connection) = 0;
  virtual void endConnection(ConnectionSptr connection) = 0;
  virtual void removeConnection(ConnectionSptr connection) = 0;
};

class DeviceAdapterFunctional {
 public:
  DeviceAdapterFunctional()
      : device_adapter_(0),
        controller_(0) {
  }
  virtual ~DeviceAdapterFunctional() {
  }
  virtual Error init() = 0;
  virtual void terminate() = 0;
  void setDeviceAdapter(DeviceAdapter* device_adapter) {
    device_adapter_ = device_adapter;
  }
  void setController(DeviceAdapterController* controller) {
    controller_ = controller;
  }
 protected:
  DeviceAdapter* getDeviceAdapter() const {
    return device_adapter_;
  }
  DeviceAdapterController* getController() const {
    return controller_;
  }
 private:
  DeviceAdapter* device_adapter_;
  DeviceAdapterController* controller_;
};

class DeviceScanner : virtual public DeviceAdapterFunctional {
 public:
  virtual Error scan() = 0;
  virtual ~DeviceScanner() {
  }
};

class DataTransmitter : virtual public DeviceAdapterFunctional {
 public:
  virtual void registerConnection(ConnectionSptr) = 0;
  virtual void unregisterConnection(ConnectionSptr) = 0;
  virtual void notifyDataAvailable(ConnectionSptr) = 0;
  virtual ~DataTransmitter() {
  }
};

class ServerConnectionProcessor : virtual public DeviceAdapterFunctional {
 public:
  virtual void createConnection(DeviceHandle, ApplicationHandle, SessionID) = 0;
  virtual ~ServerConnectionProcessor() {
  }
};

class ClientConnectionListener : virtual public DeviceAdapterFunctional {
 public:
  virtual ~ClientConnectionListener() {
  }
};

class Disconnector : virtual public DeviceAdapterFunctional {
 public:
  virtual void disconnect(ConnectionSptr) = 0;
  virtual ~Disconnector() {
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

  virtual DeviceAdapterListener* getListener();
  virtual DeviceHandleGenerator* getDeviceHandleGenerator();

  virtual void setDevices(const DeviceVector&devices);
  virtual void addDevice(DeviceSptr device);

  virtual void addConnection(ConnectionSptr connection);
  virtual void endConnection(ConnectionSptr connection);
  virtual void removeConnection(ConnectionSptr connection);

  void setDeviceScanner(DeviceScanner* device_scanner);
  void setDataTransmitter(DataTransmitter* data_transmitter);
  void setServerConnectionProcessor(
      ServerConnectionProcessor* server_connection_processor);
  void setClientConnectionListener(
      ClientConnectionListener* client_connection_listener);
  void setDisconnector(Disconnector* disconnector);

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
  ConnectionMap connected_connections_;

  ConnectionMap finished_connections_;

  /**
   * @brief Mutex restricting access to connections map.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connections_map
   **/
  mutable pthread_mutex_t connections_mutex_;

  std::auto_ptr<DeviceScanner> device_scanner_;
  std::auto_ptr<DataTransmitter> data_transmitter_;
  std::auto_ptr<ServerConnectionProcessor> server_connection_processor_;
  std::auto_ptr<ClientConnectionListener> client_connection_listener_;
  std::auto_ptr<Disconnector> disconnector_;

  typedef std::set<DeviceAdapterFunctional*> DeviceAdapterFunctionals;
  DeviceAdapterFunctionals functionals_;
};

extern log4cxx::LoggerPtr logger_;

}  // namespace device_adapter

}  // namespace transport_manager

#endif // #ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_
