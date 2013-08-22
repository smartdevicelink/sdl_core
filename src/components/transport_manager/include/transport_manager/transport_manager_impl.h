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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_

#include <queue>
#include <map>
#include <list>
#include <algorithm>

#include "utils/logger.h"
#include "protocol_handler/protocol_handler.h"//YK: temp solution until B1.0 release
#include "transport_manager/common.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/device_adapter/device_adapter.h"
#include "transport_manager/device_adapter/device_adapter_listener_impl.h"
#include <transport_manager/timer.h>

using ::transport_manager::AdapterIterator;
using ::transport_manager::device_adapter::DeviceAdapterSptr;
using ::transport_manager::device_adapter::DeviceAdapterListener;

/**
 * namespace transport_manager
 * @brief transport_manager namespace
 */
namespace transport_manager {

/**
 * @brief Transport manager states ???
 */
enum {
  E_SUCCESS = 0,
  E_TM_IS_NOT_INITIALIZED,
  E_INVALID_HANDLE,
  E_CONNECTION_IS_TO_SHUTDOWN,
  E_SEARCH_IN_PROGRESS,
  E_CONNECTION_EXISTS,
  E_INTERNAL_ERROR,
};

struct TransportManagerAttr {
  unsigned long disconnectTimeout; /* milliseconds */
};

/**
 * @brief Interface of transport manager.
 * @interface TransportManager
 **/
class TransportManagerImpl : public TransportManager {
 public:
  struct Connection {
    ConnectionUID id;
    DeviceUID device;
    ApplicationHandle application;
  };

  /**
   * @brief Destructor.
   **/
  virtual ~TransportManagerImpl(void);

  /**
   * @brief initialize TM
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   */
  int init(void);

  /**
   * @brief Start scanning for new devices.
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_device_management
   **/
  virtual int searchDevices(void);

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param DeviceHandle Handle of device to connect to.
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int connectDevice(const DeviceHandle &device_id);

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param DeviceHandle Handle of device to disconnect from.
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int disconnectDevice(const DeviceHandle &device_id);

  static void disconnectFailedRoutine(void* p);

  /**
   * @brief Disconnect from applications connected on device by connection id.
   *
   * @param ConnectionUID connection id.
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int disconnect(const ConnectionUID &connection);

  virtual int disconnectForce(const ConnectionUID &connection);
  /**
   * @brief post new mesage into TM's queue
   *
   * @param new message container
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   *
   **/
  virtual int sendMessageToDevice(const RawMessageSptr message);

  /**
   * @brief receive event from device
   *
   * @param new event
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int receiveEventFromDevice(const DeviceAdapterEvent &event);

  /**
   * @brief register event listener
   *
   * @param event listener
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int addEventListener(TransportManagerListener *listener);

  /**
   * @brief unregister event listener
   *
   * @param event listener
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int removeEventListener(TransportManagerListener *listener);

  /**
   * @brief add new device adapter
   *
   * @param device adapter
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int addDeviceAdapter(device_adapter::DeviceAdapterSptr device_adapter);

  int removeDeviceAdapter(device_adapter::DeviceAdapterSptr device_adapter);

  /**
   * @brief remove device from internal storages
   *
   * @param event device id
   *
   * @return code error
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  virtual int removeDevice(const DeviceHandle &device);

  /*
   * turns on or off visibility of SDL to mobile devices
   * when visibility is ON (on_off = true) mobile devices are able to connect
   * otherwise ((on_off = false)) SDL is not visible from outside
   *
   * @return code error
   */
  virtual int Visibility(const bool &on_off)const;

  const std::vector<DeviceInfo>& getDeviceList() const;

  virtual void set_protocol_handler(protocol_handler::ProtocolHandler *ph);//YK: temp solution until B1.0 release

  std::vector<Connection> getConnectionList();

  /**
   * @brief default constructor
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  explicit TransportManagerImpl(const TransportManagerAttr &config);

 protected:
  TransportManagerAttr config_;

  template<class Proc, class ... Args>
  void raiseEvent(Proc proc, Args ... args) {
    for (TransportManagerListenerList::iterator it = transport_manager_listener_
        .begin(); it != transport_manager_listener_.end(); ++it) {
      ((*it)->*proc)(args...);
    }
  }

  /**
   * @brief Post new message into TM's message queue
   *
   * @param message shared pointer to message
   **/
  void postMessage(const RawMessageSptr message);

