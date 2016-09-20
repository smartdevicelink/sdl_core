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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_EXT_QUERIES_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_EXT_QUERIES_H_

#include <string>

namespace policy {
namespace sql_pt_ext {

extern const std::string kSelectKeepContext;
extern const std::string kSelectStealFocus;
extern const std::string kResetDeviceConsents;
extern const std::string kResetAppConsents;
extern const std::string kCountDeviceConsentGroup;
extern const std::string kCountDevice;
extern const std::string kSelectDeviceConsentedGroup;
extern const std::string kUpdateDeviceConsentedGroup;
extern const std::string kUpdateDevice;
extern const std::string kInsertDeviceConsentedGroup;
extern const std::string kInsertDevice;
extern const std::string kSelectDeviceData;
extern const std::string kSelectConsentGroup;
extern const std::string kInsertPreconsentedGroups;
extern const std::string kSelectPreconsentedGroups;
extern const std::string kDeletePreconsentedGroups;
extern const std::string kSelectUsageAndErrorCount;
extern const std::string kSelectAppLevels;
extern const std::string kUpdateGlobalCounters;
extern const std::string kInsertDeviceData;
extern const std::string kDeleteDeviceData;
extern const std::string kInsertConsentGroups;
extern const std::string kCountUnconsentedGroups;
extern const std::string kSelectModuleMeta;
extern const std::string kUpdateMetaParams;
extern const std::string kUpdateModuleMetaVinParam;
extern const std::string kSaveModuleMeta;
extern const std::string kSelectMetaParams;
extern const std::string kCountAppLevel;
extern const std::string kUpdateGroupPermissions;
extern const std::string kSelectDefaultHmi;
extern const std::string kInsertApplication;
extern const std::string kSelectFriendlyMsg;
extern const std::string kCollectFriendlyMsg;
extern const std::string kSelectAppGroupsId;
extern const std::string kSelectConsentedGroupsId;
extern const std::string kCountAppConsents;
extern const std::string kSelectPreconsentedGroupsId;
extern const std::string kSelectFunctionalGroupNames;
extern const std::string kSelectAppPolicies;
extern const std::string kUpdateMetaLanguage;
extern const std::string kDeleteDeviceConsent;
extern const std::string kDeleteAppGroupConsent;
extern const std::string kSelectGroupId;
extern const std::string kSelectApplicationIsPreData;
extern const std::string kUpdateIsPredata;
extern const std::string kHasAppPreloadedGroups;
extern const std::string kUpdateUnpairedDevice;
extern const std::string kSelectUnpairedDevices;
extern const std::string kHasMsgLanguageCode;
extern const std::string kDeletePreconsentedGroupsByApplicationId;
extern const std::string kDeleteAppConsent;

}  // namespace sql_pt_ext
}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_EXT_QUERIES_H_
