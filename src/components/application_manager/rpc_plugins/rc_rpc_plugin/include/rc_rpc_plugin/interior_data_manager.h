#ifndef INTERIOR_DATA_MANAGER_H
#define INTERIOR_DATA_MANAGER_H

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

  virtual void OnDisablingRC() = 0;
};

}  // namespace rc_rpc_plugin

#endif  // INTERIOR_DATA_MANAGER_H
