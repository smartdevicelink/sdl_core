// This file is generated, do not edit
#include "./types.h"
#include "rpc_base/rpc_base_json_inl.h"

namespace rpc {
namespace policy_table_interface_base {

  std::string PolicyTableTypeToString(const PolicyTableType pt_type) {
      switch (pt_type) {
        case PT_PRELOADED: {
          return "PT_PRELOADED";
        }
        case PT_UPDATE: {
          return "PT_UPDATE";
        }
        case PT_SNAPSHOT: {
          return "PT_SNAPSHOT";
        }
        default :{
          return "INVALID_PT_TYPE";
        }
      }
  }

  // PolicyBase methods
  PolicyBase::PolicyBase()
    : CompositeType(kUninitialized) {
  }

  PolicyBase::PolicyBase(Priority priority)
    : CompositeType(kUninitialized),
      priority(priority) {
  }

  PolicyBase::~PolicyBase() {
  }

  PolicyBase::PolicyBase(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject)),
      priority(impl::ValueMember(value__, "priority")) {
  }

  Json::Value PolicyBase::ToJsonValue() const {
    Json::Value result__(Json::objectValue);
    impl::WriteJsonField("priority", priority, &result__);
    return result__;
  }

  bool PolicyBase::is_valid() const {
    if (!priority.is_valid()) {
      return false;
    }
    return Validate();
  }

  bool PolicyBase::is_initialized() const {
    return (initialization_state__ != kUninitialized) || (!struct_empty());
  }

  bool PolicyBase::struct_empty() const {
    if (priority.is_initialized()) {
      return false;
    }
    return true;
  }

  void PolicyBase::ReportErrors(rpc::ValidationReport* report__) const {
    if (struct_empty()) {
      rpc::CompositeType::ReportErrors(report__);
    }
    if (!priority.is_valid()) {
      priority.ReportErrors(&report__->ReportSubobject("priority"));
    }
  }

  void PolicyBase::SetPolicyTableType(PolicyTableType pt_type) {
    CompositeType::SetPolicyTableType(pt_type);
    priority.SetPolicyTableType(pt_type);
  }

  // DevicePolicy methods
  DevicePolicy::DevicePolicy()
    : PolicyBase() {
  }

  DevicePolicy::DevicePolicy(Priority priority)
    : PolicyBase(priority) {
  }

  DevicePolicy::~DevicePolicy() {
  }

  DevicePolicy::DevicePolicy(const Json::Value* value__)
    : PolicyBase(value__) {
  }

  // AppPoliciesSection methods
  ApplicationPoliciesSection::ApplicationPoliciesSection()
    : CompositeType(kUninitialized) {
  }

  ApplicationPoliciesSection::ApplicationPoliciesSection(const ApplicationPolicies& apps, const DevicePolicy& device)
    : CompositeType(kUninitialized),
      apps(apps),
      device(device) {
  }

  ApplicationPoliciesSection::~ApplicationPoliciesSection() {
  }

  ApplicationPoliciesSection::ApplicationPoliciesSection(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject)),
      apps(value__),
      device(impl::ValueMember(value__, "device")) {
    // Since "device" is moved to separate struct, we have to delete it from
    // parsed apps to avoid validation issues due to possible wrong params in
    // device section
    apps.erase("device");
  }

  Json::Value ApplicationPoliciesSection::ToJsonValue() const {
    Json::Value result__(Json::objectValue);
    result__ = apps.ToJsonValue();
    impl::WriteJsonField("device", device, &result__);
    return result__;
  }

  bool ApplicationPoliciesSection::is_valid() const {
    if (!device.is_valid()) {
      return false;
    }
    if (!apps.is_valid()) {
      return false;
    }
    return Validate();
  }

  bool ApplicationPoliciesSection::is_initialized() const {
    return (initialization_state__ != kUninitialized) || (!struct_empty());
  }

  bool ApplicationPoliciesSection::struct_empty() const {
    if (device.is_initialized()) {
      return false;
    }
    if (apps.is_initialized()) {
      return false;
    }
    return true;
  }

  void ApplicationPoliciesSection::ReportErrors(rpc::ValidationReport* report__) const {
    if (struct_empty()) {
      rpc::CompositeType::ReportErrors(report__);
    }
    if (!device.is_valid()) {
      device.ReportErrors(&report__->ReportSubobject("device"));
    }
    if (!apps.is_valid()) {
      apps.ReportErrors(&report__->ReportSubobject("apps"));
    }
  }

  void ApplicationPoliciesSection::SetPolicyTableType(PolicyTableType pt_type) {
    CompositeType::SetPolicyTableType(pt_type);
    device.SetPolicyTableType(pt_type);
    apps.SetPolicyTableType(pt_type);
  }

// ApplicationParams methods
ApplicationParams::ApplicationParams()
  : PolicyBase(),
    groups() {
}

ApplicationParams::ApplicationParams(const Strings& groups, Priority priority)
  : PolicyBase(priority),
    groups(groups) {
}

ApplicationParams::~ApplicationParams() {
}

ApplicationParams::ApplicationParams(const Json::Value* value__)
  : PolicyBase(value__),
    groups(impl::ValueMember(value__, "groups")),
    nicknames(impl::ValueMember(value__, "nicknames")),
    AppHMIType(impl::ValueMember(value__, "AppHMIType")),
    RequestType(impl::ValueMember(value__, "RequestType")),
    memory_kb(impl::ValueMember(value__, "memory_kb"), 0),
    heart_beat_timeout_ms(impl::ValueMember(value__, "heart_beat_timeout_ms")),
    certificate(impl::ValueMember(value__, "certificate"), "not_specified") {
}

