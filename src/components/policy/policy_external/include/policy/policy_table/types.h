/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_TABLE_TYPES_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_TABLE_TYPES_H_

#include <climits>
#include "utils/macro.h"

#include "policy/policy_table/enums.h"
#include "policy/policy_table/policy_enum_schema_factory.h"
#include "rpc_base/rpc_message.h"
#include "utils/helpers.h"

namespace Json {
class Value;
}  // namespace Json
namespace rpc {
namespace policy_table_interface_base {
struct AppLevel;
struct ApplicationParams;
struct ConsentRecords;
struct DeviceParams;
struct EndpointProperty;
struct MessageLanguages;
struct MessageString;
struct RpcParameters;
struct Rpcs;
struct AppServiceHandledRpc;
struct AppServiceInfo;
}  // namespace policy_table_interface_base
}  // namespace rpc

namespace rpc {
namespace policy_table_interface_base {

// According to HMI API
#define ODO_MAX 17000000

typedef Array<String<1, 255>, 0, 255> Strings;

typedef Array<Enum<AppHMIType>, 0, 255> AppHMITypes;

typedef Array<Enum<HmiLevel>, 0, 4> HmiLevels;

typedef Array<String<0, 255>, 0, 100> Parameters;

typedef Map<RpcParameters, 0, UINT_MAX> Rpc;

typedef Array<String<10, INT_MAX>, 1, 3> URL;

typedef Map<URL, 1, 255> URLList;

typedef Map<URLList, 1, 255> ServiceEndpoints;

typedef Map<EndpointProperty, 0, 255> ServiceEndpointProperties;

typedef uint8_t NumberOfNotificationsType;
typedef Map<Integer<NumberOfNotificationsType, 0, 255>, 0, 7>
    NumberOfNotificationsPerMinute;

typedef Array<Integer<uint16_t, 1, 1000>, 0, 5> SecondsBetweenRetries;

typedef Map<MessageString, 0, 600> Languages;

typedef Map<MessageLanguages, 0, 255> Messages;

typedef Map<AppLevel, 0, 255> AppLevels;

typedef Map<Boolean, 0, 255> ConsentGroups;

typedef Map<ConsentRecords, 0, 1000> UserConsentRecords;

typedef Map<Stringifyable<Nullable<ApplicationParams> >, 1, 1000>
    ApplicationPolicies;

typedef Map<Rpcs, 1, 255> FunctionalGroupings;

typedef Map<DeviceParams, 0, 255> DeviceData;

typedef Array<Enum<RequestType>, 0, 255> RequestsTypeArray;

typedef Strings RequestSubTypes;

typedef String<0, 255> AppServiceType;
typedef String<0, 255> AppServiceName;
typedef Array<AppServiceName, 0, 255> AppServiceNames;
typedef Array<AppServiceHandledRpc, 0, 255> AppServiceHandledRpcs;
typedef Map<AppServiceInfo, 0, 255> AppServiceParameters;
typedef Integer<int32_t, 0, INT32_MAX> FunctionIDInt;

typedef Map<Strings, 0, 255> RemoteRpcs;
typedef Map<RemoteRpcs, 0, 255> AccessModules;
typedef Array<Enum<ModuleType>, 0, 255> ModuleTypes;
typedef AppHMIType AppHmiType;
typedef std::vector<AppHMIType> AppHmiTypes;

struct AppServiceHandledRpc : CompositeType {
 public:
  FunctionIDInt function_id;

 public:
  AppServiceHandledRpc();
  ~AppServiceHandledRpc();
  AppServiceHandledRpc(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);
  void ReportErrors(rpc::ValidationReport* report__) const;

 private:
  bool Validate() const;
};

struct AppServiceInfo : CompositeType {
 public:
  Optional<AppServiceNames> service_names;
  AppServiceHandledRpcs handled_rpcs;

 public:
  AppServiceInfo();
  ~AppServiceInfo();
  AppServiceInfo(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);
  void ReportErrors(rpc::ValidationReport* report__) const;

