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
#include <algorithm>
#include <utility>
#include "utils/logger.h"
#include "policy/sql_pt_ext_representation.h"
#include "policy/sql_wrapper.h"
#include "policy/sql_pt_queries.h"
#include "policy/sql_pt_ext_queries.h"
#include "policy/policy_helper.h"
#include "policy/cache_manager.h"

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "SQLPTRepresentation")

bool SQLPTExtRepresentation::CanAppKeepContext(const std::string& app_id) {
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectKeepContext)) {
    query.Bind(0, app_id);
    if (query.Exec()) {
      return query.GetBoolean(0);
    }
  }
  return false;
}

bool SQLPTExtRepresentation::CanAppStealFocus(const std::string& app_id) {
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectStealFocus)) {
    query.Bind(0, app_id);
    if (query.Exec()) {
      return query.GetBoolean(0);
    }
  }
  return false;
}

bool SQLPTExtRepresentation::ResetUserConsent() {
  return ResetDeviceConsents() && ResetAppConsents();
}

bool SQLPTExtRepresentation::ResetDeviceConsents() {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kResetDeviceConsents)) {
    LOG4CXX_WARN(logger_, "Incorrect delete statement from device_consents.");
    return false;
  }
  return query.Exec();
}

bool SQLPTExtRepresentation::ResetAppConsents() {
  return dbms::SQLQuery(db()).Exec(sql_pt_ext::kResetAppConsents);
}

bool SQLPTExtRepresentation::GetUserPermissionsForDevice(
  const std::string& device_id, StringArray* consented_groups,
  StringArray* disallowed_groups) {
  LOG4CXX_INFO(logger_, "GetUserPermissionsForDevice");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectDeviceConsentedGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect select from device consented groups");
    return false;
  }
  query.Bind(0, device_id);
  while (query.Next()) {
    if (query.GetBoolean(2)) {
      if (!consented_groups) {
        continue;
      }
      consented_groups->push_back(query.GetString(1));
    } else {
      if (!disallowed_groups) {
        continue;
      }
      disallowed_groups->push_back(query.GetString(1));
    }
  }

  return true;
}

bool SQLPTExtRepresentation::GetPermissionsForApp(
  const std::string& device_id, const std::string& policy_app_id,
  FunctionalIdType* group_types) {
  LOG4CXX_INFO(logger_, "GetPermissionsForApp");
  if (!group_types) {
    LOG4CXX_WARN(logger_, "Input parameter for group types is null.");
    return false;
  }
  // Get all app groups for specified device and application
  FunctionalGroupIDs all_groups;
  if (!GetAllAppGroups(policy_app_id, all_groups)) {
    return false;
  }
  // Get preconsented group
  FunctionalGroupIDs preconsented_groups;
  if (!GetPreconsentedGroups(policy_app_id, preconsented_groups)) {
    return false;
  }
  // Get consented (allowed/disallowed) groups
  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs disallowed_groups;
  if (!GetConsentedGroups(policy_app_id, device_id,
                          allowed_groups, disallowed_groups)) {
    return false;
  }
  // Get all default groups
  FunctionalGroupIDs default_groups;
  if (!GetAllAppGroups(kDefaultId, default_groups)) {
    return false;
  }

  // Get all pre_DataConsent groups
  FunctionalGroupIDs predataconsented_groups;
  if (!GetAllAppGroups(kPreDataConsentId, predataconsented_groups)) {
    return false;
  }

  // Get all device groups
  FunctionalGroupIDs device_groups;
  if (!GetAllAppGroups(kDeviceId, device_groups)) {
    return false;
  }

  (*group_types)[kTypeDefault] = default_groups;
  (*group_types)[kTypeAllowed] = allowed_groups;
  (*group_types)[kTypeDisallowed] = disallowed_groups;
  (*group_types)[kTypePreconsented] = preconsented_groups;
  (*group_types)[kTypeGeneral] = all_groups;
  (*group_types)[kTypePreDataConsented] = predataconsented_groups;
  (*group_types)[kTypeDevice] = device_groups;

  return true;
}

bool SQLPTExtRepresentation::GetDeviceGroupsFromPolicies(
  policy_table::Strings* groups, policy_table::Strings* preconsented_groups) {
  LOG4CXX_INFO(logger_, "GetDeviceGroupsFromPolicies");
  if (groups) {
    GatherAppGroup(kDeviceId, groups);
  }
  if (preconsented_groups) {
    GatherPreconsentedGroup(kDeviceId, preconsented_groups);
  }
  return true;
}

bool SQLPTExtRepresentation::SetDeviceData(const std::string& device_id,
    const std::string& hardware,
    const std::string& firmware,
    const std::string& os,
    const std::string& os_version,
    const std::string& carrier,
    const uint32_t number_of_ports,
    const std::string& connection_type) {
  LOG4CXX_INFO(logger_, "SetDeviceData");
  dbms::SQLQuery count_query(db());
  if (!count_query.Prepare(sql_pt_ext::kCountDevice)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for count of device.");
    return false;
  }

  count_query.Bind(0, device_id);

  if (!count_query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect count of device.");
    return false;
  }

  bool update = count_query.GetInteger(0);

  // Update old value
  if (update) {
    dbms::SQLQuery update_query(db());
    if (!update_query.Prepare(sql_pt_ext::kUpdateDevice)) {
      LOG4CXX_WARN(logger_, "Incorrect statement for udpate device.");
      return false;
    }

    update_query.Bind(0, hardware);
    update_query.Bind(1, firmware);
    update_query.Bind(2, os);
    update_query.Bind(3, os_version);
    update_query.Bind(4, carrier);
    update_query.Bind(5, static_cast<int>(number_of_ports));
    update_query.Bind(6, device_id);
    update_query.Bind(7, connection_type);

    if (!update_query.Exec() || !update_query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect update for device.");
      return false;
    }

    return true;
  }

  // Insert new data
  dbms::SQLQuery insert_query(db());
  if (!insert_query.Prepare(sql_pt_ext::kInsertDevice)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for device.");
    return false;
  }

  insert_query.Bind(0, device_id);
  insert_query.Bind(1, hardware);
  insert_query.Bind(2, firmware);
  insert_query.Bind(3, os);
  insert_query.Bind(4, os_version);
  insert_query.Bind(5, carrier);
  insert_query.Bind(6, static_cast<int>(number_of_ports));
  insert_query.Bind(7, connection_type);

  if (!insert_query.Exec() || !insert_query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert to device.");
    return false;
  }

  SetPreloaded(false);

  return true;
}