Json::Value ApplicationParams::ToJsonValue() const {
  Json::Value result__(PolicyBase::ToJsonValue());
  impl::WriteJsonField("groups", groups, &result__);
  impl::WriteJsonField("nicknames", nicknames, &result__);
  impl::WriteJsonField("AppHMIType", AppHMIType, &result__);
  impl::WriteJsonField("RequestType", RequestType, &result__);
  impl::WriteJsonField("memory_kb", memory_kb, &result__);
  impl::WriteJsonField("heart_beat_timeout_ms", heart_beat_timeout_ms, &result__);
  impl::WriteJsonField("certificate", certificate, &result__);
  return result__;
}

bool ApplicationParams::is_valid() const {
  // RequestType is not validated since there is high-level validation logic,
  // which takes into account information not available here.
  if (!PolicyBase::is_valid()) {
    return false;
  }
  if (!groups.is_valid()) {
    return false;
  }
  if (!nicknames.is_valid()) {
    return false;
  }
  if (!AppHMIType.is_valid()) {
    return false;
  }
  if (!memory_kb.is_valid()) {
    return false;
  }
  if (!heart_beat_timeout_ms.is_valid()) {
    return false;
  }
  if (!certificate.is_valid()) {
    return false;
  }
  return Validate();
}

bool ApplicationParams::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}

bool ApplicationParams::struct_empty() const {
  if (!PolicyBase::is_initialized()) {
    return false;
  }
  if (groups.is_initialized()) {
    return false;
  }
  if (nicknames.is_initialized()) {
    return false;
  }
  if (AppHMIType.is_initialized()) {
    return false;
  }
  if (RequestType.is_initialized()) {
    return false;
  }
  if (memory_kb.is_initialized()) {
    return false;
  }
  if (heart_beat_timeout_ms.is_initialized()) {
    return false;
  }
  if (certificate.is_initialized()) {
    return false;
  }
  return true;
}

void ApplicationParams::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!groups.is_valid()) {
    groups.ReportErrors(&report__->ReportSubobject("groups"));
  }
  if (!nicknames.is_valid()) {
    nicknames.ReportErrors(&report__->ReportSubobject("nicknames"));
  }
  if (!AppHMIType.is_valid()) {
    AppHMIType.ReportErrors(&report__->ReportSubobject("AppHMIType"));
  }
  if (!RequestType.is_valid()) {
    RequestType.ReportErrors(&report__->ReportSubobject("RequestType"));
  }
  if (!priority.is_valid()) {
    priority.ReportErrors(&report__->ReportSubobject("priority"));
  }
  if (!memory_kb.is_valid()) {
    memory_kb.ReportErrors(&report__->ReportSubobject("memory_kb"));
  }
  if (!heart_beat_timeout_ms.is_valid()) {
    heart_beat_timeout_ms.ReportErrors(&report__->ReportSubobject("heart_beat_timeout_ms"));
  }
  if (!certificate.is_valid()) {
    certificate.ReportErrors(&report__->ReportSubobject("certificate"));
  }
}

void ApplicationParams::SetPolicyTableType(PolicyTableType pt_type) {
  PolicyBase::SetPolicyTableType(pt_type);
  groups.SetPolicyTableType(pt_type);
  AppHMIType.SetPolicyTableType(pt_type);
  RequestType.SetPolicyTableType(pt_type);
  memory_kb.SetPolicyTableType(pt_type);
  heart_beat_timeout_ms.SetPolicyTableType(pt_type);
  certificate.SetPolicyTableType(pt_type);
}

// RpcParameters methods
RpcParameters::RpcParameters()
  : CompositeType(kUninitialized) {
}
RpcParameters::RpcParameters(const HmiLevels& hmi_levels)
  : CompositeType(kUninitialized),
    hmi_levels(hmi_levels) {
}
RpcParameters::~RpcParameters() {
}
RpcParameters::RpcParameters(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    hmi_levels(impl::ValueMember(value__, "hmi_levels")),
    parameters(impl::ValueMember(value__, "parameters")) {
}
Json::Value RpcParameters::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("hmi_levels", hmi_levels, &result__);
  impl::WriteJsonField("parameters", parameters, &result__);
  return result__;
}
bool RpcParameters::is_valid() const {
  if (!hmi_levels.is_valid()) {
    return false;
  }
  if (!parameters.is_valid()) {
    return false;
  }
  return Validate();
}
bool RpcParameters::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool RpcParameters::struct_empty() const {
  if (hmi_levels.is_initialized()) {
    return false;
  }
  if (parameters.is_initialized()) {
    return false;
  }

  return true;
}
void RpcParameters::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!hmi_levels.is_valid()) {
    hmi_levels.ReportErrors(&report__->ReportSubobject("hmi_levels"));
  }
  if (!parameters.is_valid()) {
    parameters.ReportErrors(&report__->ReportSubobject("parameters"));
  }
}

void RpcParameters::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  hmi_levels.SetPolicyTableType(pt_type);
  parameters.SetPolicyTableType(pt_type);
}

