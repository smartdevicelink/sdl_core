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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_PLUGIN_INTERFACE_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_PLUGIN_INTERFACE_H_

#include <queue>
#include <string>
#include "functional_module/generic_module.h"
#include "remote_control/request_controller.h"
#include "remote_control/event_engine/event_dispatcher.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/shared_ptr.h"

namespace remote_control {

class RCAppExtension;
class ResourceAllocationManager;

typedef utils::SharedPtr<RCAppExtension> RCAppExtensionPtr;

class RemotePluginInterface : public functional_modules::GenericModule {
 public:
  RemotePluginInterface() : GenericModule(kCANModuleID) {}
  virtual ~RemotePluginInterface() {}
  virtual functional_modules::PluginInfo GetPluginInfo() const = 0;
  virtual functional_modules::ProcessResult ProcessMessage(
      application_manager::MessagePtr msg) = 0;
  virtual functional_modules::ProcessResult ProcessHMIMessage(
      application_manager::MessagePtr msg) = 0;

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

  typedef rc_event_engine::EventDispatcher<application_manager::MessagePtr,
                                           std::string> RCPluginEventDispatcher;

  virtual RCPluginEventDispatcher& event_dispatcher() = 0;

  virtual ResourceAllocationManager& resource_allocation_manager() = 0;

 protected:
  /**
   * @brief Remove extension for all applications
   */
  virtual void RemoveAppExtensions() = 0;

  static const functional_modules::ModuleID kCANModuleID = 153;
};

}  // namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_PLUGIN_INTERFACE_H_