bool SQLPTExtRepresentation::SetUserPermissionsForDevice(
  const std::string& device_id, const StringArray& consented_groups,
  const StringArray& disallowed_groups) {
  LOG4CXX_TRACE(logger_, "SetUserPermissionsForDevice");
  dbms::SQLQuery count_query(db());
  if (!count_query.Prepare(sql_pt_ext::kCountDeviceConsentGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect count of device consented groups");
    return false;
  }

  count_query.Bind(0, device_id);

  if (!count_query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed count of device consented groups");
    return false;
  }

  bool update = count_query.GetInteger(0);

  // TODO(AOleynik): Split to several methods?
  dbms::SQLQuery query(db());
  // Update old values
  if (update) {
    if (!query.Prepare(sql_pt_ext::kUpdateDeviceConsentedGroup)) {
      LOG4CXX_WARN(logger_, "Incorrect statement for updating consented groups on device");
      return false;
    }

    StringArray::const_iterator it_consented_groups = consented_groups.begin();
    StringArray::const_iterator it_consented_groups_end =
      consented_groups.end();
    for (; it_consented_groups != it_consented_groups_end;
         ++it_consented_groups) {
      query.Bind(0, true);
      query.Bind(1, std::string("GUI"));
      query.Bind(2, device_id);
      query.Bind(3, *it_consented_groups);
      // TODO(AOleynik): Get this info from external data
      if (!query.Exec() || !query.Reset()) {
        LOG4CXX_WARN(logger_, "Failed update of device allowed consented groups.");
        return false;
      }
    }

    StringArray::const_iterator it_disallowed_groups =
      disallowed_groups.begin();
    StringArray::const_iterator it_disallowed_groups_end =
      disallowed_groups.end();
    for (; it_disallowed_groups != it_disallowed_groups_end;
         ++it_disallowed_groups) {
      query.Bind(0, false);
      query.Bind(1);
      query.Bind(2, device_id);
      query.Bind(3, *it_disallowed_groups);
      if (!query.Exec() || !query.Reset()) {
        LOG4CXX_WARN(logger_, "Failed update of device disallowed consented groups.");
        return false;
      }
    }

    return true;
  }

  // Insert new values
  if (!query.Prepare(sql_pt_ext::kInsertDeviceConsentedGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect statement of inserting to device consented groups");
    return false;
  }

  StringArray::const_iterator it_consented_groups = consented_groups.begin();
  StringArray::const_iterator it_consented_groups_end = consented_groups.end();
  for (; it_consented_groups != it_consented_groups_end;
       ++it_consented_groups) {
    query.Bind(0, device_id);
    query.Bind(1, *it_consented_groups);
    query.Bind(2, true);
    // TODO(AOleynik): Get this info from external data
    query.Bind(3, std::string("GUI"));
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Failed insert to device allowed consented groups.");
      return false;
    }
  }

  StringArray::const_iterator it_disallowed_groups = disallowed_groups.begin();
  StringArray::const_iterator it_disallowed_groups_end =
    disallowed_groups.end();
  for (; it_disallowed_groups != it_disallowed_groups_end;
       ++it_disallowed_groups) {
    query.Bind(0, device_id);
    query.Bind(1, *it_disallowed_groups);
    query.Bind(2, false);
    query.Bind(3);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Failed insert to device disallowed consented groups.");
      return false;
    }
  }

  return true;
}

bool SQLPTExtRepresentation::ReactOnUserDevConsentForApp(
  const std::string& app_id,
  bool is_device_allowed) {
  bool result = true;
  if (is_device_allowed) {
    // If app has pre_DataConsented groups it should be 'promoted' to default
    // If app has only pre_DataConsented flag it should be only set to false and
    // all groups get restored automatically
    if (IsPredataPolicy(app_id)) {
      dbms::SQLQuery query(db());
      if (!query.Prepare(sql_pt_ext::kHasAppPreloadedGroups)) {
        LOG4CXX_WARN(logger_, "Incorrect statement for has app preloaded groups");
        return false;
      }
      query.Bind(0, app_id);
      query.Bind(1, kPreDataConsentId);
      if (!query.Exec()) {
        LOG4CXX_WARN(logger_, "Incorrect select for app has predataconsted groups");
        return false;
      }
      if (query.GetInteger(0) > 0) {
        result = result && SetDefaultPolicy(app_id);
      } else {
        result = result && SetIsPredata(app_id, false);
      }
    }
  } else {
    // If app has default groups change them to pre_DataConsented
    // If app has 'normal' groups leave them as is and set
    // pre_DataConsented flag to true.
    if (IsDefaultPolicy(app_id)) {
      result = result && SetPredataPolicy(app_id);
    } else {
      result = result && SetIsPredata(app_id, true);
    }
  }
  return result;
}