// Rpcs methods
Rpcs::Rpcs()
  : CompositeType(kUninitialized) {
}
Rpcs::Rpcs(const Rpc& rpcs)
  : CompositeType(kUninitialized),
    rpcs(rpcs) {
}
Rpcs::~Rpcs() {
}
Rpcs::Rpcs(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    user_consent_prompt(impl::ValueMember(value__, "user_consent_prompt")),
    rpcs(impl::ValueMember(value__, "rpcs")) {
}
Json::Value Rpcs::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("user_consent_prompt", user_consent_prompt, &result__);
  impl::WriteJsonField("rpcs", rpcs, &result__);
  return result__;
}
bool Rpcs::is_valid() const {
  if (!user_consent_prompt.is_valid()) {
    return false;
  }
  if (!rpcs.is_valid()) {
    return false;
  }
  return Validate();
}
bool Rpcs::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool Rpcs::struct_empty() const {
  if (user_consent_prompt.is_initialized()) {
    return false;
  }
  if (rpcs.is_initialized()) {
    return false;
  }

  return true;
}
void Rpcs::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!user_consent_prompt.is_valid()) {
    user_consent_prompt.ReportErrors(&report__->ReportSubobject("user_consent_prompt"));
  }
  if (!rpcs.is_valid()) {
    rpcs.ReportErrors(&report__->ReportSubobject("rpcs"));
  }
}

void Rpcs::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  user_consent_prompt.SetPolicyTableType(pt_type);
  rpcs.SetPolicyTableType(pt_type);
}

