/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_USAGE_STATISTICS_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_USAGE_STATISTICS_H_

#include <string>
#include <memory>
#include "usage_statistics/counter.h"
#include "usage_statistics/app_stopwatch.h"
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

class UsageStatistics {
 public:
  UsageStatistics(
      const std::string& app_id,
      utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager);
  UsageStatistics(
      const std::string& app_id,
      utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
      usage_statistics::AppStopwatch* time_in_hmi_state_ptr);
  void RecordHmiStateChanged(mobile_apis::HMILevel::eType new_hmi_level);
  void RecordAppRegistrationGuiLanguage(
      mobile_apis::Language::eType gui_language);
  void RecordAppRegistrationVuiLanguage(
      mobile_apis::Language::eType vui_language);
  void RecordRpcSentInHMINone();
  void RecordPolicyRejectedRpcCall();
  void RecordAppUserSelection();
  void RecordRunAttemptsWhileRevoked();
  void RecordRemovalsForBadBehavior();
  void RecordTLSError();

 private:
  std::auto_ptr<usage_statistics::AppStopwatch> time_in_hmi_state_sptr_;
  usage_statistics::AppInfo app_registration_language_gui_;
  usage_statistics::AppInfo app_registration_language_vui_;
  usage_statistics::AppCounter count_of_rejected_rpc_calls_;
  usage_statistics::AppCounter count_of_rpcs_sent_in_hmi_none_;
  usage_statistics::AppCounter count_of_user_selections_;
  usage_statistics::AppCounter count_of_run_attempts_while_revoked_;
  usage_statistics::AppCounter count_of_removals_for_bad_behavior_;
  usage_statistics::AppCounter count_of_tls_error_;
  DISALLOW_COPY_AND_ASSIGN(UsageStatistics);
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_USAGE_STATISTICS_H_