bool SQLPTExtRepresentation::SetUserPermissionsForApp(
  const PermissionConsent& permissions) {
  LOG4CXX_INFO(logger_, "SetUserPermissionsForApp");
  // TODO(AOleynik): Handle situation, when no application was specified, i.e.
  // general permissions were set
  std::vector<FunctionalGroupPermission>::const_iterator it = permissions
      .group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end = permissions
      .group_permissions.end();

  dbms::SQLQuery query(db());
  for (; it != it_end; ++it) {
    dbms::SQLQuery counter(db());
    if (!counter.Prepare(sql_pt_ext::kCountAppConsents)) {
      LOG4CXX_WARN(logger_, "Incorrect statement for consent group count.");
      return false;
    }

    counter.Bind(0, permissions.device_id);
    counter.Bind(1, permissions.policy_app_id);
    counter.Bind(2, static_cast<int>((*it).group_id));
    if (!counter.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrent count on consent groups.");
      return false;
    }

    bool update_required = counter.GetInteger(0);

    // Update already present consent record
    if (update_required) {
      if (!query.Prepare(sql_pt_ext::kUpdateGroupPermissions)) {
        LOG4CXX_WARN(logger_, "Incorrect statement for update consent groups.");
        return false;
      }

      // Skip consent saving, if user didn't choose any state
      if (policy::kGroupUndefined == (*it).state) {
        continue;
      }
      query.Bind(0, (*it).state == kGroupAllowed ? 1 : 0);
      query.Bind(1, permissions.consent_source);
      query.Bind(2, permissions.policy_app_id);
      query.Bind(3, static_cast<int>((*it).group_id));
      query.Bind(4, permissions.device_id);

      if (!query.Exec() || !query.Reset()) {
        LOG4CXX_WARN(logger_, "Incorrect update on user defined permissions "
                     "for app groups.");
        return false;
      }
      continue;
    }

    // Insert new consent record
    if (!query.Prepare(sql_pt_ext::kInsertConsentGroups)) {
      LOG4CXX_WARN(logger_,
                   "Incorrect statement for update app group permissions.");
      return false;
    }

    // Skip consent saving, if user didn't choose any state
    if (policy::kGroupUndefined == (*it).state) {
      continue;
    }
    query.Bind(0, permissions.device_id);
    query.Bind(1, permissions.policy_app_id);
    query.Bind(2, static_cast<int>((*it).group_id));
    query.Bind(3, (*it).state == kGroupAllowed ? 1 : 0);
    query.Bind(4, permissions.consent_source);

    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert to user defined permissions "
                   "for app groups.");
      return false;
    }
    continue;
  }
  return true;
}

std::vector<UserFriendlyMessage> SQLPTExtRepresentation::GetUserFriendlyMsg(
  const std::vector<std::string>& msg_codes, const std::string& language) {  
  dbms::SQLQuery query(db());
  std::vector<UserFriendlyMessage> result;
  if (!query.Prepare(sql_pt_ext::kSelectFriendlyMsg)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for select friendly messages.");
    return result;
  }

  const std::string fallback_language = "en-us";
  std::vector<std::string>::const_iterator it = msg_codes.begin();
  std::vector<std::string>::const_iterator it_end = msg_codes.end();
  for (; it != it_end; ++it) {
    std::string msg_language = language;
    // If message has no records with required language, fallback language
    // should be used instead.
    if (!IsMsgLanguagePresent((*it), language)) {
      msg_language = fallback_language;
    }
    query.Bind(0, *it);
    query.Bind(1, msg_language);

    if (!query.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect select from friendly messages.");
      return result;
    }

    UserFriendlyMessage msg;

    msg.message_code = *it;
    msg.tts = query.GetString(0);
    msg.label = query.GetString(1);
    msg.line1 = query.GetString(2);
    msg.line2 = query.GetString(3);
    msg.text_body = query.GetString(4);

    result.push_back(msg);

    if (!query.Reset()) {
      LOG4CXX_WARN(logger_, "Failed reset statement for selecting friendly "
                   "messages.");
      return result;
    }
  }

  return result;
}

bool SQLPTExtRepresentation::GatherConsumerFriendlyMessages(
    policy_table::ConsumerFriendlyMessages* messages) const {
  if (NULL == messages) {
    LOG4CXX_ERROR(logger_, "NULL pointer has been passed to fill");
    return false;
  }

  if (!SQLPTRepresentation::GatherConsumerFriendlyMessages(messages)) {
    return false;
  }

  dbms::SQLQuery query(db());
  bool result = query.Prepare(sql_pt_ext::kCollectFriendlyMsg);

  if (result) {
    while (query.Next()) {

      UserFriendlyMessage msg;

      msg.tts = query.GetString(1);
      msg.label = query.GetString(2);
      msg.line1 = query.GetString(3);
      msg.line2 = query.GetString(4);
      msg.text_body = query.GetString(5);
      msg.message_code = query.GetString(7);

      std::string language = query.GetString(6);

      *(*messages->messages)[msg.message_code].languages[language].tts = msg.tts;
      *(*messages->messages)[msg.message_code].languages[language].label = msg.label;
      *(*messages->messages)[msg.message_code].languages[language].line1 = msg.line1;
      *(*messages->messages)[msg.message_code].languages[language].line2 = msg.line2;
      *(*messages->messages)[msg.message_code].languages[language].textBody = msg.text_body;
    }
  } else {
    LOG4CXX_WARN(logger_, "Incorrect statement for select friendly messages.");
  }
  return result;
}

bool SQLPTExtRepresentation::SetMetaInfo(const std::string& ccpu_version,
    const std::string& wers_country_code,
    const std::string& language) {
  LOG4CXX_TRACE_ENTER(logger_);
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateMetaParams)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for insert to module meta.");
    return false;
  }

  query.Bind(0, ccpu_version);
  query.Bind(1, wers_country_code);
  query.Bind(2, language);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert to module meta.");
    return false;
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool SQLPTExtRepresentation::IsMetaInfoPresent() {
  LOG4CXX_INFO(logger_, "IsMetaInfoPresent");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectMetaParams)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for selecting meta info.");
    return false;
  }

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect select from module meta.");
    return false;
  }

  return !query.IsNull(0) && !query.IsNull(1) && !query.IsNull(2);
}

bool SQLPTExtRepresentation::SetSystemLanguage(const std::string& language) {
  LOG4CXX_INFO(logger_, "SetSystemLanguage");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateMetaLanguage)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for update meta language.");
    return false;
  }

  query.Bind(0, language);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect update for meta language.");
    return false;
  }

  return true;
}

