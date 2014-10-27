BEGIN TRANSACTION; 
  CREATE TABLE IF NOT EXISTS `device`( 
    `id` VARCHAR(100) PRIMARY KEY NOT NULL, 
    `hardware` VARCHAR(45), 
    `firmware_rev` VARCHAR(45), 
    `os` VARCHAR(45), 
    `os_version` VARCHAR(45), 
    `carrier` VARCHAR(45), 
    `max_number_rfcom_ports` INTEGER ,
    `connection_type` VARCHAR(45),
    `unpaired` BOOL 
  ); 
  CREATE TABLE IF NOT EXISTS `usage_and_error_count`( 
    `count_of_iap_buffer_full` INTEGER, 
    `count_sync_out_of_memory` INTEGER, 
    `count_of_sync_reboots` INTEGER 
  ); 
  INSERT OR IGNORE INTO `usage_and_error_count` ( 
    `count_of_iap_buffer_full`, `count_sync_out_of_memory`, 
    `count_of_sync_reboots`) VALUES (0, 0, 0); 
  CREATE TABLE IF NOT EXISTS `module_meta`( 
    `ccpu_version` VARCHAR(45), 
    `language` VARCHAR(45), 
    `wers_country_code` VARCHAR(45), 
    `pt_exchanged_at_odometer_x` INTEGER NOT NULL DEFAULT 0, 
    `pt_exchanged_x_days_after_epoch` INTEGER NOT NULL DEFAULT 0, 
    `ignition_cycles_since_last_exchange` INTEGER NOT NULL DEFAULT 0, 
    `vin` VARCHAR(45),
    `flag_update_required` BOOL NOT NULL 
  ); 
  INSERT OR IGNORE INTO `module_meta` (`pt_exchanged_at_odometer_x`, 
    `pt_exchanged_x_days_after_epoch`, `ignition_cycles_since_last_exchange`,
    `flag_update_required`) 
    VALUES (0, 0, 0, 0); 
  CREATE TABLE IF NOT EXISTS `module_config`( 
    `preloaded_pt` BOOL NOT NULL, 
    `is_first_run` BOOL NOT NULL,
    `exchange_after_x_ignition_cycles` INTEGER NOT NULL, 
    `exchange_after_x_kilometers` INTEGER NOT NULL, 
    `exchange_after_x_days` INTEGER NOT NULL, 
    `timeout_after_x_seconds` INTEGER NOT NULL, 
    `vehicle_make` VARCHAR(45), 
    `vehicle_model` VARCHAR(45), 
    `vehicle_year` VARCHAR(4)  
  ); 
  INSERT OR IGNORE INTO `module_config` (`preloaded_pt`, `is_first_run`,
    `exchange_after_x_ignition_cycles`, `exchange_after_x_kilometers`, 
    `exchange_after_x_days`, `timeout_after_x_seconds`) 
    VALUES(1, 1, 0, 0, 0, 0); 
  CREATE TABLE IF NOT EXISTS `functional_group`( 
    `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
    `user_consent_prompt` TEXT UNIQUE ON CONFLICT REPLACE, 
    `name` VARCHAR(100) NOT NULL 
  ); 
  CREATE TABLE IF NOT EXISTS `priority`( 
    `value` VARCHAR(45) PRIMARY KEY NOT NULL 
  ); 
  INSERT OR IGNORE INTO `priority`(`value`) VALUES ('EMERGENCY');
  INSERT OR IGNORE INTO `priority`(`value`) VALUES ('NAVIGATION');
  INSERT OR IGNORE INTO `priority`(`value`) VALUES ('VOICECOMMUNICATION');
  INSERT OR IGNORE INTO `priority`(`value`) VALUES ('COMMUNICATION');
  INSERT OR IGNORE INTO `priority`(`value`) VALUES ('NORMAL');
  INSERT OR IGNORE INTO `priority`(`value`) VALUES ('NONE');
  CREATE TABLE IF NOT EXISTS `hmi_level`( 
    `value` VARCHAR(45) PRIMARY KEY NOT NULL 
  ); 
  INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('FULL');
  INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('LIMITED');
  INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('BACKGROUND');
  INSERT OR IGNORE INTO `hmi_level`(`value`) VALUES ('NONE');
  CREATE TABLE IF NOT EXISTS `notifications_by_priority`( 
    `priority_value` VARCHAR(45) PRIMARY KEY NOT NULL, 
    `value` INTEGER NOT NULL, 
    CONSTRAINT `fk_notifications_by_priority_priority1` 
      FOREIGN KEY(`priority_value`) 
      REFERENCES `priority`(`value`) 
  ); 
  CREATE INDEX IF NOT EXISTS 
  `notifications_by_priority.fk_notifications_by_priority_priority1_idx` 
    ON `notifications_by_priority`(`priority_value`); 
  CREATE TABLE IF NOT EXISTS `language`( 
    `code` VARCHAR(25) PRIMARY KEY NOT NULL 
  ); 
  CREATE TABLE IF NOT EXISTS `message_type`( 
    `name` VARCHAR(45) PRIMARY KEY NOT NULL 
  ); 
  CREATE TABLE IF NOT EXISTS `version`( 
    `number` VARCHAR(45) NOT NULL 
  ); 
  INSERT OR IGNORE INTO `version` (`number`) VALUES('0'); 
  CREATE TABLE IF NOT EXISTS `rpc`( 
    `id` INTEGER PRIMARY KEY NOT NULL, 
    `name` VARCHAR(45) NOT NULL, 
    `parameter` VARCHAR(45), 
    `hmi_level_value` VARCHAR(45) NOT NULL, 
    `functional_group_id` INTEGER NOT NULL, 
    CONSTRAINT `fk_rpc_hmi_level1` 
      FOREIGN KEY(`hmi_level_value`) 
      REFERENCES `hmi_level`(`value`), 
    CONSTRAINT `fk_rpc_functional_group1` 
      FOREIGN KEY(`functional_group_id`) 
      REFERENCES `functional_group`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS `rpc.fk_rpc_hmi_level1_idx` 
    ON `rpc`(`hmi_level_value`); 
  CREATE INDEX IF NOT EXISTS `rpc.fk_rpc_functional_group1_idx` 
    ON `rpc`(`functional_group_id`); 
  CREATE INDEX `rpc.select_rpc_name_hmi_level` 
    ON `rpc`(`name`,`hmi_level_value`);
  CREATE TABLE IF NOT EXISTS `application`( 
    `id` VARCHAR(45) PRIMARY KEY NOT NULL, 
    `keep_context` BOOLEAN, 
    `steal_focus` BOOLEAN, 
    `default_hmi` VARCHAR(45), 
    `priority_value` VARCHAR(45), 
    `is_revoked` BOOLEAN, 
    `is_default` BOOLEAN, 
    `is_predata` BOOLEAN,
    `memory_kb` INTEGER NOT NULL, 
    `heart_beat_timeout_ms` INTEGER NOT NULL, 
    `certificate` VARCHAR(45), 
    CONSTRAINT `fk_application_hmi_level1` 
      FOREIGN KEY(`default_hmi`) 
      REFERENCES `hmi_level`(`value`), 
    CONSTRAINT `fk_application_priorities1` 
      FOREIGN KEY(`priority_value`) 
      REFERENCES `priority`(`value`) 
  ); 
  CREATE INDEX IF NOT EXISTS `application.fk_application_hmi_level1_idx` 
    ON `application`(`default_hmi`); 
  CREATE INDEX IF NOT EXISTS `application.fk_application_priorities1_idx` 
    ON `application`(`priority_value`); 
  CREATE TABLE IF NOT EXISTS `app_group`( 
    `application_id` VARCHAR(45) NOT NULL, 
    `functional_group_id` INTEGER NOT NULL, 
    PRIMARY KEY(`application_id`,`functional_group_id`), 
    CONSTRAINT `fk_application_has_functional_group_application1` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`), 
    CONSTRAINT `fk_application_has_functional_group_functional_group1` 
      FOREIGN KEY(`functional_group_id`) 
      REFERENCES `functional_group`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS `app_group.fk_application_has_functional_group_functional_group1_idx` 
    ON `app_group`(`functional_group_id`); 
  CREATE INDEX IF NOT EXISTS `app_group.fk_application_has_functional_group_application1_idx` 
    ON `app_group`(`application_id`); 
  CREATE TABLE IF NOT EXISTS `preconsented_group`( 
    `application_id` VARCHAR(45) NOT NULL, 
    `functional_group_id` INTEGER NOT NULL, 
    PRIMARY KEY(`application_id`,`functional_group_id`), 
    CONSTRAINT `fk_application_has_functional_group_application2` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`), 
    CONSTRAINT `fk_application_has_functional_group_functional_group2` 
      FOREIGN KEY(`functional_group_id`) 
      REFERENCES `functional_group`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS 
  `preconsented_group.fk_application_has_functional_group_functional_group2_idx` 
    ON `preconsented_group`(`functional_group_id`); 
  CREATE INDEX IF NOT EXISTS 
  `preconsented_group.fk_application_has_functional_group_application2_idx` 
    ON `preconsented_group`(`application_id`); 
  CREATE TABLE IF NOT EXISTS `seconds_between_retry`( 
    `index` INTEGER PRIMARY KEY NOT NULL, 
    `value` INTEGER NOT NULL 
  ); 
  CREATE TABLE IF NOT EXISTS `device_consent_group`( 
    `device_id` VARCHAR(100) NOT NULL, 
    `functional_group_id` INTEGER NOT NULL, 
    `is_consented` BOOL NOT NULL, 
    `input` VARCHAR(45), 
    `time_stamp` DATETIME DEFAULT CURRENT_TIMESTAMP, 
    PRIMARY KEY(`device_id`,`functional_group_id`), 
    CONSTRAINT `fk_device_has_functional_group_device1` 
      FOREIGN KEY(`device_id`) 
      REFERENCES `device`(`id`), 
    CONSTRAINT `fk_device_has_functional_group_functional_group1` 
      FOREIGN KEY(`functional_group_id`) 
      REFERENCES `functional_group`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS 
  `device_consent_group.fk_device_has_functional_group_functional_group1_idx` 
    ON `device_consent_group`(`functional_group_id`); 
  CREATE INDEX IF NOT EXISTS 
  `device_consent_group.fk_device_has_functional_group_device1_idx` 
    ON `device_consent_group`(`device_id`); 
  CREATE TABLE IF NOT EXISTS `app_level`( 
    `application_id` VARCHAR(45) PRIMARY KEY NOT NULL, 
    `minutes_in_hmi_full` INTEGER DEFAULT 0, 
    `minutes_in_hmi_limited` INTEGER DEFAULT 0, 
    `minutes_in_hmi_background` INTEGER DEFAULT 0, 
    `minutes_in_hmi_none` INTEGER DEFAULT 0, 

    `count_of_user_selections` INTEGER DEFAULT 0, 
    `count_of_rejections_sync_out_of_memory` INTEGER DEFAULT 0, 
    `count_of_rejections_nickname_mismatch` INTEGER DEFAULT 0, 
    `count_of_rejections_duplicate_name` INTEGER DEFAULT 0, 
    `count_of_rejected_rpcs_calls` INTEGER DEFAULT 0, 
    `count_of_rpcs_sent_in_hmi_none` INTEGER DEFAULT 0, 
    `count_of_removals_for_bad_behavior` INTEGER DEFAULT 0, 
    `count_of_run_attempts_while_revoked` INTEGER DEFAULT 0, 
    `app_registration_language_gui` VARCHAR(25), 
    `app_registration_language_vui` VARCHAR(25), 
    CONSTRAINT `fk_app_levels_application1` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`), 
    CONSTRAINT `fk_app_level_language1` 
      FOREIGN KEY(`app_registration_language_gui`) 
      REFERENCES `language`(`code`), 
    CONSTRAINT `fk_app_level_language2` 
      FOREIGN KEY(`app_registration_language_vui`) 
      REFERENCES `language`(`code`) 
  ); 
  CREATE INDEX IF NOT EXISTS `app_level.fk_app_levels_application1_idx` 
    ON `app_level`(`application_id`); 
  CREATE INDEX IF NOT EXISTS `app_level.fk_app_level_language1_idx` 
    ON `app_level`(`app_registration_language_gui`); 
  CREATE INDEX IF NOT EXISTS `app_level.fk_app_level_language2_idx` 
    ON `app_level`(`app_registration_language_vui`); 
  CREATE TABLE IF NOT EXISTS `nickname`( 
    `name` VARCHAR(100) NOT NULL, 
    `application_id` VARCHAR(45) NOT NULL, 
    PRIMARY KEY(`name`,`application_id`), 
    CONSTRAINT `fk_nickname_application1` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS `nickname.fk_nickname_application1_idx` 
    ON `nickname`(`application_id`); 
  CREATE TABLE IF NOT EXISTS `app_type`( 
    `name` VARCHAR(50) NOT NULL, 
    `application_id` VARCHAR(45) NOT NULL, 
    PRIMARY KEY(`name`,`application_id`), 
    CONSTRAINT `fk_app_type_application1` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS `app_type.fk_app_type_application1_idx` 
    ON `app_type`(`application_id`); 
  CREATE TABLE IF NOT EXISTS `consent_group`( 
    `device_id` VARCHAR(100) NOT NULL, 
    `application_id` VARCHAR(45) NOT NULL, 
    `functional_group_id` INTEGER NOT NULL, 
    `is_consented` BOOL NOT NULL, 
    `input` VARCHAR(45), 
    `time_stamp` DATETIME DEFAULT CURRENT_TIMESTAMP, 
    PRIMARY KEY(`application_id`,`functional_group_id`,`device_id`), 
    CONSTRAINT `fk_consent_group_device1` 
      FOREIGN KEY(`device_id`) 
      REFERENCES `device`(`id`), 
    CONSTRAINT `fk_consent_group_application1` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`), 
    CONSTRAINT `fk_consent_group_functional_group1` 
      FOREIGN KEY(`functional_group_id`) 
      REFERENCES `functional_group`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS 
  `consent_group.fk_consent_group_device1_idx` 
    ON `device_consent_group`(`device_id`); 
  CREATE INDEX IF NOT EXISTS `consent_group.fk_consent_group_functional_group1_idx` 
    ON `consent_group`(`functional_group_id`); 
  CREATE TABLE IF NOT EXISTS `endpoint`( 
    `service` INTEGER NOT NULL, 
    `url` VARCHAR(100) NOT NULL, 
    `application_id` VARCHAR(45) NOT NULL, 
    CONSTRAINT `fk_endpoint_application1` 
      FOREIGN KEY(`application_id`) 
      REFERENCES `application`(`id`) 
  ); 
  CREATE INDEX IF NOT EXISTS `endpoint.fk_endpoint_application1_idx` 
    ON `endpoint`(`application_id`); 
  CREATE TABLE IF NOT EXISTS `message`( 
    `id` INTEGER PRIMARY KEY NOT NULL, 
    `tts` TEXT, 
    `label` TEXT, 
    `line1` TEXT, 
    `line2` TEXT, 
    `textBody` TEXT, 
    `language_code` VARCHAR(25) NOT NULL, 
    `message_type_name` VARCHAR(45) NOT NULL, 
    CONSTRAINT `fk_messages_languages1` 
      FOREIGN KEY(`language_code`) 
      REFERENCES `language`(`code`), 
    CONSTRAINT `fk_message_consumer_friendly_messages1` 
      FOREIGN KEY(`message_type_name`) 
      REFERENCES `message_type`(`name`) 
  ); 
  CREATE INDEX IF NOT EXISTS `message.fk_messages_languages1_idx` 
    ON `message`(`language_code`);
  CREATE INDEX IF NOT EXISTS `message.fk_message_consumer_friendly_messages1_idx` 
    ON `message`(`message_type_name`);
COMMIT;
