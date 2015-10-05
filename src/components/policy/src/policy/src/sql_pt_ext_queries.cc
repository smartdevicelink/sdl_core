/*
 Copyright (c) 2013, " Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, " with or without
 modification, " are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, " this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice, "
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, " INCLUDING, " BUT NOT LIMITED TO, " THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, " INDIRECT, " INCIDENTAL, " SPECIAL, " EXEMPLARY, " OR
 CONSEQUENTIAL DAMAGES (INCLUDING, " BUT NOT LIMITED TO, " PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, " DATA, " OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, " WHETHER IN
 CONTRACT, " STRICT LIABILITY, " OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, " EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "policy/sql_pt_ext_queries.h"

namespace policy {
namespace sql_pt_ext {

const std::string kSelectKeepContext =
  "SELECT `keep_context` FROM `application` WHERE `id` = ? LIMIT 1";

const std::string kSelectStealFocus =
  "SELECT `steal_focus` FROM `application` WHERE `id` = ? LIMIT 1";

const std::string kSelectDefaultHmi =
  "SELECT `default_hmi` FROM `application` WHERE `id` = ? LIMIT 1";

const std::string kResetDeviceConsents = "DELETE FROM `device_consent_group`";

const std::string kResetAppConsents = "DELETE FROM `consent_group`";

const std::string kCountDeviceConsentGroup = "SELECT COUNT (`device_id`) "
    "FROM `device_consent_group` WHERE `device_id` = ?";

const std::string kCountDevice = "SELECT COUNT (`id`) "
                                 "FROM `device` WHERE `id` = ?";

const std::string kSelectDeviceConsentedGroup =
  "SELECT * FROM `device_consent_group` WHERE `device_id` = ?";

const std::string kUpdateDeviceConsentedGroup =
  "UPDATE `device_consent_group` SET `is_consented` = ?, `input` = ? WHERE "
  "(`device_id` = ? AND `functional_group_id` = ?)";

const std::string kUpdateDevice =
  "UPDATE `device` SET `hardware` = ?, `firmware_rev` = ?, `os` = ?, "
  "`os_version` = ?, `carrier` = ?, `max_number_rfcom_ports` = ?, "
  " `connection_type` = ? WHERE `id` = ? ";

const std::string kInsertDeviceConsentedGroup =
  "INSERT OR REPLACE INTO `device_consent_group` "
  "(`device_id`, `functional_group_id`, `is_consented`, `input`, `time_stamp`) "
  "VALUES (?,?,?,?,?)";

const std::string kInsertDevice =
  "INSERT OR IGNORE INTO `device` "
  "(`id`, `hardware`, `firmware_rev`, `os`, `os_version`, `carrier`,"
    "`max_number_rfcom_ports`, `connection_type`) "
  "VALUES (?,?,?,?,?,?,?,?)";

const std::string kSelectDeviceData = "SELECT * FROM `device`";

const std::string kSelectConsentGroup =
  "SELECT * FROM `consent_group` WHERE `device_id` = ? ";

const std::string kInsertPreconsentedGroups =
  "INSERT INTO `preconsented_group` (`application_id`, `functional_group_id`)"
  "  SELECT ?, `id` FROM `functional_group` WHERE `name` = ? LIMIT 1";

const std::string kSelectPreconsentedGroups =
  "SELECT `f`.`name` FROM `preconsented_group` AS `p`"
  "  LEFT JOIN `functional_group` AS `f` "
  "    ON (`f`.`id` = `p`.`functional_group_id`)"
  "  WHERE `p`.`application_id` = ?";

const std::string kDeletePreconsentedGroups = "DELETE FROM `preconsented_group`";

const std::string kSelectUsageAndErrorCount =
  "SELECT `count_of_iap_buffer_full`, `count_sync_out_of_memory`, "
  "  `count_of_sync_reboots` "
  "FROM `usage_and_error_count` LIMIT 1";

const std::string kSelectAppLevels =
  "SELECT `application_id`, `minutes_in_hmi_full`, `minutes_in_hmi_limited`, "
  "  `minutes_in_hmi_background`, `minutes_in_hmi_none`, "
  "  `count_of_user_selections`, "
  "  `count_of_rejections_sync_out_of_memory`, "
  "  `count_of_rejections_nickname_mismatch`, "
  "  `count_of_rejections_duplicate_name`, "
  "  `count_of_rejected_rpcs_calls`, "
  "  `count_of_rpcs_sent_in_hmi_none`, "
  "  `count_of_removals_for_bad_behavior`, "
  "  `count_of_run_attempts_while_revoked`, "
  "  `app_registration_language_gui`, "
  "  `app_registration_language_vui`, "
  "  `count_of_tls_errors` "
  "FROM `app_level`";

const std::string kUpdateGlobalCounters = "UPDATE `usage_and_error_count` SET "
    "`count_of_iap_buffer_full` = ?, "
    "`count_sync_out_of_memory` = ?, "
    "`count_of_sync_reboots` = ? ";

const std::string kInsertDeviceData =
  "INSERT OR IGNORE INTO `device` "
  "(`id`, `hardware`, `firmware_rev`, `os`, `os_version`, `carrier`, "
  "`max_number_rfcom_ports`,`connection_type`) VALUES (?,?,?,?,?,?,?,?) ";

const std::string kInsertConsentGroups =
  "INSERT OR REPLACE INTO `consent_group` "
  "(`device_id`, `application_id`, `functional_group_id`, `is_consented`, `input`, `time_stamp`) "
  "VALUES (?,?,?,?,?,?)";

const std::string kDeleteAppGroupConsent = "DELETE FROM `consent_group` WHERE "
  "`application_id` = ? AND `functional_group_id` = ? ";

 const std::string kSelectGroupId =
     "SELECT `id` FROM `functional_group` WHERE `name` = ? ";

const std::string kCountUnconsentedGroups =
  "SELECT COUNT(`a`.`functional_group_id`) FROM `app_group` AS `a` "
  " WHERE `a`.`application_id` = ?  AND NOT EXISTS "
  " (SELECT NULL FROM `preconsented_group` AS `p` WHERE "
  " (`p`.`functional_group_id` = `a`.`functional_group_id` AND "
  " `p`.`application_id` = `a`.`application_id`)) "
  " AND NOT EXISTS (SELECT NULL FROM `consent_group` AS `c` "
  " WHERE (`c`.`application_id` = `a`.`application_id` "
  " AND `c`.`functional_group_id` = `a`.`functional_group_id` "
  " AND `c`.`device_id` = ?)) AND NOT EXISTS "
  " (SELECT NULL FROM `app_group` AS `def` WHERE "
  " (`def`.`application_id` = ? OR "
  " `def`.`application_id` = ?) "
  " AND `def`.`functional_group_id` = `a`.`functional_group_id`)"
  " AND NOT EXISTS (SELECT NULL FROM `functional_group` AS `f` "
  " WHERE (`a`.`functional_group_id` = `f`.`id`"
  " AND`f`.`user_consent_prompt` IS NULL))";

const std::string kSelectModuleMeta = "SELECT* FROM `module_meta`";

const std::string kUpdateMetaParams = "UPDATE `module_meta` SET "
                                      "`ccpu_version` = ?, `wers_country_code` = ?, `language` = ? ";

const std::string kUpdateModuleMetaVinParam =
    "UPDATE `module_meta` SET `vin` = ? ";

const std::string kSaveModuleMeta =
    "UPDATE `module_meta` SET `ccpu_version` = ?, `language` = ?,"
    "`wers_country_code` = ?, `pt_exchanged_at_odometer_x` = ?,"
    "`pt_exchanged_x_days_after_epoch` = ?,"
    "`ignition_cycles_since_last_exchange` = ?, `vin` = ?";

const std::string kSelectMetaParams = "SELECT `ccpu_version`, "
                   "`wers_country_code`, `language` from `module_meta`";

const std::string kUpdateMetaLanguage = "UPDATE `module_meta` SET `language` = ? ";

const std::string kCountAppLevel =
  "SELECT COUNT(`application_id`) FROM `app_level`"
  " WHERE `application_id` = ? ";

const std::string kUpdateGroupPermissions =
  "UPDATE `consent_group` "
  "SET `is_consented` = ?, `input` = ? "
  "WHERE(`application_id` = ? AND `functional_group_id` = ? AND `device_id` = ?) ";

const std::string kInsertApplication =
  "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, `steal_focus`, "
  " `default_hmi`, `priority_value`, `is_revoked`, `memory_kb`, "
  " `heart_beat_timeout_ms`, `certificate`) VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?) ";

const std::string kCollectFriendlyMsg = "SELECT * FROM `message`";

const std::string kSelectFriendlyMsg =
  "SELECT `tts`, `label`, `line1`, `line2`, `textBody` FROM `message` "
  "WHERE `message_type_name` = ? AND `language_code` = ? LIMIT 1";

const std::string kSelectAppGroupsId = "SELECT `functional_group_id` "
                                       "FROM `app_group` WHERE `application_id` = ? ";

const std::string kSelectConsentedGroupsId =
  "SELECT `functional_group_id`, `is_consented` "
  "FROM `consent_group` WHERE(`application_id` = ? AND `device_id` = ?) ";

const std::string kCountAppConsents = "SELECT COUNT(*) from `consent_group`"
                                      "WHERE(`device_id` = ? AND `application_id` = ? AND "
                                      "`functional_group_id` = ?) ";

const std::string kSelectPreconsentedGroupsId = "SELECT `functional_group_id` "
    "FROM `preconsented_group` WHERE `application_id` = ? ";

const std::string kSelectAppPolicies =
  "SELECT `id`, `priority_value`, `default_hmi`, `keep_context`, `steal_focus`, "
  " `memory_kb`, `heart_beat_timeout_ms`, `certificate` FROM `application`";

const std::string kSelectFunctionalGroupNames = "SELECT `id`, `user_consent_prompt`, `name`"
    " FROM `functional_group`";

const std::string kDeleteDeviceConsent = "DELETE FROM `device_consent_group` "
    "WHERE `device_id` = ? ";

const std::string kDeleteAppConsent = "DELETE FROM `consent_group` "
                                      "WHERE `device_id` = ? ";

const std::string kSelectApplicationIsPreData =
  "SELECT `is_predata` FROM `application` WHERE `id` = ? ";

const std::string kUpdateIsPredata =
  "UPDATE `application` SET `is_predata` = ? WHERE `id` = ? ";

const std::string kHasAppPreloadedGroups =
  "SELECT COUNT(`a1`.`functional_group_id`) FROM `app_group` "
  " AS `a1` JOIN `app_group` AS `a2` "
  " ON `a1`.`functional_group_id` = `a2`.`functional_group_id` "
  " WHERE `a1`.`application_id` = ? AND `a2`.`application_id` = ? ";

const std::string kUpdateUnpairedDevice =
  "UPDATE `device` SET `unpaired` = ? WHERE `id` = ? ";

const std::string kSelectUnpairedDevices =
  "SELECT `id` FROM `device` WHERE `unpaired` = 1";

const std::string kHasMsgLanguageCode = "SELECT COUNT (`id`) FROM message "
        "WHERE `message_type_name` = ? AND `language_code` = ? ";

const std::string kDeletePreconsentedGroupsByApplicationId =
  "DELETE FROM `preconsented_group` WHERE `application_id` = ?";

}  // namespace sql_pt_ext
}  // namespace policy
