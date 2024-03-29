/*
 * Copyright (c) 2020, Livio
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
#pragma once

#define BOOST_LOG_DYN_LINK 1

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include "utils/ilogger.h"

namespace logger {

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace attrs = boost::log::attributes;

class BoostLogger : public ThirdPartyLoggerInterface {
 public:
  BoostLogger(const std::string& filename);
  void Init() override;
  void DeInit() override;
  bool IsEnabledFor(const std::string& component,
                    LogLevel log_level) const override;
  void PushLog(const LogMessage& log_message) override;

 private:
  struct LogAttributes {
    attrs::mutable_constant<boost::posix_time::ptime> timestamp_;
    attrs::mutable_constant<std::thread::id> thread_id_;
    attrs::mutable_constant<std::string> component_;
    attrs::mutable_constant<std::string> file_name_;
    attrs::mutable_constant<int> line_num_;
    attrs::mutable_constant<std::string> trace_;
  };

  boost::posix_time::ptime GetLocalPosixTime(
      const logger::TimePoint& timestamp);

  std::string GetFilteredFunctionTrace(
      const std::string& full_function_signature);

  std::string filename_;
  src::severity_logger<logging::trivial::severity_level> slg_;
  LogAttributes lg_attr_;
};

}  // namespace logger