// ModuleConfig methods
ModuleConfig::ModuleConfig()
  : CompositeType(kUninitialized) {
}
ModuleConfig::ModuleConfig(uint8_t exchange_after_x_ignition_cycles, int64_t exchange_after_x_kilometers, uint8_t exchange_after_x_days, uint16_t timeout_after_x_seconds, const SecondsBetweenRetries& seconds_between_retries, const ServiceEndpoints& endpoints, const NumberOfNotificationsPerMinute& notifications_per_minute_by_priority)
  : CompositeType(kUninitialized),
    exchange_after_x_ignition_cycles(exchange_after_x_ignition_cycles),
    exchange_after_x_kilometers(exchange_after_x_kilometers),
    exchange_after_x_days(exchange_after_x_days),
    timeout_after_x_seconds(timeout_after_x_seconds),
    seconds_between_retries(seconds_between_retries),
    endpoints(endpoints),
    notifications_per_minute_by_priority(notifications_per_minute_by_priority) {
}
ModuleConfig::~ModuleConfig() {
}
ModuleConfig::ModuleConfig(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    device_certificates(impl::ValueMember(value__, "device_certificates")),
    preloaded_pt(impl::ValueMember(value__, "preloaded_pt")),
    exchange_after_x_ignition_cycles(impl::ValueMember(value__, "exchange_after_x_ignition_cycles")),
    exchange_after_x_kilometers(impl::ValueMember(value__, "exchange_after_x_kilometers")),
    exchange_after_x_days(impl::ValueMember(value__, "exchange_after_x_days")),
    timeout_after_x_seconds(impl::ValueMember(value__, "timeout_after_x_seconds")),
    seconds_between_retries(impl::ValueMember(value__, "seconds_between_retries")),
    endpoints(impl::ValueMember(value__, "endpoints")),
    notifications_per_minute_by_priority(impl::ValueMember(value__, "notifications_per_minute_by_priority")),
    vehicle_make(impl::ValueMember(value__, "vehicle_make")),
    vehicle_model(impl::ValueMember(value__, "vehicle_model")),
    vehicle_year(impl::ValueMember(value__, "vehicle_year")),
    preloaded_date(impl::ValueMember(value__, "preloaded_date")),
    certificate(impl::ValueMember(value__, "certificate")){
}

void ModuleConfig::SafeCopyFrom(const ModuleConfig& from) {
//  device_certificates = from.device_certificates;  // According to the requirements this is optional.
  exchange_after_x_ignition_cycles = from.exchange_after_x_ignition_cycles;
  exchange_after_x_kilometers = from.exchange_after_x_kilometers;
  exchange_after_x_days = from.exchange_after_x_days;
  timeout_after_x_seconds = from.timeout_after_x_seconds;
  seconds_between_retries = from.seconds_between_retries;
  endpoints = from.endpoints;
  notifications_per_minute_by_priority = from.notifications_per_minute_by_priority;

  vehicle_make.assign_if_valid(from.vehicle_make);
  vehicle_model.assign_if_valid(from.vehicle_model);
  vehicle_year.assign_if_valid(from.vehicle_year);
  certificate .assign_if_valid(from.certificate);

}

Json::Value ModuleConfig::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("device_certificates", device_certificates, &result__);
  impl::WriteJsonField("preloaded_pt", preloaded_pt, &result__);
  impl::WriteJsonField("exchange_after_x_ignition_cycles", exchange_after_x_ignition_cycles, &result__);
  impl::WriteJsonField("exchange_after_x_kilometers", exchange_after_x_kilometers, &result__);
  impl::WriteJsonField("exchange_after_x_days", exchange_after_x_days, &result__);
  impl::WriteJsonField("timeout_after_x_seconds", timeout_after_x_seconds, &result__);
  impl::WriteJsonField("seconds_between_retries", seconds_between_retries, &result__);
  impl::WriteJsonField("endpoints", endpoints, &result__);
  impl::WriteJsonField("notifications_per_minute_by_priority", notifications_per_minute_by_priority, &result__);
  impl::WriteJsonField("vehicle_make", vehicle_make, &result__);
  impl::WriteJsonField("vehicle_model", vehicle_model, &result__);
  impl::WriteJsonField("vehicle_year", vehicle_year, &result__);
  impl::WriteJsonField("certificate", certificate, &result__);
  impl::WriteJsonField("preloaded_date", preloaded_date, &result__);
  return result__;
}
bool ModuleConfig::is_valid() const {
  if (!device_certificates.is_valid()) {
    return false;
  }
  if (!preloaded_pt.is_valid()) {
    return false;
  }
  if (!exchange_after_x_ignition_cycles.is_valid()) {
    return false;
  }
  if (!exchange_after_x_kilometers.is_valid()) {
    return false;
  }
  if (!exchange_after_x_days.is_valid()) {
    return false;
  }
  if (!timeout_after_x_seconds.is_valid()) {
    return false;
  }
  if (!seconds_between_retries.is_valid()) {
    return false;
  }
  if (!endpoints.is_valid()) {
    return false;
  }
  if (!notifications_per_minute_by_priority.is_valid()) {
    return false;
  }
  if (!vehicle_make.is_valid()) {
    return false;
  }
  if (!vehicle_model.is_valid()) {
    return false;
  }
  if (!vehicle_year.is_valid()) {
    return false;
  }
  if (!certificate.is_valid()) {
    return false;
  }
  if (!preloaded_date.is_valid()) {
    return false;
  }
  return Validate();
}
bool ModuleConfig::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ModuleConfig::struct_empty() const {
  if (device_certificates.is_initialized()) {
    return false;
  }
  if (preloaded_pt.is_initialized()) {
    return false;
  }

  if (exchange_after_x_ignition_cycles.is_initialized()) {
    return false;
  }
  if (exchange_after_x_kilometers.is_initialized()) {
    return false;
  }

  if (exchange_after_x_days.is_initialized()) {
    return false;
  }
  if (timeout_after_x_seconds.is_initialized()) {
    return false;
  }

  if (seconds_between_retries.is_initialized()) {
    return false;
  }
  if (endpoints.is_initialized()) {
    return false;
  }

  if (notifications_per_minute_by_priority.is_initialized()) {
    return false;
  }
  if (vehicle_make.is_initialized()) {
    return false;
  }

  if (vehicle_model.is_initialized()) {
    return false;
  }
  if (vehicle_year.is_initialized()) {
    return false;
  }

  return true;
}
void ModuleConfig::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!device_certificates.is_valid()) {
    device_certificates.ReportErrors(&report__->ReportSubobject("device_certificates"));
  }
  if (!preloaded_pt.is_valid()) {
    preloaded_pt.ReportErrors(&report__->ReportSubobject("preloaded_pt"));
  }
  if (!exchange_after_x_ignition_cycles.is_valid()) {
    exchange_after_x_ignition_cycles.ReportErrors(&report__->ReportSubobject("exchange_after_x_ignition_cycles"));
  }
  if (!exchange_after_x_kilometers.is_valid()) {
    exchange_after_x_kilometers.ReportErrors(&report__->ReportSubobject("exchange_after_x_kilometers"));
  }
  if (!exchange_after_x_days.is_valid()) {
    exchange_after_x_days.ReportErrors(&report__->ReportSubobject("exchange_after_x_days"));
  }
  if (!timeout_after_x_seconds.is_valid()) {
    timeout_after_x_seconds.ReportErrors(&report__->ReportSubobject("timeout_after_x_seconds"));
  }
  if (!seconds_between_retries.is_valid()) {
    seconds_between_retries.ReportErrors(&report__->ReportSubobject("seconds_between_retries"));
  }
  if (!endpoints.is_valid()) {
    endpoints.ReportErrors(&report__->ReportSubobject("endpoints"));
  }
  if (!notifications_per_minute_by_priority.is_valid()) {
    notifications_per_minute_by_priority.ReportErrors(&report__->ReportSubobject("notifications_per_minute_by_priority"));
  }
  if (!vehicle_make.is_valid()) {
    vehicle_make.ReportErrors(&report__->ReportSubobject("vehicle_make"));
  }
  if (!vehicle_model.is_valid()) {
    vehicle_model.ReportErrors(&report__->ReportSubobject("vehicle_model"));
  }
  if (!vehicle_year.is_valid()) {
    vehicle_year.ReportErrors(&report__->ReportSubobject("vehicle_year"));
  }
  if (PT_PRELOADED == GetPolicyTableType()) {
    std::string validation_info = ommited_validation_info +
                                  PolicyTableTypeToString(GetPolicyTableType());
    rpc::ValidationReport* ommited_field_report;
    if (vehicle_make.is_initialized()) {
      ommited_field_report = &report__->ReportSubobject("vehicle_make");
      ommited_field_report->set_validation_info(validation_info);
    }
    if (vehicle_year.is_initialized()) {
      ommited_field_report = &report__->ReportSubobject("vehicle_year");
      ommited_field_report->set_validation_info(validation_info);
    }
    if (vehicle_model.is_initialized()) {
      ommited_field_report = &report__->ReportSubobject("vehicle_model");
      ommited_field_report->set_validation_info(validation_info);
    }
  }
}

void ModuleConfig::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  device_certificates.SetPolicyTableType(pt_type);
  preloaded_pt.SetPolicyTableType(pt_type);
  exchange_after_x_ignition_cycles.SetPolicyTableType(pt_type);
  exchange_after_x_kilometers.SetPolicyTableType(pt_type);
  exchange_after_x_days.SetPolicyTableType(pt_type);
  timeout_after_x_seconds.SetPolicyTableType(pt_type);
  seconds_between_retries.SetPolicyTableType(pt_type);
  endpoints.SetPolicyTableType(pt_type);
  notifications_per_minute_by_priority.SetPolicyTableType(pt_type);
  vehicle_make.SetPolicyTableType(pt_type);
  vehicle_model.SetPolicyTableType(pt_type);
  vehicle_year.SetPolicyTableType(pt_type);
}

