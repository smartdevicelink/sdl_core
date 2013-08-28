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

#include "config_profile/ini_file.h"
#include "config_profile/profile.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"

#include <string.h>

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Profile"));

namespace profile {
Profile::Profile()
  : config_file_name_("smartDeviceLink.ini")
  , policies_file_name_("policy_table.json")
  , server_address_("127.0.0.1")
  , help_promt_()
  , time_out_promt_()
  , server_port_(8087)
  , min_tread_stack_size_(threads::Thread::kMinStackSize)
  , is_mixing_audio_supported_(false)
  , max_cmd_id_(2000000000)
  , default_timeout_(10000) {
  UpdateValues();
}

Profile::~Profile() {
}

Profile* Profile::instance() {
  static Profile instance;
  return &instance;
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

const std::string& Profile::policies_file_name() const {
  return policies_file_name_;
}

const std::string& Profile::server_address() const {
  return server_address_;
}

const std::vector<std::string>& Profile::help_promt() const {
  return help_promt_;
}

const std::vector<std::string>& Profile::time_out_promt() const {
  return time_out_promt_;
}

const std::vector<std::string>& Profile::vr_commands() const {
  return vr_commands_;
}

const unsigned int Profile::max_cmd_id() const {
  return max_cmd_id_;
}

const unsigned int Profile::default_timeout() const {
  return default_timeout_;
}

const std::string& Profile::vr_help_title() const {
  return vr_help_title_;
}

const uint16_t& Profile::server_port() const {
  return server_port_;
}

const uint64_t& Profile::thread_min_stach_size() const {
  return min_tread_stack_size_;
}

bool Profile::is_mixing_audio_supported() const {
  return is_mixing_audio_supported_;
}

void Profile::UpdateValues() {
  LOG4CXX_INFO(logger_, "Profile::UpdateValues");

  char value[INI_LINE_LEN + 1];
  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "HMI", "ServerAddress", value))
      && ('\0' != *value)) {
    server_address_ = value;
    LOG4CXX_INFO(logger_, "Set server address to " << server_address_);
  }

  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "MAIN", "PoliciesTable", value))
      && ('\0' != *value)) {
    policies_file_name_ = value;
    LOG4CXX_INFO(logger_, "Set server address to " << policies_file_name_);
  }

  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "HMI", "ServerPort", value))
      && ('\0' != *value)) {
    server_port_ = atoi(value);
    LOG4CXX_INFO(logger_, "Set server port to " << server_port_);
  }

  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "MAIN", "ThreadStackSize", value))
      && ('\0' != *value)) {
    min_tread_stack_size_ = atoi(value);
    if (min_tread_stack_size_ < threads::Thread::kMinStackSize) {
      min_tread_stack_size_ = threads::Thread::kMinStackSize;
    }
    LOG4CXX_INFO(logger_, "Set threadStackMinSize to " << min_tread_stack_size_);
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
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "MAIN", "MaxCmdID", value))
      && ('\0' != *value)) {
    max_cmd_id_ = atoi(value);
    if (max_cmd_id_ < 0) {
      max_cmd_id_ = 20000000000;
    }
    LOG4CXX_INFO(logger_, "Set Maximum Command ID to " << max_cmd_id_);
  }

  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "MAIN", "DefaultTimeout", value))
      && ('\0' != *value)) {
    default_timeout_ = atoi(value);
    if (default_timeout_ <= 0) {
      default_timeout_ = 10000;
    }
    LOG4CXX_INFO(logger_, "Set Default timeout to " << default_timeout_);
  }

  help_promt_.clear();
  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "GLOBAL PROPERTIES", "HelpPromt", value))
      && ('\0' != *value)) {
    char* str = NULL;
    str = strtok(value, ",");
    while (str != NULL) {
      LOG4CXX_INFO(logger_, "Add HelpPromt string" << str);
      help_promt_.push_back(std::string(str));
      str = strtok(NULL, ",");
    }
  }

  time_out_promt_.clear();
  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "GLOBAL PROPERTIES", "TimeOutPromt", value))
      && ('\0' != *value)) {
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
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "GLOBAL PROPERTIES", "HelpTitle", value))
      && ('\0' != *value)) {
    vr_help_title_ = value;
    LOG4CXX_INFO(logger_, "Add HelpTitle string" << vr_help_title_);
  }

  vr_commands_.clear();
  *value = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(),
                           "VR COMMANDS", "HelpCommand", value))
      && ('\0' != *value)) {
    char* str = NULL;
    str = strtok(value, ",");
    while (str != NULL) {
      LOG4CXX_INFO(logger_, "Add vr command string" << str);
      vr_commands_.push_back(std::string(str));
      str = strtok(NULL, ",");
    }
  }
}

bool Profile::ReadValue(bool* value, const char* const pSection,
                        const char* const pKey) const {
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(), pSection, pKey, buf))
      && ('\0' != *buf)) {
    const int tmpVal = atoi(buf);
    if (0 == tmpVal) {
      *value = false;
    } else {
      *value = true;
    }

    ret = true;
  }

  return ret;
}

bool Profile::ReadValue(std::string* value,
                        const char* const pSection,
                        const char* const pKey) const {
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
}  //  namespace profile
