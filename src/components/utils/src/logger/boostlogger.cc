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

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/from_settings.hpp>
#include <boost/log/utility/setup/settings_parser.hpp>
#include <boost/regex.hpp>
#include <fstream>

namespace logger {

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

BoostLogger::BoostLogger(const std::string& filename)
    : filename_(filename)
    , slg_()
    , lg_attr_({.timestamp_ = attrs::mutable_constant<boost::posix_time::ptime>(
                    boost::posix_time::not_a_date_time),
                .thread_id_ = attrs::mutable_constant<std::thread::id>(
                    std::this_thread::get_id()),
                .component_ = attrs::mutable_constant<std::string>(""),
                .file_name_ = attrs::mutable_constant<std::string>(""),
                .line_num_ = attrs::mutable_constant<int>(0),
                .trace_ = attrs::mutable_constant<std::string>("")}) {}

void BoostLogger::Init() {
  // Add formatting parameters to INI file
  // Allows %Severity% to be used in ini config file for property Format.
  boost::log::register_simple_formatter_factory<
      boost::log::trivial::severity_level,
      char>("Severity");
  boost::log::register_simple_formatter_factory<std::string, char>("Component");
  boost::log::register_simple_formatter_factory<boost::posix_time::ptime, char>(
      "TimeStamp");
  boost::log::register_simple_formatter_factory<std::thread::id, char>(
      "ThreadId");
  boost::log::register_simple_formatter_factory<std::string, char>("FileName");
  boost::log::register_simple_formatter_factory<int, char>("LineNum");
  boost::log::register_simple_formatter_factory<std::string, char>("Trace");

  // Add filter parameters to INI file
  // Allows %Severity% to be used in ini config file for property Filter.
  boost::log::
      register_simple_filter_factory<boost::log::trivial::severity_level, char>(
          "Severity");
  boost::log::register_simple_filter_factory<std::string, char>("Component");
  boost::log::register_simple_filter_factory<std::string, char>("Trace");

  std::ifstream file(filename_);
  boost::log::settings settings = boost::log::parse_settings(file);

  slg_.add_attribute("TimeStamp", lg_attr_.timestamp_);
  slg_.add_attribute("ThreadId", lg_attr_.thread_id_);
  slg_.add_attribute("Component", lg_attr_.component_);
  slg_.add_attribute("FileName", lg_attr_.file_name_);
  slg_.add_attribute("LineNum", lg_attr_.line_num_);
  slg_.add_attribute("Trace", lg_attr_.trace_);

  // Custom Settings

  if (settings.has_section("Sinks")) {
    auto& sinks = settings.property_tree().get_child("Sinks");

    for (auto it = sinks.begin(); it != sinks.end();) {
      bool to_be_removed = false;
      std::string sink_name = "Sinks." + it->first;

      // Disable logging for particular sinks
      if (boost::optional<std::string> param =
              settings[sink_name]["DisableLogging"]) {
        to_be_removed = (param.get() == "true");
      }

      if (to_be_removed) {
        it = sinks.erase(it);
      } else {
        it++;
      }
    }
  }

  logging::init_from_settings(settings);
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
      return logging::trivial::severity_level::trace;
  }
}

boost::posix_time::ptime BoostLogger::GetLocalPosixTime(
    const logger::TimePoint& timestamp) {
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(
                  timestamp.time_since_epoch())
                  .count();

  boost::posix_time::ptime time_epoch(boost::gregorian::date(1970, 1, 1));
  boost::posix_time::ptime utc_time =
      time_epoch + boost::posix_time::microseconds(time);

  typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime>
      local_adj;
  auto local_time = local_adj::utc_to_local(utc_time);

  return local_time;
}

std::string BoostLogger::GetFilteredFunctionTrace(
    const std::string& full_function_signature) {
  auto start_pos = full_function_signature.find(' ');
  auto end_pos = full_function_signature.find('(', start_pos);

  if (start_pos == std::string::npos || end_pos == std::string::npos ||
      start_pos + 1 == full_function_signature.length()) {
    return full_function_signature;
  }
  return full_function_signature.substr(start_pos + 1, end_pos - start_pos - 1);
}

bool BoostLogger::IsEnabledFor(const std::string& component,
                               LogLevel log_level) const {
  // Basic filtering based on component name and log level is currently handled
  // using the Filter attribute. This function should be implemented if we add
  // custom attributes for filtering or additional logic for filtering based on
  // component name and log level.
  return true;
}

void BoostLogger::PushLog(const LogMessage& log_message) {
  auto local_time = GetLocalPosixTime(log_message.timestamp_);
  std::string func_name =
      GetFilteredFunctionTrace(log_message.location_.function_name);

  lg_attr_.timestamp_.set(local_time);
  lg_attr_.thread_id_.set(log_message.thread_id_);
  lg_attr_.component_.set(log_message.component_);
  lg_attr_.file_name_.set(log_message.location_.file_name);
  lg_attr_.line_num_.set(log_message.location_.line_number);
  lg_attr_.trace_.set(func_name);

  BOOST_LOG_SEV(slg_, getBoostLogLevel(log_message.log_level_))
      << log_message.log_event_;
}

}  // namespace logger
