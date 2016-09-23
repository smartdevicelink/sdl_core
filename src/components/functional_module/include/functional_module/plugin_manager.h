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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANAGER_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANAGER_H_

#include <map>
#include <string>
#include "functional_module/generic_module.h"
#include "application_manager/service.h"
#include "application_manager/message.h"
#include "protocol_handler/remote_services_message_observer.h"
#include "connection_handler/remote_services_observer.h"
#include "utils/singleton.h"

namespace protocol_handler {
  class ProtocolHandler;
}  // namespace protocol_handler

namespace functional_modules {

class PluginManager : public utils::Singleton<PluginManager>,
  public ModuleObserver,
  public protocol_handler::RemoteServicesMessageObserver,
  public connection_handler::RemoteServicesObserver{
 public:
  typedef std::map<ModuleID, ModulePtr> Modules;
  int LoadPlugins(const std::string& plugin_path);
  void UnloadPlugins();
  void ProcessMessage(application_manager::MessagePtr msg);
  ProcessResult ProcessHMIMessage(application_manager::MessagePtr msg);
  void SetServiceHandler(application_manager::ServicePtr service) {
    service_ = service;
  }
  bool IsMessageForPlugin(application_manager::MessagePtr msg);
  bool IsHMIMessageForPlugin(application_manager::MessagePtr msg);
  void OnServiceStateChanged(ServiceState state);
  void OnHMIResponse(application_manager::MessagePtr msg);
  void OnError(ModuleObserver::Errors error, ModuleID module_id);

  /**
   * @brief Remove extension created for specified application
   * @param app_id application id
   */
  void RemoveAppExtension(uint32_t app_id);

  /**
   * @brief Check if app cooperates with one or more plugins, init it
   * inside plugin accordingly if needed.
   * @param app Application in question.
   */
  bool IsAppForPlugins(application_manager::ApplicationSharedPtr app);

  /**
   * Check if app cooperates with plugin
   * @param app application
   * @param module_id unique identifier of plugin
   * @return true if application cooperates with this plugin
   */
  bool IsAppForPlugin(application_manager::ApplicationSharedPtr app,
                      ModuleID module_id) const;

  /**
   * @brief Notify plugins about change of HMILevel of app
   * if app is related to plugin
   * @param app App with new HMILevel
   * @param old_level Old HMILevel of app
   */
  void OnAppHMILevelChanged(application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType old_level);

  /**
   * @brief Checks if plugin hasn't put restrictions on app's HMI Level
   * @param app App with old HMILevel
   * @param new_level HMILevel which is about to be set to app
   * @return false if any of the plugins returns false.
   */
  bool CanAppChangeHMILevel(application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType new_level);

  /**
   * Handles removing (disconnecting) device
   * @param device removed
   */
  void OnDeviceRemoved(const connection_handler::DeviceHandle& device);

  /**
   * @brief RemoteServiceMessageObserver interface realization
   */
  virtual bool OnMessageReceived(
      const protocol_handler::RawMessagePtr message) const;

  /**
   * @brief RemoteServicesObserver interface realization
   */
  virtual bool OnServiceStartedCallback(
      const uint32_t& connection_key,
      const protocol_handler::ServiceType& type,
      const std::string& device_mac_address) const;

  /**
   * @brief RemoteServicesObserver interface realization
   */
  virtual void OnServiceEndedCallback(
      const uint32_t& connection_key,
      const protocol_handler::ServiceType& type) const ;

  /**
   * @brief Sends message to app remote service
   * @param message that will be sent through ProtcolHandler
   */
  virtual void SendMessageToRemoteMobileService(
       const protocol_handler::RawMessagePtr message) const ;

  /**
   * @brief Sets pointer to ProtocolHandler
   * @param protocol_handler - pointer that will be used for sending message to
   *                           mobile through remote service
   */
  void set_protocol_handler(
      protocol_handler::ProtocolHandler* protocol_handler);

 private:
  PluginManager();
  ~PluginManager();

  ModulePtr FindPluginForSpecifiedService(
      const protocol_handler::ServiceType& type) const;

  Modules plugins_;
  std::map<ModuleID, void*> dlls_;
  std::map<MobileFunctionID, ModulePtr> mobile_subscribers_;
  std::map<HMIFunctionID, ModulePtr> hmi_subscribers_;
  std::map<ServiceType, ModulePtr> remote_services_subscribers_;
  application_manager::ServicePtr service_;
  protocol_handler::ProtocolHandler* protocol_handler_;

  friend class PluginManagerTest;
  FRIEND_BASE_SINGLETON_CLASS(PluginManager);
  DISALLOW_COPY_AND_ASSIGN(PluginManager);
};

}  //  namespace functional_modules

#endif  // SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANAGER_H_