bool SQLPTExtRepresentation::SaveApplicationPolicies(
  const policy_table::ApplicationPolicies& apps) {
  LOG4CXX_INFO(logger_, "SaveApplicationPolicies ext");
  dbms::SQLQuery query_delete(db());
  if (!query_delete.Exec(sql_pt::kDeleteAppGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from app_group.");
    return false;
  }

  dbms::SQLQuery query_delete_preconsented(db());
  if (!query_delete_preconsented.Exec(sql_pt_ext::kDeletePreconsentedGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from preconsented_group.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteApplication)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from application.");
    return false;
  }

  // First, all predefined apps (e.g. default, pre_DataConsent) should be saved,
  // otherwise another app with the predefined permissions can get incorrect
  // permissions
  policy_table::ApplicationPolicies::const_iterator it_default =
      apps.find(kDefaultId);
  if (apps.end() != it_default) {
    if (!SaveSpecificAppPolicy(*it_default)) {
      return false;
    }
  }
  policy_table::ApplicationPolicies::const_iterator it_pre_data_consent =
      apps.find(kPreDataConsentId);
  if (apps.end() != it_pre_data_consent) {
    if (!SaveSpecificAppPolicy(*it_pre_data_consent)) {
      return false;
    }
  }
  policy_table::ApplicationPolicies::const_iterator it_device =
      apps.find(kDeviceId);
  if (apps.end() != it_device) {
    if (!SaveSpecificAppPolicy(*it_device)) {
      return false;
    }
  }

  policy_table::ApplicationPolicies::const_iterator it;  
  for (it = apps.begin(); it != apps.end(); ++it) {
    // Skip saving of predefined app, since they should be saved before
    if (IsPredefinedApp(*it)) {
      continue;
    }
    if (!SaveSpecificAppPolicy(*it)) {
      return false;
    }
  }

  return true;
}

bool SQLPTExtRepresentation::SaveSpecificAppPolicy(
    const policy_table::ApplicationPolicies::value_type& app) {
  if (app.second.is_string()) {
    if (kDefaultId.compare(app.second.get_string()) == 0) {
      if (!SetDefaultPolicy(app.first)) {
        return false;
      }
    } else if (kPreDataConsentId.compare(app.second.get_string()) == 0) {
      if (!SetPredataPolicy(app.first)) {
        return false;
      }
    }

    // Stop saving other params, since predefined permissions already set
    return true;
  }

  dbms::SQLQuery app_query(db());
  if (!app_query.Prepare(sql_pt_ext::kInsertApplication)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement into application.");
    return false;
  }

  app_query.Bind(0, app.first);
  app_query.Bind(1, app.second.keep_context);
  app_query.Bind(2, app.second.steal_focus);
  app_query.Bind(
    3, std::string(policy_table::EnumToJsonString(app.second.default_hmi)));
  app_query.Bind(
    4, std::string(policy_table::EnumToJsonString(app.second.priority)));
  app_query.Bind(
    5, app.second.is_null());
  app_query.Bind(6, *app.second.memory_kb);
  app_query.Bind(7, *app.second.heart_beat_timeout_ms);
  app.second.certificate.is_initialized() ?
  app_query.Bind(8, *app.second.certificate) : app_query.Bind(8, std::string());

  if (!app_query.Exec() || !app_query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into application.");
    return false;
  }

  if (!SaveAppGroup(app.first, app.second.groups)) {
    return false;
  }
  // TODO(IKozyrenko): Check logic if optional container is missing
  if (!SaveNickname(app.first, *app.second.nicknames)) {
    return false;
  }
  // TODO(IKozyrenko): Check logic if optional container is missing
  if (!SaveAppType(app.first, *app.second.AppHMIType)) {
    return false;
  }
  // TODO(IKozyrenko): Check logic if optional container is missing
  if (!SavePreconsentedGroup(app.first, *app.second.preconsented_groups)) {
    return false;
  }

  return true;
}

bool SQLPTExtRepresentation::GatherApplicationPolicies(
  policy_table::ApplicationPolicies* apps) const {
  LOG4CXX_INFO(logger_, "Gather applications policies");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectAppPolicies)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app_policies");
    return false;
  }

  while (query.Next()) {
    rpc::Nullable<policy_table::ApplicationParams> params;
    const std::string& app_id = query.GetString(0);
    if (IsApplicationRevoked(app_id)) {
      params.set_to_null();
      (*apps)[app_id] = params;
      continue;
    }
    if (IsDefaultPolicy(app_id)) {
      (*apps)[app_id].set_to_string(kDefaultId);
    }
    if (IsPredataPolicy(app_id)) {
      (*apps)[app_id].set_to_string(kPreDataConsentId);
    }
    policy_table::Priority priority;
    policy_table::EnumFromJsonString(query.GetString(1), &priority);
    params.priority = priority;
    policy_table::HmiLevel hmi;
    policy_table::EnumFromJsonString(query.GetString(2), &hmi);
    params.default_hmi = hmi;
    params.keep_context = query.GetBoolean(3);
    params.steal_focus = query.GetBoolean(4);
    *params.memory_kb = query.GetInteger(5);
    *params.heart_beat_timeout_ms = query.GetInteger(6);
    if (!query.IsNull(7)) {
      *params.certificate = query.GetString(7);
    }
    if (!GatherAppGroup(app_id, &params.groups)) {
      return false;
    }
    // TODO(IKozyrenko): Check logic if optional container is missing
    if (!GatherNickName(app_id, &*params.nicknames)) {
      return false;
    }
    // TODO(IKozyrenko): Check logic if optional container is missing
    if (!GatherAppType(app_id, &*params.AppHMIType)) {
      return false;
    }
    // TODO(IKozyrenko): Check logic if optional container is missing
    GatherPreconsentedGroup(app_id, &*params.preconsented_groups);
    (*apps)[app_id] = params;
  }
  return true;
}

void SQLPTExtRepresentation::GatherPreconsentedGroup(
  const std::string& app_id, policy_table::Strings* groups) const {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectPreconsentedGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect select from preconsented group");
    return;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    groups->push_back(query.GetString(0));
  }
}

bool SQLPTExtRepresentation::GatherUsageAndErrorCounts(
  policy_table::UsageAndErrorCounts* counts) const {
  LOG4CXX_INFO(logger_, "Gather Usage and Error Counts.");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectUsageAndErrorCount) || !query.Exec()) {
    LOG4CXX_INFO(logger_, "Failed select from user_and_error_count");
    return false;
  }

  *counts->count_of_iap_buffer_full = query.GetInteger(0);
  *counts->count_sync_out_of_memory = query.GetInteger(1);
  *counts->count_of_sync_reboots = query.GetInteger(2);

  return GatherAppLevels(&*counts->app_level);
}

