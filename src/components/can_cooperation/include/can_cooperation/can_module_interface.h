/*
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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_INTERFACE_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_INTERFACE_H_

#include <queue>
#include <string>
#include "functional_module/generic_module.h"
#include "can_cooperation/can_connection.h"
#include "can_cooperation/request_controller.h"
#include "can_cooperation/event_engine/event_dispatcher.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/shared_ptr.h"

namespace can_cooperation {

class CANAppExtension;
typedef utils::SharedPtr<CANAppExtension> CANAppExtensionPtr;

struct MessageFromCAN : public Json::Value {
  explicit MessageFromCAN() {}
  explicit MessageFromCAN(const Json::Value& other) : Json::Value(other) {}
};

typedef Json::Value MessageFromMobile;

class CANModuleInterface
    : public functional_modules::GenericModule,
      public CANConnectionObserver,
      public threads::MessageLoopThread<std::queue<MessageFromCAN> >::Handler,
      public threads::MessageLoopThread<
          std::queue<MessageFromMobile> >::Handler {
 public:
  CANModuleInterface() : GenericModule(kCANModuleID) {}
  virtual ~CANModuleInterface() {}
  virtual functional_modules::PluginInfo GetPluginInfo() const = 0;
  virtual functional_modules::ProcessResult ProcessMessage(
      application_manager::MessagePtr msg) = 0;
  virtual functional_modules::ProcessResult ProcessHMIMessage(
      application_manager::MessagePtr msg) = 0;
  virtual void OnCANMessageReceived(const CANMessage& message) = 0;
  virtual void OnCANConnectionError(ConnectionState state,
                                    const std::string& info) = 0;
  virtual void Handle(const MessageFromMobile message) = 0;
  virtual void Handle(const MessageFromCAN message) = 0;

  /**
   * @brief Sends response to mobile application
   * @param msg response mesage
   */
  virtual void SendResponseToMobile(application_manager::MessagePtr msg) = 0;

  /**
   * @brief Sends timeout response to mobile application
   * @param msg response mesage
   */
  virtual void SendTimeoutResponseToMobile(
      application_manager::MessagePtr msg) = 0;

  /**
   * @brief Post message to can to queue
   * @param msg response mesage
   */
  virtual void SendMessageToCan(const MessageFromMobile& msg) = 0;

  /**
   * @brief Checks if radio scan started
   * @return true if radio scan started
   */
  virtual bool IsScanStarted() const = 0;

  /**
   * @brief Checks if radio scan started
   * @param is_scan_sarted true - scan started? false - scan stopped
   */
  virtual void SetScanStarted(bool is_scan_started) = 0;

  /**
   * @brief Remove extension created for specified application
   * @param app_id application id
   */
  virtual void RemoveAppExtension(uint32_t app_id) = 0;

  /**
   * @brief Check registering app can be handled by plugin
   * @param msg Registration message
   * @param app Application basis already create by Core
   */
  virtual bool IsAppForPlugin(
      application_manager::ApplicationSharedPtr app) = 0;

  /**
   * @brief Notify about change of HMILevel of plugin's app
   * @param app App with new HMILevel
   * @param old_level Old HMILevel of app
   */
  virtual void OnAppHMILevelChanged(
      application_manager::ApplicationSharedPtr app,
      mobile_apis::HMILevel::eType old_level) = 0;

  /**
   * @brief Checks if plugin hasn't put restrictions on app's HMI Level
   * @param app App with old HMILevel
   * @param new_level HMILevel which is about to be set to app
   */
  virtual bool CanAppChangeHMILevel(
      application_manager::ApplicationSharedPtr app,
      mobile_apis::HMILevel::eType new_level) = 0;

  /**
   * Handles removing (disconnecting) device
   * @param device removed
   */
  virtual void OnDeviceRemoved(
      const connection_handler::DeviceHandle& device) = 0;

  virtual void SendHmiStatusNotification(
      application_manager::ApplicationSharedPtr app) = 0;

  virtual void UnsubscribeAppForAllZones(uint32_t hmi_app_id,
                                         CANAppExtensionPtr app) = 0;

  virtual can_event_engine::EventDispatcher<application_manager::MessagePtr,
                                            std::string>&
  event_dispatcher() = 0;

 protected:
  /**
   * @brief Remove extension for all applications
   */
  virtual void RemoveAppExtensions() = 0;

  // TODO(VS): must be uid
  static const functional_modules::ModuleID kCANModuleID = 153;
};

}  // namespace can_cooperation

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_INTERFACE_H_
