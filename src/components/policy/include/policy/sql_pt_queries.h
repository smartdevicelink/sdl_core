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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_QUERIES_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_QUERIES_H_

#include <string>

namespace policy {
namespace sql_pt {

extern const std::string kSelectPriority;
extern const std::string kCreateSchema;
extern const std::string kDropSchema;
extern const std::string kDeleteData;
extern const std::string kCheckPgNumber;
extern const std::string kCheckDBIntegrity;
extern const std::string kSelectRpc;
extern const std::string kSelectPreloaded;
extern const std::string kIsFirstRun;
extern const std::string kSetNotFirstRun;
extern const std::string kSelectEndpoint;
extern const std::string kSelectLockScreenIcon;
extern const std::string kSelectModuleConfig;
extern const std::string kSelectEndpoints;
extern const std::string kSelectNotificationsPerMin;
extern const std::string kSelectNotificationsPerPriority;
extern const std::string kSelectAppLevels;
extern const std::string kSelectDeviceData;
extern const std::string kSelectFunctionalGroups;
extern const std::string kSelectAllRpcs;
extern const std::string kSelectUserMsgsVersion;
extern const std::string kSelectAppPolicies;
extern const std::string kSelectAppGroups;
extern const std::string kSelectNicknames;
extern const std::string kSelectAppTypes;
extern const std::string kSelectRequestTypes;
extern const std::string kSelectSecondsBetweenRetries;
extern const std::string kSelectIgnitionCycles;
extern const std::string kSelectKilometers;
extern const std::string kSelectDays;
extern const std::string kSelectTimeoutResponse;
extern const std::string kInsertFunctionalGroup;
extern const std::string kInsertRpc;
extern const std::string kInsertRpcWithParameter;
extern const std::string kInsertApplication;
extern const std::string kInsertAppGroup;
extern const std::string kInsertNickname;
extern const std::string kInsertAppType;
extern const std::string kInsertRequestType;
extern const std::string kInsertMessageType;
extern const std::string kInsertLanguage;
extern const std::string kInsertMessageString;
extern const std::string kUpdateVersion;
extern const std::string kUpdateModuleConfig;
extern const std::string kInsertEndpoint;
extern const std::string kInsertSecondsBetweenRetry;
extern const std::string kInsertNotificationsByPriority;
extern const std::string kInsertDeviceData;
extern const std::string kInsertAppLevel;
extern const std::string kDeleteSecondsBetweenRetries;
extern const std::string kDeleteEndpoint;
extern const std::string kDeleteAppLevel;
extern const std::string kDeleteMessageString;
extern const std::string kDeleteFunctionalGroup;
extern const std::string kDeleteRpc;
extern const std::string kDeleteAppGroup;
extern const std::string kDeleteApplication;
extern const std::string kDeleteRequestType;
extern const std::string kDeleteDevice;
extern const std::string kIncrementIgnitionCycles;
extern const std::string kResetIgnitionCycles;
extern const std::string kUpdateFlagUpdateRequired;
extern const std::string kSelectFlagUpdateRequired;
extern const std::string kUpdateCountersSuccessfulUpdate;
extern const std::string kSelectApplicationRevoked;
extern const std::string kUpdateApplicationCustomData;
extern const std::string kSelectApplicationRepresented;
extern const std::string kSelectApplicationIsDefault;
extern const std::string kUpdateIsDefault;
extern const std::string kInsertInitData;
extern const std::string kDeleteAppGroupByApplicationId;
extern const std::string kInsertApplicationFull;
extern const std::string kDeletePreconsentedGroupsByApplicationId;
extern const std::string kSelectApplicationFull;
extern const std::string kUpdatePreloaded;
extern const std::string kSelectDBVersion;
extern const std::string kUpdateDBVersion;
}  // namespace sql_pt
}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_QUERIES_H_
