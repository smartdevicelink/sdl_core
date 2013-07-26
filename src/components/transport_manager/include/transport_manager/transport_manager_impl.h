/**
 * \file transport_manager_impl.h
 * \brief Class transport_manager_implr header.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL

#include <queue>
#include <map>
#include <list>

#include "utils/logger.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/device_adapter.h"
#include "transport_manager/device_adapter_listener_impl.h"
#include <transport_manager/timer.h>

namespace transport_manager {

enum {
  E_SUCCESS = 0,
  E_TM_IS_NOT_INITIALIZED,
  E_INVALID_HANDLE,
  E_CONNECTION_IS_TO_SHUTDOWN
};


//todo: add no_copy_constr where necessary
//todo: add explicit where necessary

struct TransportManagerAttr {
  unsigned long disconnectTimeout;
};

/**
 * @brief Interface of transport manager.
 * @interface TransportManager
 **/
class TransportManagerImpl : public TransportManager {
  static TransportManagerAttr default_config_;
 public:
  /**
   * @brief provide default instance of transport manager
   *
   * @see @ref components_transportmanager_client_device_management
   **/
  static TransportManagerImpl *instance(void);

  /**
   * @brief Destructor.
   **/
  virtual ~TransportManagerImpl(void);

  /**
   * @brief initialize TM
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   */
  void init(void);

  /**
   * @brief Start scanning for new devices.
   *
   * @see @ref components_transportmanager_client_device_management
   **/
  virtual int searchDevices(void);

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param DeviceHandle Handle of device to connect to.
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void connectDevice(const DeviceHandle &device_id,
                             const ApplicationHandle &app_id);

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param DeviceHandle Handle of device to disconnect from.
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void disconnectDevice(const DeviceHandle &device_id);

  static void disconnectRoutine(void* p);
  virtual void disconnect(const ConnectionUID &connection);
  virtual void disconnectForce(const ConnectionUID &connection);
  /**
   * @brief post new mesage into TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   *
   * @return true if succeed, false if connection is going to shut down
   **/
  virtual int sendMessageToDevice(const RawMessageSptr message);

  /**
   * @brief receive event from device
   *
   * @param new event
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void receiveEventFromDevice(const DeviceAdapterEvent &event);

  /**
   * @brief register event listener
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void addEventListener(TransportManagerListener *listener);

  /**
   * @brief unregister event listener
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void removeEventListener(TransportManagerListener *listener);

  /**
   * @brief add new device adapter
   *
   * @param device adapter
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void addDeviceAdapter(device_adapter::DeviceAdapter *device_adapter);

  /**
   * @brief register listener that would be used to catch adapter's events
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
//todo: discuss with Alexandr and Polina - do we nedd this feature?
//  virtual void addAdapterListener(device_adapter::DeviceAdapter *adapter, device_adapter::DeviceAdapterListener *listener);
  /**
   * @brief remove device from internal storages
   *
   * @param event device id
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void removeDevice(const DeviceHandle &device);

  /**
   * @brief interface function to wake up adapter listener thread
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  pthread_cond_t *getDeviceListenerThreadWakeup(void);

 protected:
  TransportManagerAttr config_;

  template<class Proc, class ... Args>
  void raiseEvent(Proc proc, Args ... args) {
    //todo(YK) change it to c++2003 std.
    for (TransportManagerListenerList::iterator  it = transport_manager_listener_.begin(); it != transport_manager_listener_.end(); ++it) {
      ((*it)->*proc)(args...);
    }
  }

  /**
   * @brief post new mesage into TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void postMessage(const RawMessageSptr message);

  /**
   * @brief update message in queue
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  /*not clear when this function shall be used
   * void updateMessage(const RawMessageSptr old_message, const RawMessageSptr new_message);*/

