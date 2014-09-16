/*
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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_STATISTICS_MANAGER_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_STATISTICS_MANAGER_H_

#include <stdint.h>
#include <string>

namespace usage_statistics {

enum GlobalCounterId {
  IAP_BUFFER_FULL,
  SYNC_OUT_OF_MEMORY,
  SYNC_REBOOTS
};

enum AppInfoId {
  LANGUAGE_GUI,
  LANGUAGE_VUI
};

enum AppStopwatchId {
  SECONDS_HMI_FULL,
  SECONDS_HMI_LIMITED,
  SECONDS_HMI_BACKGROUND,
  SECONDS_HMI_NONE
};

enum AppCounterId {
  USER_SELECTIONS,
  REJECTIONS_SYNC_OUT_OF_MEMORY,
  REJECTIONS_NICKNAME_MISMATCH,
  REJECTIONS_DUPLICATE_NAME,
  REJECTED_RPC_CALLS,
  RPCS_IN_HMI_NONE,
  REMOVALS_MISBEHAVED,
  RUN_ATTEMPTS_WHILE_REVOKED
};

class StatisticsManager {
 public:
  virtual ~StatisticsManager() {}
  virtual void Increment(GlobalCounterId type) = 0;
  virtual void Increment(const std::string& app_id, AppCounterId type) = 0;
  virtual void Set(const std::string& app_id, AppInfoId type,
                   const std::string& value) = 0;
  virtual void Add(const std::string& app_id,
                   AppStopwatchId type,
                   int32_t timespan_seconds) = 0;

  static std::string EnumToStdString(GlobalCounterId global_counter_id) {
    std::string counter;
    switch (global_counter_id) {
      case usage_statistics::IAP_BUFFER_FULL:
        counter = "count_of_iap_buffer_full";
        break;
      case usage_statistics::SYNC_OUT_OF_MEMORY:
        counter = "count_sync_out_of_memory";
        break;
      case usage_statistics::SYNC_REBOOTS:
        counter = "count_of_sync_reboots";
        break;
    }
    return counter;
  }

  static std::string EnumToStdString(AppInfoId app_info_id) {
    std::string info;
    switch (app_info_id) {
      case usage_statistics::LANGUAGE_GUI:
        info = "app_registration_language_gui";
        break;
      case usage_statistics::LANGUAGE_VUI:
        info = "app_registration_language_vui";
        break;
    }
    return info;
  }

  static std::string EnumToStdString(AppStopwatchId app_stop_watch_id) {
    std::string stopwatch;
    switch (app_stop_watch_id) {
      // TODO(KKolodiy): rename fields in database
      case usage_statistics::SECONDS_HMI_FULL:
        stopwatch = "minutes_in_hmi_full";
        break;
      case usage_statistics::SECONDS_HMI_LIMITED:
        stopwatch = "minutes_in_hmi_limited";
        break;
      case usage_statistics::SECONDS_HMI_BACKGROUND:
        stopwatch = "minutes_in_hmi_background";
        break;
      case usage_statistics::SECONDS_HMI_NONE:
        stopwatch = "minutes_in_hmi_none";
        break;
    }
    return stopwatch;
  }

  static std::string EnumToStdString(AppCounterId app_counter_id) {
    std::string counter;
    switch (app_counter_id) {
      case usage_statistics::USER_SELECTIONS:
        counter = "count_of_user_selections";
        break;
      case usage_statistics::REJECTIONS_SYNC_OUT_OF_MEMORY:
        counter = "count_of_rejections_sync_out_of_memory";
        break;
      case usage_statistics::REJECTIONS_NICKNAME_MISMATCH:
        counter = "count_of_rejections_nickname_mismatch";
        break;
      case usage_statistics::REJECTIONS_DUPLICATE_NAME:
        counter = "count_of_rejections_duplicate_name";
        break;
      case usage_statistics::REJECTED_RPC_CALLS:
        counter = "count_of_rejected_rpcs_calls";
        break;
      case usage_statistics::RPCS_IN_HMI_NONE:
        counter = "count_of_rpcs_sent_in_hmi_none";
        break;
      case usage_statistics::REMOVALS_MISBEHAVED:
        counter = "count_of_removals_for_bad_behavior";
        break;
      case usage_statistics::RUN_ATTEMPTS_WHILE_REVOKED:
        counter = "count_of_run_attempts_while_revoked";
        break;
    }
    return counter;
  }
};

}  //  namespace usage_statistics

#endif  //  SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_STATISTICS_MANAGER_H_