  /**
   * @brief update message in queue
   *
   * @param message shared pointer to raw massage
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
   * @brief constructor used to create new TM with device adapter
   *
   * @param
   *
   * @see @ref components_transportmanager_client_connection_management
   **/
  explicit TransportManagerImpl(
      std::vector<device_adapter::DeviceAdapterSptr> device_adapter_list);

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

  pthread_cond_t message_queue_cond;

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
   * @brief Conditional event thread
   **/
  pthread_t event_queue_thread_;

  /**
   * @brief Condition variable to wake up event
   **/
  pthread_cond_t device_listener_thread_wakeup_;

  /**
   * @brief Mutex restricting access to events.
   **/
  mutable pthread_mutex_t event_queue_mutex_;

  /**
   * @brief Flag that TM is initialized
   */
  bool is_initialized_;
 private:

  std::vector<DeviceInfo> device_list_;

  struct Handle2GUIDConverter {
    typedef std::vector<DeviceUID> ConversionTable;

    DeviceHandle UidToHandle(const DeviceUID &dev_uid) {
      bool is_new = true;
      return UidToHandle(dev_uid, is_new);
    }

    DeviceHandle UidToHandle(const DeviceUID &dev_uid, bool &is_new) {
      ConversionTable::iterator it = std::find(conversion_table_.begin(),
                                               conversion_table_.end(),
                                               dev_uid);
      if (it != conversion_table_.end()) {
        is_new = false;
        return std::distance(conversion_table_.begin(), it) + 1;  // handle begin since 1 (one)
      }
      is_new = true;
      conversion_table_.push_back(dev_uid);
      return conversion_table_.size();  // handle begin since 1 (one)
    }

    DeviceUID HandleToUid(DeviceHandle handle) {
      if (handle == 0 || handle > conversion_table_.size()) {
        return DeviceUID();
      }
      return conversion_table_[handle - 1];  // handle begin since 1 (one)
    }

    ConversionTable conversion_table_;
  };

  /**
   * @brief Converter variable (Device ID -> Device Handle; Device Handle -> Device ID)
   */
  Handle2GUIDConverter converter_;

  /**
   * @brief Structure that contains internal connection parameters
   */
  struct ConnectionInternal {
    DeviceAdapterSptr device_adapter;
    ConnectionUID id;
    DeviceUID device;
    ApplicationHandle application;
    Timer timer;
    bool shutDown;
    int messages_count;

    ConnectionInternal(DeviceAdapterSptr device_adapter, const ConnectionUID &id, const DeviceUID &dev_id,
               const ApplicationHandle &app_id)
        : device_adapter(device_adapter),
          id(id),
          device(dev_id),
          application(app_id),
          shutDown(false),
          messages_count(0) {
    }
  };

  explicit TransportManagerImpl(const TransportManagerImpl&);
  int connection_id_counter_;
  std::vector<ConnectionInternal> connections_;
  std::map<DeviceUID, DeviceAdapterSptr> device_to_adapter_map_;
  std::vector<DeviceAdapterSptr> device_adapters_;
  int da_scanned; // Count of device adapters have reported scan result
  bool search_in_progress_; // If search in progress, another search process doesn't start
  /** For keep listeners which were add TMImpl */
  std::map<DeviceAdapterSptr, DeviceAdapterListenerImpl*> device_adapter_listeners_;

  void addConnection(const ConnectionInternal& c);
  void removeConnection(int id);
  ConnectionInternal* getConnection(const ConnectionUID &id);
  ConnectionInternal* getConnection(const DeviceUID& device,
                            const ApplicationHandle& application);

  protocol_handler::ProtocolHandler *protocol_handler_;//YK: temp solution until B1.0 release
  void addDataToContainer(ConnectionUID id, std::map<ConnectionUID, std::pair<unsigned int, unsigned char *>> &container, unsigned char * data, unsigned int data_size);
  bool getFrameSize(unsigned char *data,  unsigned int data_size, unsigned int &frame_size);
  bool getFrame(std::map<ConnectionUID, std::pair<unsigned int, unsigned char *>> &container, ConnectionUID id, unsigned int frame_size, unsigned char **frame);

  friend bool DeviceAdapterListenerImpl::FindSharedPtr(const DeviceAdapter*, AdapterIterator&);
};//class ;

}  // namespace transport_manager

#endif