  /**
   * @brief remove mesage from TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void removeMessage(const RawMessageSptr message);

  void removeEvent(const DeviceAdapterEvent &event);

  /**
   * @brief post new event from device
   *
   * @param new event
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void postEvent(const DeviceAdapterEvent &event);

  class AdapterHandler {
   public:
    typedef std::vector<device_adapter::DeviceAdapter *> AdapterList;
    device_adapter::DeviceAdapter *getAdapterBySession(ConnectionUID sid);
    device_adapter::DeviceAdapter *getAdapterByDevice(
        transport_manager::DeviceHandle did);
    void addSession(device_adapter::DeviceAdapter *da, ConnectionUID sid);
    void removeSession(device_adapter::DeviceAdapter *da, ConnectionUID sid);
    void addDevice(device_adapter::DeviceAdapter *da,
                   transport_manager::DeviceHandle did);
    void addAdapter(device_adapter::DeviceAdapter *da);
    void removeDevice(const transport_manager::DeviceHandle &device);
    const AdapterList &device_adapters(void);

    ~AdapterHandler();
    AdapterHandler();

   private:
    /**
     * @brief Device adapters.
     **/
    AdapterList device_adapters_;

    /**
     * @brief container that used to get device id by session id
     **/

    std::map<ConnectionUID, device_adapter::DeviceAdapter *> session_to_adapter_map_;

    /**
     * @brief container that used to get adapter id by device id
     * filled after search process done and used in connect function
     **/
    // FIXME: Team had decided one device cannot be shared between multiple adapters.
    //         Change multimap to map
    std::map<transport_manager::DeviceHandle, device_adapter::DeviceAdapter *> device_to_adapter_map_;

  };

  /**
   * @brief type for message queue
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  typedef std::list<RawMessageSptr> MessageQueue;

  /**
   * @brief type for message queue
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  typedef std::vector<DeviceAdapterEvent> EventQueue;

  /**
   * @brief default constructor
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  TransportManagerImpl(const TransportManagerAttr &config);

  /**
   * @brief constructor used to create new TM with device adapter
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  TransportManagerImpl(
      std::vector<device_adapter::DeviceAdapter *> device_adapter_list);

  static void *messageQueueStartThread(void *data);
  /**
   * @brief scan message's queue and pull messages according to priority and serial number
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   */
  void messageQueueThread(void);

  static void *eventListenerStartThread(void *);
  /**
   * @brief wait until event happens
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   */
  void eventListenerThread(void);

  /**
   * \brief For logging.
   */
  static log4cxx::LoggerPtr logger_;

  AdapterHandler adapter_handler_;

  /**
   * @brief store messages
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  MessageQueue message_queue_;

  /**
   * @brief Mutex restricting access to messages.
   **/
  mutable pthread_mutex_t message_queue_mutex_;

  /**
   * @brief store events from comming device
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  EventQueue event_queue_;

  /**
   * @brief flag that indicates that thread is active
   * if it is false then threads exist main loop
   **/
  volatile bool all_thread_active_;

  typedef std::list<TransportManagerListener *> TransportManagerListenerList;
  /**
   * @brief listener that would be called when TM's event happened.
   **/
  TransportManagerListenerList transport_manager_listener_;

  /**
   * @brief ID of message queue processing thread
   **/
  pthread_t messsage_queue_thread_;

  /**
   * @brief conditional event thread
   **/
  pthread_t event_queue_thread_;

  /**
   * @brief condition variable to wake up event
   **/
  pthread_cond_t device_listener_thread_wakeup_;

  /**
   * @brief Mutex restricting access to events.
   **/
  mutable pthread_mutex_t event_queue_mutex_;

  bool is_initialized_;
 private:
  struct Connection {
    ConnectionUID id;
    DeviceHandle device;
    ApplicationHandle application;
    Timer timer;
    bool shutDown;
    int messages_count;

    Connection(const ConnectionUID &id, const DeviceHandle &dev_id,
               const ApplicationHandle &app_id)
        : id(id),
          device(dev_id),
          application(app_id),
          shutDown(false),
          messages_count(0) {
    }
    Connection()
        : id(-1),
          application(-1),
          shutDown(false),
          messages_count(-1) {
    }
  };
  int connection_id_counter_;
  std::map<ConnectionUID, Connection> id_to_connections_map_;
  std::map<std::pair<DeviceHandle, ApplicationHandle>, Connection> dev_app_to_connections_map_;

  Connection& getConnection(const ConnectionUID &id);
  Connection& getConnection(const DeviceHandle& device,
                            const ApplicationHandle& application);
};
//class
}//namespace

#endif
