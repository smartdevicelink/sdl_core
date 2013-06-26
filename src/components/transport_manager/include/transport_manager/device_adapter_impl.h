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

class DeviceAdapterListener;
class DeviceHandleGenerator;

/**
 * @brief Base class for @link components_transportmanager_internal_design_device_adapters device adapters @endlink.
 **/
class DeviceAdapterImpl : public DeviceAdapter {
 public:
  /**
   * @brief Constructor.
   *
   * @param Listener Listener for device adapter notifications.
   * @param HandleGenerator Handle generator implementation.
   **/
  DeviceAdapterImpl(DeviceAdapterListener& listener,
                 DeviceHandleGenerator & handle_generator);

  /**
   * @brief Destructor.
   **/
  virtual ~DeviceAdapterImpl();

  /**
   * @brief Run device adapter.
   *
   * Called from transport manager to start device adapter.
   **/
  virtual void run();

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
  virtual Error connect(const DeviceHandle device_handle, const ApplicationHandle app_handle, const int session_id);

  /**
   * @brief Disconnect from specified session.
   *
   * @param session_id Session identifier to disconnect.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_disconnecting_devices
   **/
  virtual Error disconnect(const int session_id);

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
  virtual void sendFrame(ConnectionHandle connection_handle,
                         const uint8_t* data, size_t data_size, int user_data);

 protected:
  /**
   * @brief Frame.
   *
   * Used to store data frames that must be sent to remote device.
   **/
  class Frame {
   public:
    /**
     * @brief Constructor.
     *
     * @param UserData User data
     * @param Data Frame data. SFrame stores a copy of this data,
     *             i.e. data may be freed after SFrame object is constructed.
     * @param DataSize Size of frame data in bytes.
     **/
    Frame(int user_data, const uint8_t* data, const size_t data_size);

    /**
     * @brief Destructor.
     *
     * Frees stored frame data.
     **/
    ~Frame(void);

   private:
    /**
     * @brief User data
     **/
    int user_data_;

    /**
     * @brief Frame data.
     **/
    uint8_t * data_;

    /**
     * @brief Frame data size in bytes.
     **/
    size_t data_size_;
  };

  /**
   * @brief Frame queue.
   **/
  typedef std::queue<Frame*> FrameQueue;

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
   * @brief Vector of devices.
   **/
  typedef std::vector<Device*> DeviceVector;

  /**
   * @brief Devices map.
   **/
  typedef std::map<DeviceHandle, Device*> DeviceMap;

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

    void set_connection_socket(int socket) {
      connection_socket_ = socket;
    }

    int connection_socket() const {
      return connection_socket_;
    }

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
    const int session_id_;

    /**
     * @brief Thread that handles connection.
     **/
    pthread_t connection_thread_;

    /**
     * @brief File descriptors of notification pipe.
     *
     * Notification pipe is used to wake up connection thread
     * on external event (e.g. new data is available to send or
     * connection is requested to be terminated).
     *
     * mNotificationPipeFds[0] is a descriptor of the read end of the pipe
     * (the one that is used in poll() by connection thread) and
     * mNotificationPipeFds[1] is a descriptor of the write end of the pipe
     * (the one that is used in methods exposed to transport manager to
     * wake up connection thread when necessary).
     *
     * @note eventfd cannot be used instead of a pipe because it does not
     *       conform to POSIX (eventfd is Linux-specific).
     **/
    int notification_pipe_fds_[2];

    /**
     * @brief Descriptor of connection socket.
     **/
    int connection_socket_;

    /**
     * @brief Frames that must be sent to remote device.
     **/
    FrameQueue frames_to_send_;

