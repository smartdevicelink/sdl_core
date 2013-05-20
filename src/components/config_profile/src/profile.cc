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

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Profile"));

namespace profile {
Profile::Profile()
: configFileName_("smartDeviceLink.ini")
, serverAddress_("127.0.0.1")
, serverport_(8087)
, minTreadStackSize_(threads::Thread::kMinStackSize) {
  updateValues();
}

Profile::~Profile() {
}

Profile* Profile::getInstance() {
  static Profile instance;
  return &instance;
}

void Profile::setConfigFileName(const std::string& fileName) {
  if (false == fileName.empty()) {
     LOG4CXX_INFO(logger_, "setConfigFileName " << fileName);
     configFileName_ = fileName;
     updateValues();
  }
}

const std::string& Profile::getConfigFileName() const {
  return configFileName_;
}

const std::string& Profile::getServerAddress() const {
  return serverAddress_;
}

const uint16_t& Profile::getServerPort() const {
  return serverport_;
}

const uint64_t& Profile::getThreadStachSize() const {
  return minTreadStackSize_;
}

void Profile::updateValues() {
  LOG4CXX_INFO(logger_, "Profile::updateValues");

  char value[INI_LINE_LEN + 1];
  *value = '\0';
  if ((0 != ini_read_value(configFileName_.c_str(),
                           "HMI", "ServerAddress", value))
      && ('\0' != *value)) {
    serverAddress_ = value;
    LOG4CXX_INFO(logger_, "Set server address to " << serverAddress_);
  }

  *value = '\0';
  if ((0 != ini_read_value(configFileName_.c_str(),
                           "HMI", "ServerPort", value))
      && ('\0' != *value)) {
    serverport_ = atoi(value);
    LOG4CXX_INFO(logger_, "Set server port to " << serverport_);
  }

  *value = '\0';
  if ((0 != ini_read_value(configFileName_.c_str(),
                           "MAIN", "ThreadStackSize", value))
      && ('\0' != *value)) {
    minTreadStackSize_ = atoi(value);
    LOG4CXX_INFO(logger_, "Set threadStackMinSize " << minTreadStackSize_);
  }
}

bool Profile::readValue(bool* value, const char* const pSection,
                        const char* const pKey) const {
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(configFileName_.c_str(), pSection, pKey, buf))
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

bool Profile::readValue(std::string* value,
                        const char* const pSection,
                        const char* const pKey) const {
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(configFileName_.c_str(), pSection, pKey, buf))
      && ('\0' != *buf)) {
    *value = buf;
    ret = true;
  }

  return ret;
}
}  //  namespace profile
