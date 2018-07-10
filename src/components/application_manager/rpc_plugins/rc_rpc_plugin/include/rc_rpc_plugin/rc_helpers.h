#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_HELPERS_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_HELPERS_H

#include <map>
#include <string>
#include "application_manager/application.h"
#include "rc_rpc_plugin/rc_app_extension.h"

namespace rc_rpc_plugin {

/**
 * @brief The RCHelpers class this contains frequently used static data
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
  static const std::map<std::string, std::string>& GetModuleTypeToDataMapping();

  /**
   * @brief GetRCExtension extract RC extension from application
   * @param app application to extract extension
   * @return rc extension of app is rc applicaiton, otherwise return emty shared
   * pointer.
   */
  static RCAppExtensionPtr GetRCExtension(
      application_manager::Application& app);
};

}  // rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_HELPERS_H
