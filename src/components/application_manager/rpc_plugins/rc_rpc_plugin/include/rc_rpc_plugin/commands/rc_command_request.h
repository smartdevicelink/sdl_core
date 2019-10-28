/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_RC_COMMAND_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_RC_COMMAND_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "rc_rpc_plugin/commands/rc_command_params.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_capabilities_manager.h"
#include "rc_rpc_plugin/rc_consent_manager.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

enum TypeAccess { kDisallowed, kAllowed };

namespace commands {

class RCCommandRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * @brief RCCommandRequest class constructor
   * @param message MessageSharedPtr
   * @param application_manager ApplicationManager
   * @param rpc_service RPCService
   * @param hmi_capabilities HMICapabilities
   * @param policy_handle PolicyHandlerInterface
   * @param resource_allocation_manager ResourceAllocationManager
   **/
  RCCommandRequest(
      const application_manager::commands::MessageSharedPtr& message,
      const RCCommandParams& params);

  virtual ~RCCommandRequest();

  void onTimeOut() OVERRIDE;

  void Run() OVERRIDE;

  virtual void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

 protected:
  bool is_subscribed;
  bool auto_allowed_;

  ResourceAllocationManager& resource_allocation_manager_;
  InteriorDataCache& interior_data_cache_;
  InteriorDataManager& interior_data_manager_;
  RCCapabilitiesManager& rc_capabilities_manager_;
  RCConsentManager& rc_consent_manager_;
  /**
   * @brief AcquireResource try to allocate resource for application
   * In case if allocation of resource is not required, return ALLOWED by
   * default.
   * This method should be overrided in RPCs that requires resource allocation
   * @return result of resource allocation, in case if allocation os not
   * required, return ALLOWED
   */
  virtual AcquireResult::eType AcquireResource(
      const app_mngr::commands::MessageSharedPtr& message) {
    return AcquireResult::ALLOWED;
  }

  /**
   * @brief IsResourceFree check resource state
   * This is default implementation which has to be redefined for RPCs which
   * need to manage the resources
   * @param module_type Resource name
   * @param module_id Resource id
   * @return True if free, otherwise - false
   */
  virtual bool IsResourceFree(const std::string& module_type,
                              const std::string& module_id) const {
    UNUSED(module_type);
    UNUSED(module_id);
    return true;
  }
  /**
   * @brief SetResourceState changes state of resource
   * This is default implementation which has to be redefined for RPCs which
   * need to manage the resources
   * @param module_type Resource name
   * @param State to set for resource
   */
  virtual void SetResourceState(const std::string& module_type,
                                const ResourceState::eType) {}

  /**
   * Checks if module for application is present in policy table
   * @param app_id id of application
   * @param module type Resource name
   * @return kAllowed if module is present, otherwise - kDisallowed
   */
  TypeAccess CheckModule(const std::string& module_type,
                         application_manager::ApplicationSharedPtr app);

  bool auto_allowed() const {
    return auto_allowed_;
  }

  void set_auto_allowed(const bool value) {
    auto_allowed_ = value;
  }

  /**
   * @brief executes specific logic of children classes
   */
  void virtual Execute() = 0;

  void set_disallowed_info(const std::string& info) {
    disallowed_info_ = info;
  }

  virtual std::string ModuleType() const = 0;

  /**
   * @brief Extracts ModuleId from command message. Each inherited class should
   * implement its own functionality
   *
   * @return ModuleId as string.
   */
  virtual std::string ModuleId() const = 0;

  /**
   * @brief IsModuleIdProvided checks if moduleId parameter
   * is provided in the hmi response
   * @param hmi_response response from hmi
   * @return true if provided, otherwise - false
   */

  bool IsModuleIdProvided(const smart_objects::SmartObject& hmi_response) const;

 private:
  /**
   * @brief CheckDriverConsent checks driver consent defined in policy table
   * @return True if no consent is required, otherwise - false
   */
  bool CheckDriverConsent();

  /**
   * @brief AcquireResources checks whether resource status is busy or not and
   * then tries to acquire this resource. In case driver consent is required -
   * sends consent request to HMI.
   * @return True in case of resource is free and successfully acquired,
   * otherwise false
   */
  bool AcquireResources();
  void SendDisallowed(TypeAccess access);

  /**
   * @brief SendGetUserConsent sends consent request to HMI
   * @param module_type Resource name
   * @param module_ids Array of module IDs of the module type that needed user
   * consent for acquiring their resources
   */
  void SendGetUserConsent(const std::string& module_type,
                          const smart_objects::SmartObject& module_ids);

  void ProcessAccessResponse(const app_mngr::event_engine::Event& event);

  /**
   * @brief Precesses consents result which has been received from HMI
   * If module resource consented, resource state will be switched to state BUSY
   * and called method Execute. Otherwise will be sent response to Mobile with
   * result code REJECTED.
   * @param is_allowed consent result
   * @param module_type Module type
   * @param module_id Module ID
   * @param app_id Application, which has asked for module resource consent.
   */
  void ProcessConsentResult(const bool is_allowed,
                            const std::string& module_type,
                            const std::string& module_id,
                            const uint32_t app_id);
  /**
   * @brief Processes ASK_DRIVE Mode. Tries to retrieve module consents from
   * LastState. If consent is absent in LastState, will send
   * GetInteriorVehicleDataConsent to HMI. Otherwise will start to process
   * consent result.
   * @param module_type Module type
   * @param module_id Module ID
   */
  void ProcessAskDriverMode(const std::string& module_type,
                            const std::string& module_id);
  bool IsInterfaceAvailable(
      const app_mngr::HmiInterfaces::InterfaceID interface) const;

  std::string disallowed_info_;
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_RC_COMMAND_REQUEST_H_
