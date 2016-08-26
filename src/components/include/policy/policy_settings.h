#ifndef POLICY_HANDLER_SETTINGS_H
#define POLICY_HANDLER_SETTINGS_H
#include <string>
#include <stdint.h>

namespace policy {
class PolicySettings {
 public:
  /**
   * @brief Should Policy be turned off? (Library not loaded)
   * @return Flag
   */
  virtual bool enable_policy() const = 0;
  /*
   * @brief Path to preloaded policy file
   */
  virtual const std::string& preloaded_pt_file() const = 0;

  /**
    * @brief Returns application storage path
    */
  virtual const std::string& app_storage_folder() const = 0;

  virtual uint16_t attempts_to_open_policy_db() const = 0;

  virtual uint16_t open_attempt_timeout_ms() const = 0;

  /**
   * @brief Path to policies snapshot file
   * @return file path
   */
  virtual const std::string& policies_snapshot_file_name() const = 0;

  /**
    * @brief Returns system files folder path
    */
  virtual const std::string& system_files_path() const = 0;

  virtual ~PolicySettings() {}
};
}  // namespace policy
#endif  // POLICY_HANDLER_SETTINGS_H
