/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_SETTINGS_H_

#include "application_manager/request_controller_settings.h"
#include "application_manager/app_launch_settings.h"

#include <stdint.h>
#include <string>

namespace application_manager {
class ApplicationManagerSettings : public RequestControlerSettings,
                                   public app_launch::AppLaunchSettings {
 public:
  virtual const uint32_t video_data_stopped_timeout() const = 0;
  virtual const uint32_t audio_data_stopped_timeout() const = 0;
  virtual const std::pair<uint32_t, int32_t>& read_did_frequency() const = 0;
  virtual const std::pair<uint32_t, int32_t>& get_vehicle_data_frequency()
      const = 0;
  virtual uint32_t hash_string_size() const = 0;
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
  virtual const std::vector<std::string>& help_prompt() const = 0;
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
  virtual const uint32_t& app_resuming_timeout() const = 0;
  virtual uint16_t attempts_to_open_resumption_db() const = 0;
  virtual uint16_t open_attempt_timeout_ms_resumption_db() const = 0;
  virtual void set_config_file_name(const std::string& fileName) = 0;
  virtual const std::pair<uint32_t, int32_t>& start_stream_retry_amount()
      const = 0;
  virtual const std::string& app_icons_folder() const = 0;
  virtual const uint32_t& app_icons_folder_max_size() const = 0;
  virtual const uint32_t& app_icons_amount_to_remove() const = 0;
  virtual const uint32_t& list_files_response_size() const = 0;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_SETTINGS_H_
