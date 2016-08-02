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

#ifndef SRC_COMPONENTS_VR_COOPERATION_INCLUDE_VR_COOPERATION_VR_MODULE_H_
#define SRC_COMPONENTS_VR_COOPERATION_INCLUDE_VR_COOPERATION_VR_MODULE_H_

#include <queue>
#include <string>
#include "functional_module/generic_module.h"
#include "vr_cooperation/request_controller.h"
#include "utils/singleton.h"

namespace vr_cooperation {

class VRModule : public functional_modules::GenericModule,
  public utils::Singleton<VRModule> {
 public:
  functional_modules::PluginInfo GetPluginInfo() const;
  virtual functional_modules::ProcessResult ProcessMessage(
    application_manager::MessagePtr msg);
  virtual functional_modules::ProcessResult ProcessHMIMessage(
    application_manager::MessagePtr msg);

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
  bool IsAppForPlugin(
      application_manager::ApplicationSharedPtr app);

  /**
   * @brief Notify about change of HMILevel of plugin's app
   * @param app App with new HMILevel
   * @param old_level Old HMILevel of app
   */
  void OnAppHMILevelChanged(application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType old_level);

  /**
   * Handles removing (disconnecting) device
   * @param device removed
   */
  void OnDeviceRemoved(const connection_handler::DeviceHandle& device);

  void SendMessageToHMI();
  void ReceiveMessageFromHMI();
  void SendMessageToMobile();
  void ReceiveMessageFromMobile();

 private:
  VRModule();
  ~VRModule();

  void SubcribeToRPCMessage();
  static const functional_modules::ModuleID kVRModuleID = 154;
  functional_modules::PluginInfo plugin_info_;
  request_controller::RequestController request_controller_;

  FRIEND_BASE_SINGLETON_CLASS(VRModule);
};

}  // namespace vr_cooperation

#endif  // SRC_COMPONENTS_VR_COOPERATION_INCLUDE_VR_COOPERATION_VR_MODULE_H_
