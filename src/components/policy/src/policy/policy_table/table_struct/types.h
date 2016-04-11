// This file is generated, do not edit
#ifndef POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_TYPES_H_
#define POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_TYPES_H_
#include <climits>

#include "./enums.h"
#include "rpc_base/rpc_message.h"
namespace Json {
class Value;
}  // namespace Json
namespace rpc {
namespace policy_table_interface_base {
struct AppLevel;
struct ApplicationParams;
struct DeviceParams;
struct MessageLanguages;
struct MessageString;
struct RpcParameters;
struct Rpcs;
}  // namespace policy_table_interface_base
}  // namespace rpc


namespace rpc {
namespace policy_table_interface_base {

typedef Array< String<1, 255>, 0, 255 > Strings;

typedef Array< Enum<AppHMIType>, 0, 255 > AppHMITypes;

typedef Array< Enum<HmiLevel>, 0, 4 > HmiLevels;

typedef Array< Enum<Parameter>, 0, 24 > Parameters;

typedef Map< RpcParameters, 0, 50 > Rpc;

typedef Array< String<10, 255>, 1, 255 > URL;

typedef Map< URL, 1, 255 > URLList;

typedef Map< URLList, 1, 255 > ServiceEndpoints;

typedef uint8_t NumberOfNotificationsType;
typedef Map< Integer<NumberOfNotificationsType, 0, 255>, 0, 6 > NumberOfNotificationsPerMinute;

typedef Array< Integer<uint16_t, 1, 1000>, 0, 10 > SecondsBetweenRetries;

typedef Map< MessageString, 0, 500 > Languages;

typedef Map< MessageLanguages, 0, 255 > Messages;

typedef Map< AppLevel, 0, 255 > AppLevels;

typedef Map< Stringifyable < Nullable< ApplicationParams > >, 1, 1000 > ApplicationPolicies;

typedef Map< Rpcs, 1, 255 > FunctionalGroupings;

typedef Map< DeviceParams, 0, 255 > DeviceData;

typedef Array< Enum<RequestType>, 0, 255 > RequestTypes;

struct PolicyBase : CompositeType {
  public:
    Enum<Priority> priority;
  public:
    PolicyBase();
    PolicyBase(Priority priority);
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
    DevicePolicy(Priority priority);
    ~DevicePolicy();
    explicit DevicePolicy(const Json::Value* value__);
};

struct ApplicationParams : PolicyBase {
  public:
    Strings groups;
    Optional< Strings > nicknames;
    Optional< AppHMITypes > AppHMIType;
    Optional< RequestTypes > RequestType;
    Optional< Integer<uint16_t, 0, 65225> > memory_kb;
    Optional< Integer<uint32_t, 0, UINT_MAX> > heart_beat_timeout_ms;
    Optional< String<0, 255> > certificate;
  public:
    ApplicationParams();
    ApplicationParams(const Strings& groups, Priority priority);
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
};

struct ApplicationPoliciesSection : CompositeType {
  public:
    mutable ApplicationPolicies apps;
    DevicePolicy device;
  public:
    ApplicationPoliciesSection();
    ApplicationPoliciesSection(const ApplicationPolicies& apps, const DevicePolicy& device);
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
    Optional< Parameters > parameters;
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

struct Rpcs : CompositeType {
  public:
    Optional< String<1, 255> > user_consent_prompt;
    Nullable< Rpc > rpcs;
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

struct ModuleConfig : CompositeType {
  public:
    Optional< Map< String<0, 100>, 0, 255 > > device_certificates;
    Optional< Boolean > preloaded_pt;
    Integer<uint8_t, 0, 255> exchange_after_x_ignition_cycles;
    Integer<int64_t, 0, 4294967296ll> exchange_after_x_kilometers;
    Integer<uint8_t, 0, 255> exchange_after_x_days;
    Integer<uint16_t, 0, 65535> timeout_after_x_seconds;
    SecondsBetweenRetries seconds_between_retries;
    ServiceEndpoints endpoints;
    NumberOfNotificationsPerMinute notifications_per_minute_by_priority;
    Optional< String<1, 100> > vehicle_make;
    Optional< String<1, 100> > vehicle_model;
    Optional< String<4, 4> > vehicle_year;
    Optional< String<0, 10> > preloaded_date;
    Optional< String<0, 65535> > certificate;
  public:
    ModuleConfig();
    ModuleConfig(uint8_t exchange_after_x_ignition_cycles, int64_t exchange_after_x_kilometers, uint8_t exchange_after_x_days, uint16_t timeout_after_x_seconds, const SecondsBetweenRetries& seconds_between_retries, const ServiceEndpoints& endpoints, const NumberOfNotificationsPerMinute& notifications_per_minute_by_priority);
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
    Optional< String<1, 65535> > line1;
    Optional< String<1, 65535> > line2;
    Optional< String<1, 65535> > tts;
    Optional< String<1, 65535> > label;
    Optional< String<1, 65535> > textBody;
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
};

struct ConsumerFriendlyMessages : CompositeType {
  public:
    String<1, 100> version;
    Optional< Messages > messages;
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
  public:
    ModuleMeta();
    ~ModuleMeta();
    explicit ModuleMeta(const Json::Value* value__);
    Json::Value ToJsonValue() const;
    bool is_valid() const;
    bool is_initialized() const;
    bool struct_empty() const;
    void ReportErrors(rpc::ValidationReport* report__) const;
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
    AppLevel(uint16_t minutes_in_hmi_full, const std::string& app_registration_language_gui, const std::string& app_registration_language_vui, uint16_t minutes_in_hmi_limited, uint16_t minutes_in_hmi_background, uint16_t minutes_in_hmi_none, uint16_t count_of_user_selections, uint16_t count_of_rejections_sync_out_of_memory, uint16_t count_of_rejections_nickname_mismatch, uint16_t count_of_rejections_duplicate_name, uint16_t count_of_rejected_rpc_calls, uint16_t count_of_rpcs_sent_in_hmi_none, uint16_t count_of_removals_for_bad_behavior,
uint16_t count_of_tls_errors, uint16_t count_of_run_attempts_while_revoked);
    ~AppLevel();
    explicit AppLevel(const Json::Value* value__);
    Json::Value ToJsonValue() const;
    bool is_valid() const;
    bool is_initialized() const;
    bool struct_empty() const;
    void ReportErrors(rpc::ValidationReport* report__) const;
  private:
    bool Validate() const;
};

struct UsageAndErrorCounts : CompositeType {
  public:
    Optional< AppLevels > app_level;
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

struct DeviceParams : CompositeType {
  public:
  public:
    DeviceParams();
    ~DeviceParams();
    explicit DeviceParams(const Json::Value* value__);
    Json::Value ToJsonValue() const;
    bool is_valid() const;
    bool is_initialized() const;
    bool struct_empty() const;
    void ReportErrors(rpc::ValidationReport* report__) const;
  private:
    bool Validate() const;
};

struct PolicyTable : CompositeType {
  public:
    ApplicationPoliciesSection app_policies_section;
    FunctionalGroupings functional_groupings;
    Optional < ConsumerFriendlyMessages > consumer_friendly_messages;
    ModuleConfig module_config;
    Optional< ModuleMeta > module_meta;
    Optional< UsageAndErrorCounts > usage_and_error_counts;
    Optional< DeviceData > device_data;
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

#endif  // POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_TYPES_H_