bool SQLPTExtRepresentation::GatherAppLevels(
  policy_table::AppLevels* apps) const {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectAppLevels)) {
    LOG4CXX_INFO(logger_, "Failed select from app_level. SQLError = "
                 << query.LastError().text());
    return false;
  }
  while (query.Next()) {
    policy_table::AppLevel level;
    // value of time fields database is seconds
    level.minutes_in_hmi_full = query.GetInteger(1);
    level.minutes_in_hmi_limited = query.GetInteger(2);
    level.minutes_in_hmi_background = query.GetInteger(3);
    level.minutes_in_hmi_none = query.GetInteger(4);
    level.count_of_user_selections = query.GetInteger(5);
    level.count_of_rejections_sync_out_of_memory = query.GetInteger(6);
    level.count_of_rejections_nickname_mismatch = query.GetInteger(7);
    level.count_of_rejections_duplicate_name = query.GetInteger(8);
    level.count_of_rejected_rpc_calls = query.GetInteger(9);
    level.count_of_rpcs_sent_in_hmi_none = query.GetInteger(10);
    level.count_of_removals_for_bad_behavior = query.GetInteger(11);
    level.count_of_run_attempts_while_revoked = query.GetInteger(12);
    level.app_registration_language_gui = query.GetString(13);
    level.app_registration_language_vui = query.GetString(14);
    (*apps)[query.GetString(0)] = level;
  }

  return true;
}

void SQLPTExtRepresentation::GatherDeviceData(
  policy_table::DeviceData* data) const {
  LOG4CXX_INFO(logger_, "Gather device data.");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectDeviceData)) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for device data.");
    return;
  }
  data->mark_initialized();
  while (query.Next()) {
    policy_table::DeviceParams* specific_device = &(*data)[query.GetString(0)];
    *specific_device->hardware = query.GetString(1);
    *specific_device->firmware_rev = query.GetString(2);
    *specific_device->os = query.GetString(3);
    *specific_device->os_version = query.GetString(4);
    *specific_device->carrier = query.GetString(5);
    *specific_device->max_number_rfcom_ports = query.GetInteger(6);

    // TODO(IKozyrenko): Check logic if optional container is missing
    GatherConsentGroup(query.GetString(0),
                       &(*specific_device->user_consent_records));
  }
}

void SQLPTExtRepresentation::GatherConsentGroup(
  const std::string& device_id,
  policy_table::UserConsentRecords* records) const {
  LOG4CXX_INFO(logger_, "Gather consent records.");
  dbms::SQLQuery query(db());
  // Fill data for device
  if (!query.Prepare(sql_pt_ext::kSelectDeviceConsentedGroup)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect select statement for device consented groups.");
    return;
  }

  query.Bind(0, device_id);

  // Fill device_data -> user_consent_records -> "device"
  while (query.Next()) {
    policy_table::ConsentRecords* device_consent_records = &(*records)[kDeviceId];
    // TODO(IKozyrenko): Check logic if optional container is missing
    policy_table::ConsentGroups& consent_groups = *device_consent_records->consent_groups;
    consent_groups[query.GetString(1)] = query.GetBoolean(2);
    policy_table::Input input;
    policy_table::EnumFromJsonString(query.GetString(3), &input);
    *device_consent_records->input = input;
    *device_consent_records->time_stamp = query.GetString(4);
  }

  if (!query.Reset()) {
    return;
  }

  // Fill data for applications
  if (!query.Prepare(sql_pt_ext::kSelectConsentGroup)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect select statement for app consented groups.");
    return;
  }

  query.Bind(0, device_id);

  // Fill device_data -> user_consent_records -> <app_id>
  while (query.Next()) {
    policy_table::ConsentRecords* app_consent_records = &(*records)[query
        .GetString(1)];
    // TODO(IKozyrenko): Check logic if optional container is missing
    policy_table::ConsentGroups& consent_groups = *app_consent_records->consent_groups;

    consent_groups[query.GetString(2)] = query.GetBoolean(3);
    policy_table::Input input;
    policy_table::EnumFromJsonString(query.GetString(4), &input);
    *app_consent_records->input = input;
    *app_consent_records->time_stamp = query.GetString(5);
  }
}

bool SQLPTExtRepresentation::SaveDeviceData(
  const policy_table::DeviceData& devices) {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kInsertDeviceData)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for device data.");
    return false;
  }

  policy_table::DeviceData::const_iterator it = devices.begin();
  policy_table::DeviceData::const_iterator it_end = devices.end();
  for (; it != it_end; ++it) {
    query.Bind(0, it->first);
    query.Bind(1, *(it->second.hardware));
    query.Bind(2, *(it->second.firmware_rev));
    query.Bind(3, *(it->second.os));
    query.Bind(4, *(it->second.os_version));
    query.Bind(5, *(it->second.carrier));
    query.Bind(6, *(it->second.max_number_rfcom_ports));
    query.Bind(7, *(it->second.connection_type));

    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into device data.");
      return false;
    }

    // TODO(IKozyrenko): Check logic if optional container is missing
    if (!SaveConsentGroup(it->first, *it->second.user_consent_records)) {
      return false;
    }
  }

  return true;
}

bool SQLPTExtRepresentation::SaveConsentGroup(
  const std::string& device_id,
  const policy_table::UserConsentRecords& records) {
  LOG4CXX_INFO(logger_, "SaveConsentGroup");
  dbms::SQLQuery query(db());

  policy_table::UserConsentRecords::const_iterator it = records.begin();
  policy_table::UserConsentRecords::const_iterator it_end = records.end();
  for (; it != it_end; ++it) {
    // TODO(IKozyrenko): Check logic if optional container is missing
    policy_table::ConsentGroups::const_iterator it_groups = it->second
        .consent_groups->begin();
    policy_table::ConsentGroups::const_iterator it_groups_end = it->second
        .consent_groups->end();
    for (; it_groups != it_groups_end; ++it_groups) {
      if (kDeviceId == it->first) {
        if (!query.Prepare(sql_pt_ext::kInsertDeviceConsentedGroup)) {
          LOG4CXX_WARN(logger_,
                       "Incorrect insert statement for device consent group.");
          return false;
        }
        query.Bind(0, device_id);
        query.Bind(1, it_groups->first);
        query.Bind(2, it_groups->second);
        query.Bind(
          3,
          std::string(policy_table::EnumToJsonString(*(it->second.input))));
      } else {
        if (!query.Prepare(sql_pt_ext::kInsertConsentGroups)) {
          LOG4CXX_WARN(logger_,
                       "Incorrect insert statement for consent group.");
          return false;
        }
        query.Bind(0, device_id);
        query.Bind(1, it->first);
        query.Bind(2, it_groups->first);
        query.Bind(3, it_groups->second);
        query.Bind(
          4,
          std::string(policy_table::EnumToJsonString(*(it->second.input))));
      }

      if (!query.Exec() || !query.Reset()) {
        LOG4CXX_WARN(logger_, "Incorrect insert into consent group.");
        return false;
      }
    }
  }

  return true;
}

