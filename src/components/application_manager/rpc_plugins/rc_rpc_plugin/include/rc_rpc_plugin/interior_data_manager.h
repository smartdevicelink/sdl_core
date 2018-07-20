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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_H_

#include "application_manager/application.h"
#include "application_manager/plugin_manager/rpc_plugin.h"

namespace rc_rpc_plugin {

namespace app_mngr = application_manager;
namespace plugins = application_manager::plugin_manager;

class InteriorDataManager {
 public:
  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  virtual void OnPolicyEvent(app_mngr::plugin_manager::PolicyEvent event) = 0;

  /**
    * @brief OnApplicationEvent Notifies modules on certain application events
    * @param event Event
    * @param application Pointer to application struct
    */
  virtual void OnApplicationEvent(
      plugins::ApplicationEvent event,
      app_mngr::ApplicationSharedPtr application) = 0;

  /**
   * @brief OnDisablingRC process disable RC event. Unsubscribe from all modules
   * and clear cache
   */
  virtual void OnDisablingRC() = 0;

  /**
   * @brief StoreRequestToHMITime save information and time stamp of
   * current interior data subscriptions
   */
  virtual void StoreRequestToHMITime(const std::string& module_type) = 0;

  /**
 * @brief CheckRequestsToHMIFrequency check that rate limits are not allowed of
 * bounce during current time frame.
 * calculate amount of requests per module type in time frame and checks if it
 * bigger then allowed by ini file
 * @param module_type moduletype to calculate frequency on
 * @return true if amount of requests was not exceeded, otherwise return false.
 */
  virtual bool CheckRequestsToHMIFrequency(const std::string& module_type) = 0;
};

}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_H_