 private:
  bool Validate() const;
};

struct RequestTypes : public RequestsTypeArray {
  RequestTypes();
  explicit RequestTypes(Json::Value* value);
  explicit RequestTypes(const Json::Value* value);

  void CleanUp();

  bool is_valid() const OVERRIDE;
  bool is_omitted() const;
  bool is_empty() const;
  bool is_cleaned_up() const;

 private:
  bool is_cleaned_up_;
};

struct PolicyBase : CompositeType {
 public:
  Strings groups;
  Optional<Strings> preconsented_groups;
  Enum<Priority> priority;
  Enum<HmiLevel> default_hmi;
  Boolean keep_context;
  Boolean steal_focus;

 public:
  PolicyBase();
  PolicyBase(const Strings& groups,
             Priority priority,
             HmiLevel default_hmi,
             bool keep_context,
             bool steal_focus);
  virtual ~PolicyBase();
  explicit PolicyBase(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct DevicePolicy : PolicyBase {
 public:
  DevicePolicy();
  DevicePolicy(const Strings& groups,
               Priority priority,
               HmiLevel default_hmi,
               bool keep_context,
               bool steal_focus);
  ~DevicePolicy();
  explicit DevicePolicy(const Json::Value* value__);
};

struct ApplicationParams : PolicyBase {
 public:
  Optional<Strings> nicknames;
  Optional<AppHMITypes> AppHMIType;
  Optional<RequestTypes> RequestType;
  Optional<RequestSubTypes> RequestSubType;
  Optional<Integer<uint32_t, 0, UINT_MAX> > heart_beat_timeout_ms;
  mutable Optional<ModuleTypes> moduleType;
  Optional<String<0, 65535> > certificate;
  // Cloud application params
  Optional<Enum<HybridAppPreference> > hybrid_app_preference;
  Optional<String<0, 255> > endpoint;
  Optional<Boolean> enabled;
  Optional<String<0, 65535> > auth_token;
  Optional<String<0, 255> > cloud_transport_type;
  Optional<String<0, 65535> > icon_url;

  // App Service Params
  Optional<AppServiceParameters> app_service_parameters;
  Optional<Boolean> allow_unknown_rpc_passthrough;
  Optional<Boolean> encryption_required;

 public:
  ApplicationParams();
  ApplicationParams(const Strings& groups,
                    Priority priority,
                    HmiLevel default_hmi,
                    bool keep_context,
                    bool steal_focus);
  ~ApplicationParams();
  explicit ApplicationParams(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
  bool ValidateModuleTypes() const;
};

struct ApplicationPoliciesSection : CompositeType {
 public:
  mutable ApplicationPolicies apps;
  DevicePolicy device;

 public:
  ApplicationPoliciesSection();
  ApplicationPoliciesSection(const ApplicationPolicies& apps,
                             const DevicePolicy& device);
  ~ApplicationPoliciesSection();
  explicit ApplicationPoliciesSection(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct RpcParameters : CompositeType {
 public:
  HmiLevels hmi_levels;
  Optional<Parameters> parameters;

 public:
  RpcParameters();
  explicit RpcParameters(const HmiLevels& hmi_levels);
  ~RpcParameters();
  explicit RpcParameters(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct ExternalConsentEntity : CompositeType {
 public:
  typedef Integer<int32_t, 0, 128> EntityInt;
  EntityInt entity_type;
  EntityInt entity_id;

 public:
  ExternalConsentEntity();
  explicit ExternalConsentEntity(const Json::Value* value__);
  ExternalConsentEntity(const int32_t type, const int32_t id);
  Json::Value ToJsonValue() const;
  bool operator==(const ExternalConsentEntity& rhs) const;
  bool is_valid() const;
  bool is_initialized() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  void SetPolicyTableType(PolicyTableType pt_type) OVERRIDE;
};

typedef Array<ExternalConsentEntity, 0, 100>
    DisallowedByExternalConsentEntities;

struct Rpcs : CompositeType {
 public:
  Optional<String<1, 255> > user_consent_prompt;
  Nullable<Rpc> rpcs;
  Optional<DisallowedByExternalConsentEntities>
      disallowed_by_external_consent_entities_on;
  Optional<DisallowedByExternalConsentEntities>
      disallowed_by_external_consent_entities_off;
  Optional<Boolean> encryption_required;

 public:
  Rpcs();
  explicit Rpcs(const Rpc& rpcs);
  ~Rpcs();
  explicit Rpcs(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct EndpointProperty : CompositeType {
 public:
  Optional<String<0, 100> > version;

  EndpointProperty();
  ~EndpointProperty();
  explicit EndpointProperty(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  void SetPolicyTableType(PolicyTableType pt_type) OVERRIDE;

 private:
  bool Validate() const;
};

struct ModuleConfig : CompositeType {
 public:
  Integer<uint8_t, 0, 255> exchange_after_x_ignition_cycles;
  Integer<int64_t, 0, 4294967296ll> exchange_after_x_kilometers;
  Integer<uint8_t, 0, 255> exchange_after_x_days;
  Integer<uint16_t, 0, 65535> timeout_after_x_seconds;
  SecondsBetweenRetries seconds_between_retries;
  ServiceEndpoints endpoints;
  Optional<ServiceEndpointProperties> endpoint_properties;
  NumberOfNotificationsPerMinute notifications_per_minute_by_priority;
  Optional<NumberOfNotificationsPerMinute>
      subtle_notifications_per_minute_by_priority;
  Optional<String<0, 100> > vehicle_make;
  Optional<String<0, 100> > vehicle_model;
  Optional<String<0, 4> > vehicle_year;
  Optional<String<0, 10> > preloaded_date;
  Optional<String<0, 65535> > certificate;
  Optional<Boolean> preloaded_pt;
  Optional<Boolean> full_app_id_supported;
  Optional<Boolean> lock_screen_dismissal_enabled;

  static const std::string kDefaultOemMappingServiceName;

 public:
  ModuleConfig();
  ModuleConfig(uint8_t exchange_after_x_ignition_cycles,
               int64_t exchange_after_x_kilometers,
               uint8_t exchange_after_x_days,
               uint16_t timeout_after_x_seconds,
               const SecondsBetweenRetries& seconds_between_retries,
               const ServiceEndpoints& endpoints,
               const ServiceEndpointProperties& endpoint_properties,
               const NumberOfNotificationsPerMinute&
                   notifications_per_minute_by_priority);
  ~ModuleConfig();
  explicit ModuleConfig(const Json::Value* value__);
  void SafeCopyFrom(const ModuleConfig& from);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct MessageString : CompositeType {
 public:
  Optional<String<1, 65535> > line1;
  Optional<String<1, 65535> > line2;
  Optional<String<1, 65535> > tts;
  Optional<String<1, 65535> > label;
  Optional<String<1, 65535> > textBody;

 public:
  MessageString();
  ~MessageString();
  explicit MessageString(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct MessageLanguages : CompositeType {
 public:
  Languages languages;

 public:
  MessageLanguages();
  explicit MessageLanguages(const Languages& languages);
  ~MessageLanguages();
  explicit MessageLanguages(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
  static const std::string default_language_;
};

struct ConsumerFriendlyMessages : CompositeType {
 public:
  String<1, 100> version;
  Optional<Messages> messages;

 public:
  ConsumerFriendlyMessages();
  explicit ConsumerFriendlyMessages(const std::string& version);
  ~ConsumerFriendlyMessages();
  explicit ConsumerFriendlyMessages(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct ModuleMeta : CompositeType {
 public:
  Optional<String<0, 500> > ccpu_version;
  Optional<String<0, 250> > language;
  Optional<String<0, 250> > wers_country_code;
  Optional<String<0, 500> > hardware_version;
  Optional<Integer<uint32_t, 0, ODO_MAX> > pt_exchanged_at_odometer_x;
  Optional<Integer<uint16_t, 0, 65535> > pt_exchanged_x_days_after_epoch;
  Optional<Integer<uint16_t, 0, 65535> > ignition_cycles_since_last_exchange;
  Optional<String<0, 250> > vin;

 public:
  ModuleMeta();
  ~ModuleMeta();
  explicit ModuleMeta(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct AppLevel : CompositeType {
 public:
  Integer<uint16_t, 0, 65535> minutes_in_hmi_full;
  String<1, 10> app_registration_language_gui;
  String<0, 10> app_registration_language_vui;
  Integer<uint16_t, 0, 65535> minutes_in_hmi_limited;
  Integer<uint16_t, 0, 65535> minutes_in_hmi_background;
  Integer<uint16_t, 0, 65535> minutes_in_hmi_none;
  Integer<uint16_t, 0, 65535> count_of_user_selections;
  Integer<uint16_t, 0, 65535> count_of_rejections_sync_out_of_memory;
  Integer<uint16_t, 0, 65535> count_of_rejections_nickname_mismatch;
  Integer<uint16_t, 0, 65535> count_of_rejections_duplicate_name;
  Integer<uint16_t, 0, 65535> count_of_rejected_rpc_calls;
  Integer<uint16_t, 0, 65535> count_of_rpcs_sent_in_hmi_none;
  Integer<uint16_t, 0, 65535> count_of_removals_for_bad_behavior;
  Integer<uint16_t, 0, 65535> count_of_tls_errors;
  Integer<uint16_t, 0, 65535> count_of_run_attempts_while_revoked;

 public:
  AppLevel();
  AppLevel(uint16_t minutes_in_hmi_full,
           const std::string& app_registration_language_gui,
           const std::string& app_registration_language_vui,
           uint16_t minutes_in_hmi_limited,
           uint16_t minutes_in_hmi_background,
           uint16_t minutes_in_hmi_none,
           uint16_t count_of_user_selections,
           uint16_t count_of_rejections_sync_out_of_memory,
           uint16_t count_of_rejections_nickname_mismatch,
           uint16_t count_of_rejections_duplicate_name,
           uint16_t count_of_rejected_rpc_calls,
           uint16_t count_of_rpcs_sent_in_hmi_none,
           uint16_t count_of_removals_for_bad_behavior,
           uint16_t count_of_tls_errors,
           uint16_t count_of_run_attempts_while_revoked);
  ~AppLevel();
  explicit AppLevel(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct UsageAndErrorCounts : CompositeType {
 public:
  Optional<Integer<uint16_t, 0, 65535> > count_of_iap_buffer_full;
  Optional<Integer<uint16_t, 0, 65535> > count_sync_out_of_memory;
  Optional<Integer<uint16_t, 0, 65535> > count_of_sync_reboots;
  Optional<AppLevels> app_level;

 public:
  UsageAndErrorCounts();
  ~UsageAndErrorCounts();
  explicit UsageAndErrorCounts(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct ConsentRecords : CompositeType {
 public:
  Optional<ConsentGroups> consent_groups;
  Optional<ConsentGroups> external_consent_status_groups;
  Optional<Enum<Input> > input;
  Optional<String<1, 255> > time_stamp;
  int64_t consent_last_updated;
  int64_t ext_consent_last_updated;

 public:
  ConsentRecords();
  ~ConsentRecords();
  explicit ConsentRecords(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct DeviceParams : CompositeType {
 public:
  Optional<String<0, 255> > hardware;
  Optional<Nullable<String<0, 255> > > firmware_rev;
  Optional<String<0, 255> > os;
  Optional<String<0, 255> > os_version;
  Optional<Nullable<String<0, 255> > > carrier;
  Optional<UserConsentRecords> user_consent_records;
  Optional<Integer<uint8_t, 0, 255> > max_number_rfcom_ports;
  Optional<String<0, 255> > connection_type;

 public:
  DeviceParams();
  ~DeviceParams();
  explicit DeviceParams(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct VehicleDataItem : CompositeType {
 public:
  static const std::vector<std::string> kPODTypes;

  static const std::string kInteger;
  static const std::string kStruct;
  static const std::string kString;
  static const std::string kFloat;
  static const std::string kDouble;
  static const std::string kBoolean;
  static const std::string kName;
  static const std::string kParams;

  String<1, 255> name;
  String<0, 255> type;
  String<1, 255> key;
  Boolean mandatory;
  Optional<Array<VehicleDataItem, 0, 255> > params;

  Optional<Boolean> array;
  Optional<String<0, 255> > since;
  Optional<String<0, 255> > until;
  Optional<Boolean> removed;
  Optional<Boolean> deprecated;
  Optional<String<0, UINT32_MAX> > defvalue;
  Optional<Float<-INT32_MAX, INT32_MAX> > minvalue;
  Optional<Float<-INT32_MAX, INT32_MAX> > maxvalue;
  Optional<Integer<uint32_t, 0, UINT32_MAX> > minsize;
  Optional<Integer<uint32_t, 0, UINT32_MAX> > maxsize;
  Optional<Integer<uint32_t, 0, UINT32_MAX> > minlength;
  Optional<Integer<uint32_t, 0, UINT_MAX> > maxlength;

  VehicleDataItem();
  VehicleDataItem(const VehicleDataItem& policy_table);
  explicit VehicleDataItem(const Json::Value* value__);

  bool operator==(const VehicleDataItem& vd);

  ~VehicleDataItem();

  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_not_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

  /**
   * @brief Validates type of vehicle data item according to
   * POD types and generated from API enums.
   * @return true if type is valid.
   */
  bool ValidateTypes() const;
  /**
   * @brief Validates default value of vehicle data item based
   * on type, unable to validate enum values
   * @return true if defvalue is valid.
   */
  bool ValidateDefault() const;
  bool IsPrimitiveType() const;
  bool ValidateNaming(std::string str) const;

 private:
  bool Validate() const;
};

typedef Array<VehicleDataItem, 0, 255> VehicleDataItems;

struct VehicleData : CompositeType {
 public:
  Optional<String<0, 100> > schema_version;
  Optional<VehicleDataItems> schema_items;

  VehicleData();
  VehicleData(const VehicleData& vehicle_data);
  VehicleData(const Json::Value* value__);
  ~VehicleData();

  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct PolicyTable : CompositeType {
 public:
  ApplicationPoliciesSection app_policies_section;
  FunctionalGroupings functional_groupings;
  Optional<ConsumerFriendlyMessages> consumer_friendly_messages;
  ModuleConfig module_config;
  Optional<ModuleMeta> module_meta;
  Optional<UsageAndErrorCounts> usage_and_error_counts;
  Optional<DeviceData> device_data;
  Optional<VehicleData> vehicle_data;

 public:
  PolicyTable();
  PolicyTable(const ApplicationPoliciesSection& app_policies_section,
              const FunctionalGroupings& functional_groupings,
              const ConsumerFriendlyMessages& consumer_friendly_messages,
              const ModuleConfig& module_config);
  ~PolicyTable();
  explicit PolicyTable(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};

struct Table : CompositeType {
 public:
  PolicyTable policy_table;

 public:
  Table();
  explicit Table(const PolicyTable& policy_table);
  ~Table();
  explicit Table(const Json::Value* value__);
  Json::Value ToJsonValue() const;
  bool is_valid() const;
  bool is_initialized() const;
  bool struct_empty() const;
  void ReportErrors(rpc::ValidationReport* report__) const;
  virtual void SetPolicyTableType(PolicyTableType pt_type);

 private:
  bool Validate() const;
};
}  // namespace policy_table_interface_base
}  // namespace rpc

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_TABLE_TYPES_H_
