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

#include "utils/logger.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/device_adapter.h"
#include "transport_manager/device_adapter_listener_impl.h"

namespace transport_manager {
const uint MAX_TM_THREADS = 2;

//todo: add no_copy_constr where necessary
//todo: add explicit where necessary

/**
 * @brief Interface of transport manager.
 * @interface TransportManager
 **/
class TransportManagerImpl : public TransportManager {
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
  virtual void searchDevices(void);

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param DeviceHandle Handle of device to connect to.
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void connectDevice(const DeviceHandle &device_id,
                             const ApplicationHandle &app_id,
                             const SessionID &session_id);

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param DeviceHandle Handle of device to disconnect from.
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void disconnectDevice(const SessionID &session_id);

  /**
   * @brief post new mesage into TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void sendMessageToDevice(const protocol_handler::RawMessage &message);

  /**
   * @brief receive event from device
   *
   * @param new event
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void receiveEventFromDevice(
      const DeviceAdapterListenerImpl::DeviceAdapterEvent &event);

  /**
   * @brief register event listener
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void registerEventListener(TransportManagerListener *listener);

  /**
   * @brief add new device adapter
   *
   * @param device adapter
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void addDeviceAdapter(DeviceAdapter *device_adapter);

  /**
   * @brief register listener that would be used to catch adapter's events
   *
   * @param event listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void registerAdapterListener(DeviceAdapterListener *listener);

  /**
   * @brief remove device from internal storages
   *
   * @param event device id
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void removeDevice(const DeviceHandle &device);

  /**
   * @brief accept device originated connection
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void acceptConnect(const DeviceHandle &device_id,
                             const ApplicationHandle &app_id,
                             const SessionID &session_id);

  /**
   * @brief decline device originated connection
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual void declineConnect(const DeviceHandle &device_id,
                              const ApplicationHandle &app_id);

  /**
   * @brief set new listener
   *
   * @param listener
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void set_device_adapter_listener(DeviceAdapterListener *listener);

  /**
   * @brief set tm's event listener
   *
   * @param event type, function address
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void set_transport_manager_listener(TransportManagerListener *listener);

  /**
   * @brief interface function to wake up adapter listener thread
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  pthread_cond_t *getDeviceListenerThreadWakeup(void);

 protected:

  /**
   * @brief post new mesage into TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void postMessage(const protocol_handler::RawMessage &message);

  /**
   * @brief update message in queue
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void updateMessage(const protocol_handler::RawMessage &message);

  /**
   * @brief remove mesage from TM's queue
   *
   * @param new message container
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void removeMessage(const protocol_handler::RawMessage &message);

  void removeEvent(const DeviceAdapterListenerImpl::DeviceAdapterEvent &event);

  /**
   * @brief post new event from device
   *
   * @param new event
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  void postEvent(const DeviceAdapterListenerImpl::DeviceAdapterEvent &event);

  class AdapterHandler {
   public:
    typedef std::vector<transport_manager::DeviceAdapter *> AdapterList;
    transport_manager::DeviceAdapter *getAdapterBySession(
        transport_manager::SessionID sid);
    transport_manager::DeviceAdapter *getAdapterByDevice(
        transport_manager::DeviceHandle did);
    void addSession(transport_manager::DeviceAdapter *da,
                    transport_manager::SessionID sid);
    void addDevice(transport_manager::DeviceAdapter *da,
                   transport_manager::DeviceHandle did);
    void addAdapter(transport_manager::DeviceAdapter *da);
    void removeSession(transport_manager::DeviceAdapter *da,
                       transport_manager::SessionID sid);
    void removeDevice(const transport_manager::DeviceHandle &device);
    AdapterList device_adapters(void);

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
    std::map<transport_manager::SessionID, transport_manager::DeviceAdapter *> session_to_adapter_map_;

    /**
     * @brief container that used to get adapter id by device id
     * filled after search process done and used in connect function
     **/
    std::multimap<transport_manager::DeviceHandle,
        transport_manager::DeviceAdapter *> device_to_adapter_multimap_;

  };

  /**
   * @brief type for mesage queue
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  typedef std::vector<protocol_handler::RawMessage> MessageQueue;

  /**
   * @brief type for mesage queue
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  typedef std::vector<DeviceAdapterListenerImpl::DeviceAdapterEvent> EventQueue;

  /**
   * @brief type for
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  typedef struct {
    DeviceAdapter *device_adapter;
    DeviceHandle device_handle;
  } ConnectionHandle;

  /**
   * @brief default constructor
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  TransportManagerImpl();

  /**
   * @brief constructor used to create new TM with device adapter
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  TransportManagerImpl(DeviceAdapter *device_adapter);

  /**
   * @brief constructor used to create new TM with device adapter
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  TransportManagerImpl(std::vector<DeviceAdapter *> device_adapter_list);

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
   * @brief whait until event happens
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
  mutable bool all_thread_active_;

  /**
   * @brief Device adapter listener.
   **/
  DeviceAdapterListener *device_adapter_listener_;

  /**
   * @brief Device handle generator.
   **/
  DeviceHandleGenerator *device_handle_generator_;

  /**
   * @brief listener that would be called when TM's event happened.
   **/
  TransportManagerListener *transport_manager_listener_;

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

};
//class
}//namespace

#endif
