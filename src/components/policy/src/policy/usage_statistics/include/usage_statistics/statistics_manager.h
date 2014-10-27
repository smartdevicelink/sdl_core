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
};

}  //  namespace usage_statistics

#endif  //  SRC_COMPONENTS_POLICY_INCLUDE_POLICY_USAGE_STATISTICS_STATISTICS_MANAGER_H_