bool SQLPTExtRepresentation::SavePreconsentedGroup(
  const std::string& app_id, const policy_table::Strings& groups) {
  LOG4CXX_INFO(logger_, "SavePreconsentedGroup");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kInsertPreconsentedGroups)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect insert statement for preconsented groups");
    return false;
  }

  policy_table::Strings::const_iterator it;
  for (it = groups.begin(); it != groups.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into preconsented groups.");
      return false;
    }
  }

  return true;
}

void SQLPTExtRepresentation::GatherModuleMeta(
  policy_table::ModuleMeta* meta) const {
  LOG4CXX_INFO(logger_, "Gather Module Meta Info");
  dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt_ext::kSelectModuleMeta) && query.Next()) {
    *meta->ccpu_version = query.GetString(0);
    *meta->language = query.GetString(1);
    *meta->wers_country_code = query.GetString(2);
    *meta->pt_exchanged_at_odometer_x = query.GetInteger(3);
    *meta->pt_exchanged_x_days_after_epoch = query.GetInteger(4);
    *meta->ignition_cycles_since_last_exchange = query.GetInteger(5);
    *meta->vin = query.GetString(6);
  }
}

void SQLPTExtRepresentation::Increment(const std::string& type) const {
  dbms::SQLQuery query(db());
  std::string update_counter = "UPDATE `usage_and_error_count` SET `" + type
                               + "` = `" + type + "` + 1";
  if (!query.Exec(update_counter)) {
    LOG4CXX_INFO(logger_, "Failed updating global counter");
  }
}

bool SQLPTExtRepresentation::IsExistAppLevel(const std::string& app_id) const {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kCountAppLevel)) {
    LOG4CXX_INFO(logger_, "Incorrect statement of count app_level");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Failed count app_level");
    return false;
  }
  return query.GetInteger(0) > 0;
}

bool SQLPTExtRepresentation::GetAllAppGroups(const std::string& policy_app_id,
    FunctionalGroupIDs& all_groups) {
  LOG4CXX_INFO(logger_, "GetAllAppGroups for '" << policy_app_id << "'");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectAppGroupsId)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for select app groups id.");
    return false;
  }

  query.Bind(0, policy_app_id);

  while (query.Next()) {
    all_groups.push_back(query.GetInteger(0));
  }

  return true;
}

bool SQLPTExtRepresentation::GetConsentedGroups(
  const std::string& policy_app_id, const std::string& device_id,
  FunctionalGroupIDs& allowed_groups, FunctionalGroupIDs& disallowed_groups) {

  LOG4CXX_INFO(logger_, "GetConsentedGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectConsentedGroupsId)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for select consent groups id.");
    return false;
  }

  query.Bind(0, policy_app_id);
  query.Bind(1, device_id);

  while (query.Next()) {
    if (query.GetBoolean(1)) {
      allowed_groups.push_back(query.GetInteger(0));
    } else {
      disallowed_groups.push_back(query.GetInteger(0));
    }
  }

  return true;
}

bool SQLPTExtRepresentation::GetPreconsentedGroups(
  const std::string& policy_app_id, FunctionalGroupIDs& preconsented_groups) {
  LOG4CXX_INFO(logger_, "GetPreconsentedGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectPreconsentedGroupsId)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect statement for select preconsented groups id.");
    return false;
  }

  query.Bind(0, policy_app_id);

  while (query.Next()) {
    preconsented_groups.push_back(query.GetInteger(0));
  }

  return true;
}

bool SQLPTExtRepresentation::GetFunctionalGroupNames(
  FunctionalGroupNames& names) {
  LOG4CXX_INFO(logger_, "GetFunctionalGroupNames");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectFunctionalGroupNames)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect statement for select functional groups names.");
    return false;
  }

  while (query.Next()) {
    // Some of functional grous doesn't have filled user_consent_prompt
    if (query.IsNull(1)) {
      names[query.GetInteger(0)] =
        std::make_pair<std::string, std::string>("", query.GetString(2));
    } else {
      names[query.GetInteger(0)] =
        std::make_pair<std::string, std::string>(query.GetString(1), query.GetString(2));
    }

  }

  return true;
}

void SQLPTExtRepresentation::FillFunctionalGroupPermissions(
  FunctionalGroupIDs& ids, FunctionalGroupNames& names, GroupConsent state,
  std::vector<FunctionalGroupPermission>& permissions) {
  FunctionalGroupIDs::const_iterator it = ids.begin();
  FunctionalGroupIDs::const_iterator it_end = ids.end();
  for (; it != it_end; ++it) {
    FunctionalGroupPermission current_group;
    current_group.group_id = *it;
    current_group.group_alias = names[*it].first;
    current_group.group_name = names[*it].second;
    current_group.state = state;
    permissions.push_back(current_group);
  }
}

void SQLPTExtRepresentation::Increment(const std::string& app_id,
                                       const std::string& type) const {
  dbms::SQLQuery query(db());
  std::string sql_counter;
  if (IsExistAppLevel(app_id)) {
    // update
    sql_counter = "UPDATE `app_level` SET `" + type + "` = `" + type
                  + "` + 1 WHERE `application_id` = ?";
  } else {
    // insert
    sql_counter = "INSERT INTO `app_level` (`application_id`, `" + type + "`) "
                  "VALUES (?, 1)";
  }
  if (!query.Prepare(sql_counter)) {
    LOG4CXX_INFO(logger_, "Incorrect statement of update app counter");
    return;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Failed updating app counter");
  }
}

