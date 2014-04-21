/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "config_profile/profile.h"

#include <string.h>
#include <stdlib.h>
#include <sstream>

#include "config_profile/ini_file.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/file_system.h"

namespace {
const char* kMainSection = "MAIN";
const char* kPolicySection = "Policy";

const char* kDefaultPoliciesSnapshotFileName = "sdl_snapshot.json";
// Heartbeat is disabled by default
const uint32_t kDefaultHeartBeatTimeout = 0;
}  // namespace

namespace profile {

CREATE_LOGGERPTR_GLOBAL(logger_, "Profile")

Profile::Profile()
    : launch_hmi_(true),
      app_config_folder_(""),
      app_storage_folder_(""),
      config_file_name_("smartDeviceLink.ini"),
      policies_file_name_("policy_table.json"),
      hmi_capabilities_file_name_("hmi_capabilities.json"),
      server_address_("127.0.0.1"),
      server_port_(8087),
      video_streaming_port_(5050),
      audio_streaming_port_(5080),
      help_prompt_(),
      time_out_promt_(),
      min_tread_stack_size_(threads::Thread::kMinStackSize),
      is_mixing_audio_supported_(false),
      is_redecoding_enabled_(false),
      max_cmd_id_(2000000000),
      default_timeout_(10000),
      app_resuming_timeout_(5),
      app_dir_quota_(104857600),
      app_hmi_level_none_time_scale_max_requests_(100),
      app_hmi_level_none_requests_time_scale_(10),
      app_time_scale_max_requests_(100),
      app_requests_time_scale_(10),
      pending_requests_amount_(1000),
      put_file_in_none_(5),
      delete_file_in_none_(5),
      list_files_in_none_(5),
      app_info_storage_("app_info.dat"),
      heart_beat_timeout_(kDefaultHeartBeatTimeout),
      policy_snapshot_file_name_(kDefaultPoliciesSnapshotFileName),
      transport_manager_disconnect_timeout_(0),
      use_last_state_(false),
      supported_diag_modes_(),
      system_files_path_("/tmp/fs/mp/images/ivsu_cache"),
      transport_manager_tcp_adapter_port_(12345) {
}

Profile::~Profile() {
}

void Profile::config_file_name(const std::string& fileName) {
    if (false == fileName.empty()) {
        LOG4CXX_INFO(logger_, "setConfigFileName " << fileName);
        config_file_name_ = fileName;
        UpdateValues();
    }
}

const std::string& Profile::config_file_name() const {
    return config_file_name_;
}

bool Profile::launch_hmi() const {
    return launch_hmi_;
}

const std::string& Profile::app_config_folder() const {
    return app_config_folder_;
}

const std::string& Profile::app_storage_folder() const {
    return app_storage_folder_;
}


const std::string& Profile::policies_file_name() const {
    return policies_file_name_;
}

const std::string& Profile::hmi_capabilities_file_name() const {
    return hmi_capabilities_file_name_;
}

const std::string& Profile::server_address() const {
    return server_address_;
}

const std::vector<std::string>& Profile::help_prompt() const {
    return help_prompt_;
}

const std::vector<std::string>& Profile::time_out_promt() const {
    return time_out_promt_;
}

const std::vector<std::string>& Profile::vr_commands() const {
    return vr_commands_;
}

const uint32_t& Profile::max_cmd_id() const {
    return max_cmd_id_;
}

const uint32_t& Profile::default_timeout() const {
    return default_timeout_;
}

const uint32_t& Profile::app_resuming_timeout() const {
    return app_resuming_timeout_;
}

const std::string& Profile::vr_help_title() const {
    return vr_help_title_;
}

const uint16_t& Profile::server_port() const {
    return server_port_;
}

const uint16_t& Profile::video_streaming_port() const {
    return video_streaming_port_;
}

const uint16_t& Profile::audio_streaming_port() const {
    return audio_streaming_port_;
}

const uint64_t& Profile::thread_min_stack_size() const {
    return min_tread_stack_size_;
}

bool Profile::is_mixing_audio_supported() const {
    return is_mixing_audio_supported_;
}

const uint32_t& Profile::app_dir_quota() const {
    return app_dir_quota_;
}

bool Profile::is_redecoding_enabled() const {
    return is_redecoding_enabled_;
}

const std::string& Profile::video_server_type() const {
    return video_consumer_type_;
}

const std::string& Profile::audio_server_type() const {
    return audio_consumer_type_;
}

const std::string& Profile::named_video_pipe_path() const {
    return named_video_pipe_path_;
}

const std::string& Profile::named_audio_pipe_path() const {
    return named_audio_pipe_path_;
}

const uint32_t& Profile::app_hmi_level_none_time_scale() const {
    return app_hmi_level_none_requests_time_scale_;
}

const uint32_t& Profile::app_hmi_level_none_time_scale_max_requests() const {
    return app_hmi_level_none_time_scale_max_requests_;
}

const std::string& Profile::video_stream_file() const {
    return video_stream_file_;
}

const std::string& Profile::audio_stream_file() const {
    return audio_stream_file_;
}

const uint32_t& Profile::app_time_scale() const {
    return app_requests_time_scale_;
}

const uint32_t& Profile::app_time_scale_max_requests() const {
    return app_time_scale_max_requests_;
}

const uint32_t& Profile::pending_requests_amount() const {
    return pending_requests_amount_;
}

const uint32_t& Profile::put_file_in_none() const {
    return put_file_in_none_;
}

const uint32_t& Profile::delete_file_in_none() const {
    return delete_file_in_none_;
}

const uint32_t& Profile::list_files_in_none() const {
    return list_files_in_none_;
}

const std::string& Profile::app_info_storage() const {
    return app_info_storage_;
}

int32_t Profile::heart_beat_timeout() const {
  return heart_beat_timeout_;
}

const std::string& Profile::preloaded_pt_file() const {
    return preloaded_pt_file_;
}

const std::string& Profile::policies_snapshot_file_name() const {
    return policy_snapshot_file_name_;
}

uint32_t Profile::transport_manager_disconnect_timeout() const {
    return transport_manager_disconnect_timeout_;
}

bool Profile::use_last_state() const {
    return use_last_state_;
}

const std::string& Profile::system_files_path() const {
    return system_files_path_;
}

const std::vector<uint32_t>& Profile::supported_diag_modes() const {
    return supported_diag_modes_;
}

uint16_t Profile::transport_manager_tcp_adapter_port() const {
    return transport_manager_tcp_adapter_port_;
}

void Profile::UpdateValues() {
    LOG4CXX_INFO(logger_, "Profile::UpdateValues");

    char value[INI_LINE_LEN + 1];
    *value = '\0';

    if ((0 != ini_read_value(config_file_name_.c_str(), "HMI", "LaunchHMI", value))
            && ('\0' != *value)) {
        if (0 == strcmp("true", value)) {
            launch_hmi_ = true;
        } else {
            launch_hmi_ = false;
        }
        LOG4CXX_INFO(logger_, "Set launch HMI to " << launch_hmi_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN", "AppConfigFolder",
                              value)) && ('\0' != *value)) {
        app_config_folder_ = value;
    } else {
        // use current working directory
        app_config_folder_ = file_system::CurrentWorkingDirectory();
    }
    LOG4CXX_INFO(logger_, "Set App config folder to " << app_config_folder_);

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN", "AppStorageFolder",
                              value)) && ('\0' != *value)) {
        app_storage_folder_ = value;
    } else {
        // use current working directory
        app_storage_folder_ = file_system::CurrentWorkingDirectory();
    }
    LOG4CXX_INFO(logger_, "Set App storage folder to " << app_storage_folder_);

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "AppInfo", "AppInfoStorage",
                              value)) && ('\0' != *value)) {
        app_info_storage_ = app_storage_folder_ + "/" + value;
        LOG4CXX_INFO(
            logger_,
            "Set Application information storage to " << app_info_storage_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "HMI", "ServerAddress",
                              value)) && ('\0' != *value)) {
        server_address_ = value;
        LOG4CXX_INFO(logger_, "Set server address to " << server_address_);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             kPolicySection, "PoliciesTable", value))
            && ('\0' != *value)) {
        policies_file_name_ = app_config_folder_ + '/' + value;
        LOG4CXX_INFO(logger_, "Set policy file to " << policies_file_name_);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             kPolicySection, "PreloadedPT", value))
            && ('\0' != *value)) {
        preloaded_pt_file_ = app_config_folder_ + '/' + value;
        LOG4CXX_INFO(logger_, "Set preloaded policy file to "
                     << preloaded_pt_file_);
    }

    (void) ReadStringValue(&policy_snapshot_file_name_,
                           kDefaultPoliciesSnapshotFileName,
                           kPolicySection, "PathToSnapshot");
    policy_snapshot_file_name_ = system_files_path_ +
                                 '/' + policy_snapshot_file_name_;

    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN", "HMICapabilities",
                              value)) && ('\0' != *value)) {
        hmi_capabilities_file_name_ = app_config_folder_ + "/" +  value;
        LOG4CXX_INFO(logger_,
                     "Set hmi capabilities file to " << hmi_capabilities_file_name_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "HMI", "ServerPort", value))
            && ('\0' != *value)) {
        server_port_ = atoi(value);
        LOG4CXX_INFO(logger_, "Set server port to " << server_port_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "HMI", "VideoStreamingPort",
                              value)) && ('\0' != *value)) {
        video_streaming_port_ = atoi(value);
        LOG4CXX_INFO(logger_,
                     "Set video streaming port to " << video_streaming_port_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "HMI", "AudioStreamingPort",
                              value)) && ('\0' != *value)) {
        audio_streaming_port_ = atoi(value);
        LOG4CXX_INFO(logger_,
                     "Set audio streaming port to " << audio_streaming_port_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN", "ThreadStackSize",
                              value)) && ('\0' != *value)) {
        min_tread_stack_size_ = atoi(value);
        if (min_tread_stack_size_ < threads::Thread::kMinStackSize) {
            min_tread_stack_size_ = threads::Thread::kMinStackSize;
        }
        LOG4CXX_INFO(logger_,
                     "Set threadStackMinSize to " << min_tread_stack_size_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MEDIA MANAGER",
                              "EnableRedecoding", value)) && ('\0' != *value)) {
        if (0 == strcmp("true", value)) {
            is_redecoding_enabled_ = true;
        }
        LOG4CXX_INFO(logger_, "Set RedecodingEnabled to " << value);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MEDIA MANAGER",
                              "VideoStreamConsumer", value)) && ('\0' != *value)) {
        video_consumer_type_ = value;
        LOG4CXX_INFO(logger_,
                     "Set VideoStreamConsumer to " << video_consumer_type_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MEDIA MANAGER",
                              "AudioStreamConsumer", value)) && ('\0' != *value)) {
        audio_consumer_type_ = value;
        LOG4CXX_INFO(logger_,
                     "Set AudioStreamConsumer to " << audio_consumer_type_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MEDIA MANAGER",
                              "NamedVideoPipePath", value)) && ('\0' != *value)) {
        named_video_pipe_path_ = value;
        LOG4CXX_INFO(logger_, "Set server address to " << named_video_pipe_path_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MEDIA MANAGER",
                              "NamedAudioPipePath", value)) && ('\0' != *value)) {
        named_audio_pipe_path_ = value;
        LOG4CXX_INFO(logger_, "Set server address to " << named_audio_pipe_path_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MEDIA MANAGER",
                              "VideoStreamFile", value)) && ('\0' != *value)) {
        video_stream_file_ = app_storage_folder_ + "/" + value;
        LOG4CXX_INFO(logger_, "Set video stream file to " << video_stream_file_);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             "MEDIA MANAGER", "AudioStreamFile", value))
            && ('\0' != *value)) {
        audio_stream_file_ = app_storage_folder_ + "/" + value;
        LOG4CXX_INFO(logger_, "Set audio stream file to " << audio_stream_file_);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             "MAIN", "MixingAudioSupported", value))
            && ('\0' != *value)) {
        if (0 == strcmp("true", value)) {
            is_mixing_audio_supported_ = true;
        }
        LOG4CXX_INFO(logger_, "Set MixingAudioSupported to " << value);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(), "MAIN", "MaxCmdID", value))
            && ('\0' != *value)) {
        max_cmd_id_ = atoi(value);
        if (max_cmd_id_ < 0) {
            max_cmd_id_ = 2000000000;
        }
        LOG4CXX_INFO(logger_, "Set Maximum Command ID to " << max_cmd_id_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "FILESYSTEM RESTRICTIONS",
                              "PutFileRequest", value)) && ('\0' != *value)) {
        put_file_in_none_ = atoi(value);
        if (put_file_in_none_ < 0) {
            put_file_in_none_ = 5;
        }
        LOG4CXX_INFO(logger_, "Max allowed number of PutFile requests for one "
                     "application in NONE to " << put_file_in_none_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "FILESYSTEM RESTRICTIONS",
                              "DeleteFileRequest", value)) && ('\0' != *value)) {
        delete_file_in_none_ = atoi(value);
        if (delete_file_in_none_ < 0) {
            delete_file_in_none_ = 5;
        }
        LOG4CXX_INFO(logger_, "Max allowed number of DeleteFile requests for one "
                     "application in NONE to " << delete_file_in_none_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "FILESYSTEM RESTRICTIONS",
                              "ListFilesRequest", value)) && ('\0' != *value)) {
        list_files_in_none_ = atoi(value);
        if (list_files_in_none_ < 0) {
            list_files_in_none_ = 5;
        }
        LOG4CXX_INFO(logger_, "Max allowed number of ListFiles requests for one "
                     "application in NONE to " << list_files_in_none_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN", "DefaultTimeout",
                              value)) && ('\0' != *value)) {
        default_timeout_ = atoi(value);
        if (default_timeout_ <= 0) {
            default_timeout_ = 10000;
        }
        LOG4CXX_INFO(logger_, "Set Default timeout to " << default_timeout_);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             "MAIN", "ApplicationResumingTimeout", value))
            && ('\0' != *value)) {
        app_resuming_timeout_ = atoi(value);
        if (app_resuming_timeout_ <= 0) {
            app_resuming_timeout_ = 5;
        }
        LOG4CXX_INFO(logger_, "Set Resuming timeout to " << app_resuming_timeout_);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             "MAIN", "AppDirectoryQuota", value))
            && ('\0' != *value)) {
        app_dir_quota_ = atoi(value);
        if (app_dir_quota_ <= 0) {
            app_dir_quota_ = 104857600;
        }
        LOG4CXX_INFO(logger_, "Set App Directory Quota " << app_dir_quota_);
    }

    help_prompt_.clear();
    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "GLOBAL PROPERTIES",
                              "HelpPromt", value)) && ('\0' != *value)) {
        char* str = NULL;
        str = strtok(value, ",");
        while (str != NULL) {
            LOG4CXX_INFO(logger_, "Add HelpPromt string" << str);
            help_prompt_.push_back(std::string(str));
            str = strtok(NULL, ",");
        }
    }

    time_out_promt_.clear();
    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "GLOBAL PROPERTIES",
                              "TimeOutPromt", value)) && ('\0' != *value)) {
        char* str = NULL;
        str = strtok(value, ",");
        while (str != NULL) {
            LOG4CXX_INFO(logger_, "Add TimeOutPromt string" << str);
            time_out_promt_.push_back(std::string(str));
            str = strtok(NULL, ",");
        }
    }

    vr_help_title_ = "";
    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "GLOBAL PROPERTIES",
                              "HelpTitle", value)) && ('\0' != *value)) {
        vr_help_title_ = value;
        LOG4CXX_INFO(logger_, "Add HelpTitle string" << vr_help_title_);
    }

    vr_commands_.clear();
    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "VR COMMANDS", "HelpCommand",
                              value)) && ('\0' != *value)) {
        char* str = NULL;
        str = strtok(value, ",");
        while (str != NULL) {
            LOG4CXX_INFO(logger_, "Add vr command string" << str);
            vr_commands_.push_back(std::string(str));
            str = strtok(NULL, ",");
        }
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "AppTimeScaleMaxRequests", value))
            && ('\0' != *value)) {
        app_time_scale_max_requests_ = atoi(value);
        LOG4CXX_INFO(logger_, "Set max amount of requests per application"
                     " time scale " << app_time_scale_max_requests_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "AppRequestsTimeScale", value)) && ('\0' != *value)) {
        app_requests_time_scale_ = atoi(value);
        LOG4CXX_INFO(logger_, "Set Application time scale for max amount"
                     " of requests " << app_requests_time_scale_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "AppHMILevelNoneTimeScaleMaxRequests", value))
            && ('\0' != *value)) {
        app_hmi_level_none_time_scale_max_requests_ = atoi(value);
        LOG4CXX_INFO(logger_, "Set max amount of requests per application"
                     " time scale " << app_hmi_level_none_time_scale_max_requests_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "AppHMILevelNoneRequestsTimeScale", value))
            && ('\0' != *value)) {
        app_hmi_level_none_requests_time_scale_ = atoi(value);
        LOG4CXX_INFO(logger_, "Set Application time scale for max amount"
                     " of requests " << app_hmi_level_none_requests_time_scale_);
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "PendingRequestsAmount", value)) && ('\0' != *value)) {
        pending_requests_amount_ = atoi(value);
        if (app_dir_quota_ <= 0) {
            pending_requests_amount_ = 1000;
        }
        LOG4CXX_INFO(
            logger_,
            "Set system pending requests amount " << pending_requests_amount_);
    }

    supported_diag_modes_.clear();
    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "SupportedDiagModes", value)) && ('\0' != *value)) {
        char* str = NULL;
        str = strtok(value, ",");
        while (str != NULL) {
            supported_diag_modes_.push_back(strtol(str, NULL, 16));
            str = strtok(NULL, ",");
        }
    }

    *value = '\0';
    if ((0
            != ini_read_value(config_file_name_.c_str(), "MAIN",
                              "SystemFilesPath", value)) && ('\0' != *value)) {
        system_files_path_ = value;
        LOG4CXX_INFO(logger_, "Set system files path to " << system_files_path_);
    }

    (void) ReadIntValue(&heart_beat_timeout_, kDefaultHeartBeatTimeout,
                        kMainSection, "HeartBeatTimeout");

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             "MAIN", "UseLastState", value))
            && ('\0' != *value)) {
        if (0 == strcmp("true", value)) {
            use_last_state_ = true;
        }
        LOG4CXX_INFO(logger_, "Set UseLastState to " << value);
    }

    *value = '\0';
    if ((0 != ini_read_value(config_file_name_.c_str(),
                             "TransportManager", "TCPAdapterPort", value)) && ('\0' != *value)) {

        transport_manager_tcp_adapter_port_ = atoi(value);
        LOG4CXX_INFO(logger_, "Set TCPAdapterPort to " << value);
    }
}

