﻿/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_COUNTER_H
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_COUNTER_H

#include <ctime>
#include "usage_statistics/statistics_manager.h"
#include "utils/shared_ptr.h"
#include "utils/timer_thread.h"

namespace usage_statistics {

class GlobalCounter {
 public:
  GlobalCounter(utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
                GlobalCounterId counter_type);
  void operator++() const;
 private:
  GlobalCounterId counter_type_;
  utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager_;
};

class AppCounter {
 public:
  AppCounter(utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
             const std::string& app_id,
             AppCounterId counter_type);
  void operator++() const;
 private:
  std::string app_id_;
  AppCounterId counter_type_;
  utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager_;
};

class AppInfo {
 public:
  AppInfo(utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
          const std::string& app_id,
          AppInfoId info_type);
  void Update(const std::string& new_info) const;
 private:
  std::string app_id_;
  AppInfoId info_type_;
  utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager_;
};

class AppStopwatch {
 public:
  AppStopwatch(utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
               const std::string& app_id);
  AppStopwatch(utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
               const std::string& app_id,
               std::uint32_t time_out);
  ~AppStopwatch();
  void Start(AppStopwatchId stopwatch_type);
  void Switch(AppStopwatchId stopwatch_type);
  void WriteTime();
 private:
  // Fields
  std::string app_id_;
  AppStopwatchId stopwatch_type_;
  utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager_;
  typedef timer::TimerThread<AppStopwatch> Timer;
  Timer* timer_;
  const std::uint32_t time_out_;
};

}  // namespace usage_statistics

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_COUNTER_H