void SQLPTExtRepresentation::Set(const std::string& app_id,
                                 const std::string& type,
                                 const std::string& value) const {
  dbms::SQLQuery query(db());
  std::string sql_info;
  if (IsExistAppLevel(app_id)) {
    // update
    sql_info = "UPDATE `app_level` SET `" + type + "` = ? "
               "WHERE `application_id` = ?";
  } else {
    // insert
    sql_info = "INSERT INTO `app_level` (`" + type + "`, `application_id`) "
               "VALUES (?, ?)";
  }
  if (!query.Prepare(sql_info)) {
    LOG4CXX_INFO(logger_, "Incorrect statement of update app info");
    return;
  }
  query.Bind(0, value);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Failed updating app info");
  }
}

void SQLPTExtRepresentation::Add(const std::string& app_id,
                                 const std::string& type, int seconds) const {
  dbms::SQLQuery query(db());
  std::string sql_stopwatch;
  if (IsExistAppLevel(app_id)) {
    // update
    sql_stopwatch = "UPDATE `app_level` SET `" + type + "` = `" + type
                    + "` + ? WHERE `application_id` = ?";
  } else {
    // insert
    sql_stopwatch = "INSERT INTO `app_level` (`" + type
                    + "`, `application_id`) "
                    "VALUES (?, ?)";
  }
  if (!query.Prepare(sql_stopwatch)) {
    LOG4CXX_INFO(logger_, "Incorrect statement of update app stopwatch");
    return;
  }
  query.Bind(0, seconds);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Failed updating app stopwatch");
  }
}

bool SQLPTExtRepresentation::GetDefaultHMI(const std::string& policy_app_id,
    std::string* default_hmi) {
  LOG4CXX_INFO(logger_, "GetDefaultHMI");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectDefaultHmi)) {
    LOG4CXX_INFO(logger_, "Incorrect statement for default hmi.");
    return false;
  }

  query.Bind(0, policy_app_id);

  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Error during default hmi getting.");
    return false;
  }

  if (query.IsNull(0)) {
    default_hmi->clear();
    return true;
  }

  default_hmi->assign(query.GetString(0));

  return true;
}

bool SQLPTExtRepresentation::CountUnconsentedGroups(
  const std::string& policy_app_id,
  const std::string& device_id,
  int* result) const {
  LOG4CXX_INFO(logger_, "CountUnconsentedGroups");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kCountUnconsentedGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect select for unconsented groups.");
    return false;
  }

  query.Bind(0, policy_app_id);
  query.Bind(1, device_id);
  query.Bind(2, kDefaultId);
  query.Bind(3, kPreDataConsentId);

  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Error during executing unconsented groups.");
    return false;
  }
  *result = query.GetInteger(0);
  return true;
}

bool SQLPTExtRepresentation::IsMsgLanguagePresent(const std::string &message,
                                                  const std::string &language) {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kHasMsgLanguageCode)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for message language check.");
    return false;
  }

  query.Bind(0, message);
  query.Bind(1, language);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed to check message language code.");
    return false;
  }

  return query.GetInteger(0) != 0;
}

bool SQLPTExtRepresentation::SaveMessageString(
  const std::string& type, const std::string& lang,
  const policy_table::MessageString& strings) {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertMessageString)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for message.");
    return false;
  }

  query.Bind(0, *strings.tts);
  query.Bind(1, *strings.label);
  query.Bind(2, *strings.line1);
  query.Bind(3, *strings.line2);
  query.Bind(4, lang);
  query.Bind(5, type);
  query.Bind(6, *strings.textBody);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into message.");
    return false;
  }

  return true;
}

bool SQLPTExtRepresentation::SaveUsageAndErrorCounts(
    const policy_table::UsageAndErrorCounts& counts) {
  dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteAppLevel)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from app level.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertAppLevel)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app level.");
    return false;
  }

  policy_table::AppLevels::const_iterator it;
  const policy_table::AppLevels& app_levels = *counts.app_level;
  for (it = app_levels.begin(); it != app_levels.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second.minutes_in_hmi_full);
    query.Bind(2, it->second.minutes_in_hmi_limited);
    query.Bind(3, it->second.minutes_in_hmi_background);
    query.Bind(4, it->second.minutes_in_hmi_none);
    query.Bind(5, it->second.count_of_user_selections);
    query.Bind(6, it->second.count_of_rejections_sync_out_of_memory);
    query.Bind(7, it->second.count_of_rejections_nickname_mismatch);
    query.Bind(8, it->second.count_of_rejections_duplicate_name);
    query.Bind(9, it->second.count_of_rejected_rpc_calls);
    query.Bind(10, it->second.count_of_rpcs_sent_in_hmi_none);
    query.Bind(11, it->second.count_of_removals_for_bad_behavior);
    query.Bind(12, it->second.count_of_run_attempts_while_revoked);
    query.Bind(13, it->second.app_registration_language_gui);
    query.Bind(14, it->second.app_registration_language_vui);
    if (!query.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into app level.");
      return false;
    }
  }
  return true;
}

bool SQLPTExtRepresentation::CleanupUnpairedDevices(
  const DeviceIds& device_ids) const {
  LOG4CXX_INFO(logger_, "CleanupUnpairedDevices");
  dbms::SQLQuery delete_device_query(db());
  if (!delete_device_query.Prepare(sql_pt::kDeleteDevice)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for device delete.");
    return true;
  }

  dbms::SQLQuery delete_device_consent_query(db());
  if (!delete_device_consent_query.Prepare(sql_pt_ext::kDeleteDeviceConsent)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for delete device consent.");
    return false;
  }

  dbms::SQLQuery delete_app_consent_query(db());
  if (!delete_app_consent_query.Prepare(sql_pt_ext::kDeleteAppConsent)) {
    LOG4CXX_WARN(logger_, "Incorrect statement for delete app consent.");
    return false;
  }

  DeviceIds::const_iterator it = device_ids.begin();
  DeviceIds::const_iterator it_end = device_ids.end();
  for (; it != it_end; ++it) {
    delete_device_query.Bind(0, (*it));
    if (!delete_device_query.Exec() || !delete_device_query.Reset()) {
      LOG4CXX_WARN(logger_, "Failed to delete from device");
      return false;
    }

    delete_device_consent_query.Bind(0, (*it));
    if (!delete_device_consent_query.Exec() ||
        !delete_device_consent_query.Reset()) {
      LOG4CXX_WARN(logger_, "Failed to delete from device consent.");
      return false;
    }

    delete_app_consent_query.Bind(0, (*it));
    if (!delete_app_consent_query.Exec() || !delete_app_consent_query.Reset()) {
      LOG4CXX_WARN(logger_, "Failed to delete from app consent.");
      return false;
    }
  }
  return true;
}

