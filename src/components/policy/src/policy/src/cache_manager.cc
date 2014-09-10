#include "policy/cache_manager.h"
#include "utils/file_system.h"
#include "json/reader.h"
#include "utils/logger.h"

#ifdef EXTENDED_POLICY
#  include "policy/sql_pt_ext_representation.h"
#else  // EXTENDED_POLICY
#  include "policy/sql_pt_representation.h"
#endif  // EXTENDED_POLICY


#include <algorithm>
#include <math.h>
#include <functional>

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "CacheManager")

CacheManager::CacheManager()
  : backup_(
      #ifdef EXTENDED_POLICY
                     new SQLPTExtRepresentation()
      #else  // EXTENDED_POLICY
                     new SQLPTRepresentation()
      #endif  // EXTENDED_POLICY
    ),
    update_required(false) {

}

bool CacheManager::CanAppKeepContext(const std::string &app_id) {
  bool result = true;
#ifdef EXTENDED_POLICY
  if (AppExists(app_id)) {
    result = pt->policy_table.app_policies[app_id].keep_context;
  }
#endif // EXTENDED_POLICY
  return result;
}

bool CacheManager::CanAppStealFocus(const std::string &app_id) {
  bool result = true;
#ifdef EXTENDED_POLICY
  if (AppExists(app_id)) {
    result = pt->policy_table.app_policies[app_id].steal_focus;
  }
#endif // EXTENDED_POLICY
  return result;
}

bool CacheManager::GetDefaultHMI(const std::string &app_id,
                                 std::string& default_hmi) {
  bool result = true;
#ifdef EXTENDED_POLICY
  default_hmi.clear();
  if (AppExists(app_id)) {
      default_hmi = EnumToJsonString(pt->policy_table.
                                     app_policies[app_id].default_hmi);
  }
  result = !default_hmi.empty();
#endif // EXTENDED_POLICY

  return result;
}

bool CacheManager::ResetUserConsent() {
#ifdef EXTENDED_POLICY
  policy_table::DeviceData::iterator iter =
      pt->policy_table.device_data->begin();
  policy_table::DeviceData::iterator iter_end =
      pt->policy_table.device_data->end();

  for (; iter != iter_end; ++iter) {
    (*iter).second.user_consent_records->clear();
  }
#endif // EXTENDED_POLICY
  return true;
}


bool CacheManager::GetUserPermissionsForDevice(const std::string &device_id,
                                               StringArray& consented_groups,
                                               StringArray& disallowed_groups) {

  LOG4CXX_INFO(logger_, "GetUserPermissionsForDevice");
#ifdef EXTENDED_POLICY
  const policy_table::DeviceParams& params = (*pt->policy_table.device_data)[device_id];
  const policy_table::UserConsentRecords& ucr = *(params.user_consent_records);
  policy_table::UserConsentRecords::const_iterator iter = ucr.begin();
  policy_table::UserConsentRecords::const_iterator iter_end = ucr.end();

  for (; iter != iter_end; ++iter) {
    policy_table::ConsentGroups::const_iterator con_iter;
    policy_table::ConsentGroups::const_iterator con_iter_end;

    con_iter = (*iter).second.consent_groups->begin();
    con_iter_end = (*iter).second.consent_groups->end();
    for (; con_iter != con_iter_end; ++con_iter) {
      if(true == (*con_iter).second) {
        consented_groups.push_back((*con_iter).first);
      } else {
        disallowed_groups.push_back((*con_iter).first);
      }
    }
  }
#endif // EXTENDED_POLICY
  return true;
}

void CacheManager::GetAllAppGroups(const std::string& app_id,
                                   FunctionalGroupIDs& all_group_ids) {

  LOG4CXX_INFO(logger_, "GetAllGroups");
#ifdef EXTENDED_POLICY
  policy_table::ApplicationPolicies::const_iterator app_params_iter =
      pt->policy_table.app_policies.find(app_id);

  if (pt->policy_table.app_policies.end() != app_params_iter) {
    policy_table::Strings::const_iterator iter =
        (*app_params_iter).second.groups.begin();
    policy_table::Strings::const_iterator iter_end =
        (*app_params_iter).second.groups.end();

    for (; iter != iter_end; ++iter) {
      const uint32_t group_id =
          static_cast<uint32_t> (abs(GenerateHash(*iter)));
      all_group_ids.push_back(group_id);
    }
  }
#endif // EXTENDED_POLICY
}