bool Profile::ReadValue(bool* value, const char* const pSection,
                        const char* const pKey) const {
  DCHECK(value);
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(), pSection, pKey, buf))
      && ('\0' != *buf)) {
    const int32_t tmpVal = atoi(buf);
    if (0 == tmpVal) {
        *value = false;
    } else {
        *value = true;
    }

    ret = true;
  }
  return ret;
}

bool Profile::ReadValue(std::string* value, const char* const pSection,
                        const char* const pKey) const {
  DCHECK(value);
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(), pSection, pKey, buf))
      && ('\0' != *buf)) {
    *value = buf;
    ret = true;
  }
  return ret;
}

bool Profile::ReadStringValue(std::string* value, const char* default_value,
                              const char* const pSection,
                              const char* const pKey) const {
  DCHECK(value);
  if (!ReadValue(value, pSection, pKey)) {
    *value = default_value;
    return false;
  }
  return true;
}

bool Profile::ReadIntValue(int32_t* value, const int32_t default_value,
                           const char* const pSection,
                           const char* const pKey) const {
  DCHECK(value);
  std::string string_value;
  if (!ReadValue(&string_value, pSection, pKey)) {
    *value = default_value;
    return false;
  } else {
    *value = atoi(string_value.c_str());
    return true;
  }
}

