#ifndef APPLICATION_MANAGER_SETTINGS
#define APPLICATION_MANAGER_SETTINGS

#include "application_manager/request_controller_settings.h"

#include <stdint.h>
#include <string>

namespace application_manager {
class ApplicationManagerSettings : public RequestControlerSettings {
 public:
  virtual const uint32_t video_data_stopped_timeout() const = 0;
  virtual const std::uint32_t audio_data_stopped_timeout() const = 0;
  virtual const std::pair<uint32_t, int32_t>& read_did_frequency() const = 0;
  virtual const std::pair<uint32_t, int32_t>& get_vehicle_data_frequency()
      const = 0;
  virtual uint32_t hash_string_size() const = 0;
  virtual const std::string& app_storage_folder() const = 0;
  virtual const uint32_t& app_dir_quota() const = 0;
  virtual uint32_t stop_streaming_timeout() const = 0;
  virtual uint32_t application_list_update_timeout() const = 0;
  virtual uint32_t heart_beat_timeout() const = 0;
  virtual const std::string& recording_file_name() const = 0;
  virtual const std::string& system_files_path() const = 0;
  virtual bool is_mixing_audio_supported() const = 0;
  virtual uint16_t tts_global_properties_timeout() const = 0;
  virtual uint16_t max_supported_protocol_version() const = 0;
  virtual const uint32_t& default_timeout() const = 0;
  virtual const uint32_t& max_cmd_id() const = 0;
  virtual bool launch_hmi() const = 0;
  virtual const uint32_t& delete_file_in_none() const = 0;
  virtual const std::vector<uint32_t>& supported_diag_modes() const = 0;
  virtual const uint32_t& list_files_in_none() const = 0;
  virtual const std::string& tts_delimiter() const = 0;
  virtual const uint32_t& put_file_in_none() const = 0;
  virtual const std::string& sdl_version() const = 0;
  virtual const std::vector<std::string>& time_out_promt() const = 0;
  virtual const std::string& hmi_capabilities_file_name() const = 0;
  virtual const std::string& video_server_type() const = 0;
  virtual const std::string& audio_server_type() const = 0;
  virtual const std::string& server_address() const = 0;
  virtual const uint16_t video_streaming_port() const = 0;
  virtual const uint16_t audio_streaming_port() const = 0;
  virtual const std::string& named_video_pipe_path() const = 0;
  virtual const std::string& named_audio_pipe_path() const = 0;
  virtual const std::string& video_stream_file() const = 0;
  virtual const std::string& audio_stream_file() const = 0;

  virtual bool use_db_for_resumption() const = 0;
  virtual const uint32_t& app_resumption_save_persistent_data_timeout()
      const = 0;
  virtual uint32_t resumption_delay_before_ign() const = 0;
  virtual uint32_t resumption_delay_after_ign() const = 0;
  virtual const uint32_t& app_resuming_timeout() const = 0;
  virtual uint16_t attempts_to_open_resumption_db() const = 0;
  virtual uint16_t open_attempt_timeout_ms_resumption_db() const = 0;
  virtual void config_file_name(const std::string& fileName) = 0;
  virtual const std::pair<uint32_t, int32_t>& start_stream_retry_amount()
      const = 0;
  virtual const std::string& app_icons_folder() const = 0;
  virtual const uint32_t& app_icons_folder_max_size() const = 0;
  virtual const uint32_t& app_icons_amount_to_remove() const = 0;
  virtual const uint32_t& list_files_response_size() const = 0;
};
}

#endif  // APPLICATION_MANAGER_SETTINGS