void CacheManager::GetPreConsentedGroups(const std::string &app_id,
                                         FunctionalGroupIDs& preconsented_groups) {

  LOG4CXX_INFO(logger_, "GetPreConsentedGroups");
#ifdef EXTENDED_POLICY
  policy_table::ApplicationPolicies::const_iterator app_param_iter =
      pt->policy_table.app_policies.find(app_id);
  if (pt->policy_table.app_policies.end() != app_param_iter) {

    policy_table::Strings::const_iterator iter = (*app_param_iter).second.preconsented_groups->begin();
    policy_table::Strings::const_iterator iter_end = (*app_param_iter).second.preconsented_groups->end();
    for (; iter != iter_end; ++iter) {
      const uint32_t group_id =
          static_cast<uint32_t> (abs(GenerateHash(*iter)));

      preconsented_groups.push_back(group_id);
    }
  }
#endif // EXTENDED_POLICY
}

void CacheManager::GetConsentedGroups(const std::string &device_id,
                                      const std::string &app_id,
                                      FunctionalGroupIDs& allowed_groups,
                                      FunctionalGroupIDs& disallowed_groups) {

  LOG4CXX_INFO(logger_, "GetConsentedGroups");
#ifdef EXTENDED_POLICY
  policy_table::DeviceData::iterator dev_params_iter =
      pt->policy_table.device_data->find(device_id);

  if (pt->policy_table.device_data->end() != dev_params_iter) {
    const policy_table::DeviceParams& dev_par = (*dev_params_iter).second;

    policy_table::UserConsentRecords::const_iterator iter =
        dev_par.user_consent_records->find(app_id);

    if (dev_par.user_consent_records->end() != iter) {
      policy_table::ConsentGroups::const_iterator consent_iter =
          (*iter).second.consent_groups->begin();
      policy_table::ConsentGroups::const_iterator consent_iter_end =
          (*iter).second.consent_groups->end();

      for (; consent_iter != consent_iter_end; ++consent_iter) {
        const uint32_t group_id =
            static_cast<uint32_t> (abs(GenerateHash((*consent_iter).first)));

        if (true == (*consent_iter).second) {
          allowed_groups.push_back(group_id);
        } else {
          disallowed_groups.push_back(group_id);
        }
      }
    }
  }
#endif // EXTENDED_POLICY
}

bool CacheManager::ApplyUpdate(const policy_table::Table& update_pt) {
  LOG4CXX_INFO(logger_, "ApplyUpdate");
#ifdef EXTENDED_POLICY
  pt->policy_table.functional_groupings =
      update_pt.policy_table.functional_groupings;

  pt->policy_table.app_policies = update_pt.policy_table.app_policies;
  pt->policy_table.module_config = update_pt.policy_table.module_config;

  if (update_pt.policy_table.consumer_friendly_messages.is_initialized()) {
    pt->policy_table.consumer_friendly_messages =
        update_pt.policy_table.consumer_friendly_messages;
  }
  Backup();
#endif // EXTENDED_POLICY
  return true;
}

void CacheManager::Backup() {
  LOG4CXX_INFO(logger_, "Start backup.");
  backup_->Save(*pt);
  backup_->SaveUpdateRequired(update_required);


  policy_table::ApplicationPolicies::const_iterator app_policy_iter =
      pt->policy_table.app_policies.begin();
  policy_table::ApplicationPolicies::const_iterator app_policy_iter_end =
      pt->policy_table.app_policies.end();

  for (; app_policy_iter != app_policy_iter_end; ++app_policy_iter) {

    const std::string app_id = (*app_policy_iter).first;
    backup_->SaveApplicationCustomData(app_id,
                                      is_revoked[app_id],
                                      is_default[app_id],
                                      is_predata[app_id]);
  }
  LOG4CXX_INFO(logger_, "End backup.");
}