// MessageString methods
MessageString::MessageString()
  : CompositeType(kUninitialized) {
}
MessageString::~MessageString() {
}
MessageString::MessageString(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    line1(impl::ValueMember(value__, "line1")),
    line2(impl::ValueMember(value__, "line2")),
    tts(impl::ValueMember(value__, "tts")),
    label(impl::ValueMember(value__, "label")),
    textBody(impl::ValueMember(value__, "textBody")) {
}
Json::Value MessageString::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("line1", line1, &result__);
  impl::WriteJsonField("line2", line2, &result__);
  impl::WriteJsonField("tts", tts, &result__);
  impl::WriteJsonField("label", label, &result__);
  impl::WriteJsonField("textBody", textBody, &result__);
  return result__;
}
bool MessageString::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized && Validate();
  }
  if (!line1.is_valid()) {
    return false;
  }
  if (!line2.is_valid()) {
    return false;
  }
  if (!tts.is_valid()) {
    return false;
  }
  if (!label.is_valid()) {
    return false;
  }
  if (!textBody.is_valid()) {
    return false;
  }
  return Validate();
}
bool MessageString::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool MessageString::struct_empty() const {
  if (line1.is_initialized()) {
    return false;
  }
  if (line2.is_initialized()) {
    return false;
  }

  if (tts.is_initialized()) {
    return false;
  }
  if (label.is_initialized()) {
    return false;
  }

  if (textBody.is_initialized()) {
    return false;
  }
  return true;
}
void MessageString::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!line1.is_valid()) {
    line1.ReportErrors(&report__->ReportSubobject("line1"));
  }
  if (!line2.is_valid()) {
    line2.ReportErrors(&report__->ReportSubobject("line2"));
  }
  if (!tts.is_valid()) {
    tts.ReportErrors(&report__->ReportSubobject("tts"));
  }
  if (!label.is_valid()) {
    label.ReportErrors(&report__->ReportSubobject("label"));
  }
  if (!textBody.is_valid()) {
    textBody.ReportErrors(&report__->ReportSubobject("textBody"));
  }
}

void MessageString::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  line1.SetPolicyTableType(pt_type);
  line2.SetPolicyTableType(pt_type);
  tts.SetPolicyTableType(pt_type);
  label.SetPolicyTableType(pt_type);
  textBody.SetPolicyTableType(pt_type);
}

// MessageLanguages methods
MessageLanguages::MessageLanguages()
  : CompositeType(kUninitialized) {
}
MessageLanguages::MessageLanguages(const Languages& languages)
  : CompositeType(kUninitialized),
    languages(languages) {
}
MessageLanguages::~MessageLanguages() {
}
MessageLanguages::MessageLanguages(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    languages(impl::ValueMember(value__, "languages")) {
}
Json::Value MessageLanguages::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("languages", languages, &result__);
  return result__;
}
bool MessageLanguages::is_valid() const {
  if (!languages.is_valid()) {
    return false;
  }
  return Validate();
}
bool MessageLanguages::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool MessageLanguages::struct_empty() const {
  if (languages.is_initialized()) {
    return false;
  }
  return true;
}

void MessageLanguages::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (PT_SNAPSHOT == GetPolicyTableType()) {
    if (languages.is_initialized()) {
      std::string validation_info = ommited_validation_info +
                                    PolicyTableTypeToString(GetPolicyTableType());
      report__->ReportSubobject("languages").set_validation_info(validation_info);
    }
  }
  if (!languages.is_valid()) {
    languages.ReportErrors(&report__->ReportSubobject("languages"));
  }
}

void MessageLanguages::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  languages.SetPolicyTableType(pt_type);
}

// ConsumerFriendlyMessages methods
ConsumerFriendlyMessages::ConsumerFriendlyMessages()
  : CompositeType(kUninitialized) {
}
ConsumerFriendlyMessages::ConsumerFriendlyMessages(const std::string& version)
  : CompositeType(kUninitialized),
    version(version) {
}
ConsumerFriendlyMessages::~ConsumerFriendlyMessages() {
}
ConsumerFriendlyMessages::ConsumerFriendlyMessages(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    version(impl::ValueMember(value__, "version")),
    messages(impl::ValueMember(value__, "messages")) {
}
Json::Value ConsumerFriendlyMessages::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("version", version, &result__);
  impl::WriteJsonField("messages", messages, &result__);
  return result__;
}
bool ConsumerFriendlyMessages::is_valid() const {
  if (!version.is_valid()) {
    return false;
  }
  if (!messages.is_valid()) {
    return false;
  }
  return Validate();
}
bool ConsumerFriendlyMessages::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ConsumerFriendlyMessages::struct_empty() const {
  if (version.is_initialized()) {
    return false;
  }
  if (messages.is_initialized()) {
    return false;
  }

  return true;
}
void ConsumerFriendlyMessages::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!version.is_valid()) {
    version.ReportErrors(&report__->ReportSubobject("version"));
  }
  if (PT_SNAPSHOT == GetPolicyTableType()) {
    if (messages.is_initialized()) {
      std::string validation_info = ommited_validation_info +
                                    PolicyTableTypeToString(GetPolicyTableType());
      report__->ReportSubobject("messages").set_validation_info(validation_info);
    }
  }
  if (!messages.is_valid()) {
    messages.ReportErrors(&report__->ReportSubobject("messages"));
  }
}

