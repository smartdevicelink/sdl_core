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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_IMPL_H_
#include "rc_rpc_plugin/interior_data_manager.h"
#include "utils/date_time.h"

namespace application_manager {
class ApplicationManager;
namespace rpc_service {
class RPCService;
}
}
namespace rc_rpc_plugin {

class InteriorDataCache;
class RCRPCPlugin;

class InteriorDataManagerImpl : public InteriorDataManager {
 public:
  InteriorDataManagerImpl(
      RCRPCPlugin& rc_plugin,
      InteriorDataCache& cache,
      application_manager::ApplicationManager& app_mngr,
      application_manager::rpc_service::RPCService& rpc_service);

  void OnPolicyEvent(app_mngr::plugin_manager::PolicyEvent event) OVERRIDE;

  void OnApplicationEvent(plugins::ApplicationEvent event,
                          app_mngr::ApplicationSharedPtr application) OVERRIDE;

  void OnDisablingRC() OVERRIDE;

  void StoreRequestToHMITime(const std::string& module_type) OVERRIDE;

  bool CheckRequestsToHMIFrequency(const std::string& module_type) OVERRIDE;

 private:
  /**
   * @brief UpdateHMISubscriptionsOnPolicyUpdated process policy update event.
   * If some modules was disabeled by policies and there are no applications
   * that subscribed to them - send  RC.GetInteriorVehicleData(subscribe=false)
   * and clear cache
   */
  void UpdateHMISubscriptionsOnPolicyUpdated();

  /**
   * @brief UpdateHMISubscriptionsOnAppUnregistered process AppUnregistered
   * event and  unsubscribed from not actual module types
   * @param app application that was unregistered
   */
  void UpdateHMISubscriptionsOnAppUnregistered(
      application_manager::Application& app);

  /**
   * @brief UnsubscribeFromInteriorVehicleData remove module_type from cache and
   * send RC.GetInteriorVehicleData(subscribe=false) to HMI
   * @param module_type module type that need to be unsubscribed
   */
  void UnsubscribeFromInteriorVehicleData(const std::string& module_type);

  void ClearOldRequestsToHMIHistory();
  /**
   * @brief AppsModules mapping from applications to list of modules
   */
  typedef std::map<application_manager::ApplicationSharedPtr,
                   std::vector<std::string> > AppsModules;

  /**
   * @brief AppsSubscribedModules get mapping of application to list of
   * subscribed modules
   * @return map of applications to list of subscribed modules
   */
  AppsModules AppsSubscribedModules();

  /**
   * @brief RequestsToHMIHistory mapping from module type to vector of time
   * stamps
   */
  typedef std::map<std::string, std::deque<date_time::TimeDuration> >
      RequestsToHMIHistory;
  RequestsToHMIHistory requests_to_hmi_history_;
  mutable sync_primitives::Lock requests_to_hmi_history_lock_;

  RCRPCPlugin& rc_plugin_;
  InteriorDataCache& cache_;
  application_manager::ApplicationManager& app_mngr_;
  application_manager::rpc_service::RPCService& rpc_service_;
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_IMPL_H_
