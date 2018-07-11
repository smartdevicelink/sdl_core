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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_SETTINGS_H_

#include <string>
#include "gmock/gmock.h"
#include "application_manager/application_manager_settings.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockApplicationManagerSettings
    : public application_manager::ApplicationManagerSettings {
 public:
  MOCK_CONST_METHOD0(video_data_stopped_timeout, const uint32_t());
  MOCK_CONST_METHOD0(audio_data_stopped_timeout, const std::uint32_t());
  // The following line won't really compile, as the return
  // type has multiple template arguments.  To fix it, use a
  // typedef for the return type.
  MOCK_CONST_METHOD0(read_did_frequency, const std::pair<uint32_t, int32_t>&());
  // The following line won't really compile, as the return
  // type has multiple template arguments.  To fix it, use a
  // typedef for the return type.
  MOCK_CONST_METHOD0(get_vehicle_data_frequency,
                     const std::pair<uint32_t, int32_t>&());
  MOCK_CONST_METHOD0(hash_string_size, uint32_t());
  MOCK_CONST_METHOD0(app_storage_folder, const std::string&());
  MOCK_CONST_METHOD0(app_info_storage, const std::string&());
  MOCK_CONST_METHOD0(app_dir_quota, const uint32_t&());
  MOCK_CONST_METHOD0(stop_streaming_timeout, uint32_t());
  MOCK_CONST_METHOD0(application_list_update_timeout, uint32_t());
  MOCK_CONST_METHOD0(heart_beat_timeout, uint32_t());
  MOCK_CONST_METHOD0(recording_file_name, const std::string&());
  MOCK_CONST_METHOD0(system_files_path, const std::string&());
  MOCK_CONST_METHOD0(is_mixing_audio_supported, bool());
  MOCK_CONST_METHOD0(tts_global_properties_timeout, uint16_t());
  MOCK_CONST_METHOD0(max_supported_protocol_version, uint16_t());
  MOCK_CONST_METHOD0(default_timeout, const uint32_t&());
  MOCK_CONST_METHOD0(max_cmd_id, const uint32_t&());
  MOCK_CONST_METHOD0(launch_hmi, bool());
  MOCK_CONST_METHOD0(delete_file_in_none, const uint32_t&());
  MOCK_CONST_METHOD0(supported_diag_modes, const std::vector<uint32_t>&());
  MOCK_CONST_METHOD0(list_files_in_none, const uint32_t&());
  MOCK_CONST_METHOD0(tts_delimiter, const std::string&());
  MOCK_CONST_METHOD0(put_file_in_none, const uint32_t&());
  MOCK_CONST_METHOD0(sdl_version, const std::string&());
  MOCK_CONST_METHOD0(vr_help_title, const std::string&());
  MOCK_CONST_METHOD0(help_prompt, const std::vector<std::string>&());
  MOCK_CONST_METHOD0(time_out_promt, const std::vector<std::string>&());
  MOCK_CONST_METHOD0(hmi_capabilities_file_name, const std::string&());
  MOCK_CONST_METHOD0(video_server_type, const std::string&());
  MOCK_CONST_METHOD0(audio_server_type, const std::string&());
  MOCK_CONST_METHOD0(server_address, const std::string&());
  MOCK_CONST_METHOD0(video_streaming_port, const uint16_t());
  MOCK_CONST_METHOD0(audio_streaming_port, const uint16_t());
  MOCK_CONST_METHOD0(named_video_pipe_path, const std::string&());
  MOCK_CONST_METHOD0(named_audio_pipe_path, const std::string&());
  MOCK_CONST_METHOD0(video_stream_file, const std::string&());
  MOCK_CONST_METHOD0(audio_stream_file, const std::string&());
  MOCK_CONST_METHOD0(use_db_for_resumption, bool());
  MOCK_CONST_METHOD0(app_resumption_save_persistent_data_timeout,
                     const uint32_t&());
  MOCK_CONST_METHOD0(resumption_delay_before_ign, uint32_t());
  MOCK_CONST_METHOD0(resumption_delay_after_ign, const uint32_t());
  MOCK_CONST_METHOD0(app_resuming_timeout, const uint32_t&());
  MOCK_CONST_METHOD0(attempts_to_open_resumption_db, uint16_t());
  MOCK_CONST_METHOD0(open_attempt_timeout_ms_resumption_db, uint16_t());
  MOCK_CONST_METHOD0(transport_required_for_resumption_map,
                     std::map<std::string, std::vector<std::string> >&());
  MOCK_CONST_METHOD0(navigation_lowbandwidth_resumption_level,
                     const std::string&());
  MOCK_CONST_METHOD0(projection_lowbandwidth_resumption_level,
                     const std::string&());
  MOCK_CONST_METHOD0(media_lowbandwidth_resumption_level, const std::string&());
  MOCK_METHOD1(set_config_file_name, void(const std::string& fileName));
  // The following line won't really compile, as the return
  // type has multiple template arguments.  To fix it, use a
  // typedef for the return type.
  MOCK_CONST_METHOD0(start_stream_retry_amount,
                     const std::pair<uint32_t, int32_t>&());
  MOCK_CONST_METHOD0(app_icons_folder, const std::string&());
  MOCK_CONST_METHOD0(plugins_folder, const std::string&());
  MOCK_CONST_METHOD0(app_icons_folder_max_size, const uint32_t&());
  MOCK_CONST_METHOD0(app_icons_amount_to_remove, const uint32_t&());
  MOCK_CONST_METHOD0(list_files_response_size, const uint32_t&());

  // request controller settings section
  MOCK_CONST_METHOD0(thread_pool_size, uint32_t());
  MOCK_CONST_METHOD0(app_hmi_level_none_time_scale, const uint32_t&());
  MOCK_CONST_METHOD0(app_hmi_level_none_time_scale_max_requests,
                     const uint32_t&());
  MOCK_CONST_METHOD0(app_time_scale, const uint32_t&());
  MOCK_CONST_METHOD0(app_time_scale_max_requests, const uint32_t&());
  MOCK_CONST_METHOD0(pending_requests_amount, const uint32_t&());

  // app_launch::AppLaunchSettings
  MOCK_CONST_METHOD0(app_launch_wait_time, const uint16_t());
  MOCK_CONST_METHOD0(app_launch_max_retry_attempt, const uint16_t());
  MOCK_CONST_METHOD0(app_launch_retry_wait_time, const uint16_t());
  MOCK_CONST_METHOD0(remove_bundle_id_attempts, const uint16_t());
  MOCK_CONST_METHOD0(max_number_of_ios_device, const uint16_t());
  MOCK_CONST_METHOD0(wait_time_between_apps, const uint16_t());
  MOCK_CONST_METHOD0(enable_app_launch_ios, const bool());
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_SETTINGS_H_
