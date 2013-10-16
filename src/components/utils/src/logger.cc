/**
 * \file LOG4CXXLogger.hpp
 * \brief Definitions required by logger.
 * Stores device information
 *
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


#include "utils/logger.h"

namespace log4cxx {

LoggerPtr::LoggerPtr(const std::string& name)
  : name_(name),
    output_stream_(NULL) {
}

LoggerPtr::LoggerPtr()
  : output_stream_(NULL) {}

LoggerPtr::LoggerPtr(const LoggerPtr& other)
  : name_(other.name_),
    output_stream_(other.output_stream_) {
}

LoggerPtr::~LoggerPtr() {

}

const std::string& LoggerPtr::name() const {
  return name_;
}

const LoggerPtr& Logger::getLogger(const std::string& name) {
  static std::map<std::string, LoggerPtr> loggers_ ;
  if (loggers_.end() == loggers_.find(name)) {
    LoggerPtr fresh_logger(name);
    loggers_.insert(std::pair<std::string, LoggerPtr>(name, fresh_logger));
  }
  return loggers_[name];
  // TODO: clear
}

void Logger::Configure(const std::string& file_name) {
  l4l::InternalLogger::Configure(file_name);
}

char* get_time() {
  time_t rawtime;
  time(&rawtime);
  return ctime(&rawtime);
}

}  //  namespace log4cxx