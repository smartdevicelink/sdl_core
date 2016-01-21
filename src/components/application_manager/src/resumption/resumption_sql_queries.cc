/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "application_manager/resumption/resumption_sql_queries.h"

namespace resumption {

const std::string kCreateSchema =
    "BEGIN ; "
    "CREATE TABLE IF NOT EXISTS `resumption`( "
    "  `idresumption` INTEGER PRIMARY KEY, "
    "  `last_ign_off_time` INTEGER "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `image`( "
    "  `idimage` INTEGER PRIMARY KEY NOT NULL, "
    "  `imageType` INTEGER, "
    "  `value` TEXT UNIQUE  "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `applicationChoiceSet`( "
    "  `idapplicationChoiceSet` INTEGER PRIMARY KEY NOT NULL, "
    "  `grammarID` INTEGER, "
    "  `interactionChoiceSetID` INTEGER "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `file`( "
    "  `idfile` INTEGER PRIMARY KEY NOT NULL, "
    "  `fileType` INTEGER, "
    "  `is_download_complete` BOOL, "
    "  `persistentFile` BOOL, "
    "  `syncFileName` TEXT "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `subMenu`( "
    "  `idsubMenu` INTEGER PRIMARY KEY NOT NULL, "
    "  `menuID` INTEGER, "
    "  `menuName` TEXT, "
    "  `position` INTEGER "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `TTSChunk`( "
    "  `idTTSChunk` INTEGER PRIMARY KEY NOT NULL, "
    "  `type` INTEGER, "
    "  `text` TEXT "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `vrHelpItem`( "
    "  `idvrHelpItem` INTEGER PRIMARY KEY NOT NULL, "
    "  `text` TEXT, "
    "  `position` INTEGER, "
    "  `idimage` INTEGER, "
    "  CONSTRAINT `fk_image` "
    "    FOREIGN KEY(`idimage`) "
    "    REFERENCES `image`(`idimage`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`vrHelpItem.fk_image_idx` ON `vrHelpItem`(`idimage`); "
    "CREATE TABLE IF NOT EXISTS `tableLimitedCharacterList`( "
    "  `idtableLimitedCharacterList` INTEGER PRIMARY KEY NOT NULL, "
    "  `limitedCharacterList` VARCHAR(45) "
    "  ); "
    "CREATE TABLE IF NOT EXISTS `characterArray`( "
    "  `idcharacterArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idglobalProperties` INTEGER, "
    "  `idtableLimitedCharacterList` INTEGER, "
    "  CONSTRAINT `fk_globalProperties` "
    "    FOREIGN KEY(`idglobalProperties`) "
    "    REFERENCES `globalProperties`(`idglobalProperties`), "
    "  CONSTRAINT `fk_tableLimitedCharacterList` "
    "    FOREIGN KEY(`idtableLimitedCharacterList`) "
    "    REFERENCES `tableLimitedCharacterList`(`idtableLimitedCharacterList`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`characterArray.fk_globalProperties_idx` "
    "  ON `characterArray`(`idglobalProperties`); "
    "CREATE INDEX IF NOT EXISTS "
    "`characterArray.fk_tableLimitedCharacterList_idx` "
    "  ON `characterArray`(`idtableLimitedCharacterList`); "
    "CREATE TABLE IF NOT EXISTS `choice`( "
    "  `idchoice` INTEGER PRIMARY KEY NOT NULL, "
    "  `choiceID` INTEGER, "
    "  `menuName` TEXT, "
    "  `secondaryText` TEXT, "
    "  `tertiaryText` TEXT, "
    "  `idimage` INTEGER, "
    "  `idsecondaryImage` INTEGER, "
    "  CONSTRAINT `fk_image` "
    "    FOREIGN KEY(`idimage`,`idsecondaryImage`) "
    "    REFERENCES `image`(`idimage`,`idimage`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`choice.fk_image_idx` ON `choice`(`idimage`,`idsecondaryImage`); "
    "CREATE TABLE IF NOT EXISTS `command`( "
    "  `idcommand` INTEGER PRIMARY KEY NOT NULL, "
    "  `cmdID` INTEGER, "
    "  `menuName` TEXT, "
    "  `parentID` INTEGER, "
    "  `position` INTEGER, "
    "  `idimage` INTEGER, "
    "  CONSTRAINT `fk_image` "
    "    FOREIGN KEY(`idimage`) "
    "    REFERENCES `image`(`idimage`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`command.fk_image_idx` ON `command`(`idimage`); "
    "CREATE TABLE IF NOT EXISTS `globalProperties`( "
    "  `idglobalProperties` INTEGER PRIMARY KEY NOT NULL, "
    "  `vrHelpTitle` TEXT, "
    "  `menuTitle` TEXT, "
    "  `idmenuIcon` INTEGER, "
    "  `language` INTEGER, "
    "  `keyboardLayout` INTEGER, "
    "  `keypressMode` INTEGER, "
    "  `autoCompleteText` TEXT, "
    "  CONSTRAINT `fk_image` "
    "    FOREIGN KEY(`idmenuIcon`) "
    "    REFERENCES `image`(`idimage`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`globalProperties.fk_image_idx` ON `globalProperties`(`idmenuIcon`); "
    "CREATE TABLE IF NOT EXISTS `choiceArray`( "
    "  `idchoiceArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idapplicationChoiceSet` INTEGER, "
    "  `idchoice` INTEGER, "
    "  CONSTRAINT `fk_applicationChoiceSet` "
    "    FOREIGN KEY(`idapplicationChoiceSet`) "
    "    REFERENCES `applicationChoiceSet`(`idapplicationChoiceSet`), "
    "  CONSTRAINT `fk_choice` "
    "    FOREIGN KEY(`idchoice`) "
    "    REFERENCES `choice`(`idchoice`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`choiceArray.fk_applicationChoiceSet_idx` "
    "  ON `choiceArray`(`idapplicationChoiceSet`); "
    "CREATE INDEX IF NOT EXISTS "
    "`choiceArray.fk_choice_idx` ON `choiceArray`(`idchoice`); "
    "CREATE TABLE IF NOT EXISTS `vrCommandsArray`( "
    "  `idvrCommandsArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idchoice` INTEGER, "
    "  `vrCommand` TEXT, "
    "  `idcommand` INTEGER, "
    "  CONSTRAINT `fk_choice` "
    "    FOREIGN KEY(`idchoice`) "
    "    REFERENCES `choice`(`idchoice`), "
    "  CONSTRAINT `fk_command` "
    "    FOREIGN KEY(`idcommand`) "
    "    REFERENCES `command`(`idcommand`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`vrCommandsArray.fk_choice_idx` ON `vrCommandsArray`(`idchoice`); "
    "CREATE INDEX IF NOT EXISTS "
    "`vrCommandsArray.fk_command_idx` ON `vrCommandsArray`(`idcommand`); "
    "CREATE TABLE IF NOT EXISTS `helpTimeoutPromptArray`( "
    "  `idhelpTimeoutPromptArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idhelpPrompt` INTEGER, "
    "  `idtimeoutPrompt` INTEGER, "
    "  `idglobalProperties` INTEGER, "
    "   CONSTRAINT `fk_globalProperties` "
    "     FOREIGN KEY(`idglobalProperties`) "
    "     REFERENCES `globalProperties`(`idglobalProperties`), "
    "   CONSTRAINT `fk_TTSChunk` "
    "     FOREIGN KEY(`idtimeoutPrompt`,`idhelpPrompt`) "
    "     REFERENCES `TTSChunk`(`idTTSChunk`,`idTTSChunk`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`helpTimeoutPromptArray.fk_globalProperties_idx` "
    "  ON `helpTimeoutPromptArray`(`idglobalProperties`); "
    "CREATE INDEX IF NOT EXISTS "
    "`helpTimeoutPromptArray.fk_TTSChunk_idx` "
    "  ON `helpTimeoutPromptArray`(`idtimeoutPrompt`,`idhelpPrompt`); "
    "CREATE TABLE IF NOT EXISTS `vrHelpItemArray`( "
    "  `idvrHelpItemArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idglobalProperties` INTEGER, "
    "  `idvrHelpItem` INTEGER, "
    "  CONSTRAINT `fk_vrHelpItem` "
    "    FOREIGN KEY(`idvrHelpItem`) "
    "    REFERENCES `vrHelpItem`(`idvrHelpItem`), "
    "  CONSTRAINT `fk_vrglobalProperties` "
    "    FOREIGN KEY(`idglobalProperties`) "
    "    REFERENCES `globalProperties`(`idglobalProperties`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`vrHelpItemArray.fk_vrHelpItem_idx` ON `vrHelpItemArray`(`idvrHelpItem`); "
    "CREATE INDEX IF NOT EXISTS "
    "`vrHelpItemArray.fk_vrglobalProperties_idx` ON "
    "`vrHelpItemArray`(`idglobalProperties`); "
    "CREATE TABLE IF NOT EXISTS `application`( "
    "  `idApplication` INTEGER PRIMARY KEY NOT NULL, "
    "  `appID` TEXT, "
    "  `connection_key` INTEGER, "
    "  `grammarID` INTEGER, "
    "  `hashID` TEXT, "
    "  `hmiAppID` INTEGER, "
    "  `hmiLevel` INTEGER, "
    "  `ign_off_count` INTEGER, "
    "  `timeStamp` INTEGER, "
    "  `deviceID` TEXT, "
    "  `idglobalProperties` INTEGER, "
    "  `isMediaApplication` BOOL, "
    "  CONSTRAINT `fk_globalProperties` "
    "    FOREIGN KEY(`idglobalProperties`) "
    "    REFERENCES `globalProperties`(`idglobalProperties`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`application.fk_globalProperties_idx` ON "
    "`application`(`idglobalProperties`); "
    "CREATE TABLE IF NOT EXISTS `applicationChoiceSetArray`( "
    "  `idapplicationChoiceSetArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idapplicationChoiceSet` INTEGER, "
    "  `idApplication` INTEGER, "
    "   CONSTRAINT `fk_applicationChoiceSet` "
    "     FOREIGN KEY(`idapplicationChoiceSet`) "
    "     REFERENCES `applicationChoiceSet`(`idapplicationChoiceSet`), "
    "   CONSTRAINT `fk_Aplication` "
    "     FOREIGN KEY(`idApplication`) "
    "     REFERENCES `application`(`idApplication`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationChoiceSetArray.fk_applicationChoiceSet_idx` "
    "  ON `applicationChoiceSetArray`(`idapplicationChoiceSet`); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationChoiceSetArray.fk_Aplication_idx` "
    "  ON `applicationChoiceSetArray`(`idApplication`); "
    "CREATE TABLE IF NOT EXISTS `applicationCommandsArray`( "
    "  `idapplicationCommandsArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idApplication` INTEGER, "
    "  `idcommand` INTEGER, "
    "  CONSTRAINT `fk_Application` "
    "    FOREIGN KEY(`idApplication`) "
    "    REFERENCES `application`(`idApplication`), "
    "  CONSTRAINT `fk_command` "
    "    FOREIGN KEY(`idcommand`) "
    "    REFERENCES `command`(`idcommand`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationCommandsArray.fk_Application_idx` "
    "  ON `applicationCommandsArray`(`idApplication`); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationCommandsArray.fk_command_idx` "
    "  ON `applicationCommandsArray`(`idcommand`); "
    "CREATE TABLE IF NOT EXISTS `applicationFilesArray`( "
    "  `idapplicationFilesArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idApplication` INTEGER, "
    "  `idfile` INTEGER, "
    "  CONSTRAINT `fk_Application` "
    "    FOREIGN KEY(`idApplication`) "
    "    REFERENCES `application`(`idApplication`), "
    "  CONSTRAINT `fk_file` "
    "    FOREIGN KEY(`idfile`) "
    "    REFERENCES `file`(`idfile`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationFilesArray.fk_Application_idx` "
    "  ON `applicationFilesArray`(`idApplication`); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationFilesArray.fk_file_idx` ON `applicationFilesArray`(`idfile`); "
    "CREATE TABLE IF NOT EXISTS `applicationSubMenuArray`( "
    "  `idapplicationSubMenuArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `idApplication` INTEGER, "
    "  `idsubMenu` INTEGER, "
    "  CONSTRAINT `fk_subMenu`  "
    "    FOREIGN KEY(`idsubMenu`) "
    "    REFERENCES `subMenu`(`idsubMenu`), "
    "  CONSTRAINT `fk_Application` "
    "    FOREIGN KEY(`idApplication`) "
    "    REFERENCES `application`(`idApplication`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationSubMenuArray.fk_subMenu_idx` "
    "  ON `applicationSubMenuArray`(`idsubMenu`); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationSubMenuArray.fk_Application_idx` "
    "  ON `applicationSubMenuArray`(`idApplication`); "
    "CREATE TABLE IF NOT EXISTS `applicationSubscribtionsArray`( "
    "  `idapplicationSubscribtionsArray` INTEGER PRIMARY KEY NOT NULL, "
    "  `vehicleValue` INTEGER, "
    "  `ButtonNameValue` INTEGER, "
    "  `idApplication` INTEGER, "
    "  CONSTRAINT `fk_Application` "
    "    FOREIGN KEY(`idApplication`) "
    "    REFERENCES `application`(`idApplication`) "
    "  ); "
    "CREATE INDEX IF NOT EXISTS "
    "`applicationSubscribtionsArray.fk_Application_idx` "
    "  ON `applicationSubscribtionsArray`(`idApplication`); "
    "CREATE TABLE IF NOT EXISTS `_internal_data`( "
    "   `db_version_hash` INTEGER "
    "  ); "
    "COMMIT;";

const std::string kDropSchema =
    "BEGIN; "
    "DROP INDEX IF EXISTS "
    "`message.fk_message_consumer_friendly_messages1_idx`; "
    "DROP TABLE IF EXISTS `resumption`; "
    "DROP TABLE IF EXISTS `image`; "
    "DROP TABLE IF EXISTS `applicationChoiceSet`; "
    "DROP TABLE IF EXISTS `file`; "
    "DROP TABLE IF EXISTS `subMenu`; "
    "DROP TABLE IF EXISTS `TTSChunk`; "
    "DROP TABLE IF EXISTS `vrHelpItem`; "
    "DROP INDEX IF EXISTS `vrHelpItem.fk_image_idx`; "
    "DROP TABLE IF EXISTS `tableLimitedCharacterList`; "
    "DROP TABLE IF EXISTS `characterArray`; "
    "DROP INDEX IF EXISTS `characterArray.fk_globalProperties_idx`; "
    "DROP INDEX IF EXISTS `characterArray.fk_tableLimitedCharacterList_idx`; "
    "DROP TABLE IF EXISTS `choice`; "
    "DROP INDEX IF EXISTS `choice.fk_image_idx`; "
    "DROP TABLE IF EXISTS `command`; "
    "DROP INDEX IF EXISTS `command.fk_image_idx`; "
    "DROP TABLE IF EXISTS `globalProperties`; "
    "DROP INDEX IF EXISTS `globalProperties.fk_image_idx`; "
    "DROP TABLE IF EXISTS `choiceArray`; "
    "DROP INDEX IF EXISTS `choiceArray.fk_applicationChoiceSet_idx`; "
    "DROP INDEX IF EXISTS `choiceArray.fk_choice_idx`; "
    "DROP TABLE IF EXISTS `vrCommandsArray`; "
    "DROP INDEX IF EXISTS `vrCommandsArray.fk_choice_idx`; "
    "DROP INDEX IF EXISTS `vrCommandsArray.fk_command_idx`; "
    "DROP TABLE IF EXISTS `helpTimeoutPromptArray`; "
    "DROP INDEX IF EXISTS `helpTimeoutPromptArray.fk_globalProperties_idx`; "
    "DROP INDEX IF EXISTS `helpTimeoutPromptArray.fk_TTSChunk_idx`; "
    "DROP TABLE IF EXISTS `vrHelpItemArray`; "
    "DROP INDEX IF EXISTS `vrHelpItemArray.fk_vrHelpItem_idx`; "
    "DROP INDEX IF EXISTS `vrHelpItemArray.fk_vrglobalProperties_idx`; "
    "DROP TABLE IF EXISTS `application`; "
    "DROP INDEX IF EXISTS `application.fk_globalProperties_idx`; "
    "DROP TABLE IF EXISTS `applicationChoiceSetArray`; "
    "DROP INDEX IF EXISTS "
    "`applicationChoiceSetArray.fk_applicationChoiceSet_idx`; "
    "DROP INDEX IF EXISTS `applicationChoiceSetArray.fk_Aplication_idx`; "
    "DROP TABLE IF EXISTS `applicationCommandsArray`; "
    "DROP INDEX IF EXISTS `applicationCommandsArray.fk_Application_idx`; "
    "DROP INDEX IF EXISTS `applicationCommandsArray.fk_command_idx`; "
    "DROP TABLE IF EXISTS `applicationFilesArray`; "
    "DROP INDEX IF EXISTS `applicationFilesArray.fk_Application_idx`; "
    "DROP INDEX IF EXISTS `applicationFilesArray.fk_file_idx`; "
    "DROP TABLE IF EXISTS `applicationSubMenuArray`; "
    "DROP INDEX IF EXISTS `applicationSubMenuArray.fk_subMenu_idx`; "
    "DROP INDEX IF EXISTS `applicationSubMenuArray.fk_Application_idx`; "
    "DROP TABLE IF EXISTS `applicationSubscribtionsArray`; "
    "DROP INDEX IF EXISTS `applicationSubscribtionsArray.fk_Application_idx`; "
    "DROP TABLE IF EXISTS `_internal_data`; "
    "COMMIT; "
    "VACUUM;";

const std::string kInsertInitData =
    "INSERT OR IGNORE INTO `resumption` (`last_ign_off_time`) VALUES (0); "
    "INSERT OR IGNORE INTO `_internal_data` (`db_version_hash`) VALUES(0); ";

const std::string kChecksResumptionData =
    " SELECT COUNT(`idresumption`) "
    " FROM `resumption`;";

const std::string kSelectCountHMILevel =
    "SELECT COUNT(`hmiLevel`) FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kSelectHMILevel =
    "SELECT `hmiLevel` FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kCheckHMIId =
    "SELECT COUNT(`hmiAppID`) FROM `application` "
    "WHERE `hmiAppID` = ?";

const std::string kSelectHMIId =
    "SELECT `hmiAppID` FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kSelectCountHMIId =
    "SELECT COUNT(`hmiAppID`) FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kCountHashId =
    "SELECT COUNT(`hashID`) FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kSelectHashId =
    "SELECT `hashID` FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kSelectIgnOffTime =
    "SELECT `last_ign_off_time` FROM `resumption`";

const std::string kCheckApplication =
    "SELECT COUNT (`deviceID`) FROM `application` "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kCountApplications =
    "SELECT COUNT(`appID`) FROM `application`;";

const std::string kSelectDataForLoadResumeData =
    "SELECT `hmiLevel`, `ign_off_count`, `timeStamp`, `appID`, `deviceID` "
    "FROM `application`;";

const std::string kUpdateHMILevel =
    "UPDATE `application` "
    "SET `hmiLevel` = ? "
    "WHERE `deviceID` = ? AND `appID` = ?";

const std::string kUpdateIgnOffCount =
    "UPDATE `application` "
    "SET `ign_off_count` = `ign_off_count` - 1 "
    "WHERE `ign_off_count` > 0";

const std::string kCountApplicationsIgnOff =
    "SELECT COUNT(`ign_off_count`) "
    "FROM `application` "
    "WHERE `ign_off_count` >= ?";

const std::string kSelectApplicationsIgnOffCount =
    "SELECT `deviceID`, `appID` "
    "FROM `application` "
    "WHERE `ign_off_count` >= ?";

const std::string kUpdateSuspendData =
    "UPDATE `application` "
    "SET `ign_off_count` = `ign_off_count` + 1";

const std::string KUpdateLastIgnOffTime =
    "UPDATE `resumption` "
    "SET `last_ign_off_time` = ?";

const std::string kDeleteFile =
    "DELETE FROM `file` "
    "WHERE `idfile` IN (SELECT `idfile` "
    "FROM `applicationFilesArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteApplicationFilesArray =
    "DELETE FROM `applicationFilesArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteSubMenu =
    "DELETE FROM `subMenu` "
    "WHERE `idsubMenu` IN (SELECT `idsubMenu` "
    "FROM `applicationSubMenuArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteApplicationSubMenuArray =
    "DELETE FROM `applicationSubMenuArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteApplicationSubscribtionsArray =
    "DELETE FROM `applicationSubscribtionsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteImageFromCommands =
    "DELETE FROM `image` "
    "WHERE `idimage` IN (SELECT `idimage` "
    "FROM `command` "
    "WHERE `idcommand` IN (SELECT `idcommand` "
    "FROM `applicationCommandsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)))";

const std::string kDeleteVrCommands =
    "DELETE FROM `vrCommandsArray` "
    "WHERE `idcommand` IN (SELECT `idcommand` "
    "FROM `applicationCommandsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteCommands =
    "DELETE FROM `command` "
    "WHERE `idcommand` IN (SELECT `idcommand` "
    "FROM `applicationCommandsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteApplicationCommandsArray =
    "DELETE FROM `applicationCommandsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteImageFromChoiceSet =
    "DELETE FROM `image` "
    "WHERE `idimage` IN (SELECT `idimage` "
    "FROM `choice` "
    "WHERE `idchoice` IN (SELECT `idchoice` "
    "FROM `choiceArray` "
    "WHERE `idapplicationChoiceSet` IN (SELECT `idapplicationChoiceSet` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))) "
    "UNION "
    "SELECT `idsecondaryImage` "
    "FROM `choice` "
    "WHERE `idchoice` IN (SELECT `idchoice` "
    "FROM `choiceArray` "
    "WHERE `idapplicationChoiceSet` IN (SELECT `idapplicationChoiceSet` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))))";

const std::string kDeleteVrCommandsFromChoiceSet =
    "DELETE FROM `vrCommandsArray` "
    "WHERE `idchoice` IN (SELECT `idchoice` "
    "FROM `choiceArray` "
    "WHERE `idapplicationChoiceSet` IN (SELECT `idapplicationChoiceSet` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)))";

const std::string kDeleteChoice =
    "DELETE FROM `choice` "
    "WHERE `idchoice` IN (SELECT `idchoice` "
    "FROM `choiceArray` "
    "WHERE `idapplicationChoiceSet` IN (SELECT `idapplicationChoiceSetArray` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)))";

const std::string kDeleteChoiceArray =
    "DELETE FROM `choiceArray` "
    "WHERE `idapplicationChoiceSet` IN (SELECT `idapplicationChoiceSetArray` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteApplicationChoiceSet =
    "DELETE FROM `applicationChoiceSet` "
    "WHERE `idapplicationChoiceSet` IN (SELECT `idapplicationChoiceSetArray` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteApplicationChoiceSetArray =
    "DELETE FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteImageFromGlobalProperties =
    "DELETE FROM `image` "
    "WHERE `idimage` IN (SELECT `idimage` "
    "FROM `vrHelpItem` "
    "WHERE `idvrHelpItem` IN (SELECT `idvrHelpItem` "
    "FROM `vrHelpItemArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)) "
    "UNION "
    "SELECT `idmenuIcon` "
    "FROM `globalProperties` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeletevrHelpItem =
    "DELETE FROM `vrHelpItem` "
    "WHERE `idvrHelpItem` IN (SELECT `idvrHelpItem` "
    "FROM `vrHelpItemArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeletevrHelpItemArray =
    "DELETE FROM `vrHelpItemArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteTableLimitedCharacterList =
    "DELETE FROM `tableLimitedCharacterList` "
    "WHERE `idtableLimitedCharacterList`  IN (SELECT "
    "`idtableLimitedCharacterList` "
    "FROM  `characterArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` =   ?   AND deviceID = ?))";

const std::string kDeleteCharacterArray =
    "DELETE FROM `characterArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteTTSChunk =
    "DELETE FROM `TTSChunk` "
    "WHERE `idTTSChunk` IN (SELECT `idhelpPrompt` "
    "FROM `helpTimeoutPromptArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?) "
    "UNION "
    "SELECT `idtimeoutPrompt` "
    "FROM `helpTimeoutPromptArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?))";

const std::string kDeleteFromApplicationTable =
    "DELETE FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?";

const std::string kDeleteHelpTimeoutPromptArray =
    "DELETE FROM `helpTimeoutPromptArray` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kDeleteGlobalProperties =
    "DELETE FROM `globalProperties` "
    "WHERE `idglobalProperties` = (SELECT `idglobalProperties` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?)";

const std::string kSelectCountImage =
    "SELECT COUNT (`value`) "
    "FROM `image` "
    "WHERE `value` = ?";

const std::string kSelectPrimaryKeyImage =
    "SELECT `idimage` "
    "FROM `image` "
    "WHERE `value` = ?";

const std::string kInsertImage =
    "INSERT INTO `image` "
    "(`imageType`, `value`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertToFile =
    "INSERT INTO `file` "
    "(`fileType`, `is_download_complete`, `persistentFile`, `syncFileName`) "
    "VALUES "
    "(?, ?, ?, ?)";

const std::string kInsertToApplicationFilesArray =
    "INSERT INTO `applicationFilesArray` "
    "(`idApplication`, `idfile`) "
    "VALUES "
    "(?, ?)";

const std::string kInsertToSubMenu =
    "INSERT INTO `subMenu` "
    "(`menuID`, `menuName`, `position`) "
    "VALUES "
    "(?, ?, ?);";

const std::string kInsertToApplicationSubMenuArray =
    "INSERT INTO `applicationSubMenuArray` "
    "(`idApplication`, `idsubMenu`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertToCommand =
    "INSERT INTO `command` "
    "(`cmdID`, `idimage`, `menuName`, `parentID`, `position`) "
    "VALUES "
    "(?, ?, ?, ?, ?);";

const std::string kInsertApplicationCommandArray =
    "INSERT INTO `applicationCommandsArray` "
    "(`idApplication`, `idcommand`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertVrCommand =
    "INSERT INTO `vrCommandsArray` "
    "(`vrCommand`, `idcommand`, `idchoice`) "
    "VALUES "
    "(?, ?, ?);";

const std::string kInsertSubscriptions =
    "INSERT INTO `applicationSubscribtionsArray` "
    "(`idApplication`, `vehicleValue`, `ButtonNameValue`) "
    "VALUES "
    "(?, ?, ?);";

const std::string kInsertChoice =
    "INSERT INTO `choice` "
    "(`choiceID`, `menuName`, `secondaryText`, "
    "`tertiaryText`, `idimage`, `idsecondaryImage`) "
    "VALUES "
    "(?, ?, ?, ?, ?, ?);";

const std::string kInsertApplicationChoiceSet =
    "INSERT INTO `applicationChoiceSet` "
    "(`grammarID`, `interactionChoiceSetID`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertChoiceArray =
    "INSERT INTO `choiceArray` "
    "(`idapplicationChoiceSet`, `idchoice`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertApplicationChoiceSetArray =
    "INSERT INTO `applicationChoiceSetArray` "
    "(`idapplicationChoiceSet`, `idApplication`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertGlobalProperties =
    "INSERT INTO `globalProperties` "
    "(`vrHelpTitle`, `menuTitle`, `idmenuIcon`, `language`,  "
    "`keyboardLayout`, `keypressMode`, `autoCompleteText`) "
    "VALUES "
    "(?, ?, ?, ?, ?, ?, ?);";

const std::string kInsertTableLimitedCharacter =
    "INSERT INTO `tableLimitedCharacterList` "
    "(`limitedCharacterList`) "
    "VALUES "
    "(?);";

const std::string kInsertCharacterArray =
    "INSERT INTO `characterArray` "
    "(`idglobalProperties`, `idtableLimitedCharacterList`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertVRHelpItem =
    "INSERT INTO `vrHelpItem` "
    "(`text`, `position`, `idimage`) "
    "VALUES "
    "(?, ?, ?);";

const std::string kInsertVRHelpItemArray =
    "INSERT INTO `vrHelpItemArray` "
    "(`idglobalProperties`, `idvrHelpItem`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertHelpTimeoutPromptArray =
    "INSERT INTO `helpTimeoutPromptArray` "
    "(`idglobalProperties`, `idtimeoutPrompt`, `idhelpPrompt`) "
    "VALUES "
    "(?, ?, ?);";

const std::string kInsertTTSChunk =
    "INSERT INTO `TTSChunk` "
    "(`type`, `text`) "
    "VALUES "
    "(?, ?);";

const std::string kInsertApplication =
    "INSERT INTO `application` "
    "(`connection_key`, `grammarID`, `hashID`, "
    "`hmiAppID`, `hmiLevel`, `ign_off_count`, "
    "`timeStamp`, `idglobalProperties`, "
    "`isMediaApplication`, `appID`, `deviceID`) "
    "VALUES "
    "(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

const std::string kSelectCountFiles =
    "SELECT COUNT (`idfile`) "
    "FROM `applicationFilesArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?);";

const std::string kSelectFiles =
    "SELECT `fileType`, `is_download_complete`, `persistentFile`, "
    "`syncFileName`"
    "FROM `file` "
    "WHERE `idfile` IN ( "
    "SELECT `idfile` "
    "FROM `applicationFilesArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?));";

const std::string kSelectCountSubMenu =
    "SELECT COUNT (`idsubMenu`) "
    "FROM `applicationSubMenuArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?);";

const std::string kSelectSubMenu =
    "SELECT `menuID`, `menuName`, `position` "
    "FROM `subMenu` "
    "WHERE `idsubMenu` IN (SELECT `idsubMenu` "
    "FROM `applicationSubMenuArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?));";

const std::string kSelectCountCommands =
    "SELECT COUNT (`idcommand`) "
    "FROM `applicationCommandsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?);";

const std::string kSelectCommands =
    "SELECT `command`.`idcommand`, `cmdID`, `menuName`, `parentID`, "
    "`position`, `value`, `imageType`, `vrCommand` "
    "FROM  `command` LEFT OUTER JOIN `image` on `command`.`idimage` = "
    "`image`.`idimage` "
    "LEFT OUTER JOIN  `vrcommandsarray` on `command`.`idcommand` = "
    "`vrcommandsarray`.`idcommand` "
    "WHERE `command`.`idcommand` IN (SELECT `idcommand` "
    "FROM `applicationCommandsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?));";

const std::string kSelectCountSubscriptions =
    "SELECT COUNT (`idApplication`) "
    "FROM `applicationSubscribtionsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?);";

const std::string kSelectSubscriptions =
    "SELECT `vehicleValue`, `ButtonNameValue` "
    "FROM `applicationSubscribtionsArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?);";

const std::string kSelectCountChoiceSet =
    "SELECT COUNT (`idApplication`) "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?);";

const std::string kSelectChoiceSets =
    "SELECT `applicationChoiceSet`.`idapplicationChoiceSet`, `grammarID`, "
    "`interactionChoiceSetID`, "
    "`choice`.`idchoice`, `choiceID`, `menuName`, `secondaryText`, "
    "`tertiaryText`, `choice`.`idimage`, "
    "`idsecondaryImage`, `vrCommand` "
    "FROM `applicationChoiceSet` INNER JOIN `choicearray` on  "
    "`applicationChoiceSet`.`idapplicationChoiceSet` = "
    "`choicearray`.`idapplicationChoiceSet` "
    "INNER JOIN `choice` on `choicearray`.`idchoice` = `choice`.`idchoice` "
    "LEFT OUTER JOIN `vrCommandsArray` on `choice`.`idchoice` = "
    "`vrCommandsArray`.`idchoice` "
    "WHERE `applicationChoiceSet`.`idapplicationChoiceSet` in  (select "
    "`idapplicationChoiceSet` "
    "FROM `applicationChoiceSetArray` "
    "WHERE `idApplication` = (SELECT `idApplication` "
    "FROM `application`  "
    "WHERE `appID` = ? AND `deviceID` = ?));";

const std::string kSelectImage =
    "SELECT `imageType`, `value` "
    "FROM `image` "
    "WHERE `idimage` = ?;";

const std::string kSelectCountGlobalProperties =
    "SELECT COUNT(idglobalProperties) "
    "FROM `application` "
    "WHERE `appID` = ? and `deviceID` = ?;";

const std::string kSelectGlobalProperties =
    "SELECT `globalProperties`.`idglobalProperties`, `vrHelpTitle`, "
    "`menuTitle`, `idmenuIcon`, "
    "`language`, `keyboardLayout`, `keypressMode`, `autoCompleteText`, "
    "`helpTimeoutPromptArray`.`idhelpPrompt`, "
    "`helpTimeoutPromptArray`.`idtimeoutPrompt` "
    "FROM `globalProperties` LEFT OUTER JOIN `helpTimeoutPromptArray` ON "
    "`globalProperties`.`idglobalProperties` = "
    "`helpTimeoutPromptArray`.`idglobalProperties` "
    "WHERE `globalProperties`.`idglobalProperties` = ( "
    "SELECT `idglobalProperties` FROM `application` WHERE `appID` = ? AND "
    "`deviceID` = ?);";

const std::string kChecksVrHelpItem =
    "SELECT COUNT(`idvrHelpItem`) "
    "FROM `vrHelpItemArray` "
    "WHERE `idglobalProperties`= ?;";

const std::string kSelectVrHelpItem =
    "SELECT `text`, `position`, `imageType`, `value` "
    "FROM `vrHelpItem` LEFT OUTER JOIN `image` ON "
    "`vrHelpItem`.`idimage` = `image`.`idimage` "
    "WHERE `idvrHelpItem` in (SELECT `idvrHelpItem` "
    "FROM `vrHelpItemArray` "
    "WHERE `idglobalProperties` = ?);";

const std::string kChecksCharacter =
    "SELECT COUNT(`idtableLimitedCharacterList`) "
    "FROM `characterArray` "
    "WHERE `idglobalProperties`= ?;";

const std::string kSelectCharacter =
    "SELECT `limitedCharacterList` "
    "FROM `tableLimitedCharacterList` "
    "WHERE `idtableLimitedCharacterList` "
    "in (SELECT `idtableLimitedCharacterList` "
    "FROM `characterArray` "
    "WHERE `idglobalProperties` = ?);";

const std::string kSelectTTSChunk =
    "SELECT `text`, `type` "
    "FROM `TTSChunk` "
    "WHERE `idTTSChunk` = ?;";

const std::string kSelectAppTable =
    "SELECT `appID`, `connection_key`, `grammarID`, `hashID`, `hmiAppID`, "
    "`hmiLevel`, `ign_off_count`, "
    "`timeStamp`, `deviceID`, `isMediaApplication` "
    "FROM `application` "
    "WHERE `appID` = ? AND `deviceID` = ?;";

const std::string kSelectAllApps =
    "SELECT `appID`, `deviceID` FROM `application`;";

const std::string kUpdateApplicationData =
    "UPDATE `application` "
    "SET `hmiLevel` = ?, `timeStamp` = ? "
    "WHERE `appID` = ? AND `deviceID` = ?;";

const std::string kSelectDBVersion =
    "SELECT `db_version_hash` from `_internal_data`; ";

const std::string kUpdateDBVersion =
    "UPDATE `_internal_data` SET `db_version_hash` = ? ; ";

const std::string kUpdateGrammarID =
    "UPDATE `application` "
    "SET `grammarID` = ? "
    "WHERE `appID` = ? AND `deviceID` = ?;";

}  // namespace resumption
