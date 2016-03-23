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

#include "application_manager/usage_statistics.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/enum_schema_item.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "utils/macro.h"
#include "utils/make_shared.h"

using namespace mobile_apis;
using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace usage_statistics;

namespace application_manager {

namespace {

std::string LanguageIdToString(Language::eType lang_id) {
  const char* str;
  const bool ok = EnumConversionHelper<Language::eType>::EnumToCString(lang_id, &str);
  return ok ? str : "unknown";
}

}  // namespace

UsageStatistics::UsageStatistics(
    const std::string& app_id,
    utils::SharedPtr<StatisticsManager> statistics_manager)
    : time_in_hmi_state_sptr_(
          new usage_statistics::AppStopwatchImpl(statistics_manager, app_id))
    , app_registration_language_gui_(statistics_manager, app_id, LANGUAGE_GUI)
    , app_registration_language_vui_(statistics_manager, app_id, LANGUAGE_VUI)
    , count_of_rejected_rpc_calls_(
          statistics_manager, app_id, REJECTED_RPC_CALLS)
    , count_of_rpcs_sent_in_hmi_none_(
          statistics_manager, app_id, RPCS_IN_HMI_NONE)
    , count_of_user_selections_(statistics_manager, app_id, USER_SELECTIONS)
    , count_of_run_attempts_while_revoked_(
          statistics_manager, app_id, RUN_ATTEMPTS_WHILE_REVOKED)
    , count_of_removals_for_bad_behavior_(
          statistics_manager, app_id, REMOVALS_MISBEHAVED)
    , count_of_tls_error_(statistics_manager, app_id, COUNT_OF_TLS_ERRORS) {
  time_in_hmi_state_sptr_->Start(SECONDS_HMI_NONE);
}

UsageStatistics::UsageStatistics(const std::string& app_id,
    utils::SharedPtr<StatisticsManager> statistics_manager,
      AppStopwatch* time_in_hmi_state_ptr)
    : time_in_hmi_state_sptr_(time_in_hmi_state_ptr)
    , app_registration_language_gui_(statistics_manager, app_id, LANGUAGE_GUI)
    , app_registration_language_vui_(statistics_manager, app_id, LANGUAGE_VUI)
    , count_of_rejected_rpc_calls_(
          statistics_manager, app_id, REJECTED_RPC_CALLS)
    , count_of_rpcs_sent_in_hmi_none_(
          statistics_manager, app_id, RPCS_IN_HMI_NONE)
    , count_of_user_selections_(statistics_manager, app_id, USER_SELECTIONS)
    , count_of_run_attempts_while_revoked_(
          statistics_manager, app_id, RUN_ATTEMPTS_WHILE_REVOKED)
    , count_of_removals_for_bad_behavior_(
          statistics_manager, app_id, REMOVALS_MISBEHAVED)
    , count_of_tls_error_(statistics_manager, app_id, COUNT_OF_TLS_ERRORS) {
  DCHECK(time_in_hmi_state_sptr_.get());
  time_in_hmi_state_sptr_->Start(SECONDS_HMI_NONE);
}

void UsageStatistics::RecordHmiStateChanged(HMILevel::eType new_hmi_level) {
  using namespace mobile_apis::HMILevel;
  AppStopwatchId next_stopwatch = SECONDS_HMI_NONE;
  switch (new_hmi_level) {
    case HMI_FULL:
      next_stopwatch = SECONDS_HMI_FULL;
      break;
    case HMI_LIMITED:
      next_stopwatch = SECONDS_HMI_LIMITED;
      break;
    case HMI_BACKGROUND:
      next_stopwatch = SECONDS_HMI_BACKGROUND;
      break;
    case HMI_NONE:
      next_stopwatch = SECONDS_HMI_NONE;
      break;
    default:
      NOTREACHED();
  }
  time_in_hmi_state_sptr_->Switch(next_stopwatch);
}

void UsageStatistics::RecordAppRegistrationGuiLanguage(
    Language::eType gui_language) {
  app_registration_language_gui_.Update(LanguageIdToString(gui_language));
}

void UsageStatistics::RecordAppRegistrationVuiLanguage(
    Language::eType vui_language) {
  app_registration_language_gui_.Update(LanguageIdToString(vui_language));
}

void UsageStatistics::RecordRpcSentInHMINone() {
  ++count_of_rpcs_sent_in_hmi_none_;
}

void UsageStatistics::RecordPolicyRejectedRpcCall() {
  ++count_of_rejected_rpc_calls_;
}

void UsageStatistics::RecordAppUserSelection() {
  ++count_of_user_selections_;
}

void UsageStatistics::RecordRunAttemptsWhileRevoked() {
  ++count_of_run_attempts_while_revoked_;
}

void UsageStatistics::RecordRemovalsForBadBehavior() {
  ++count_of_removals_for_bad_behavior_;
}

void UsageStatistics::RecordTLSError() {
  ++count_of_tls_error_;
}

}  // namespace application_manager
