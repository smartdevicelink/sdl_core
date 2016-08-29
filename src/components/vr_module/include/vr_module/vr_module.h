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

#ifndef SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_VR_MODULE_H_
#define SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_VR_MODULE_H_

#include "functional_module/generic_module.h"
#include "json/value.h"
#include "utils/macro.h"
#include "vr_module/commands/factory_interface.h"
#include "vr_module/request_controller.h"
#include "vr_module/service_module.h"
#include "vr_module/vr_proxy.h"
#include "vr_module/vr_proxy_listener.h"

namespace vr_module {

typedef Json::Value MessageFromMobile;

class VRModule : public functional_modules::GenericModule,
    public VRProxyListener, public ServiceModule {
 public:
  VRModule();
  ~VRModule();
  virtual functional_modules::PluginInfo GetPluginInfo() const;
  virtual functional_modules::ProcessResult ProcessHMIMessage(
      application_manager::MessagePtr msg);

  /**
   * @brief Process messages from mobile(called from SDL part through interface)
   * @param msg request mesage
   * @return processing result
   */
  virtual functional_modules::ProcessResult ProcessMessage(
      application_manager::MessagePtr msg);
  virtual void RemoveAppExtension(uint32_t app_id);
  virtual void OnDeviceRemoved(const connection_handler::DeviceHandle& device);
  virtual void RemoveAppExtensions();
  virtual bool IsAppForPlugin(application_manager::ApplicationSharedPtr app);
  virtual void OnAppHMILevelChanged(
      application_manager::ApplicationSharedPtr app,
      mobile_apis::HMILevel::eType old_level);

  virtual int32_t GetNextCorrelationID() {
    static int32_t next_correlation_id = 1;
    return next_correlation_id++;
  }

  virtual void OnReady();

  /**
   * Handles received message from HMI (Applink)
   * @param message is GPB message according with protocol
   */
  virtual void OnReceived(const vr_hmi_api::ServiceMessage& message);

  virtual void ActivateService(int32_t app_id);
  virtual void DeactivateService();
  virtual void SetDefaultService(int32_t app_id);
  virtual void ResetDefaultService();
  virtual void RegisterRequest(int32_t correlation_id,
                               commands::TimedCommand* command);
  virtual void UnregisterRequest(int32_t correlation_id);
  virtual bool SendToHmi(const vr_hmi_api::ServiceMessage& message);
  virtual bool SendToMobile(const vr_mobile_api::ServiceMessage& message);

  /**
   * Registers service
   * @param app_id unique application ID
   */
  void RegisterService(int32_t app_id);

  virtual bool IsSupported() const {
    return supported_;
  }

  virtual void EnableSupport() {
    supported_ = true;
  }

  virtual void DisableSupport() {
    supported_ = false;
  }

 private:
  static const functional_modules::ModuleID kModuleID = 405;

  /**
   * @brief Subscribes plugin to mobie rpc messages
   */
  void SubscribeToRpcMessages();
  void CheckSupport();

  /**
   * Handles received message from Mobile application
   * @param message is GPB message according with protocol
   */
  void OnReceived(const vr_mobile_api::ServiceMessage& message);
  void EmitEvent(const vr_hmi_api::ServiceMessage& message);
  void EmitEvent(const vr_mobile_api::ServiceMessage& message);
  void RunCommand(commands::CommandPtr command);

  functional_modules::PluginInfo plugin_info_;

  VRProxy proxy_;
  const commands::FactoryInterface& factory_;
  request_controller::RequestController request_controller_;
  bool supported_;
  int32_t active_service_;
  int32_t default_service_;

  DISALLOW_COPY_AND_ASSIGN(VRModule);
};

EXPORT_FUNCTION(VRModule)

}  // namespace vr_module

#endif  // SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_VR_MODULE_H_
