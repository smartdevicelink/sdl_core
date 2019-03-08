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

#ifndef SRC_COMPONENTS_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_OBSERVER_H_
#define SRC_COMPONENTS_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_OBSERVER_H_

#include <string>
#include "connection_handler/device.h"
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"
#include "protocol/service_type.h"

#ifdef ENABLE_SECURITY
#include "security_manager/ssl_context.h"
#endif  // ENABLE_SECURITY

struct BsonObject;

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
      const connection_handler::DeviceMap& device_list) = 0;

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
      const connection_handler::DeviceHandle& device_handle) = 0;

  /**
   * \brief Callback function used by connection_handler
   * when Mobile Application initiates start of new service.
   * Result must be notified through NotifyServiceStartedResult().
   * \param deviceHandle Device identifier within which session has to be
   * started.
   * \param sessionKey Key of started session.
   * \param type Established service type
   * \param params Configuration parameters for this service
   */
  virtual void OnServiceStartedCallback(
      const connection_handler::DeviceHandle& device_handle,
      const int32_t& session_key,
      const protocol_handler::ServiceType& type,
      const BsonObject* params) = 0;

  /**
   * \brief Callback function used by connection_handler
   * when Mobile Application initiates service ending.
   * \param session_key Key of session which should be ended
   * \param type Type of service which should be ended
   * \param close_reson Service close reason
   */
  virtual void OnServiceEndedCallback(
      const int32_t& session_key,
      const protocol_handler::ServiceType& type,
      const connection_handler::CloseSessionReason& close_reason) = 0;

  /**
   * @brief OnDeviceSwitchingStart is invoked on device transport switching
   * start (e.g. from Bluetooth to USB)
   * @param device_from device params being switched to new transport
   * @param device_to device params on the new transport
   */
  virtual void OnDeviceSwitchingStart(const Device& device_from,
                                      const Device& device_to) = 0;

  /**
   * @brief OnDeviceSwitchingFinish is invoked on device trasport switching end
   * i.e. timeout for switching is expired
   * @param device_uid UID of device being switched
   */
  virtual void OnDeviceSwitchingFinish(const std::string& device_uid) = 0;

  /**
   * @brief Check if application with specified app_id has NAVIGATION HMI type
   * @param app_id id of application to check
   * @return true if application is navi otherwise returns false
   */
  virtual bool CheckAppIsNavi(const uint32_t app_id) const = 0;

#ifdef ENABLE_SECURITY
  /**
   * @brief Get unique handshake context by application id
   * @param key id of application
   * @return generated handshake context or empty context if application with
   * provided id does not exist
   */
  virtual security_manager::SSLContext::HandshakeContext GetHandshakeContext(
      uint32_t key) const = 0;
#endif  // ENABLE_SECURITY

  /**
   * \brief Called when secondary transport for a particular app is started.
   * \param device_handle Device identifier on which the secondary transport is
   * started.
   * \param session_key session ID representing the app
   */
  virtual void OnSecondaryTransportStartedCallback(
      const connection_handler::DeviceHandle device_handle,
      const int32_t session_key) = 0;

  /**
   * \brief Called when secondary transport for a particular app is terminated.
   * \param session_key session ID representing the app
   */
  virtual void OnSecondaryTransportEndedCallback(const int32_t session_key) = 0;

  virtual void OnConnectionStatusUpdated() = 0;

  virtual void CreatePendingApplication(
      const transport_manager::ConnectionUID connection_id,
      const transport_manager::DeviceInfo& device_info,
      connection_handler::DeviceHandle device_id) = 0;

  virtual void SetPendingApplicationState(
      const transport_manager::ConnectionUID connection_id,
      const transport_manager::DeviceInfo& device_info) = 0;

 protected:
  /**
   * \brief Destructor
   */
  virtual ~ConnectionHandlerObserver() {}
};
}  // namespace connection_handler

#endif  // SRC_COMPONENTS_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_OBSERVER_H_
