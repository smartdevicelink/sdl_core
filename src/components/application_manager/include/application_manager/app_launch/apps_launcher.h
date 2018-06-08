#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APPS_LAUNCHER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APPS_LAUNCHER_H_
#include "application_manager/app_launch/app_launch_data.h"
#include "connection_handler/connection_handler.h"
#include "utils/timer.h"

namespace app_launch {
class AppLaunchCtrlImpl;
/**
 * @brief The AppLauncher struct will manage Launching app and relaunching app
 * in case application wasn't Registered.
 * When application registered instance of this structure should be deleted to
 * stop relaunch timer
 */
class AppsLauncher {
 public:
  AppsLauncher(connection_handler::ConnectionHandler& connection_handler,
               const uint16_t max_number_of_ios_device,
               const uint16_t app_launch_max_retry_attempt,
               const uint16_t app_launch_retry_wait_time);

  /**
   * @brief StartLaunching start launching process of applicaiton
   * @param app_data applicaiton to launch
   */
  void StartLaunching(ApplicationDataPtr app_data);

  /**
   * @brief OnLaunched callback for application registration
   * @param app_data registered application
   */
  void OnLaunched(ApplicationDataPtr app_data);

  /**
   * @brief OnRetryAttemptsExhausted callback for retry attempt exhausting
   * @param app_data applicaiton that was exhausted launch attempts
   */
  void OnRetryAttemptsExhausted(ApplicationDataPtr app_data);

  struct Launcher {
   public:
    Launcher(AppsLauncher& parent,
             connection_handler::ConnectionHandler& connection_handler,
             const uint16_t app_launch_max_retry_attempt,
             const uint16_t app_launch_retry_wait_time);

    /**
     * @brief PosponedLaunch launch application after cpecial timeout
     * Will manage launch retrying in case of application wasn't registered
     * @param app_data applicaiton to launch
     */
    void PosponedLaunch(const ApplicationDataPtr& app_data);

    /**
     * @brief Stops launching timers and remove information about launching
     * application
     */
    void Clear();

    /**
     * @brief LaunchNow send Launch to device request now
     */
    void LaunchNow();
    ApplicationDataPtr app_data_;
    size_t retry_index_;
    timer::Timer retry_timer_;
    const uint16_t app_launch_max_retry_attempt_;
    const uint16_t app_launch_retry_wait_time_;
    connection_handler::ConnectionHandler& connection_handler_;
    AppsLauncher& parent_;
  };
  typedef std::shared_ptr<Launcher> LauncherPtr;
  typedef std::vector<LauncherPtr> AppLaunchers;

 private:
  sync_primitives::Lock launchers_lock_;
  AppLaunchers free_launchers_;
  AppLaunchers works_launchers_;
  friend class Launcher;
  void StopLaunching(ApplicationDataPtr app_data);
  DISALLOW_COPY_AND_ASSIGN(AppsLauncher);
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APPS_LAUNCHER_H_
