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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_H_

#include <queue>
#include <map>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>
#include <functional>

#include "utils/timer.h"
#include "utils/timer_task_impl.h"
#include "utils/rwlock.h"

#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"
#include "protocol/common.h"
#ifdef TELEMETRY_MONITOR
#include "transport_manager/telemetry_observer.h"
#include "telemetry_monitor/telemetry_observable.h"
#endif  // TELEMETRY_MONITOR
#include "utils/threads/message_loop_thread.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/transport_manager_settings.h"

namespace transport_manager {

typedef threads::MessageLoopThread<std::queue<protocol_handler::RawMessagePtr> >
    RawMessageLoopThread;
typedef threads::MessageLoopThread<std::queue<TransportAdapterEvent> >
    TransportAdapterEventLoopThread;
typedef utils::SharedPtr<timer::Timer> TimerSPtr;
typedef std::map<DeviceUID, TransportAdapter*> DeviceToAdapterMap;

/**
 * @brief Implementation of transport manager.s
 */
class TransportManagerImpl
    : public TransportManager,
      public RawMessageLoopThread::Handler,
      public TransportAdapterEventLoopThread::Handler
#ifdef TELEMETRY_MONITOR
      ,
      public telemetry_monitor::TelemetryObservable<TMTelemetryObserver>
#endif  // TELEMETRY_MONITOR
      {
 public:
  struct Connection {
    ConnectionUID id;
    DeviceUID device;
    ApplicationHandle application;
  };

 private:
  /**
   * @brief Structure that contains internal connection parameters
   */
  struct ConnectionInternal : public Connection {
    TransportManagerImpl* transport_manager;
    TransportAdapter* transport_adapter;
    TimerSPtr timer;
    bool shutdown_;
    DeviceHandle device_handle_;
    int messages_count;
    bool active_;

    ConnectionInternal(TransportManagerImpl* transport_manager,
                       TransportAdapter* transport_adapter,
                       const ConnectionUID id,
                       const DeviceUID& dev_id,
                       const ApplicationHandle& app_id,
                       const DeviceHandle device_handle);

    void DisconnectFailedRoutine();
  };

 public:
  /**
   * @brief Constructor.
   **/
  explicit TransportManagerImpl(const TransportManagerSettings& settings);

  /**
   * @brief Destructor.
   **/
  virtual ~TransportManagerImpl();

  /**
   * @brief Initialize transport manager.
   *
   * @return Code error.
   */
  int Init(resumption::LastState& last_state) OVERRIDE;

  /**
   * Reinitializes transport manager
   * @return Error code
   */
  virtual int Reinit() OVERRIDE;

  /**
   * @brief Start scanning for new devices.
   *
   * @return Code error.
   **/
  int SearchDevices() OVERRIDE;

  /**
   * @brief Connect to all applications discovered on device.
   *
   * @param device_id Handle of device to connect to.
   *
   * @return Code error.
   **/
  int ConnectDevice(const DeviceHandle device_id) OVERRIDE;

  /**
   * @brief Disconnect from all applications connected on device.
   *
   * @param device_id Handle of device to Disconnect from.
   *
   * @return Code error.
   **/
  int DisconnectDevice(const DeviceHandle device_id) OVERRIDE;

  /**
   * @brief Disconnect from applications connected on device by connection
   *unique identifier.
   *
   * @param connection Connection unique identifier.
   *
   * @return Code error.
   **/
  int Disconnect(const ConnectionUID connection_id) OVERRIDE;

  /**
   * @brief Disconnect and clear all unreceived data.
   *
   * @param connection Connection unique identifier.
   */
  int DisconnectForce(const ConnectionUID connection_id) OVERRIDE;
  /**
   * @brief Post new message in queue for massages destined to device.
   *
   * @param message Smart pointer to the raw massage.
   *
   * @return Code error.
   **/
  int SendMessageToDevice(
      const protocol_handler::RawMessagePtr message) OVERRIDE;

  /**
   * @brief RunAppOnDevice allows to run specific application on the certain
   *device.
   *
   * @param device_handle device identifier to run application on.
   *
   * @param bundle_id application id also known as bundle id on some devices to
   *run.
   */
  void RunAppOnDevice(const DeviceHandle device_handle,
                      const std::string& bundle_id) OVERRIDE;

  void OnDeviceConnectionSwitched(const DeviceHandle handle) FINAL;

  /**
   * @brief Post event in the event queue.
   *
   * @param event Current event information.
   *
   * @return Code error.
   **/
  int ReceiveEventFromDevice(const TransportAdapterEvent& event) OVERRIDE;

  /**
   * @brief Post listener to the container of transport manager listeners.
   *
   * @param listener Pointer to the transport manager listener.
   *
   * @return Code error.
   **/
  int AddEventListener(TransportManagerListener* listener) OVERRIDE;

  int Stop() OVERRIDE;

  /**
   * @brief Add device adapter to the container of device adapters.
   *
   * @param transport_adapter Smart pointer to the device adapter.
   *
   * @return Code error.
   **/
  int AddTransportAdapter(
      transport_adapter::TransportAdapter* transport_adapter) OVERRIDE;

  /**
   * @brief Remove device from the container that hold devices.
   *
   * @param device Handle of device.
   *
   * @return Code error.
   **/
  int RemoveDevice(const DeviceHandle device) OVERRIDE;

  /**
   * @brief Turns on or off visibility of SDL to mobile devices
   * when visibility is ON (on_off = true) mobile devices are able to connect
   * otherwise ((on_off = false)) SDL is not visible from outside
   *
   * @return Code error.
   */
  int Visibility(const bool& on_off) const OVERRIDE;

  /**
   * @brief Updates total device list with info from specific transport adapter.
   * @param ta Transport adapter
   */
  void UpdateDeviceList(TransportAdapter* ta);

#ifdef TELEMETRY_MONITOR
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTelemetryObserver(TMTelemetryObserver* observer);
#endif  // TELEMETRY_MONITOR

  /**
   * @brief Constructor.
   **/
  TransportManagerImpl();

  const TransportManagerSettings& get_settings() const;

 protected:
  template <class Proc, class... Args>
  void RaiseEvent(Proc proc, Args... args) {
    for (TransportManagerListenerList::iterator it =
             transport_manager_listener_.begin();
         it != transport_manager_listener_.end();
         ++it) {
      ((*it)->*proc)(args...);
    }
  }

  /**
   * @brief Put massage in the container of massages.
   *
   * @param message Smart pointer to the raw massage.
   **/
  void PostMessage(const ::protocol_handler::RawMessagePtr message);

  void Handle(::protocol_handler::RawMessagePtr msg);
  void Handle(TransportAdapterEvent msg);

  /**
   * @brief Post event to the container of events.
   *
   * @param event Event of device adapter.
   **/
  void PostEvent(const TransportAdapterEvent& event);

  typedef std::list<TransportManagerListener*> TransportManagerListenerList;
  /**
   * @brief listener that would be called when TM's event happened.
   **/
  TransportManagerListenerList transport_manager_listener_;

  /**
   * @brief Flag that TM is initialized
   */
  bool is_initialized_;

#ifdef TELEMETRY_MONITOR
  TMTelemetryObserver* metric_observer_;
#endif  // TELEMETRY_MONITOR

 private:
  /**
    * @brief Structure that contains conversion functions (Device ID -> Device
    * Handle; Device Handle -> Device ID)
    */
  struct Handle2GUIDConverter {
    /**
     * @brief ConversionTable Records uid/connection type/handle
     */
    typedef std::vector<std::tuple<DeviceUID, std::string, DeviceHandle> >
        ConversionTable;

    /**
     * @brief The HandleFinder struct helper to search for hanlde in coversion
     * table
     */
    struct HandleFinder {
      explicit HandleFinder(const DeviceHandle& look_for)
          : look_for_(look_for) {}

      bool operator()(const ConversionTable::value_type value) const {
        return look_for_ == std::get<2>(value);
      }

      const DeviceHandle look_for_;
    };

    /**
     * @brief UidToHandle Converts UID to handle considering connection type as
     * UID may be the same in case device is connected over Bluetooth/USB (e.g.
     * for IAP2)
     * @param dev_uid Device UID
     * @param connection_type Connection type
     * @return Device handle
     */
    DeviceHandle UidToHandle(const DeviceUID& dev_uid,
                             const std::string& connection_type);

    /**
     * @brief HandleToUid Converts handle to device UID
     * @param handle Device handle
     * @return Device UID
     */
    DeviceUID HandleToUid(const DeviceHandle handle);

   private:
    ConversionTable conversion_table_;
    std::hash<std::string> hash_function_;
    sync_primitives::RWLock conversion_table_lock_;
  };

  /**
   * @brief Converter variable (Device ID -> Device Handle; Device Handle ->
   * Device ID)
   */
  Handle2GUIDConverter converter_;

#ifdef BUILD_TESTS
 public:
  Handle2GUIDConverter& get_converter() {
    return converter_;
  }
#endif  // BUILD_TESTS

  explicit TransportManagerImpl(const TransportManagerImpl&);
  int connection_id_counter_;
  sync_primitives::RWLock connections_lock_;
  std::vector<ConnectionInternal> connections_;
  sync_primitives::RWLock device_to_adapter_map_lock_;
  typedef std::map<DeviceUID, TransportAdapter*> DeviceToAdapterMap;
  DeviceToAdapterMap device_to_adapter_map_;
  std::vector<TransportAdapter*> transport_adapters_;
  /** For keep listeners which were add TMImpl */
  std::map<TransportAdapter*, TransportAdapterListenerImpl*>
      transport_adapter_listeners_;
  RawMessageLoopThread message_queue_;
  TransportAdapterEventLoopThread event_queue_;
  const TransportManagerSettings& settings_;
  typedef std::vector<std::pair<const TransportAdapter*, DeviceInfo> >
      DeviceInfoList;
  sync_primitives::RWLock device_list_lock_;
  DeviceInfoList device_list_;

  timer::Timer device_switch_timer_;
  sync_primitives::Lock device_lock_;
  DeviceUID device_to_reconnect_;

  /**
   * @brief Adds new incoming connection to connections list
   * @param c New connection
   */
  void AddConnection(const ConnectionInternal& c);

  /**
     * @brief Removes connection from connections list
     * @param id Identifier of connection to be removed
     * @param transport_adapter Pointer to transport adapter
     * that holds connection
     */
  void RemoveConnection(const uint32_t id,
                        transport_adapter::TransportAdapter* transport_adapter);

  /**
     * @brief Deactivates all connections related to certain device
     * @param device_uid Device unique identifier
     */
  void DeactivateDeviceConnections(const DeviceUID& device_uid);
  /**
   * @brief Returns connection from connections list by connection identifier
   * @param id Connection identifier
   * @return Pointer to connection or NULL if connection could not be found
   */
  ConnectionInternal* GetConnection(const ConnectionUID id);

  /**
   * @brief Returns connection from connections list by device unique id
   * and application handle
   * @param device Device unique identifier
   * @param application Application handle
   * @return Pointer to connection or NULL if connection could not be found
   */
  ConnectionInternal* GetConnection(const DeviceUID& device,
                                    const ApplicationHandle& application);

  /**
     * @brief Returns active connection from connections list by device unique
   * id
     * and application handle
     * (this method returns only active connections as opposed to previous one)
     * @param device Device unique identifier
     * @param application Application handle
     * @return Pointer to connection or NULL if connection could not be found
     */
  ConnectionInternal* GetActiveConnection(const DeviceUID& device,
                                          const ApplicationHandle& application);

  bool TryDeviceSwitch(TransportAdapter* ta,
                       DeviceToAdapterMap::iterator value);

  void AddDataToContainer(
      ConnectionUID id,
      std::map<ConnectionUID, std::pair<unsigned int, unsigned char*> >&
          container,
      unsigned char* data,
      unsigned int data_size);
  bool GetFrameSize(unsigned char* data,
                    unsigned int data_size,
                    unsigned int& frame_size);
  bool GetFrame(std::map<ConnectionUID,
                         std::pair<unsigned int, unsigned char*> >& container,
                ConnectionUID id,
                unsigned int frame_size,
                unsigned char** frame);

  void OnDeviceAdded(TransportAdapter* ta);
  void OnDeviceListUpdated(TransportAdapter* ta);
  void DisconnectAllDevices();
  void TerminateAllAdapters();
  int InitAllAdapters();
  static Connection convert(const ConnectionInternal& p);
  void ReconnectionTimeout();

  /**
   * @brief UpdateDeviceMapping handles internal device-to-adapter mapping,
   * performs its update on adding/removal of devices. Also used by IAP2
   * switching flow to substitute BT with USB transport
   * @param ta Pointer to transport adapter
   */
  void UpdateDeviceMapping(TransportAdapter* ta);
};  // class TransportManagerImpl
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_IMPL_H_