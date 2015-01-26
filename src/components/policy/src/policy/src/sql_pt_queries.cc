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

#include "policy/sql_pt_queries.h"

namespace policy {
namespace sql_pt {

const std::string kSelectPriority =
    "SELECT `priority_value` FROM `application` WHERE `id` = ? LIMIT 1";
const std::string kCreateSchema =
  "BEGIN; "
  "CREATE TABLE IF NOT EXISTS `device`( "
  "  `id` VARCHAR(100) PRIMARY KEY NOT NULL, "
  "  `hardware` VARCHAR(45), "
  "  `firmware_rev` VARCHAR(45), "
  "  `os` VARCHAR(45), "
  "  `os_version` VARCHAR(45), "
  "  `carrier` VARCHAR(45), "
  "  `max_number_rfcom_ports` INTEGER,"
  "  `connection_type` VARCHAR(45), "
  "  `unpaired` BOOL "
  "); "
  "CREATE TABLE IF NOT EXISTS `usage_and_error_count`( "
  "  `count_of_iap_buffer_full` INTEGER, "
  "  `count_sync_out_of_memory` INTEGER, "
  "  `count_of_sync_reboots` INTEGER "
  "); "
  "CREATE TABLE IF NOT EXISTS `module_meta`( "
  "  `ccpu_version` VARCHAR(45), "
  "  `language` VARCHAR(45), "
  "  `wers_country_code` VARCHAR(45), "
  "  `pt_exchanged_at_odometer_x` INTEGER NOT NULL DEFAULT 0, "
  "  `pt_exchanged_x_days_after_epoch` INTEGER NOT NULL DEFAULT 0, "
  "  `ignition_cycles_since_last_exchange` INTEGER NOT NULL DEFAULT 0, "
  "  `vin` VARCHAR(45),"
  "  `flag_update_required` BOOL NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `module_config`( "
  "  `preloaded_pt` BOOL NOT NULL, "
  "  `exchange_after_x_ignition_cycles` INTEGER NOT NULL, "
  "  `exchange_after_x_kilometers` INTEGER NOT NULL, "
  "  `exchange_after_x_days` INTEGER NOT NULL, "
  "  `timeout_after_x_seconds` INTEGER NOT NULL, "
  "  `vehicle_make` VARCHAR(45), "
  "  `vehicle_model` VARCHAR(45), "
  "  `vehicle_year` VARCHAR(4) "
  "); "
  "CREATE TABLE IF NOT EXISTS `functional_group`( "
  "  `id` INTEGER PRIMARY KEY NOT NULL, "
  "  `user_consent_prompt` TEXT UNIQUE ON CONFLICT REPLACE, "
  "  `name` VARCHAR(100) NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `priority`( "
  "  `value` VARCHAR(45) PRIMARY KEY NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `hmi_level`( "
  "  `value` VARCHAR(45) PRIMARY KEY NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `notifications_by_priority`( "
  "  `priority_value` VARCHAR(45) PRIMARY KEY NOT NULL, "
  "  `value` INTEGER NOT NULL, "
  "  CONSTRAINT `fk_notifications_by_priority_priority1` "
  "    FOREIGN KEY(`priority_value`) "
  "    REFERENCES `priority`(`value`) "
  "); "
  "CREATE INDEX IF NOT EXISTS "
  "`notifications_by_priority.fk_notifications_by_priority_priority1_idx` "
  "  ON `notifications_by_priority`(`priority_value`); "
  "CREATE TABLE IF NOT EXISTS `language`( "
  "  `code` VARCHAR(25) PRIMARY KEY NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `message_type`( "
  "  `name` VARCHAR(45) PRIMARY KEY NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `version`( "
  "  `number` VARCHAR(45) NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `rpc`( "
  "  `id` INTEGER PRIMARY KEY NOT NULL, "
  "  `name` VARCHAR(45) NOT NULL, "
  "  `parameter` VARCHAR(45), "
  "  `hmi_level_value` VARCHAR(45) NOT NULL, "
  "  `functional_group_id` INTEGER NOT NULL, "
  "  CONSTRAINT `fk_rpc_hmi_level1` "
  "    FOREIGN KEY(`hmi_level_value`) "
  "    REFERENCES `hmi_level`(`value`), "
  "  CONSTRAINT `fk_rpc_functional_group1` "
  "    FOREIGN KEY(`functional_group_id`) "
  "    REFERENCES `functional_group`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `rpc.fk_rpc_hmi_level1_idx` "
  "  ON `rpc`(`hmi_level_value`); "
  "CREATE INDEX IF NOT EXISTS `rpc.fk_rpc_functional_group1_idx` "
  "  ON `rpc`(`functional_group_id`); "
  "CREATE INDEX `rpc.select_rpc_name_hmi_level` "
  "  ON `rpc`(`name`,`hmi_level_value`);"
  "CREATE TABLE IF NOT EXISTS `application`( "
  "  `id` VARCHAR(45) PRIMARY KEY NOT NULL, "
  "  `keep_context` BOOLEAN, "
  "  `steal_focus` BOOLEAN, "
  "  `default_hmi` VARCHAR(45), "
  "  `priority_value` VARCHAR(45), "
  "  `is_revoked` BOOLEAN, "
  "  `is_default` BOOLEAN, "
  "  `is_predata` BOOLEAN, "
  "  `memory_kb` INTEGER NOT NULL, "
  "  `heart_beat_timeout_ms` INTEGER NOT NULL, "
  "  `certificate` VARCHAR(45), "
  "  CONSTRAINT `fk_application_hmi_level1` "
  "    FOREIGN KEY(`default_hmi`) "
  "    REFERENCES `hmi_level`(`value`), "
  "  CONSTRAINT `fk_application_priorities1` "
  "    FOREIGN KEY(`priority_value`) "
  "    REFERENCES `priority`(`value`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `application.fk_application_hmi_level1_idx` "
  "  ON `application`(`default_hmi`); "
  "CREATE INDEX IF NOT EXISTS `application.fk_application_priorities1_idx` "
  "  ON `application`(`priority_value`); "
  "CREATE TABLE IF NOT EXISTS `app_group`( "
  "  `application_id` VARCHAR(45) NOT NULL, "
  "  `functional_group_id` INTEGER NOT NULL, "
  "  PRIMARY KEY(`application_id`,`functional_group_id`), "
  "  CONSTRAINT `fk_application_has_functional_group_application1` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`), "
  "  CONSTRAINT `fk_application_has_functional_group_functional_group1` "
  "    FOREIGN KEY(`functional_group_id`) "
  "    REFERENCES `functional_group`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `app_group.fk_application_has_functional_group_functional_group1_idx` "
  "  ON `app_group`(`functional_group_id`); "
  "CREATE INDEX IF NOT EXISTS `app_group.fk_application_has_functional_group_application1_idx` "
  "  ON `app_group`(`application_id`); "
  "CREATE TABLE IF NOT EXISTS `preconsented_group`( "
  "  `application_id` VARCHAR(45) NOT NULL, "
  "  `functional_group_id` INTEGER NOT NULL, "
  "  PRIMARY KEY(`application_id`,`functional_group_id`), "
  "  CONSTRAINT `fk_application_has_functional_group_application2` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`), "
  "  CONSTRAINT `fk_application_has_functional_group_functional_group2` "
  "    FOREIGN KEY(`functional_group_id`) "
  "    REFERENCES `functional_group`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS "
  "`preconsented_group.fk_application_has_functional_group_functional_group2_idx` "
  "  ON `preconsented_group`(`functional_group_id`); "
  "CREATE INDEX IF NOT EXISTS "
  "`preconsented_group.fk_application_has_functional_group_application2_idx` "
  "  ON `preconsented_group`(`application_id`); "
  "CREATE TABLE IF NOT EXISTS `seconds_between_retry`( "
  "  `index` INTEGER PRIMARY KEY NOT NULL, "
  "  `value` INTEGER NOT NULL "
  "); "
  "CREATE TABLE IF NOT EXISTS `device_consent_group`( "
  "  `device_id` VARCHAR(100) NOT NULL, "
  "  `functional_group_id` INTEGER NOT NULL, "
  "  `is_consented` BOOL NOT NULL, "
  "  `input` VARCHAR(45), "
  "  `time_stamp` DATETIME DEFAULT CURRENT_TIMESTAMP, "
  "  PRIMARY KEY(`device_id`,`functional_group_id`), "
  "  CONSTRAINT `fk_device_has_functional_group_device1` "
  "    FOREIGN KEY(`device_id`) "
  "    REFERENCES `device`(`id`), "
  "  CONSTRAINT `fk_device_has_functional_group_functional_group1` "
  "    FOREIGN KEY(`functional_group_id`) "
  "    REFERENCES `functional_group`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS "
  "`device_consent_group.fk_device_has_functional_group_functional_group1_idx` "
  "  ON `device_consent_group`(`functional_group_id`); "
  "CREATE INDEX IF NOT EXISTS "
  "`device_consent_group.fk_device_has_functional_group_device1_idx` "
  "  ON `device_consent_group`(`device_id`); "
  "CREATE TABLE IF NOT EXISTS `app_level`( "
  "  `application_id` VARCHAR(45) PRIMARY KEY NOT NULL, "
  "  `minutes_in_hmi_full` INTEGER DEFAULT 0, "
  "  `minutes_in_hmi_limited` INTEGER DEFAULT 0, "
  "  `minutes_in_hmi_background` INTEGER DEFAULT 0, "
  "  `minutes_in_hmi_none` INTEGER DEFAULT 0, "
  "  `count_of_user_selections` INTEGER DEFAULT 0, "
  "  `count_of_rejections_sync_out_of_memory` INTEGER DEFAULT 0, "
  "  `count_of_rejections_nickname_mismatch` INTEGER DEFAULT 0, "
  "  `count_of_rejections_duplicate_name` INTEGER DEFAULT 0, "
  "  `count_of_rejected_rpcs_calls` INTEGER DEFAULT 0, "
  "  `count_of_rpcs_sent_in_hmi_none` INTEGER DEFAULT 0, "
  "  `count_of_removals_for_bad_behavior` INTEGER DEFAULT 0, "
  "  `count_of_run_attempts_while_revoked` INTEGER DEFAULT 0, "
  "  `app_registration_language_gui` VARCHAR(25), "
  "  `app_registration_language_vui` VARCHAR(25), "
  "  CONSTRAINT `fk_app_levels_application1` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`), "
  "  CONSTRAINT `fk_app_level_language1` "
  "    FOREIGN KEY(`app_registration_language_gui`) "
  "    REFERENCES `language`(`code`), "
  "  CONSTRAINT `fk_app_level_language2` "
  "    FOREIGN KEY(`app_registration_language_vui`) "
  "    REFERENCES `language`(`code`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `app_level.fk_app_levels_application1_idx` "
  "  ON `app_level`(`application_id`); "
  "CREATE INDEX IF NOT EXISTS `app_level.fk_app_level_language1_idx` "
  "  ON `app_level`(`app_registration_language_gui`); "
  "CREATE INDEX IF NOT EXISTS `app_level.fk_app_level_language2_idx` "
  "  ON `app_level`(`app_registration_language_vui`); "
  "CREATE TABLE IF NOT EXISTS `nickname`( "
  "  `name` VARCHAR(100) NOT NULL, "
  "  `application_id` VARCHAR(45) NOT NULL, "
  "  PRIMARY KEY(`name`,`application_id`), "
  "  CONSTRAINT `fk_nickname_application1` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `nickname.fk_nickname_application1_idx` "
  "  ON `nickname`(`application_id`); "
  "CREATE TABLE IF NOT EXISTS `app_type`( "
  "  `name` VARCHAR(50) NOT NULL, "
  "  `application_id` VARCHAR(45) NOT NULL, "
  "  PRIMARY KEY(`name`,`application_id`), "
  "  CONSTRAINT `fk_app_type_application1` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `app_type.fk_app_type_application1_idx` "
  "  ON `app_type`(`application_id`); "
  "CREATE TABLE IF NOT EXISTS `consent_group`( "
  "  `device_id` VARCHAR(100) NOT NULL, "
  "  `application_id` VARCHAR(45) NOT NULL, "
  "  `functional_group_id` INTEGER NOT NULL, "
  "  `is_consented` BOOL NOT NULL, "
  "  `input` VARCHAR(45), "
  "  `time_stamp` DATETIME DEFAULT CURRENT_TIMESTAMP, "
  "  PRIMARY KEY(`application_id`,`functional_group_id`,`device_id`), "
  "  CONSTRAINT `fk_consent_group_device1` "
  "    FOREIGN KEY(`device_id`) "
  "    REFERENCES `device`(`id`), "
  "  CONSTRAINT `fk_consent_group_application1` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`), "
  "  CONSTRAINT `fk_consent_group_functional_group1` "
  "    FOREIGN KEY(`functional_group_id`) "
  "    REFERENCES `functional_group`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS "
  "`consent_group.fk_consent_group_device1_idx` "
  "  ON `device_consent_group`(`device_id`); "
  "CREATE INDEX IF NOT EXISTS `consent_group.fk_consent_group_functional_group1_idx` "
  "  ON `consent_group`(`functional_group_id`); "
  "CREATE TABLE IF NOT EXISTS `endpoint`( "
  "  `service` INTEGER NOT NULL, "
  "  `url` VARCHAR(100) NOT NULL, "
  "  `application_id` VARCHAR(45) NOT NULL, "
  "  CONSTRAINT `fk_endpoint_application1` "
  "    FOREIGN KEY(`application_id`) "
  "    REFERENCES `application`(`id`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `endpoint.fk_endpoint_application1_idx` "
  "  ON `endpoint`(`application_id`); "
  "CREATE TABLE IF NOT EXISTS `message`( "
  "  `id` INTEGER PRIMARY KEY NOT NULL, "
  "  `tts` TEXT, "
  "  `label` TEXT, "
  "  `line1` TEXT, "
  "  `line2` TEXT, "
  "  `textBody` TEXT, "
  "  `language_code` VARCHAR(25) NOT NULL, "
  "  `message_type_name` VARCHAR(45) NOT NULL, "
  "  CONSTRAINT `fk_messages_languages1` "
  "    FOREIGN KEY(`language_code`) "
  "    REFERENCES `language`(`code`), "
  "  CONSTRAINT `fk_message_consumer_friendly_messages1` "
  "    FOREIGN KEY(`message_type_name`) "
  "    REFERENCES `message_type`(`name`) "
  "); "
  "CREATE INDEX IF NOT EXISTS `message.fk_messages_languages1_idx` "
  "  ON `message`(`language_code`);"
  "CREATE INDEX IF NOT EXISTS `message.fk_message_consumer_friendly_messages1_idx` "
  "  ON `message`(`message_type_name`);"
  "COMMIT;";

const std::string kInsertInitData =
  "INSERT OR IGNORE INTO `usage_and_error_count` ( "
  "  `count_of_iap_buffer_full`, `count_sync_out_of_memory`, "
  "  `count_of_sync_reboots`) VALUES (0, 0, 0); "
  "INSERT OR IGNORE INTO `module_meta` (`pt_exchanged_at_odometer_x`, "
  "  `pt_exchanged_x_days_after_epoch`, `ignition_cycles_since_last_exchange`,"
  "  `flag_update_required`) "
  "  VALUES (0, 0, 0, 0); "
  "INSERT OR IGNORE INTO `module_config` (`preloaded_pt`, "
  "  `exchange_after_x_ignition_cycles`, `exchange_after_x_kilometers`, "
  "  `exchange_after_x_days`, `timeout_after_x_seconds`) "
  "  VALUES(1, 0, 0, 0, 0); "
  "INSERT OR IGNORE INTO `priority`(`value`) VALUES ('EMERGENCY'); "
  "INSERT OR IGNORE INTO `priority`(`value`) VALUES ('NAVIGATION'); "
  "INSERT OR IGNORE INTO `priority`(`value`) VALUES ('VOICECOMMUNICATION'); "
  "INSERT OR IGNORE INTO `priority`(`value`) VALUES ('COMMUNICATION'); "
  "INSERT OR IGNORE INTO `priority`(`value`) VALUES ('NORMAL'); "
  "INSERT OR IGNORE INTO `priority`(`value`) VALUES ('NONE'); "
  "INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('FULL'); "
  "INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('LIMITED'); "
  "INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('BACKGROUND'); "
  "INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('NONE'); "
  "INSERT OR IGNORE INTO `version` (`number`) VALUES('0'); "
  "";

const std::string kDropSchema =
  "BEGIN; "
  "DROP INDEX IF EXISTS `message.fk_messages_languages1_idx`; "
  "DROP INDEX IF EXISTS `message.fk_message_consumer_friendly_messages1_idx`; "
  "DROP TABLE IF EXISTS `message`; "
  "DROP INDEX IF EXISTS `endpoint.fk_endpoint_application1_idx`; "
  "DROP TABLE IF EXISTS `endpoint`; "
  "DROP INDEX IF EXISTS `consent_group.fk_consent_group_device1_idx`; "
  "DROP INDEX IF EXISTS `consent_group.fk_consent_group_functional_group1_idx`; "
  "DROP TABLE IF EXISTS `consent_group`; "
  "DROP INDEX IF EXISTS `app_type.fk_app_type_application1_idx`; "
  "DROP TABLE IF EXISTS `app_type`; "
  "DROP INDEX IF EXISTS `nickname.fk_nickname_application1_idx`; "
  "DROP TABLE IF EXISTS `nickname`; "
  "DROP INDEX IF EXISTS `app_level.fk_app_level_language2_idx`; "
  "DROP INDEX IF EXISTS `app_level.fk_app_level_language1_idx`; "
  "DROP INDEX IF EXISTS `app_level.fk_app_levels_application1_idx`; "
  "DROP TABLE IF EXISTS `app_level`; "
  "DROP INDEX IF EXISTS `device_consent_group.fk_device_has_functional_group_device1_idx`; "
  "DROP INDEX IF EXISTS `device_consent_group.fk_device_has_functional_group_functional_group1_idx`; "
  "DROP TABLE IF EXISTS `device_consent_group`; "
  "DROP TABLE IF EXISTS `seconds_between_retry`; "
  "DROP INDEX IF EXISTS `preconsented_group.fk_application_has_functional_group_application2_idx`; "
  "DROP INDEX IF EXISTS `preconsented_group.fk_application_has_functional_group_functional_group2_idx`; "
  "DROP TABLE IF EXISTS `preconsented_group`; "
  "DROP INDEX IF EXISTS `app_group.fk_application_has_functional_group_application1_idx`; "
  "DROP INDEX IF EXISTS `app_group.fk_application_has_functional_group_functional_group1_idx`; "
  "DROP TABLE IF EXISTS `app_group`; "
  "DROP INDEX IF EXISTS `application.fk_application_priorities1_idx`; "
  "DROP INDEX IF EXISTS `application.fk_application_hmi_level1_idx`; "
  "DROP TABLE IF EXISTS `application`; "
  "DROP INDEX IF EXISTS `rpc.select_rpc_name_hmi_level`; "
  "DROP INDEX IF EXISTS `rpc.fk_rpc_functional_group1_idx`; "
  "DROP INDEX IF EXISTS `rpc.fk_rpc_hmi_level1_idx`; "
  "DROP TABLE IF EXISTS `rpc`; "
  "DROP TABLE IF EXISTS `version`; "
  "DROP TABLE IF EXISTS `message_type`; "
  "DROP TABLE IF EXISTS `language`; "
  "DROP INDEX IF EXISTS `notifications_by_priority.fk_notifications_by_priority_priority1_idx`; "
  "DROP TABLE IF EXISTS `notifications_by_priority`; "
  "DROP TABLE IF EXISTS `hmi_level`; "
  "DROP TABLE IF EXISTS `priority`; "
  "DROP TABLE IF EXISTS `functional_group`; "
  "DROP TABLE IF EXISTS `module_config`; "
  "DROP TABLE IF EXISTS `module_meta`; "
  "DROP TABLE IF EXISTS `usage_and_error_count`; "
  "DROP TABLE IF EXISTS `device`; "
  "COMMIT; "
  "VACUUM;";

const std::string kDeleteData =
  "BEGIN; "
  "DELETE FROM `message`; "
  "DELETE FROM `endpoint`; "
  "DELETE FROM `consent_group`; "
  "DELETE FROM `app_type`; "
  "DELETE FROM `nickname`; "
  "DELETE FROM `app_level`; "
  "DELETE FROM `device_consent_group`; "
  "DELETE FROM `seconds_between_retry`; "
  "DELETE FROM `preconsented_group`; "
  "DELETE FROM `app_group`; "
  "DELETE FROM `application`; "
  "DELETE FROM `rpc`; "
  "DELETE FROM `version`; "
  "DELETE FROM `message_type`; "
  "DELETE FROM `language`; "
  "DELETE FROM `notifications_by_priority`; "
  "DELETE FROM `hmi_level`; "
  "DELETE FROM `priority`; "
  "DELETE FROM `functional_group`; "
  "DELETE FROM `module_config`; "
  "DELETE FROM `module_meta`; "
  "DELETE FROM `usage_and_error_count`; "
  "DELETE FROM `device`; "
  "COMMIT; "
  "VACUUM;";

const std::string kCheckDBIntegrity = "PRAGMA integrity_check";

const std::string kCheckPgNumber = "PRAGMA page_count";

const std::string kSelectRpc =
  "SELECT DISTINCT `rpc`.`parameter` FROM `rpc` "
  "  JOIN `app_group` AS `g` ON (`g`.`functional_group_id` = `rpc`.`functional_group_id` "
  "    AND (`g`.`application_id` = ?)) "
  "WHERE `rpc`.`hmi_level_value` = ? AND `rpc`.`name` = ?";

const std::string kSelectPreloaded =
  "SELECT `preloaded_pt` FROM `module_config` "
  "WHERE `preloaded_pt` = 1 LIMIT 1";

const std::string kUpdatePreloaded =
  "UPDATE `module_config` SET `preloaded_pt` = ?";

const std::string kIsFirstRun =
  "SELECT `is_first_run` FROM `module_config` ";

const std::string kSetNotFirstRun =
  "UPDATE `module_config` SET `is_first_run`= 0 ";

const std::string kSelectEndpoint =
  "SELECT `url`, `application_id` FROM `endpoint` WHERE `service` = ? ";

const std::string kInsertFunctionalGroup =
  "INSERT INTO `functional_group` (`id`, `name`, `user_consent_prompt`) "
  "  VALUES (?, ?, ?)";

const std::string kInsertRpc =
  "INSERT INTO `rpc` (`name`, `hmi_level_value`, `functional_group_id`) "
  "  VALUES (?, ?, ?)";

const std::string kInsertRpcWithParameter =
  "INSERT INTO `rpc` (`name`, `hmi_level_value`, `parameter`, `functional_group_id`) "
  "  VALUES (?, ?, ?, ?)";

const std::string kInsertApplication =
  "INSERT OR IGNORE INTO `application` (`id`, `priority_value`, `is_revoked`, `memory_kb`,"
  " `heart_beat_timeout_ms`, `certificate`) VALUES (?,?,?,?,?,?)";

const std::string kInsertAppGroup =
  "INSERT INTO `app_group` (`application_id`, `functional_group_id`)"
  "  SELECT ?, `id` FROM `functional_group` WHERE `name` = ? LIMIT 1";

const std::string kInsertNickname =
  "INSERT OR IGNORE INTO `nickname` (`application_id`, `name`) VALUES (?, ?)";

const std::string kInsertAppType =
  "INSERT OR IGNORE INTO `app_type` (`application_id`, `name`) VALUES (?, ?)";

const std::string kUpdateVersion = "UPDATE `version` SET `number`= ?";

const std::string kInsertMessageType =
  "INSERT OR IGNORE INTO `message_type` (`name`) VALUES (?)";

const std::string kInsertLanguage =
  "INSERT OR IGNORE INTO `language` (`code`) VALUES (?)";

const std::string kInsertMessageString =
  "INSERT INTO `message` (`tts`, `label`, `line1`, `line2`, `language_code`, "
  "  `message_type_name`, `textBody`) VALUES (?, ?, ?, ?, ?, ?, ?)";

const std::string kUpdateModuleConfig =
  "UPDATE `module_config` SET `preloaded_pt` = ?, "
  "  `exchange_after_x_ignition_cycles` = ?,"
  "  `exchange_after_x_kilometers` = ?, `exchange_after_x_days` = ?, "
  "  `timeout_after_x_seconds` = ?, `vehicle_make` = ?, "
  "  `vehicle_model` = ?, `vehicle_year` = ?";

const std::string kInsertEndpoint =
  "INSERT INTO `endpoint` (`service`, `url`, `application_id`) "
  "  VALUES (?, ?, ?)";

const std::string kInsertSecondsBetweenRetry =
  "INSERT INTO `seconds_between_retry` (`index`, `value`) VALUES (?, ?)";

const std::string kInsertNotificationsByPriority =
  "INSERT OR REPLACE INTO `notifications_by_priority` (`priority_value`, `value`) "
  "  VALUES (?, ?)";

const std::string kInsertDeviceData =
  "INSERT OR IGNORE INTO `device` (`id`) VALUES (?)";

const std::string kInsertAppLevel =
  "INSERT INTO `app_level` (`application_id`, `minutes_in_hmi_full`,"
    "`minutes_in_hmi_limited` ,`minutes_in_hmi_background`,"
    "`minutes_in_hmi_none`,`count_of_user_selections`,"
    "`count_of_rejections_sync_out_of_memory`,"
    "`count_of_rejections_nickname_mismatch`,"
    "`count_of_rejections_duplicate_name`,`count_of_rejected_rpcs_calls`,"
    "`count_of_rpcs_sent_in_hmi_none`,`count_of_removals_for_bad_behavior`,"
    "`count_of_run_attempts_while_revoked`,`app_registration_language_gui`,"
    "`app_registration_language_vui`) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

const std::string kDeleteSecondsBetweenRetries =
  "DELETE FROM `seconds_between_retry`";

const std::string kDeleteEndpoint = "DELETE FROM `endpoint`";

const std::string kDeleteAppLevel = "DELETE FROM `app_level`";

const std::string kDeleteMessageString = "DELETE FROM `message`";

const std::string kDeleteFunctionalGroup = "DELETE FROM `functional_group`";

const std::string kDeleteRpc = "DELETE FROM `rpc`";

const std::string kDeleteAppGroup = "DELETE FROM `app_group`";

const std::string kSelectModuleConfig =
  "SELECT `preloaded_pt`, `exchange_after_x_ignition_cycles`, "
  " `exchange_after_x_kilometers`, `exchange_after_x_days`, "
  " `timeout_after_x_seconds`, `vehicle_make`,"
  " `vehicle_model`, `vehicle_year` "
  " FROM `module_config`";

const std::string kSelectEndpoints =
  "SELECT `url`, `service`, `application_id` "
  "FROM `endpoint` "
  "GROUP BY `application_id`";

const std::string kSelectNotificationsPerMin =
  "SELECT `priority_value`, `value` FROM notifications_by_priority";

const std::string kSelectNotificationsPerPriority =
    "SELECT `value` FROM notifications_by_priority WHERE `priority_value` = ? ";

const std::string kSelectAppLevels = "SELECT `application_id` FROM `app_level`";

const std::string kSelectDeviceData = "SELECT * FROM `device`";

const std::string kSelectFunctionalGroups =
  "SELECT `id`,`name`, `user_consent_prompt` "
  "FROM `functional_group`";

const std::string kSelectAllRpcs =
  "SELECT `name`, `hmi_level_value`, `parameter` "
  "FROM `rpc` WHERE `functional_group_id` = ? ";

const std::string kSelectUserMsgsVersion =
  "SELECT DISTINCT `number` FROM `version`";

const std::string kSelectAppPolicies = "SELECT `id`, `priority_value`, `memory_kb`, "
                                       " `heart_beat_timeout_ms`, `certificate` FROM `application`";

const std::string kSelectAppGroups = "SELECT `f`.`name` FROM `app_group` AS `a`"
                                     "  LEFT JOIN `functional_group` AS `f` "
                                     "    ON (`f`.`id` = `a`.`functional_group_id`)"
                                     "  WHERE `a`.`application_id` = ?";

const std::string kSelectNicknames = "SELECT DISTINCT `name` FROM `nickname` "
                                     "WHERE `application_id` = ?";

const std::string kSelectAppTypes = "SELECT DISTINCT `name` FROM `app_type` "
                                    "WHERE `application_id` = ?";

const std::string kSelectSecondsBetweenRetries =
  "SELECT `value` FROM `seconds_between_retry` ORDER BY `index`";

const std::string kSelectIgnitionCycles =
  "SELECT `c`.`exchange_after_x_ignition_cycles`, "
  "  `m`.`ignition_cycles_since_last_exchange` "
  "  FROM `module_config` AS `c`, `module_meta` AS `m` "
  "LIMIT 1";

const std::string kSelectKilometers =
  "SELECT `c`.`exchange_after_x_kilometers`, "
  "  `m`.`pt_exchanged_at_odometer_x` "
  "  FROM `module_config` AS `c`, `module_meta` AS `m` "
  "LIMIT 1";

const std::string kSelectDays = "SELECT `c`.`exchange_after_x_days`, "
                                "  `m`.`pt_exchanged_x_days_after_epoch` "
                                "  FROM `module_config` AS `c`, `module_meta` AS `m` "
                                "LIMIT 1";

const std::string kIncrementIgnitionCycles =
  "UPDATE `module_meta` SET `ignition_cycles_since_last_exchange` = 1 + "
  "  `ignition_cycles_since_last_exchange`";

const std::string kResetIgnitionCycles =
  "UPDATE `module_meta` SET `ignition_cycles_since_last_exchange` = 0";

const std::string kSelectTimeoutResponse =
  "SELECT `timeout_after_x_seconds` FROM `module_config` LIMIT 1";

const std::string kUpdateFlagUpdateRequired =
  "UPDATE `module_meta` SET `flag_update_required` = ?";

const std::string kSelectFlagUpdateRequired =
  "SELECT `flag_update_required` FROM `module_meta` LIMIT 1";

const std::string kUpdateCountersSuccessfulUpdate =
  "UPDATE `module_meta` SET `pt_exchanged_at_odometer_x` = ?,"
  "`pt_exchanged_x_days_after_epoch` = ?";

const std::string kDeleteApplication = "DELETE FROM `application`";

const std::string kSelectApplicationRevoked =
  "SELECT `is_revoked` FROM `application` WHERE `id` = ?";

const std::string kUpdateApplicationCustomData =
  "UPDATE `application` SET `is_revoked` = ?, `is_default` = ?,"
  "`is_predata` = ? WHERE `id` = ?";

const std::string kSelectApplicationRepresented =
  "SELECT COUNT(`id`) FROM `application` WHERE `id` = ?";

const std::string kSelectApplicationIsDefault =
  "SELECT `is_default` FROM `application` WHERE `id` = ?";

const std::string kUpdateIsDefault =
  "UPDATE `application` SET `is_default` = ? WHERE `id` = ?";

const std::string kDeleteDevice = "DELETE FROM `device` WHERE `id` = ?";

const std::string kDeleteAppGroupByApplicationId =
  "DELETE FROM `app_group` WHERE `application_id` = ?";

const std::string kInsertApplicationFull =
  "INSERT OR IGNORE INTO `application` (`id`, `keep_context`, `steal_focus`, "
  "  `default_hmi`, `priority_value`, `is_revoked`, `is_default`, `is_predata`, "
  " `memory_kb`, `heart_beat_timeout_ms`, `certificate`) "
  "  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

const std::string kSelectApplicationFull =
  "SELECT `keep_context`, `steal_focus`, `default_hmi`, `priority_value`, "
  "  `is_revoked`, `is_default`, `is_predata`, `memory_kb`,"
  "  `heart_beat_timeout_ms`, `certificate` FROM `application` WHERE `id` = ?";

}  // namespace sql_pt
}  // namespace policy