void ConsumerFriendlyMessages::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  version.SetPolicyTableType(pt_type);
  messages.SetPolicyTableType(pt_type);
}

// ModuleMeta methods
ModuleMeta::ModuleMeta()
  : CompositeType(kUninitialized) {
}
ModuleMeta::~ModuleMeta() {
}
ModuleMeta::ModuleMeta(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)) {
}
Json::Value ModuleMeta::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  return result__;
}
bool ModuleMeta::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized && Validate();
  }
  return Validate();
}
bool ModuleMeta::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool ModuleMeta::struct_empty() const {
  return true;
}
void ModuleMeta::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
}

// AppLevel methods
AppLevel::AppLevel()
  : CompositeType(kUninitialized) {
}

AppLevel::AppLevel(uint16_t minutes_in_hmi_full, const std::string& app_registration_language_gui, const std::string& app_registration_language_vui, uint16_t minutes_in_hmi_limited, uint16_t minutes_in_hmi_background, uint16_t minutes_in_hmi_none, uint16_t count_of_user_selections, uint16_t count_of_rejections_sync_out_of_memory, uint16_t count_of_rejections_nickname_mismatch, uint16_t count_of_rejections_duplicate_name, uint16_t count_of_rejected_rpc_calls, uint16_t count_of_rpcs_sent_in_hmi_none, uint16_t count_of_removals_for_bad_behavior, uint16_t count_of_tls_errors, uint16_t count_of_run_attempts_while_revoked)
  : CompositeType(kUninitialized),
    minutes_in_hmi_full(minutes_in_hmi_full),
    app_registration_language_gui(app_registration_language_gui),
    app_registration_language_vui(app_registration_language_vui),
    minutes_in_hmi_limited(minutes_in_hmi_limited),
    minutes_in_hmi_background(minutes_in_hmi_background),
    minutes_in_hmi_none(minutes_in_hmi_none),
    count_of_user_selections(count_of_user_selections),
    count_of_rejections_sync_out_of_memory(count_of_rejections_sync_out_of_memory),
    count_of_rejections_nickname_mismatch(count_of_rejections_nickname_mismatch),
    count_of_rejections_duplicate_name(count_of_rejections_duplicate_name),
    count_of_rejected_rpc_calls(count_of_rejected_rpc_calls),
    count_of_rpcs_sent_in_hmi_none(count_of_rpcs_sent_in_hmi_none),
    count_of_removals_for_bad_behavior(count_of_removals_for_bad_behavior),
    count_of_tls_errors(count_of_tls_errors),
    count_of_run_attempts_while_revoked(count_of_run_attempts_while_revoked) {
}
AppLevel::~AppLevel() {
}
AppLevel::AppLevel(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    minutes_in_hmi_full(impl::ValueMember(value__, "minutes_in_hmi_full")),
    app_registration_language_gui(impl::ValueMember(value__, "app_registration_language_gui")),
    app_registration_language_vui(impl::ValueMember(value__, "app_registration_language_vui")),
    minutes_in_hmi_limited(impl::ValueMember(value__, "minutes_in_hmi_limited")),
    minutes_in_hmi_background(impl::ValueMember(value__, "minutes_in_hmi_background")),
    minutes_in_hmi_none(impl::ValueMember(value__, "minutes_in_hmi_none")),
    count_of_user_selections(impl::ValueMember(value__, "count_of_user_selections")),
    count_of_rejections_sync_out_of_memory(impl::ValueMember(value__, "count_of_rejections_sync_out_of_memory")),
    count_of_rejections_nickname_mismatch(impl::ValueMember(value__, "count_of_rejections_nickname_mismatch")),
    count_of_rejections_duplicate_name(impl::ValueMember(value__, "count_of_rejections_duplicate_name")),
    count_of_rejected_rpc_calls(impl::ValueMember(value__, "count_of_rejected_rpc_calls")),
    count_of_rpcs_sent_in_hmi_none(impl::ValueMember(value__, "count_of_rpcs_sent_in_hmi_none")),
    count_of_removals_for_bad_behavior(impl::ValueMember(value__, "count_of_removals_for_bad_behavior")),
    count_of_tls_errors(impl::ValueMember(value__, "count_of_tls_errors")),
    count_of_run_attempts_while_revoked(impl::ValueMember(value__, "count_of_run_attempts_while_revoked")) {
}
Json::Value AppLevel::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("minutes_in_hmi_full", minutes_in_hmi_full, &result__);
  impl::WriteJsonField("app_registration_language_gui", app_registration_language_gui, &result__);
  impl::WriteJsonField("app_registration_language_vui", app_registration_language_vui, &result__);
  impl::WriteJsonField("minutes_in_hmi_limited", minutes_in_hmi_limited, &result__);
  impl::WriteJsonField("minutes_in_hmi_background", minutes_in_hmi_background, &result__);
  impl::WriteJsonField("minutes_in_hmi_none", minutes_in_hmi_none, &result__);
  impl::WriteJsonField("count_of_user_selections", count_of_user_selections, &result__);
  impl::WriteJsonField("count_of_rejections_sync_out_of_memory", count_of_rejections_sync_out_of_memory, &result__);
  impl::WriteJsonField("count_of_rejections_nickname_mismatch", count_of_rejections_nickname_mismatch, &result__);
  impl::WriteJsonField("count_of_rejections_duplicate_name", count_of_rejections_duplicate_name, &result__);
  impl::WriteJsonField("count_of_rejected_rpc_calls", count_of_rejected_rpc_calls, &result__);
  impl::WriteJsonField("count_of_rpcs_sent_in_hmi_none", count_of_rpcs_sent_in_hmi_none, &result__);
  impl::WriteJsonField("count_of_removals_for_bad_behavior", count_of_removals_for_bad_behavior, &result__);
  impl::WriteJsonField("count_of_tls_errors", count_of_tls_errors, &result__);
  impl::WriteJsonField("count_of_run_attempts_while_revoked", count_of_run_attempts_while_revoked, &result__);
  return result__;
}
bool AppLevel::is_valid() const {
  if (!minutes_in_hmi_full.is_valid()) {
    return false;
  }
  if (!app_registration_language_gui.is_valid()) {
    return false;
  }
  if (!app_registration_language_vui.is_valid()) {
    return false;
  }
  if (!minutes_in_hmi_limited.is_valid()) {
    return false;
  }
  if (!minutes_in_hmi_background.is_valid()) {
    return false;
  }
  if (!minutes_in_hmi_none.is_valid()) {
    return false;
  }
  if (!count_of_user_selections.is_valid()) {
    return false;
  }
  if (!count_of_rejections_sync_out_of_memory.is_valid()) {
    return false;
  }
  if (!count_of_rejections_nickname_mismatch.is_valid()) {
    return false;
  }
  if (!count_of_rejections_duplicate_name.is_valid()) {
    return false;
  }
  if (!count_of_rejected_rpc_calls.is_valid()) {
    return false;
  }
  if (!count_of_rpcs_sent_in_hmi_none.is_valid()) {
    return false;
  }
  if (!count_of_removals_for_bad_behavior.is_valid()) {
    return false;
  }
  if (!count_of_tls_errors.is_valid()) {
    return false;
  }
  if (!count_of_run_attempts_while_revoked.is_valid()) {
    return false;
  }
  return Validate();
}
bool AppLevel::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool AppLevel::struct_empty() const {

  if (minutes_in_hmi_full.is_initialized()) {
    return false;
  }
  if (app_registration_language_gui.is_initialized()) {
    return false;
  }

  if (app_registration_language_vui.is_initialized()) {
    return false;
  }

  if (minutes_in_hmi_limited.is_initialized()) {
    return false;
  }
  if (minutes_in_hmi_background.is_initialized()) {
    return false;
  }

  if (minutes_in_hmi_none.is_initialized()) {
    return false;
  }
  if (count_of_user_selections.is_initialized()) {
    return false;
  }

  if (count_of_rejections_sync_out_of_memory.is_initialized()) {
    return false;
  }
  if (count_of_rejections_nickname_mismatch.is_initialized()) {
    return false;
  }

  if (count_of_rejections_duplicate_name.is_initialized()) {
    return false;
  }
  if (count_of_rejected_rpc_calls.is_initialized()) {
    return false;
  }

  if (count_of_rpcs_sent_in_hmi_none.is_initialized()) {
    return false;
  }
  if (count_of_removals_for_bad_behavior.is_initialized()) {
    return false;
  }
  if (count_of_tls_errors.is_initialized()) {
    return false;
  }
  if (count_of_run_attempts_while_revoked.is_initialized()) {
    return false;
  }
  return true;
}
void AppLevel::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_UPDATE == GetPolicyTableType()) {
    std::string validation_info = ommited_validation_info +
                                      PolicyTableTypeToString(GetPolicyTableType());
    report__->set_validation_info(validation_info);
  }
}

