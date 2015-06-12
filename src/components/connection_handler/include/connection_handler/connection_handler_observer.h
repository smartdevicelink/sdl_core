/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_OBSERVER_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_OBSERVER_H_

#include "connection_handler/device.h"
#include "connection_handler/connection.h"
#include "protocol/service_type.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink connection_handler namespace.
 */
namespace connection_handler {

/**
 * \class ConnectionHandlerObserver
 * \brief ConnectionHandlerObserver class
 */
class ConnectionHandlerObserver {
  public:
  /**
   * \brief Available devices list updated.
   *
   * Called when device scanning initiated with scanForNewDevices
   * is completed.
   *
   * \param DeviceList New list of available devices.
   **/
  virtual void OnDeviceListUpdated(
      const connection_handler::DeviceMap &device_list) = 0;

  /**
   * @brief Reaction to "Find new applications" request
   */
  virtual void OnFindNewApplicationsRequest() = 0;

  /**
   * \brief Removes device.
   *
   * Called when device has been removed from a list.
   *
   * \param DeviceHandle Handle of removed device.
   **/
  virtual void RemoveDevice(
      const connection_handler::DeviceHandle &device_handle) = 0;

  /**
   * \brief Callback function used by connection_handler
   * when Mobile Application initiates start of new service.
   * \param deviceHandle Device identifier within which session has to be started.
   * \param sessionKey Key of started session.
   * \param type Established service type
   */
  virtual bool OnServiceStartedCallback(
      const connection_handler::DeviceHandle &device_handle,
      const int32_t &session_key,
      const protocol_handler::ServiceType &type) = 0;

  /**
   * \brief Callback function used by connection_handler
   * when Mobile Application initiates service ending.
   * \param sessionKey Key of session which should be ended
   */
  virtual void OnServiceEndedCallback(
      const int32_t &session_key,
      const protocol_handler::ServiceType &type) = 0;

  /**
   * \brief Callback function used by ConnectionHandler
   * when Mobile Application start message flood
   * \param connection_key used by other components as application identifier
   */
  virtual void OnApplicationFloodCallBack(const uint32_t &connection_key) = 0;

  /**
   * \brief Callback function used by ConnectionHandler
   * when Mobile Application sends malformed message
   * \param connection_key used by other components as application identifier
   */
  virtual void OnMalformedMessageCallback(const uint32_t &connection_key) = 0;

 protected:
  /**
   * \brief Destructor
   */
  virtual ~ConnectionHandlerObserver() {
  }
};
}  // namespace connection_handler

#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_OBSERVER_H_
