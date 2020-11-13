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

#include "utils/logger/boostlogger.h"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <fstream>

namespace logger {

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;

BoostLogger::BoostLogger(const std::string& filename) : filename_(filename) {}

void BoostLogger::Init() {
  // Construct the sink

  // Allows %Severity% to be used in ini config file for property Filter.
  boost::log::
      register_simple_filter_factory<boost::log::trivial::severity_level, char>(
          "Severity");
  // Allows %Severity% to be used in ini config file for property Format.
  boost::log::register_simple_formatter_factory<
      boost::log::trivial::severity_level,
      char>("Severity");

  std::ifstream file(filename_);
  logging::init_from_stream(file);

  // Register the sink in the logging core
  // logging::core::get()->add_sink(sink);
}

void BoostLogger::DeInit() {
  logging::core::get()->remove_all_sinks();
}

logging::trivial::severity_level getBoostLogLevel(LogLevel log_level) {
  switch (log_level) {
    case LogLevel::TRACE_LEVEL:
      return logging::trivial::severity_level::trace;
    case LogLevel::DEBUG_LEVEL:
      return logging::trivial::severity_level::debug;
    case LogLevel::INFO_LEVEL:
      return logging::trivial::severity_level::info;
    case LogLevel::WARNING_LEVEL:
      return logging::trivial::severity_level::warning;
    case LogLevel::ERROR_LEVEL:
      return logging::trivial::severity_level::error;
    case LogLevel::FATAL_LEVEL:
      return logging::trivial::severity_level::fatal;
    default:
      assert(false);
  }
}

bool BoostLogger::IsEnabledFor(const std::string& component,
                               LogLevel log_level) const {
  // To be implemented once the config file adds limiting the log level
  return true;
}

void BoostLogger::PushLog(const LogMessage& log_message) {
  auto time = std::chrono::system_clock::to_time_t(log_message.timestamp_);

  src::severity_logger<logging::trivial::severity_level> slg;

  boost::format fmt = boost::format("%1% [%2%][%3%][%4%] %5%:%6% %7%: %8%") %
                      getBoostLogLevel(log_message.log_level_) %
                      std::put_time(std::localtime(&time), "%d %b %Y %T") %
                      log_message.thread_id_ % log_message.component_ %
                      log_message.location_.file_name.c_str() %
                      log_message.location_.line_number %
                      log_message.location_.function_name.c_str() %
                      log_message.log_event_;

  BOOST_LOG_CHANNEL_SEV(
      slg, log_message.component_, getBoostLogLevel(log_message.log_level_))
      << fmt.str();
}

}  // namespace logger
