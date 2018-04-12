/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_CONTROL_PLUGIN_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_CONTROL_PLUGIN_H_

#include <queue>
#include <string>

#include "remote_control/remote_plugin_interface.h"
#include "functional_module/generic_module.h"
#include "remote_control/request_controller.h"
#include "utils/threads/message_loop_thread.h"
#include "remote_control/event_engine/event_dispatcher.h"
#include "remote_control/resource_allocation_manager_impl.h"

namespace remote_control {
typedef rc_event_engine::EventDispatcher<application_manager::MessagePtr,
                                         std::string> RCEventDispatcher;

class RemoteControlPlugin : public RemotePluginInterface {
 public:
  RemoteControlPlugin();
  ~RemoteControlPlugin();

  functional_modules::PluginInfo GetPluginInfo() const;
  virtual functional_modules::ProcessResult ProcessMessage(
      application_manager::MessagePtr msg);
  virtual functional_modules::ProcessResult ProcessHMIMessage(
      application_manager::MessagePtr msg);

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
   * @brief Remove extension created for specified application
   * @param app_id application id
   */
  virtual void RemoveAppExtension(uint32_t app_id);

  /**
   * @brief Check registering app can be handled by plugin
   * @param msg Registration message
   * @param app Application basis already create by Core
   */
  bool IsAppForPlugin(application_manager::ApplicationSharedPtr app);

  /**
   * @brief Notify about change of HMILevel of plugin's app
   * @param app App with new HMILevel
   * @param old_level Old HMILevel of app
   */
  void OnAppHMILevelChanged(application_manager::ApplicationSharedPtr app,
                            mobile_apis::HMILevel::eType old_level);

  /**
   * @brief Sends HMI status notification to mobile
   * @param app application with changed HMI status
   **/
  void SendHmiStatusNotification(
      application_manager::ApplicationSharedPtr app) OVERRIDE;

  /**
   * @brief Getter for event_dispatcher
   * @return reference to RCEventDispatcher instance
   */
  RCEventDispatcher& event_dispatcher() OVERRIDE;

  /**
   * @brief Getter for resource_allocation_manager
   * @return reference to ResourceAllocationManager instance
   */
  ResourceAllocationManager& resource_allocation_manager() OVERRIDE;

  /**
   * @brief Overriden setter for service
   * @param service pointer to new service instance
   */
  void set_service(application_manager::ServicePtr service) OVERRIDE;

  /**
   * @brief OnApplicationEvent Processes application related events
   * @param event Event
   * @param application Pointer to application struct
   */
  void OnApplicationEvent(
      functional_modules::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  void OnPolicyEvent(functional_modules::PolicyEvent event) OVERRIDE;

 protected:
  /**
   * @brief Remove extension for all applications
   */
  virtual void RemoveAppExtensions() OVERRIDE;

 private:
  /**
   * @brief Trigger actions which should be done after plugin service instance
   * have been changed
   */
  void OnPluginServiceChanged();

  /**
   * @brief Subscribes on all RC related functions
   */
  void SubscribeOnFunctions();

  functional_modules::PluginInfo plugin_info_;
  bool is_scan_started_;
  request_controller::RequestController request_controller_;

  RCEventDispatcher event_dispatcher_;

  ResourceAllocationManagerImpl resource_allocation_manager_;
  DISALLOW_COPY_AND_ASSIGN(RemoteControlPlugin);
};

}  // namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_REMOTE_CONTROL_PLUGIN_H_
