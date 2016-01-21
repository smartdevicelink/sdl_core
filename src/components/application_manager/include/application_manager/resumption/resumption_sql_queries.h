/*
 *  Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_SQL_QUERY_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_SQL_QUERY_H_

#include <string>

namespace resumption {

extern const std::string kCreateSchema;
extern const std::string kDropSchema;
extern const std::string kInsertInitData;
extern const std::string kChecksResumptionData;
extern const std::string kSelectCountHMILevel;
extern const std::string kSelectHMILevel;
extern const std::string kCheckHMIId;
extern const std::string kSelectHMIId;
extern const std::string kSelectCountHMIId;
extern const std::string kCountHashId;
extern const std::string kSelectHashId;
extern const std::string kSelectIgnOffTime;
extern const std::string kCheckApplication;
extern const std::string kCountApplications;
extern const std::string kSelectDataForLoadResumeData;
extern const std::string kUpdateHMILevel;
extern const std::string kUpdateIgnOffCount;
extern const std::string kCountApplicationsIgnOff;
extern const std::string kSelectApplicationsIgnOffCount;
extern const std::string kUpdateSuspendData;
extern const std::string KUpdateLastIgnOffTime;
extern const std::string kDeleteFile;
extern const std::string kDeleteApplicationFilesArray;
extern const std::string kDeleteSubMenu;
extern const std::string kDeleteApplicationSubMenuArray;
extern const std::string kDeleteApplicationSubscribtionsArray;
extern const std::string kDeleteImageFromCommands;
extern const std::string kDeleteVrCommands;
extern const std::string kDeleteCommands;
extern const std::string kDeleteApplicationCommandsArray;
extern const std::string kDeleteImageFromChoiceSet;
extern const std::string kDeleteVrCommandsFromChoiceSet;
extern const std::string kDeleteChoice;
extern const std::string kDeleteChoiceArray;
extern const std::string kDeleteApplicationChoiceSet;
extern const std::string kDeleteApplicationChoiceSetArray;
extern const std::string kDeleteImageFromGlobalProperties;
extern const std::string kDeletevrHelpItem;
extern const std::string kDeletevrHelpItemArray;
extern const std::string kDeleteTableLimitedCharacterList;
extern const std::string kDeleteCharacterArray;
extern const std::string kDeleteTTSChunk;
extern const std::string kDeleteFromApplicationTable;
extern const std::string kDeleteHelpTimeoutPromptArray;
extern const std::string kDeleteGlobalProperties;
extern const std::string kSelectCountImage;
extern const std::string kSelectPrimaryKeyImage;
extern const std::string kInsertImage;
extern const std::string kInsertToFile;
extern const std::string kInsertToApplicationFilesArray;
extern const std::string kInsertToSubMenu;
extern const std::string kInsertToApplicationSubMenuArray;
extern const std::string kInsertToVrCommandsArray;
extern const std::string kInsertToCommand;
extern const std::string kInsertApplicationCommandArray;
extern const std::string kInsertVrCommand;
extern const std::string kInsertSubscriptions;
extern const std::string kInsertChoice;
extern const std::string kInsertApplicationChoiceSet;
extern const std::string kInsertChoiceArray;
extern const std::string kInsertApplicationChoiceSetArray;
extern const std::string kInsertGlobalProperties;
extern const std::string kInsertTableLimitedCharacter;
extern const std::string kInsertCharacterArray;
extern const std::string kInsertVRHelpItem;
extern const std::string kInsertVRHelpItemArray;
extern const std::string kInsertHelpTimeoutPromptArray;
extern const std::string kInsertTTSChunk;
extern const std::string kInsertApplication;
extern const std::string kSelectCountFiles;
extern const std::string kSelectFiles;
extern const std::string kSelectCountSubMenu;
extern const std::string kSelectSubMenu;
extern const std::string kSelectCountCommands;
extern const std::string kSelectCommands;
extern const std::string kSelectCountSubscriptions;
extern const std::string kSelectSubscriptions;
extern const std::string kSelectCountChoiceSet;
extern const std::string kSelectChoiceSets;
extern const std::string kSelectImage;
extern const std::string kSelectCountGlobalProperties;
extern const std::string kSelectGlobalProperties;
extern const std::string kChecksVrHelpItem;
extern const std::string kSelectVrHelpItem;
extern const std::string kChecksCharacter;
extern const std::string kSelectCharacter;
extern const std::string kSelectTTSChunk;
extern const std::string kSelectAppTable;
extern const std::string kSelectAllApps;
extern const std::string kUpdateApplicationData;
extern const std::string kSelectDBVersion;
extern const std::string kUpdateDBVersion;
extern const std::string kUpdateGrammarID;
}  // namespace resumption

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_SQL_QUERY_H_
