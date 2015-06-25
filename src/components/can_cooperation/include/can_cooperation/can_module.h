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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_H_

#include <queue>
#include <string>
#include "functional_module/generic_module.h"
#include "can_cooperation/can_connection.h"
#include "can_cooperation/request_controller.h"
#include "utils/threads/message_loop_thread.h"


namespace can_cooperation {
struct MessageFromCAN : public Json::Value {
  explicit MessageFromCAN(const Json::Value& other): Json::Value(other) {}
};
typedef Json::Value MessageFromMobile;

class CANModule : public functional_modules::GenericModule,
  public utils::Singleton<CANModule>,
  public CANConnectionObserver,
  public threads::MessageLoopThread <std::queue<MessageFromCAN >>::Handler,
  public threads::MessageLoopThread <std::queue<MessageFromMobile >>::Handler {
 public:
  functional_modules::PluginInfo GetPluginInfo() const;
  virtual functional_modules::ProcessResult ProcessMessage(
    application_manager::MessagePtr msg);
  virtual functional_modules::ProcessResult ProcessHMIMessage(
    application_manager::MessagePtr msg);
  void OnCANMessageReceived(const CANMessage& message);
  void OnCANConnectionError(ConnectionState state, const std::string& info);
  void Handle(const MessageFromMobile message);
  void Handle(const MessageFromCAN message);

  /**
   * @brief Sends response to mobile application
   * @param msg response mesage
   */
  void SendResponseToMobile(application_manager::MessagePtr msg);

  /**
   * @brief Sends timeout response to mobile application
   * @param msg response mesage
   */
  void SendTimeoutResponseToMobile(application_manager::MessagePtr msg);

  /**
   * @brief Post message to can to queue
   * @param msg response mesage
   */
  void SendMessageToCan(const MessageFromMobile& msg);

  /**
   * @brief Checks if radio scan started
   * @return true if radio scan started
   */
  bool IsScanStarted() const;

  /**
   * @brief Checks if radio scan started
   * @param is_scan_sarted true - scan started? false - scan stopped
   */
  void SetScanStarted(bool is_scan_started);

  /**
   * @brief Remove extension created for specified application
   * @param app_id application id
   */
  virtual void RemoveAppExtension(uint32_t app_id);

  /*
   * @brief Check registering app can be handled by plugin
   * @param msg Registration message
   * @param app Application basis already create by Core
   */
  bool IsAppForPlugin(
      application_manager::ApplicationSharedPtr app);

 protected:
  /**
   * @brief Remove extension for all applications
   */
  virtual void RemoveAppExtensions();
 private:
  CANModule();
  ~CANModule();

  void SubscribeOnFunctions();
  void NotifyMobiles(application_manager::MessagePtr msg);

  functional_modules::ProcessResult HandleMessage(
    application_manager::MessagePtr msg);
  // TODO(VS): must be uid
  static const functional_modules::ModuleID kCANModuleID = 153;
  CANConnectionSPtr can_connection_;
  functional_modules::PluginInfo plugin_info_;
  threads::MessageLoopThread<std::queue<MessageFromCAN>> from_can_;
  threads::MessageLoopThread<std::queue<MessageFromMobile>> from_mobile_;
  bool is_scan_started_;
  request_controller::RequestController request_controller_;

  friend class CanModuleTest;
  FRIEND_BASE_SINGLETON_CLASS(CANModule);
  DISALLOW_COPY_AND_ASSIGN(CANModule);
};

EXPORT_FUNCTION(CANModule)

}  // namespace can_cooperation

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_H_
