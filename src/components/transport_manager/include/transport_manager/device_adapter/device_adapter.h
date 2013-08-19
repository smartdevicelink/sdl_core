/**
 * \file device_adapter.h
 * \brief DeviceAdapter class header file.
 *
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_DEVICE_ADAPTER_DEVICE_ADAPTER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_DEVICE_ADAPTER_DEVICE_ADAPTER

#include <string>
#include <vector>
#include <list>
#include "utils/shared_ptr.h"
#include "transport_manager/common.h"
#include "transport_manager/error.h"

namespace transport_manager {
namespace device_adapter {

class DeviceAdapterListener;
typedef std::string DeviceType;
typedef int ApplicationHandle;
typedef std::vector<ApplicationHandle> ApplicationList;
/**
 * @brief Type definition of container(vector) that holds device unique identifiers.
 */
typedef std::vector<DeviceUID> DeviceList;
/**
 * @brief Type definition for container(list) that holds pointers to device adapter listeners
 */
typedef std::list<DeviceAdapterListener *> DeviceAdapterListenerList;

class DeviceAdapter {
 public:
  enum Error {
    OK,
    FAIL,
    NOT_SUPPORTED,
    ALREADY_EXIST,
    BAD_STATE,
    BAD_PARAM
  };

 public:
  virtual ~DeviceAdapter() {
  }

  virtual DeviceType getDeviceType() const = 0;

  /* TODO
   virtual Error LoadState(DeviceAdapterState* state) = 0;
   virtual void SaveState(DeviceAdapterState* state) = 0;
   */
  virtual bool isInitialised() const = 0;

  /**
   * @brief Run device adapter
   *
   * Called from transport manager to start device adapter.
   *
   * @return error information about possible reason of initialization failure
   **/
  virtual Error init() = 0;

  /**
   * @brief Add listener to the container(list) of device adapter listeners.
   *
   * @param listener pointer to the device adapter listener
   */
  virtual void addListener(DeviceAdapterListener* listener) = 0;

  /**
   * @brief Remove listener from the container(list) of device dapter listeners.
   *
   * @param listener pointer to the device adapter listener
   */
  virtual void removeListener(DeviceAdapterListener* listener) = 0;
  virtual bool isSearchDevicesSupported() const = 0;

  /**
   * @brief Start scanning for new devices.
   *
   * List of new devices will be supplied in onDeviceListUpdated callback.
   *
   * @return error information about possible reason of searching devices failure
   **/
  virtual Error searchDevices() = 0;

  virtual bool isServerOriginatedConnectSupported() const = 0;

  /**
   * @brief Connect to the specified application discovered on device.
   *
   * @param device_handle Handle of device to connect to.
   * @param app_handle Handle of application to connect to.
   *
   * @return error information about possible reason of connection to the device failure
   **/
  virtual Error connect(const DeviceUID& device_handle,
                        const ApplicationHandle& app_handle) = 0;

  virtual bool isClientOriginatedConnectSupported() const = 0;

  /**
   * @brief Start client listener.
   *
   * @return Error information about possible reason of starting client listener failure.
   */
  virtual Error startClientListening() = 0;

  /**
   * @brief Stop client listener.
   *
   * @return Error information about possible reason of stopping client listener failure.
   */
  virtual Error stopClientListening() = 0;

  /**
   * @brief Disconnect from specified session.
   *
   * @param device_handle handle of device to disconnect from.
   * @param app_handle  handle of application.
   *
   * @return information about possible reason of disconnection from the device failure
   **/
  virtual Error disconnect(const DeviceUID& device_handle,
                           const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Disconnect from all sessions on specified device.
   *
   * @param device_handle Device handle to disconnect.
   *
   * @return information abour possible reason of disconnecting drom device failure
   **/
  virtual Error disconnectDevice(const DeviceUID& device_handle) = 0;

  /**
   * @brief Send frame.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param data smart pointer to the raw message.
   *
   * @return Error information about possible reason of sending data failure
   **/
  virtual Error sendData(const DeviceUID& device_handle,
                         const ApplicationHandle& app_handle,
                         const RawMessageSptr data) = 0;

  virtual DeviceList getDeviceList() const = 0;

  /**
   * @brief Get container(vector) of application unique identifiers that available at specified device.
   *
   * @param device_handle handle of device.
   *
   * @return container(vector) that holds application unique identifiers.
   */
  virtual ApplicationList getApplicationList(
      const DeviceUID& device_handle) const = 0;
  virtual std::string DeviceName(const DeviceUID &device_id) const = 0;
};

typedef utils::SharedPtr<DeviceAdapter> DeviceAdapterSptr;

inline bool operator < (const DeviceAdapterSptr a, const DeviceAdapterSptr b) {
  return a.get() < b.get();
}

inline bool operator == (const DeviceAdapterSptr a, const DeviceAdapter *b) {
  return a.get() == b;
}

inline bool operator == (const DeviceAdapter *a, const DeviceAdapterSptr b) {
  return a == b.get();
}

}  // namespace device_adapter
}  // namespace transport_manager

#endif  //  SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_DEVICE_ADAPTER_DEVICE_ADAPTER