// UsageAndErrorCounts methods
UsageAndErrorCounts::UsageAndErrorCounts()
  : CompositeType(kUninitialized) {
}

UsageAndErrorCounts::~UsageAndErrorCounts() {
}
UsageAndErrorCounts::UsageAndErrorCounts(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    app_level(impl::ValueMember(value__, "app_level")) {
}
Json::Value UsageAndErrorCounts::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("app_level", app_level, &result__);
  return result__;
}
bool UsageAndErrorCounts::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized && Validate();
  }
  if (!app_level.is_valid()) {
    return false;
  }
  return Validate();
}
bool UsageAndErrorCounts::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool UsageAndErrorCounts::struct_empty() const {
  if (app_level.is_initialized()) {
    return false;
  }
  return true;
}
void UsageAndErrorCounts::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_UPDATE == GetPolicyTableType()) {
    std::string validation_info = ommited_validation_info +
                                  PolicyTableTypeToString(GetPolicyTableType());
    report__->set_validation_info(validation_info);
  }
  if (!app_level.is_valid()) {
    app_level.ReportErrors(&report__->ReportSubobject("app_level"));
  }
}

void UsageAndErrorCounts::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  app_level.SetPolicyTableType(pt_type);
}

// DeviceParams methods
DeviceParams::DeviceParams()
  : CompositeType(kUninitialized) {
}
DeviceParams::~DeviceParams() {
}
DeviceParams::DeviceParams(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)) {
}
Json::Value DeviceParams::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  return result__;
}
bool DeviceParams::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized && Validate();
  }
  return Validate();
}
bool DeviceParams::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool DeviceParams::struct_empty() const {
  return true;
}
void DeviceParams::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
}