bool Profile::ReadBoolValue(bool* value, const bool default_value,
                           const char* const pSection,
                           const char* const pKey) const {
  DCHECK(value);
  bool read_value;
  const bool result = ReadValue(&read_value, pSection, pKey);
  *value = result ? read_value : default_value;
  return result;
}

std::list<int> Profile::ReadIntContainer(
    const char * const pSection, const char * const pKey,
    bool *out_result) const {
  const std::list<std::string> string_list =
      ReadStringContainer(pSection, pKey, out_result);
  std::list<int> value_list;
  for (std::list<std::string>::const_iterator it = string_list.begin();
       string_list.end() != it; ++it) {
    value_list.push_back(strtol(it->c_str(), NULL, 16));
    }
  return value_list;
}

std::list<std::string> Profile::ReadStringContainer(
    const char * const pSection, const char * const pKey,
    bool *out_result) const {
  std::string string;
  const bool result = ReadValue(&string, pSection, pKey);
  if (out_result)
    *out_result = result;
  std::list<std::string> value_container;
  if (result) {
    std::istringstream iss(string);
    std::string temp_str;
    while (iss) {
      if (!getline( iss, temp_str, ',' )) break;
      value_container.push_back(temp_str);
    }
  }
  return value_container;
}
}  //  namespace profile