bool SQLPTExtRepresentation::SetDefaultPolicy(const std::string& app_id) {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kDeleteAppGroupByApplicationId)) {
    LOG4CXX_ERROR(logger_, "Incorrect statement to delete from app_group.");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_, "Failed deleting from app_group.");
    return false;
  }

  if (!query.Prepare(sql_pt_ext::kDeletePreconsentedGroupsByApplicationId)) {
    LOG4CXX_ERROR(logger_, "Incorrect statement to delete from app_group.");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_, "Failed deleting from app_group.");
    return false;
  }

  if (!CopyApplication(kDefaultId, app_id)) {
    return false;
  }

  SetPreloaded(false);

  policy_table::Strings default_groups;
  policy_table::Strings default_preconsented_groups;
  GatherAppGroup(kDefaultId, &default_groups);
  GatherPreconsentedGroup(kDefaultId, &default_preconsented_groups);
  if (SaveAppGroup(app_id, default_groups) &&
      SavePreconsentedGroup(app_id, default_preconsented_groups)) {
    return SetIsDefault(app_id, true) && SetIsPredata(app_id, false);
  }

  return false;
}

bool SQLPTExtRepresentation::SetPredataPolicy(const std::string& app_id) {
  LOG4CXX_INFO(logger_, "SQLPTExtRepresentation::SetPredataPolicy for " << app_id);
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kDeleteAppGroupByApplicationId)) {
    LOG4CXX_ERROR(logger_, "Incorrect statement to delete from app_group.");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_, "Failed deleting from app_group.");
    return false;
  }

  if (!query.Prepare(sql_pt_ext::kDeletePreconsentedGroupsByApplicationId)) {
    LOG4CXX_ERROR(logger_, "Incorrect statement to delete from app_group.");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_, "Failed deleting from app_group.");
    return false;
  }

  if (!CopyApplication(kPreDataConsentId, app_id)) {
    return false;
  }

  SetPreloaded(false);

  policy_table::Strings predataconsent_groups;
  policy_table::Strings predataconsent_preconsented_groups;
  GatherAppGroup(kPreDataConsentId, &predataconsent_groups);
  GatherPreconsentedGroup(kPreDataConsentId, &predataconsent_groups);
  if (SaveAppGroup(app_id, predataconsent_groups) &&
    SavePreconsentedGroup(app_id, predataconsent_groups)) {
    return SetIsDefault(app_id, false) && SetIsPredata(app_id, true);
  }
  return false;
}

bool SQLPTExtRepresentation::IsPredataPolicy(const std::string& app_id) const {
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectApplicationIsPreData)) {
    LOG4CXX_WARN(logger_, "Incorrect select application is pre_dataConsented");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select application is pre_dataConsented");
    return false;
  }
  return query.IsNull(0) ? false : query.GetBoolean(0);
}

bool SQLPTExtRepresentation::SetIsPredata(const std::string& app_id,
    bool is_pre_data) {
  LOG4CXX_TRACE(logger_, "Set flag is_predata of application");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateIsPredata)) {
    LOG4CXX_WARN(logger_, "Incorect statement for updating is_predata");
    return false;
  }

  query.Bind(0, is_pre_data);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update is_predata");
    return false;
  }
  return true;
}

bool SQLPTExtRepresentation::SetUnpairedDevice(const std::string& device_id) const {
  LOG4CXX_TRACE(logger_, "Set unpaired device: " << device_id);
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateUnpairedDevice)) {
    LOG4CXX_WARN(logger_, "Incorect statement for updating unpaired device");
    return false;
  }

  query.Bind(0, true);
  query.Bind(1, device_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update unpaired device");
    return false;
  }
  return true;
}

bool SQLPTExtRepresentation::UnpairedDevicesList(DeviceIds* device_ids) const {
  LOG4CXX_TRACE(logger_, "Get list of unpaired devices");
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kSelectUnpairedDevices)) {
    LOG4CXX_WARN(logger_, "Incorect statement for selecting unpaired devices");
    return false;
  }

  while (query.Next()) {
    device_ids->push_back(query.GetString(0));
  }
  return true;
}

bool SQLPTExtRepresentation::SetVINValue(const std::string& value){
  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kUpdateModuleMetaVinParam)) {
    LOG4CXX_WARN(logger_, "Incorect statement for updating module_meta params");
    return false;
  }

  query.Bind(0, value);
  const bool result = query.Exec();

  if (!result) {
    LOG4CXX_WARN(logger_, "Failed update module_meta");
  }
  return result;
}

bool SQLPTExtRepresentation::RemoveAppConsentForGroup(
    const std::string& policy_app_id,
    const std::string& functional_group_name) const {
  dbms::SQLQuery query_group_id(db());
  if (!query_group_id.Prepare(sql_pt_ext::kSelectGroupId)) {
    LOG4CXX_WARN(logger_, "Incorect statement for select group name.");
    return false;
  }

  query_group_id.Bind(0, functional_group_name);

  if (!query_group_id.Exec()) {
    LOG4CXX_WARN(logger_, "Failed to select group id.");
    return false;
  }

  const int id = query_group_id.GetInteger(0);

  dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt_ext::kDeleteAppGroupConsent)) {
    LOG4CXX_WARN(logger_, "Incorect statement for remove app consent.");
    return false;
  }

  query.Bind(0, policy_app_id);
  query.Bind(1, id);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed to remove app consent.");
    return false;
  }

  return true;
}

}  // namespace policy