// PolicyTable methods
PolicyTable::PolicyTable()
  : CompositeType(kUninitialized) {
}
PolicyTable::PolicyTable(const ApplicationPoliciesSection& app_policies_section, const FunctionalGroupings& functional_groupings, const ConsumerFriendlyMessages& consumer_friendly_messages, const ModuleConfig& module_config)
  : CompositeType(kUninitialized),
    app_policies_section(app_policies_section),
    functional_groupings(functional_groupings),
    consumer_friendly_messages(consumer_friendly_messages),
    module_config(module_config) {
}
PolicyTable::~PolicyTable() {
}
PolicyTable::PolicyTable(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    app_policies_section(impl::ValueMember(value__, "app_policies")),
    functional_groupings(impl::ValueMember(value__, "functional_groupings")),
    consumer_friendly_messages(impl::ValueMember(value__, "consumer_friendly_messages")),
    module_config(impl::ValueMember(value__, "module_config")),
    module_meta(impl::ValueMember(value__, "module_meta")),
    usage_and_error_counts(impl::ValueMember(value__, "usage_and_error_counts")),
    device_data(impl::ValueMember(value__, "device_data")) {
}
Json::Value PolicyTable::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("app_policies", app_policies_section, &result__);
  impl::WriteJsonField("functional_groupings", functional_groupings, &result__);
  impl::WriteJsonField("consumer_friendly_messages", consumer_friendly_messages, &result__);
  impl::WriteJsonField("module_config", module_config, &result__);
  impl::WriteJsonField("module_meta", module_meta, &result__);
  impl::WriteJsonField("usage_and_error_counts", usage_and_error_counts, &result__);
  impl::WriteJsonField("device_data", device_data, &result__);
  return result__;
}
bool PolicyTable::is_valid() const {
  if (!app_policies_section.is_valid()) {
    return false;
  }
  if (!functional_groupings.is_valid()) {
    return false;
  }
  if (!consumer_friendly_messages.is_valid()) {
    return false;
  }
  if (!module_config.is_valid()) {
    return false;
  }
  if (!module_meta.is_valid()) {
    return false;
  }
  if (!usage_and_error_counts.is_valid()) {
    return false;
  }
  if (!device_data.is_valid()) {
    return false;
  }
  return Validate();
}
bool PolicyTable::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool PolicyTable::struct_empty() const {
  if (app_policies_section.is_initialized()) {
    return false;
  }
  if (functional_groupings.is_initialized()) {
    return false;
  }

  if (consumer_friendly_messages.is_initialized()) {
    return false;
  }
  if (module_config.is_initialized()) {
    return false;
  }

  if (module_meta.is_initialized()) {
    return false;
  }
  if (usage_and_error_counts.is_initialized()) {
    return false;
  }

  if (device_data.is_initialized()) {
    return false;
  }
  return true;
}
void PolicyTable::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_UPDATE == GetPolicyTableType()) {
    std::string validation_info = ommited_validation_info +
                                  PolicyTableTypeToString(GetPolicyTableType());

    if (device_data.is_initialized()) {
      report__->ReportSubobject("device_data").set_validation_info(validation_info);
    }
  }
  if (!app_policies_section.is_valid()) {
    app_policies_section.ReportErrors(&report__->ReportSubobject("app_policies"));
  }
  if (!functional_groupings.is_valid()) {
    functional_groupings.ReportErrors(&report__->ReportSubobject("functional_groupings"));
  }
  if (!consumer_friendly_messages.is_valid()) {
    consumer_friendly_messages.ReportErrors(&report__->ReportSubobject("consumer_friendly_messages"));
  }
  if (!module_config.is_valid()) {
    module_config.ReportErrors(&report__->ReportSubobject("module_config"));
  }
  if (!module_meta.is_valid()) {
    module_meta.ReportErrors(&report__->ReportSubobject("module_meta"));
  }
  if (!usage_and_error_counts.is_valid()) {
    usage_and_error_counts.ReportErrors(&report__->ReportSubobject("usage_and_error_counts"));
  }
  if (!device_data.is_valid()) {
    device_data.ReportErrors(&report__->ReportSubobject("device_data"));
  }
}

void PolicyTable::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  app_policies_section.SetPolicyTableType(pt_type);
  functional_groupings.SetPolicyTableType(pt_type);
  consumer_friendly_messages.SetPolicyTableType(pt_type);
  module_config.SetPolicyTableType(pt_type);
  module_meta.SetPolicyTableType(pt_type);
  usage_and_error_counts.SetPolicyTableType(pt_type);
  device_data.SetPolicyTableType(pt_type);
}

// Table methods
Table::Table()
  : CompositeType(kUninitialized) {
}
Table::Table(const PolicyTable& policy_table)
  : CompositeType(kUninitialized),
    policy_table(policy_table) {
}
Table::~Table() {
}
Table::Table(const Json::Value* value__)
  : CompositeType(InitHelper(value__, &Json::Value::isObject)),
    policy_table(impl::ValueMember(value__, "policy_table")) {
}
Json::Value Table::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("policy_table", policy_table, &result__);
  return result__;
}
bool Table::is_valid() const {
  if (!policy_table.is_valid()) {
    return false;
  }
  return Validate();
}
bool Table::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}
bool Table::struct_empty() const {
  if (policy_table.is_initialized()) {
    return false;
  }
  return true;
}
void Table::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!policy_table.is_valid()) {
    policy_table.ReportErrors(&report__->ReportSubobject("policy_table"));
  }
}

void Table::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  policy_table.SetPolicyTableType(pt_type);
}

}  // namespace policy_table_interface_base
}  // namespace rpc

