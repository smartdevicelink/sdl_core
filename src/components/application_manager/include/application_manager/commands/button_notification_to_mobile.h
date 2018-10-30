#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_BUTTON_NOTIFICATION_TO_MOBILE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_BUTTON_NOTIFICATION_TO_MOBILE_H_

#include "application_manager/application.h"
#include "command_notification_impl.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {
namespace app_mngr = application_manager;

class ButtonNotificationToMobile
    : public app_mngr::commands::CommandNotificationImpl {
 protected:
  virtual void SendButtonNotification(app_mngr::ApplicationSharedPtr app) = 0;

 public:
  ButtonNotificationToMobile(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);

  virtual ~ButtonNotificationToMobile();

  virtual void Run();

  void HandleCustomButton(
                          app_mngr::ApplicationSharedPtr app);
  void HandleOKButton(
                      app_mngr::ApplicationSharedPtr app,
                      const uint32_t btn_id);
  void HandleMediaButton(const uint32_t btn_id);

  bool IsAppIDExist() const;
};
}  // namespace commands
}  // namespace application_manager

#endif
