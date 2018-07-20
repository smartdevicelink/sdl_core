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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_HELPERS_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_HELPERS_H_

#include <map>
#include <string>
#include "application_manager/application.h"
#include "rc_rpc_plugin/rc_app_extension.h"

namespace rc_rpc_plugin {
class RCRPCPlugin;

/**
 * @brief The RCHelpers class contains frequently used static data
 * structures related strictly to RC
 * Converters, mapping, factory functions
 */
class RCHelpers {
 public:
  /**
  * @brief GetModuleTypeToDataMapping get mapping of module type enum naming to
  * actual module data filed name
  * @return module mapping from enum naming to filed name
  */
  static const std::function<std::string(const std::string& module_type)>
  GetModuleTypeToDataMapping();

  /**
   * @brief GetRCExtension extract RC extension from application
   * @param app application to extract extension
   * @return rc extension of app is rc applicaiton, otherwise return emty shared
   * pointer.
   */
  static RCAppExtensionPtr GetRCExtension(
      application_manager::Application& app);

  static smart_objects::SmartObjectSPtr CreateUnsubscribeRequestToHMI(
      const std::string& module_type, const uint32_t correlation_id);

  static std::vector<application_manager::ApplicationSharedPtr>
  AppsSubscribedToModuleType(application_manager::ApplicationManager& app_mngr,
                             const std::string& module_type);

  typedef std::map<application_manager::ApplicationSharedPtr,
                   std::vector<std::string> > AppsModules;
  static AppsModules GetApplicaitonsAllowedModules(
      application_manager::ApplicationManager& app_mngr);
};

}  // rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_HELPERS_H_