bool CacheManager::GetPermissionsForApp(const std::string &device_id,
                                        const std::string &app_id,
                                        FunctionalIdType& group_types) {

  LOG4CXX_INFO(logger_, "GetPermissionFroApp");
#ifdef EXTENDED_POLICY
  GetAllAppGroups(app_id, group_types[kTypeGeneral]);
  GetPreConsentedGroups(app_id, group_types[kTypePreconsented]);
  GetAllAppGroups(kDefaultId, group_types[kTypeDefault]);

  GetConsentedGroups(device_id, app_id,
                     group_types[kTypeAllowed], group_types[kTypeDisallowed]);

  GetAllAppGroups(kPreDataConsentId, group_types[kTypePreDataConsented]);
  GetAllAppGroups(kDeviceId, group_types[kTypeDevice]);
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::GetDeviceGroupsFromPolicies(
  policy_table::Strings& groups,
  policy_table::Strings& preconsented_groups) {

  LOG4CXX_INFO(logger_, "GetDeviceGroupsFromPolicies");
#ifdef EXTENDED_POLICY
  if (AppExists(kDeviceId)) {
    groups = pt->policy_table.app_policies[kDeviceId].groups;
    preconsented_groups = *(pt->policy_table.app_policies[kDeviceId]).preconsented_groups;
  }
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::SetDeviceData(const std::string &device_id,
                                 const std::string &hardware,
                                 const std::string &firmware,
                                 const std::string &os,
                                 const std::string &os_version,
                                 const std::string &carrier,
                                 const uint32_t number_of_ports,
                                 const std::string &connection_type) {

  sync_primitives::AutoLock auto_lock(cache_lock_);
  LOG4CXX_INFO(logger_, "SetDeviceData");
#ifdef EXTENDED_POLICY
  policy_table::DeviceParams& params =
      (*(pt->policy_table.device_data))[device_id];
  *params.hardware = hardware;
  *params.firmware_rev = firmware;
  *params.os = os;
  *params.os_version = os_version;
  *params.carrier = carrier;
  *params.max_number_rfcom_ports = number_of_ports;
  *params.connection_type = connection_type;

#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::SetUserPermissionsForDevice(
    const std::string &device_id,
    const StringArray &consented_groups,
    const StringArray &disallowed_groups) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  LOG4CXX_INFO(logger_, "SetUserPermissionForDevice");
#ifdef EXTENDED_POLICY
  policy_table::DeviceParams& params = (*pt->policy_table.device_data)[device_id];
  policy_table::UserConsentRecords& ucr = *(params.user_consent_records);
  StringArray::const_iterator consent_iter_end = consented_groups.end();
  StringArray::const_iterator consent_iter = consented_groups.begin();
  StringArray::const_iterator un_consent_iter_end = disallowed_groups.end();
  StringArray::const_iterator un_consent_iter = disallowed_groups.begin();

  for (; consent_iter != consent_iter_end; ++consent_iter) {
    (*ucr[kDeviceId].consent_groups)[*consent_iter] = true;
  }

  for (; un_consent_iter != un_consent_iter_end; ++un_consent_iter) {
    (*ucr[kDeviceId].consent_groups)[*un_consent_iter] = false;
  }
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::ReactOnUserDevConsentForApp(const std::string &app_id,
                                               bool is_device_allowed) {

  LOG4CXX_INFO(logger_, "ReactOnUserDevConsentForApp");
  bool result = true;
#ifdef EXTENDED_POLICY
  if (is_device_allowed) {
    // If app has pre_DataConsented groups it should be 'promoted' to default
    // If app has only pre_DataConsented flag it should be only set to false and
    // all groups get restored automatically
    if (IsPredataPolicy(app_id)) {

      const policy_table::ApplicationParams& pre_data_app =
          pt->policy_table.app_policies[kPreDataConsentId];
      const policy_table::ApplicationParams& specific_app =
          pt->policy_table.app_policies[app_id];

      policy_table::Strings res;
      std::set_intersection(pre_data_app.groups.begin(),
                            pre_data_app.groups.end(),
                            specific_app.groups.begin(),
                            specific_app.groups.end(),
                            std::back_inserter(res),
                            std::less<std::string>());

      if (res.empty()) {
        result = SetIsPredata(app_id, false);
      } else {
        result = SetDefaultPolicy(app_id);
      }
    }
  } else {
    if (IsDefaultPolicy(app_id)) {
      result = SetPredataPolicy(app_id);
    } else {
      result = SetIsPredata(app_id, true);
    }
  }
#endif // EXTENDED_POLICY
  return result;
}

void CacheManager::GetGroupNameByHashID(const uint32_t group_id,
                                        std::string& group_name) {

  policy_table::FunctionalGroupings::const_iterator fg_iter =
      pt->policy_table.functional_groupings.begin();
  policy_table::FunctionalGroupings::const_iterator fg_iter_end =
      pt->policy_table.functional_groupings.end();

  for (; fg_iter != fg_iter_end; ++fg_iter) {
    const uint32_t id = abs(GenerateHash((*fg_iter).first));
    if (group_id == id) {
      group_name = (*fg_iter).first;
    }
  }
}

bool CacheManager::SetUserPermissionsForApp(
    const PermissionConsent &permissions) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  LOG4CXX_INFO(logger_, "SetUserPermissionForApp");
#ifdef EXTENDED_POLICY
  std::vector<FunctionalGroupPermission>::const_iterator iter =
      permissions.group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator iter_end =
      permissions.group_permissions.end();

  std::string group_name;
  for (;iter != iter_end; ++iter) {
    if (policy::kGroupUndefined != (*iter).state) {
      policy_table::DeviceParams& params = (*pt->policy_table.device_data)[permissions.device_id];
      rpc::policy_table_interface_base::ConsentRecords& ucr =
          (*params.user_consent_records)[permissions.policy_app_id];

      GetGroupNameByHashID((*iter).group_id, group_name);

      (*ucr.consent_groups)[group_name] = ((*iter).state == policy::kGroupAllowed);
    }
  }
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::UpdateRequired() const {
  return update_required;
}

void CacheManager::SaveUpdateRequired(bool status) {

  update_required = status;
}

bool CacheManager::IsApplicationRevoked(const std::string app_id) {

  LOG4CXX_INFO(logger_, "IsAppRevoked");
  const bool app_revoked = is_revoked.end() != is_revoked.find(app_id) &&
                           is_revoked[app_id];
  return app_revoked;
}

CheckPermissionResult CacheManager::CheckPermissions(const PTString &app_id,
                                                     const PTString &hmi_level,
                                                     const PTString &rpc) {


  // It is Genivi
  return CheckPermissionResult();
}

bool CacheManager::IsPTPreloaded() {
  return *pt->policy_table.module_config.preloaded_pt;
}

int CacheManager::IgnitionCyclesBeforeExchange() {

  const uint8_t limit = std::max(
        static_cast<int>(
          pt->policy_table.module_config.exchange_after_x_ignition_cycles), 0);

  uint8_t current = 0;

#ifdef EXTENDED_POLICY
  const int last_exch = static_cast<int>
      (*pt->policy_table.module_meta->ignition_cycles_since_last_exchange);
  current = std::max(last_exch, 0);
#endif // EXTENDED_POLICY

  return std::max(limit - current, 0);
}

int CacheManager::KilometersBeforeExchange(int current) {

  const uint8_t limit = std::max(
        static_cast<int>(
          pt->policy_table.module_config.exchange_after_x_kilometers), 0);
  uint8_t last = 0;

#ifdef EXTENDED_POLICY
  const int odo_val = static_cast<int>
      (*pt->policy_table.module_meta->pt_exchanged_at_odometer_x);
  last = std::max(odo_val, 0);
#endif // EXTENDED_POLICY

  const uint8_t actual = std::max((current - last), 0);
  return std::max(limit - actual, 0);
}

bool CacheManager::SetCountersPassedForSuccessfulUpdate(int kilometers,
                                                        int days_after_epoch) {

#ifdef EXTENDED_POLICY
  *pt->policy_table.module_meta->pt_exchanged_at_odometer_x = kilometers;
  *pt->policy_table.module_meta->pt_exchanged_x_days_after_epoch = days_after_epoch;
#endif
  return true;
}

int CacheManager::DaysBeforeExchange(int current) {

  const uint8_t limit = std::max(
        static_cast<int>(
          pt->policy_table.module_config.exchange_after_x_days), 0);
  uint8_t last = 0;

#ifdef EXTENDED_POLICY
  const int odo_val = static_cast<int>
      (*pt->policy_table.module_meta->pt_exchanged_x_days_after_epoch);
  last = std::max(odo_val, 0);
#endif // EXTENDED_POLICY

  const uint8_t actaul = std::max((current - last), 0);
  return std::max(limit - actaul, 0);
}

void CacheManager::IncrementIgnitionCycles() {

#ifdef EXTENDED_POLICY
  const int ign_val = static_cast<int>
      (*pt->policy_table.module_meta->ignition_cycles_since_last_exchange);
  (*pt->policy_table.module_meta->ignition_cycles_since_last_exchange) = ign_val + 1;
#endif // EXTENDED_POLICY
}

void CacheManager::ResetIgnitionCycles() {

#ifdef EXTENDED_POLICY
  (*pt->policy_table.module_meta->ignition_cycles_since_last_exchange) = 0;
#endif // EXTENDED_POLICY
}

int CacheManager::TimeoutResponse() {
  return pt->policy_table.module_config.timeout_after_x_seconds;
}

bool CacheManager::SecondsBetweenRetries(std::vector<int>& seconds) {

  rpc::policy_table_interface_base::SecondsBetweenRetries::iterator iter =
    pt->policy_table.module_config.seconds_between_retries.begin();
  rpc::policy_table_interface_base::SecondsBetweenRetries::iterator iter_end =
    pt->policy_table.module_config.seconds_between_retries.end();

  const std::size_t size =
      pt->policy_table.module_config.seconds_between_retries.size();
  seconds.reserve(size);
  for (;iter != iter_end; ++iter) {
    seconds.push_back(*iter);
  }
  return true;
}

VehicleData CacheManager::GetVehicleData() {
  // TODO(AGaliuzov): maybe should be removed.
  return VehicleData();
}

std::vector<UserFriendlyMessage> CacheManager::GetUserFriendlyMsg(
    const std::vector<std::string> &msg_codes, const std::string &language) {

  LOG4CXX_INFO(logger_, "GetUserFriendlyMsg");
  std::vector<UserFriendlyMessage> result;

  const std::string fallback_language = "en-us";
  std::vector<std::string>::const_iterator it = msg_codes.begin();
  std::vector<std::string>::const_iterator it_end = msg_codes.end();
  for (; it != it_end; ++it) {

#ifdef EXTENDED_POLICY
    std::string msg_language = language;

    rpc::policy_table_interface_base::MessageLanguages msg_languages =
        (*pt->policy_table.consumer_friendly_messages->messages)[*it];

    // If message has no records with required language, fallback language
    // should be used instead.
    if (msg_languages.languages.end() ==
        msg_languages.languages.find(msg_language)) {
      msg_language = fallback_language;
    }

    rpc::policy_table_interface_base::MessageString message_string =
        msg_languages.languages[msg_language];

    UserFriendlyMessage msg;
    msg.message_code = *it;
    msg.tts = *message_string.tts;
    msg.label = *message_string.label;
    msg.line1 = *message_string.line1;
    msg.line2 = *message_string.line2;
    msg.text_body = *message_string.textBody;

    result.push_back(msg);
#else
    UserFriendlyMessage msg;
    msg.message_code = *it;
    result.push_back(msg);
#endif // EXTENDED_POLICY
  }

  return result;
}

EndpointUrls CacheManager::GetUpdateUrls(int service_type) {

  // TODO ask Andrey;
  EndpointUrls result;
  char buff[32];
  sprintf(buff, "%x", service_type);

  std::string serv_type(buff);
  // TODO: remove this workaround
  if (service_type <= 0x9) {
    serv_type.insert(0,"0x0", 3);
  } else {
    serv_type.insert(0,"0x", 2);
  }

  policy_table::ServiceEndpoints::const_iterator iter =
      pt->policy_table.module_config.endpoints.find(serv_type);

  if (pt->policy_table.module_config.endpoints.end() != iter) {
    policy_table::URLList::const_iterator url_list_iter = (*iter).second.begin();
    policy_table::URLList::const_iterator url_list_iter_end = (*iter).second.end();
    for (; url_list_iter != url_list_iter_end; ++url_list_iter) {
      EndpointData data;
      data.app_id = (*url_list_iter).first;
      std::copy((*url_list_iter).second.begin(), (*url_list_iter).second.end(),
              std::back_inserter(data.url));

      result.push_back(data);
    }
  }
  return result;
}

int CacheManager::GetNotificationsNumber(const std::string &priority) {

  typedef rpc::policy_table_interface_base::NumberOfNotificationsPerMinute NNPM;

  const NNPM& nnpm =
      pt->policy_table.module_config.notifications_per_minute_by_priority;

  NNPM::const_iterator priority_iter = nnpm.find(priority);

  const int result = (nnpm.end() != priority_iter ? (*priority_iter).second : 0);
  return result;
}

bool CacheManager::GetPriority(const std::string &policy_app_id,
                               std::string &priority) {

  const policy_table::ApplicationPolicies& policies =
      pt->policy_table.app_policies;

  policy_table::ApplicationPolicies::const_iterator policy_iter = policies.find(policy_app_id);
  const bool app_id_exists = policies.end() != policy_iter;
  if (app_id_exists) {
    priority = EnumToJsonString((*policy_iter).second.priority);
  }

  return app_id_exists;
}

utils::SharedPtr<policy_table::Table>
CacheManager::GenerateSnapshot() const {
  return pt;
}

bool CacheManager::GetInitialAppData(const std::string& app_id,
                                     StringArray& nicknames,
                                     StringArray& app_hmi_types) {

  LOG4CXX_INFO(logger_, "GetInitialAppData");
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      pt->policy_table.app_policies.find(app_id);

  if (pt->policy_table.app_policies.end() != policy_iter) {
    const policy_table::ApplicationParams& app_params = (*policy_iter).second;

    std::copy(app_params.nicknames->begin(), app_params.nicknames->end(),
              std::back_inserter(nicknames));

    std::transform(app_params.AppHMIType->begin(), app_params.AppHMIType->end(),
                   std::back_inserter(nicknames), AppHMITypeToString());
  }
  return true;
}

bool CacheManager::GetFunctionalGroupings(
    policy_table::FunctionalGroupings& groups) {

  LOG4CXX_INFO(logger_, "GetFunctionalGroupings");
  const policy_table::FunctionalGroupings& f_groupings =
    pt->policy_table.functional_groupings;

  groups.insert(f_groupings.begin(), f_groupings.end());

  return true;
}

bool CacheManager::CountUnconsentedGroups(const std::string& policy_app_id,
                                          const std::string& device_id,
                                          int& result) {

#ifdef EXTENDED_POLICY
  LOG4CXX_INFO(logger_, "CountUnconsentedGroups");
  policy_table::DeviceParams& params = (*pt->policy_table.device_data)[device_id];
  policy_table::UserConsentRecords& ucr = *(params.user_consent_records);

  policy_table::ConsentRecords cgr = ucr[policy_app_id];

  policy_table::ConsentGroups::const_iterator con_iter = cgr.consent_groups->begin();
  policy_table::ConsentGroups::const_iterator con_iter_end = cgr.consent_groups->end();
  policy_table::FunctionalGroupings::const_iterator groups_iter =
      pt->policy_table.functional_groupings.begin();
  policy_table::FunctionalGroupings::const_iterator groups_iter_end =
      pt->policy_table.functional_groupings.end();
  for (; groups_iter != groups_iter_end; ++groups_iter) {
    con_iter = cgr.consent_groups->find((*groups_iter).first);
    if (con_iter != cgr.consent_groups->end()) {
      ++result;
    }
  }
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::SetMetaInfo(const std::string &ccpu_version,
                               const std::string &wers_country_code,
                               const std::string &language) {

#ifdef EXTENDED_POLICY
  *pt->policy_table.module_meta->ccpu_version = ccpu_version;
  *pt->policy_table.module_meta->wers_country_code = wers_country_code;
  *pt->policy_table.module_meta->language = language;
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::IsMetaInfoPresent() const {
  bool result = true;
#ifdef EXTENDED_POLICY
  result = NULL != pt->policy_table.module_meta->ccpu_version &&
      NULL != pt->policy_table.module_meta->wers_country_code &&
      NULL != pt->policy_table.module_meta->language;
#endif // EXTENDED_POLICY
  return result;
}

bool CacheManager::SetSystemLanguage(const std::string &language) {

#ifdef EXTENDED_POLICY
  *pt->policy_table.module_meta->language = language;
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::GetFunctionalGroupNames(FunctionalGroupNames &names) {

  LOG4CXX_INFO(logger_, "GetFunctionalGroupNames");
#ifdef EXTENDED_POLICY
  rpc::policy_table_interface_base::FunctionalGroupings::iterator iter =
      pt->policy_table.functional_groupings.begin();
  rpc::policy_table_interface_base::FunctionalGroupings::iterator iter_end =
      pt->policy_table.functional_groupings.end();

  for (; iter != iter_end; ++iter) {
    const uint32_t id = static_cast<uint32_t> (abs(GenerateHash((*iter).first)));
    std::pair<std::string, std::string> value =
        std::make_pair( *(*iter).second.user_consent_prompt, (*iter).first);

    names.insert(std::pair<uint32_t, std::pair<std::string, std::string> >(id, value));
  }
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::CleanupUnpairedDevices(const DeviceIds &device_ids) {

#ifdef EXTENDED_POLICY
  DeviceIds::const_iterator iter = device_ids.begin();
  DeviceIds::const_iterator iter_end = device_ids.end();
  for (; iter != iter_end; ++iter) {
    is_unpaired.erase(*iter);
  }
#endif // EXTENDED_POLICY
  return true;
}

void CacheManager::Increment(const std::string &type) {
#ifdef EXTENDED_POLICY
  // TODO: think about it.
  if (0 == type.compare("count_of_iap_buffer_full")) {
    *pt->policy_table.usage_and_error_counts->count_of_iap_buffer_full = 1;
  } else if (0 == type.compare("count_sync_out_of_memory")) {
    *pt->policy_table.usage_and_error_counts->count_sync_out_of_memory = 1;
  } else if (0 == type.compare("count_of_sync_reboots")) {
    *pt->policy_table.usage_and_error_counts->count_of_sync_reboots = 1;
  }
#endif // EXTENDED_POLICY
}

void CacheManager::Increment(const std::string &app_id,
                             const std::string &type) const {

}

void CacheManager::Set(const std::string &app_id,
                       const std::string &type,
                       const std::string &value) const {

}

void CacheManager::Add(const std::string &app_id,
                       const std::string &type,
                       int seconds) const {

}

bool CacheManager::SetDefaultPolicy(const std::string &app_id) {

  policy_table::ApplicationPolicies::const_iterator iter = pt->policy_table.app_policies.find(kDefaultId);
  if (pt->policy_table.app_policies.end() != iter) {
    pt->policy_table.app_policies[app_id] =
        pt->policy_table.app_policies[kDefaultId];

    CopyInternalParams(kDefaultId, app_id);
  }

  return true;
}

bool CacheManager::IsDefaultPolicy(const std::string& app_id) {

  const bool result =
      is_default.end() != is_default.find(app_id) && is_default[app_id];
  return result;
}

bool CacheManager::SetIsDefault(const std::string& app_id,
                                bool is_default) {

  if (this->is_default.end() != this->is_default.find(app_id)) {
    this->is_default[app_id] = is_default;
  }
  return true;
}

void CacheManager::CopyInternalParams(const std::string &from,
                                      const std::string& to) {

  is_predata[to] = is_predata[from];
  is_default[to] = is_default[from];
  is_revoked[to] = is_revoked[from];
}

bool CacheManager::SetPredataPolicy(const std::string &app_id) {

  policy_table::ApplicationPolicies::const_iterator iter =
      pt->policy_table.app_policies.find(kPreDataConsentId);

  if (pt->policy_table.app_policies.end() != iter) {
    pt->policy_table.app_policies[app_id] =
        pt->policy_table.app_policies[kPreDataConsentId];

    CopyInternalParams(kPreDataConsentId, app_id);
  }

  return true;
}

bool CacheManager::SetIsPredata(const std::string &app_id, bool is_pre_data) {

  is_predata[app_id] = is_pre_data;
  return true;
}

bool CacheManager::IsPredataPolicy(const std::string &app_id) {
  const bool result =
      (is_predata.end() != is_predata.find(app_id)) && is_predata[app_id];
  return result;
}

bool CacheManager::SetUnpairedDevice(const std::string &device_id) {

  const bool result = is_predata.end() != is_unpaired.find(device_id);
  if (result) {
    is_unpaired[device_id] = true;
  }
  return result;
}

bool CacheManager::UnpairedDevicesList(DeviceIds& device_ids) {

  std::map<std::string, bool>::const_iterator iter = is_unpaired.begin();
  std::map<std::string, bool>::const_iterator iter_end = is_unpaired.end();

  for (; iter != iter_end; ++iter) {
    if ((*iter).second) {
      device_ids.push_back((*iter).first);
    }
  }
  return true;
}

bool CacheManager::SetVINValue(const std::string& value) {

#ifdef EXTENDED_POLICY
  *pt->policy_table.module_meta->vin = value;
#endif // EXTENDED_POLICY
  return true;
}

bool CacheManager::IsApplicationRepresented(const std::string& app_id) const {
  policy_table::ApplicationPolicies::const_iterator iter =
      pt->policy_table.app_policies.find(app_id);
  return pt->policy_table.app_policies.end() != iter;
}

void CacheManager::SetPT(
    utils::SharedPtr<rpc::policy_table_interface_base::Table> table) {

  pt = table;
}

bool CacheManager::Init(const std::string& file_name) {
  LOG4CXX_INFO(logger_, "CacheManager::Init");
  InitResult init_result = backup_->Init();
#ifdef EXTENDED_POLICY
  ex_backup_ = utils::SharedPtr<PTRepresentation>::
      dynamic_pointer_cast<PTExtRepresentation >(backup_);
#endif // EXTENDED_POLICY

  bool result = true;
  switch (init_result) {
    case InitResult::EXISTS: {
      LOG4CXX_INFO(logger_, "Policy Table exists, was loaded correctly.");
      result = LoadFromBackup();
    } break;
    case InitResult::SUCCESS: {
      LOG4CXX_INFO(logger_, "Policy Table was inited successfully");
      result = LoadFromFile(file_name);
    } break;
    default: {
      result = false;
      LOG4CXX_ERROR(logger_, "Failed to init policy table.");
    } break;
  }

  return result;
}

void CacheManager::FillAppSpecificData() {

  policy_table::ApplicationPolicies::const_iterator iter =
      pt->policy_table.app_policies.begin();
  policy_table::ApplicationPolicies::const_iterator iter_end =
      pt->policy_table.app_policies.begin();

  for (; iter != iter_end; ++iter) {
    const std::string& app_name = (*iter).first;

    is_predata.insert(std::make_pair(app_name, backup_->IsPredataPolicy(app_name)));
    is_default.insert(std::make_pair(app_name, backup_->IsDefaultPolicy(app_name)));
    is_revoked.insert(std::make_pair(app_name, backup_->IsApplicationRevoked(app_name)));
  }
}

void CacheManager::FillDeviceSpecificData() {

  DeviceIds unpaired_ids;
  ex_backup_->UnpairedDevicesList(&unpaired_ids);
  for (DeviceIds::const_iterator ids_iter= unpaired_ids.begin();
       ids_iter!= unpaired_ids.end(); ++ids_iter) {
    is_unpaired[*ids_iter] = true;
  }
}

bool CacheManager::LoadFromBackup() {
  pt = backup_->GenerateSnapshot();
  update_required = backup_->UpdateRequired();

  FillAppSpecificData();
  FillDeviceSpecificData();

  return true;
}

bool CacheManager::LoadFromFile(const std::string& file_name) {

  LOG4CXX_INFO(logger_, "CacheManager::LoadFromFile");
  BinaryMessage json_string;
  bool final_result = false;
  final_result = file_system::ReadBinaryFile(file_name, json_string);
  if (!final_result) {
    LOG4CXX_WARN(logger_, "Failed to read pt file.");
    return final_result;
  }

  Json::Value value;
  Json::Reader reader;
  std::string json(json_string.begin(), json_string.end());
  if (reader.parse(json.c_str(), value)) {
    pt = new policy_table::Table(&value);
  }

  if (!pt) {
    LOG4CXX_WARN(logger_, "Failed to parse policy table");
    //utils::SharedPtr<policy_table::Table> table = new policy_table::Table();
    return false;
  }

  if (!pt->is_valid()) {
    rpc::ValidationReport report("policy_table");
    pt->ReportErrors(&report);
    LOG4CXX_WARN(logger_, "Parsed table is not valid " <<
                 rpc::PrettyFormat(report));
  }

  final_result = backup_->Save(*pt);
  LOG4CXX_INFO(
    logger_,
    "Loading from file was " << (final_result ? "successful" : "unsuccessful"));
  return final_result;
}

bool CacheManager::ResetPT(const std::string& file_name) {
  bool result = true;
#ifdef EXTENDED_POLICY
  result = LoadFromFile(file_name);
#endif // EXTENDE_POLICY
  return result;
}

bool CacheManager::AppExists(const std::string &app_id) const {
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt->policy_table.app_policies.find(app_id);
  return pt->policy_table.app_policies.end() != policy_iter;
}

unsigned long CacheManager::GenerateHash(const std::string& str_to_hash) {

  unsigned long hash = 5381U;
  std::string::const_iterator it = str_to_hash.begin();
  std::string::const_iterator it_end = str_to_hash.end();

  for (;it != it_end; ++it) {
       hash = ((hash << 5) + hash) + (*it);
  }

  return hash;
}

}