    /**
     * @brief Terminate flag.
     *
     * This flag is set to notify connection thread that connection
     * must be closed and connection thread must be terminated.
     **/
    bool terminate_flag_;
  };

  /**
   * @brief Parameters for starting connection thread.
   **/
  struct ConnectionThreadParameters {
    /**
     * @brief Pointer to device adapter.
     **/
    DeviceAdapterImpl* device_adapter;
    Connection* connection;
  };

  /**
   * @brief Wait until all device adapter threads are terminated.
   *
   * Every device adapter must call this method in destructor to wait
   * for other threads to terminate before destroying device adapter.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_threads_termination
   **/
  void waitForThreadsTermination();


  /**
   * @brief Create connection.
   *
   * Check if connection is not in connection map
   * and if it's not, add it to map.
   *
   * @param device_handle Device handle.
   * @param app_handle Application handle.
   * @param session_id Session identifier.
   * @param connection Output pointer created connection or 0 in case of error.
   *
   * @return Status.
   **/
  DeviceAdapter::Error createConnection(const DeviceHandle device_handle,
                                        const ApplicationHandle app_handle,
                                        const int session_id,
                                        Connection** connection);

  /**
   * @brief Delete connection.
   *
   * Destroy connection object and remove it from map.
   *
   * @param connection Connection to delete.
   **/
  void deleteConnection(Connection* connection);


  /**
   * @brief Start connection.
   *
   * Start connection thread.
   *
   * @param connection Connection to start.
   *
   * @return true if connection thread has been started, false otherwise.
   **/
  Error startConnection(Connection* connection);

  /**
   * @brief Stop connection.
   *
   * This method only initiates connection termination. It returns immediately
   * without waiting for actual termination of the connection.
   *
   * @param connection Connection to stop.
   **/
  Error stopConnection(Connection* connection);

  /**
   * @brief Wait for device scan request.
   *
   * Wait until scanForNewDevices() is called or timeout
   * expires.
   *
   * @param Timeout Timeout value in seconds. 0 means no
   *                timeout.
   *
   * @return true if scanForNewDevices() has been called,
   *         false if timeout expired.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
   **/
  bool waitForDeviceScanRequest(const time_t timeout);

  /**
   * @brief Handle communication.
   *
   * Handle communication for specified connection (send/receive data
   * to/from connection socket).
   *
   * This method must be called from connection thread implementation
   * when connection is established and connection socket descriptor
   * is set for the connection.
   *
   * This methods returns when connection is terminated.
   *
   * @param ConnectionHandle Connection handle.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_handling_communication
   **/
  void handleCommunication(Connection* connection);

  /**
   * @brief Update client device list.
   *
   * This method is called when list of devices is changed to
   * notify device adapter listener about new list of devices.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_update_client_device_list
   **/
  void updateClientDeviceList(void);

  /**
   * @brief Create list of connections possible for specified device.
   *
   * This method is called from connectDevice(). Device adapter may implement
   * this method to provide list of connections that must be running on
   * connected device.
   *
   * @param DeviceHandle Device handle.
   * @param ConnectionsList Reference to connections list that must be filled.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connecting_devices
   **/
  virtual void createConnectionsListForDevice(
      const DeviceHandle device_handle,
      std::vector<Connection*>& connection_list);

  /**
   * @brief Device adapter main thread.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
   **/
  virtual void mainThread() = 0;

  /**
   * @brief Connection thread.
   *
   * This method is responsible for establishing connection and communicating
   * with remote device via specified connection. It must remove itself from
   * connection map when connection is terminated before terminating connection thread.
   *
   * @param connection Connection.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connection_thread
   **/
  virtual void connectionThread(Connection* connection) = 0;

  /**
   * @brief Connections map.
   **/
  typedef std::map<int, Connection*> ConnectionMap;

  /**
   * @brief Logger.
   **/
  static log4cxx::LoggerPtr logger_;

  /**
   * @brief Listener for device adapter notifications.
   **/
  DeviceAdapterListener& listener_;

  /**
   * @brief Handle generator implementation.
   **/
  DeviceHandleGenerator& handle_generator_;

  /**
   * @brief Flag indicating that device scan was requested.
   *
   * This flag is set in scanForNewDevices and reset after requested
   * device scan is completed.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
   **/
  bool device_scan_requested_;

  /**
   * @brief Mutex restricting access to DeviceScanRequested flag.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
   **/
  pthread_mutex_t device_scan_requested_mutex_;

  /**
   * @brief Conditional variable for signaling discovery thread about requested device scan.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
   **/
  pthread_cond_t device_scan_requested_cond_;

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

  /**
   * @brief Shutdown flag.
   *
   * This flag is set to true on shutdown to inform all device adapter
   * threads that device adapter shutdown is in progress. After setting
   * this flag device adapter waits until all its threads are terminated.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_threads_termination
   **/
  bool shutdown_flag_;

 private:
  /**
   * @brief Start routine for device adapter main thread.
   *
   * @param Data Must be pointer to CDeviceAdapter instance.
   *
   * @return Thread return value.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
   **/
  static void* mainThreadStartRoutine(void* data);

  /**
   * @brief Connection thread start routine.
   *
   * @param Data Must be pointer to SConnectionThreadParameters. Ownership
   *             of connection thread parameters is passed to connection thread, i.e.
   *             connection thread is responsible for freeing this object.
   *
   * @return Thread return value.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_connection_thread
   **/
  static void* connectionThreadStartRoutine(void *data);

  /**
   * @brief ID of device adapter main thread.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
   **/
  pthread_t main_thread_;

  /**
   * @brief Flag indicating whether the device adapter main thread has been started successfully.
   *
   * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
   **/
  bool main_thread_started_;
};

} // namespace transport_manager

#endif // #ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_IMPL_H_

