// This file is generated, do not edit
#include "policy/policy_table/types.h"
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
    default: { return "INVALID_PT_TYPE"; }
  }
}

// PolicyBase methods
PolicyBase::PolicyBase() : CompositeType(kUninitialized) {}

PolicyBase::PolicyBase(Priority priority)
    : CompositeType(kUninitialized), priority(priority) {}

PolicyBase::~PolicyBase() {}

PolicyBase::PolicyBase(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , priority(impl::ValueMember(value__, "priority")) {}
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
DevicePolicy::DevicePolicy() : PolicyBase() {}

DevicePolicy::DevicePolicy(Priority priority) : PolicyBase(priority) {}

DevicePolicy::~DevicePolicy() {}

DevicePolicy::DevicePolicy(const Json::Value* value__) : PolicyBase(value__) {}

// AppPoliciesSection methods
ApplicationPoliciesSection::ApplicationPoliciesSection()
    : CompositeType(kUninitialized) {}

ApplicationPoliciesSection::ApplicationPoliciesSection(
    const ApplicationPolicies& apps, const DevicePolicy& device)
    : CompositeType(kUninitialized), apps(apps), device(device) {}

ApplicationPoliciesSection::~ApplicationPoliciesSection() {}

ApplicationPoliciesSection::ApplicationPoliciesSection(
    const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , apps(value__)
    , device(impl::ValueMember(value__, "device")) {
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

void ApplicationPoliciesSection::ReportErrors(
    rpc::ValidationReport* report__) const {
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
// Handled RPC Methods
AppServiceHandledRpc::AppServiceHandledRpc() : CompositeType(kUninitialized) {}

AppServiceHandledRpc::~AppServiceHandledRpc() {}

AppServiceHandledRpc::AppServiceHandledRpc(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , function_id(impl::ValueMember(value__, "function_id")) {}

Json::Value AppServiceHandledRpc::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("function_id", function_id, &result__);
  return result__;
}

bool AppServiceHandledRpc::is_valid() const {
  if (!function_id.is_valid()) {
    return false;
  }
  return Validate();
}

bool AppServiceHandledRpc::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}

bool AppServiceHandledRpc::struct_empty() const {
  if (function_id.is_initialized()) {
    return false;
  }
  return true;
}

void AppServiceHandledRpc::SetPolicyTableType(PolicyTableType pt_type) {
  function_id.SetPolicyTableType(pt_type);
}

void AppServiceHandledRpc::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!function_id.is_valid()) {
    function_id.ReportErrors(&report__->ReportSubobject("function_id"));
  }
}

// AppServiceInfo methods
AppServiceInfo::AppServiceInfo() : CompositeType(kUninitialized) {}

AppServiceInfo::~AppServiceInfo() {}

AppServiceInfo::AppServiceInfo(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , service_names(impl::ValueMember(value__, "service_names"))
    , handled_rpcs(impl::ValueMember(value__, "handled_rpcs")) {}

Json::Value AppServiceInfo::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("service_names", service_names, &result__);
  impl::WriteJsonField("parameters", handled_rpcs, &result__);
  return result__;
}

bool AppServiceInfo::is_valid() const {
  if (!service_names.is_valid()) {
    return false;
  }
  if (!handled_rpcs.is_valid()) {
    return false;
  }
  return Validate();
}

bool AppServiceInfo::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}

bool AppServiceInfo::struct_empty() const {
  if (service_names.is_initialized()) {
    return false;
  }
  if (handled_rpcs.is_initialized()) {
    return false;
  }
  return true;
}

void AppServiceInfo::SetPolicyTableType(PolicyTableType pt_type) {
  service_names.SetPolicyTableType(pt_type);
  handled_rpcs.SetPolicyTableType(pt_type);
}

void AppServiceInfo::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!service_names.is_valid()) {
    service_names.ReportErrors(&report__->ReportSubobject("service_names"));
  }
  if (!handled_rpcs.is_valid()) {
    handled_rpcs.ReportErrors(&report__->ReportSubobject("handled_rpcs"));
  }
}

// ApplicationParams methods
ApplicationParams::ApplicationParams() : PolicyBase(), groups() {}

ApplicationParams::ApplicationParams(const Strings& groups, Priority priority)
    : PolicyBase(priority), groups(groups) {}

ApplicationParams::~ApplicationParams() {}

ApplicationParams::ApplicationParams(const Json::Value* value__)
    : PolicyBase(value__)
    , groups(impl::ValueMember(value__, "groups"))
    , nicknames(impl::ValueMember(value__, "nicknames"))
    , AppHMIType(impl::ValueMember(value__, "AppHMIType"))
    , RequestType(impl::ValueMember(value__, "RequestType"))
    , RequestSubType(impl::ValueMember(value__, "RequestSubType"))
    , memory_kb(impl::ValueMember(value__, "memory_kb"), 0)
    , heart_beat_timeout_ms(impl::ValueMember(value__, "heart_beat_timeout_ms"))
    , certificate(impl::ValueMember(value__, "certificate"))
    , moduleType(impl::ValueMember(value__, "moduleType"))
    , hybrid_app_preference(impl::ValueMember(value__, "hybrid_app_preference"))
    , endpoint(impl::ValueMember(value__, "endpoint"))
    , enabled(impl::ValueMember(value__, "enabled"))
    , auth_token(impl::ValueMember(value__, "auth_token"))
    , cloud_transport_type(impl::ValueMember(value__, "cloud_transport_type"))
    , icon_url(impl::ValueMember(value__, "icon_url"))
    , app_service_parameters(impl::ValueMember(value__, "app_services"))
    , allow_unknown_rpc_passthrough(
          impl::ValueMember(value__, "allow_unknown_rpc_passthrough"))
    , encryption_required(impl::ValueMember(value__, "encryption_required")) {}

Json::Value ApplicationParams::ToJsonValue() const {
  Json::Value result__(PolicyBase::ToJsonValue());
  impl::WriteJsonField("groups", groups, &result__);
  impl::WriteJsonField("nicknames", nicknames, &result__);
  impl::WriteJsonField("AppHMIType", AppHMIType, &result__);
  impl::WriteJsonField("RequestType", RequestType, &result__);
  impl::WriteJsonField("RequestSubType", RequestSubType, &result__);
  impl::WriteJsonField("memory_kb", memory_kb, &result__);
  impl::WriteJsonField(
      "heart_beat_timeout_ms", heart_beat_timeout_ms, &result__);
  impl::WriteJsonField("certificate", certificate, &result__);
  impl::WriteJsonField("moduleType", moduleType, &result__);
  impl::WriteJsonField(
      "hybrid_app_preference", hybrid_app_preference, &result__);
  impl::WriteJsonField("endpoint", endpoint, &result__);
  impl::WriteJsonField("enabled", enabled, &result__);
  impl::WriteJsonField("auth_token", auth_token, &result__);
  impl::WriteJsonField("cloud_transport_type", cloud_transport_type, &result__);
  impl::WriteJsonField("icon_url", auth_token, &result__);
  impl::WriteJsonField("app_services", app_service_parameters, &result__);
  impl::WriteJsonField("allow_unknown_rpc_passthrough",
                       allow_unknown_rpc_passthrough,
                       &result__);
  impl::WriteJsonField("encryption_required", encryption_required, &result__);

  return result__;
}

bool ApplicationParams::is_valid() const {
  // RequestType and RequestSubType are not validated since there is high-level
  // validation logic,
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
  if (!moduleType.is_valid()) {
    return false;
  }
  if (!endpoint.is_valid()) {
    return false;
  }
  if (!enabled.is_valid()) {
    return false;
  }
  if (!auth_token.is_valid()) {
    return false;
  }
  if (!cloud_transport_type.is_valid()) {
    return false;
  }
  if (!hybrid_app_preference.is_valid()) {
    return false;
  }
  if (!icon_url.is_valid()) {
    return false;
  }
  if (!app_service_parameters.is_valid()) {
    return false;
  }
  if (!allow_unknown_rpc_passthrough.is_valid()) {
    return false;
  }
  if (!encryption_required.is_valid()) {
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
  if (RequestSubType.is_initialized()) {
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
  if (moduleType.is_initialized()) {
    return false;
  }
  if (endpoint.is_initialized()) {
    return false;
  }
  if (enabled.is_initialized()) {
    return false;
  }
  if (auth_token.is_initialized()) {
    return false;
  }
  if (cloud_transport_type.is_initialized()) {
    return false;
  }
  if (hybrid_app_preference.is_initialized()) {
    return false;
  }
  if (icon_url.is_initialized()) {
    return false;
  }
  if (app_service_parameters.is_initialized()) {
    return false;
  }
  if (allow_unknown_rpc_passthrough.is_initialized()) {
    return false;
  }
  if (encryption_required.is_initialized()) {
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
  if (!RequestSubType.is_valid()) {
    RequestSubType.ReportErrors(&report__->ReportSubobject("RequestSubType"));
  }
  if (!priority.is_valid()) {
    priority.ReportErrors(&report__->ReportSubobject("priority"));
  }
  if (!memory_kb.is_valid()) {
    memory_kb.ReportErrors(&report__->ReportSubobject("memory_kb"));
  }
  if (!heart_beat_timeout_ms.is_valid()) {
    heart_beat_timeout_ms.ReportErrors(
        &report__->ReportSubobject("heart_beat_timeout_ms"));
  }
  if (!certificate.is_valid()) {
    certificate.ReportErrors(&report__->ReportSubobject("certificate"));
  }
  if (!moduleType.is_valid()) {
    moduleType.ReportErrors(&report__->ReportSubobject("moduleType"));
  }
  if (!endpoint.is_valid()) {
    moduleType.ReportErrors(&report__->ReportSubobject("endpoint"));
  }
  if (!enabled.is_valid()) {
    moduleType.ReportErrors(&report__->ReportSubobject("enabled"));
  }
  if (!auth_token.is_valid()) {
    moduleType.ReportErrors(&report__->ReportSubobject("auth_token"));
  }
  if (!cloud_transport_type.is_valid()) {
    moduleType.ReportErrors(&report__->ReportSubobject("cloud_transport_type"));
  }
  if (!hybrid_app_preference.is_valid()) {
    moduleType.ReportErrors(
        &report__->ReportSubobject("hybrid_app_preference"));
  }
  if (!icon_url.is_valid()) {
    moduleType.ReportErrors(&report__->ReportSubobject("icon_url"));
  }
  if (!app_service_parameters.is_valid()) {
    app_service_parameters.ReportErrors(
        &report__->ReportSubobject("app_services"));
  }
  if (!allow_unknown_rpc_passthrough.is_valid()) {
    allow_unknown_rpc_passthrough.ReportErrors(
        &report__->ReportSubobject("allow_unknown_rpc_passthrough"));
  }
  if (!encryption_required.is_valid()) {
    encryption_required.ReportErrors(
        &report__->ReportSubobject("encryption_required"));
  }
}

void ApplicationParams::SetPolicyTableType(PolicyTableType pt_type) {
  PolicyBase::SetPolicyTableType(pt_type);
  groups.SetPolicyTableType(pt_type);
  AppHMIType.SetPolicyTableType(pt_type);
  RequestType.SetPolicyTableType(pt_type);
  RequestSubType.SetPolicyTableType(pt_type);
  memory_kb.SetPolicyTableType(pt_type);
  heart_beat_timeout_ms.SetPolicyTableType(pt_type);
  certificate.SetPolicyTableType(pt_type);
  moduleType.SetPolicyTableType(pt_type);
  endpoint.SetPolicyTableType(pt_type);
  enabled.SetPolicyTableType(pt_type);
  cloud_transport_type.SetPolicyTableType(pt_type);
  hybrid_app_preference.SetPolicyTableType(pt_type);
  icon_url.SetPolicyTableType(pt_type);
  app_service_parameters.SetPolicyTableType(pt_type);
  allow_unknown_rpc_passthrough.SetPolicyTableType(pt_type);
}

// RpcParameters methods
RpcParameters::RpcParameters() : CompositeType(kUninitialized) {}

RpcParameters::RpcParameters(const HmiLevels& hmi_levels)
    : CompositeType(kUninitialized), hmi_levels(hmi_levels) {}

RpcParameters::~RpcParameters() {}

RpcParameters::RpcParameters(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , hmi_levels(impl::ValueMember(value__, "hmi_levels"))
    , parameters(impl::ValueMember(value__, "parameters")) {}

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
Rpcs::Rpcs() : CompositeType(kUninitialized) {}

Rpcs::Rpcs(const Rpc& rpcs) : CompositeType(kUninitialized), rpcs(rpcs) {}

Rpcs::~Rpcs() {}

Rpcs::Rpcs(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , user_consent_prompt(impl::ValueMember(value__, "user_consent_prompt"))
    , rpcs(impl::ValueMember(value__, "rpcs"))
    , encryption_required(impl::ValueMember(value__, "encryption_required")) {}

Json::Value Rpcs::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("user_consent_prompt", user_consent_prompt, &result__);
  impl::WriteJsonField("rpcs", rpcs, &result__);
  impl::WriteJsonField("encryption_required", encryption_required, &result__);
  return result__;
}

bool Rpcs::is_valid() const {
  if (!user_consent_prompt.is_valid()) {
    return false;
  }
  if (!rpcs.is_valid()) {
    return false;
  }
  if (!encryption_required.is_valid()) {
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
  if (encryption_required.is_initialized()) {
    return false;
  }
  return true;
}

void Rpcs::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!user_consent_prompt.is_valid()) {
    user_consent_prompt.ReportErrors(
        &report__->ReportSubobject("user_consent_prompt"));
  }
  if (!rpcs.is_valid()) {
    rpcs.ReportErrors(&report__->ReportSubobject("rpcs"));
  }
  if (!encryption_required.is_valid()) {
    encryption_required.ReportErrors(
        &report__->ReportSubobject("encryption_required"));
  }
}

void Rpcs::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  user_consent_prompt.SetPolicyTableType(pt_type);
  rpcs.SetPolicyTableType(pt_type);
}

// EndpointProperties methods
EndpointProperty::EndpointProperty() : CompositeType(kUninitialized) {}

EndpointProperty::~EndpointProperty() {}

EndpointProperty::EndpointProperty(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , version(impl::ValueMember(value__, "version")) {}

Json::Value EndpointProperty::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("version", version, &result__);
  return result__;
}

bool EndpointProperty::is_valid() const {
  if (!version.is_valid()) {
    return false;
  }
  return Validate();
}

bool EndpointProperty::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}

bool EndpointProperty::struct_empty() const {
  if (version.is_initialized()) {
    return false;
  }

  return true;
}

void EndpointProperty::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!version.is_valid()) {
    version.ReportErrors(&report__->ReportSubobject("version"));
  }
}

void EndpointProperty::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  version.SetPolicyTableType(pt_type);
}

// ModuleConfig methods
ModuleConfig::ModuleConfig() : CompositeType(kUninitialized) {}

const std::string ModuleConfig::kDefaultOemMappingServiceName =
    "custom_vehicle_data_mapping_url";

ModuleConfig::ModuleConfig(
    uint8_t exchange_after_x_ignition_cycles,
    int64_t exchange_after_x_kilometers,
    uint8_t exchange_after_x_days,
    uint16_t timeout_after_x_seconds,
    const SecondsBetweenRetries& seconds_between_retries,
    const ServiceEndpoints& endpoints,
    const ServiceEndpointProperties& endpoint_properties,
    const NumberOfNotificationsPerMinute& notifications_per_minute_by_priority)
    : CompositeType(kUninitialized)
    , exchange_after_x_ignition_cycles(exchange_after_x_ignition_cycles)
    , exchange_after_x_kilometers(exchange_after_x_kilometers)
    , exchange_after_x_days(exchange_after_x_days)
    , timeout_after_x_seconds(timeout_after_x_seconds)
    , seconds_between_retries(seconds_between_retries)
    , endpoints(endpoints)
    , endpoint_properties(endpoint_properties)
    , notifications_per_minute_by_priority(
          notifications_per_minute_by_priority) {}

ModuleConfig::~ModuleConfig() {}

ModuleConfig::ModuleConfig(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , device_certificates(impl::ValueMember(value__, "device_certificates"))
    , preloaded_pt(impl::ValueMember(value__, "preloaded_pt"))
    , full_app_id_supported(impl::ValueMember(value__, "full_app_id_supported"))
    , exchange_after_x_ignition_cycles(
          impl::ValueMember(value__, "exchange_after_x_ignition_cycles"))
    , exchange_after_x_kilometers(
          impl::ValueMember(value__, "exchange_after_x_kilometers"))
    , exchange_after_x_days(impl::ValueMember(value__, "exchange_after_x_days"))
    , timeout_after_x_seconds(
          impl::ValueMember(value__, "timeout_after_x_seconds"))
    , seconds_between_retries(
          impl::ValueMember(value__, "seconds_between_retries"))
    , endpoints(impl::ValueMember(value__, "endpoints"))
    , endpoint_properties(impl::ValueMember(value__, "endpoint_properties"))
    , notifications_per_minute_by_priority(
          impl::ValueMember(value__, "notifications_per_minute_by_priority"))
    , vehicle_make(impl::ValueMember(value__, "vehicle_make"))
    , vehicle_model(impl::ValueMember(value__, "vehicle_model"))
    , vehicle_year(impl::ValueMember(value__, "vehicle_year"))
    , preloaded_date(impl::ValueMember(value__, "preloaded_date"))
    , certificate(impl::ValueMember(value__, "certificate"))
    , lock_screen_dismissal_enabled(
          impl::ValueMember(value__, "lock_screen_dismissal_enabled")) {}

void ModuleConfig::SafeCopyFrom(const ModuleConfig& from) {
  //  device_certificates = from.device_certificates;  // According to the
  //  requirements this is optional.
  exchange_after_x_ignition_cycles = from.exchange_after_x_ignition_cycles;
  exchange_after_x_kilometers = from.exchange_after_x_kilometers;
  exchange_after_x_days = from.exchange_after_x_days;
  timeout_after_x_seconds = from.timeout_after_x_seconds;
  seconds_between_retries = from.seconds_between_retries;
  endpoints = from.endpoints;
  endpoint_properties = from.endpoint_properties;
  notifications_per_minute_by_priority =
      from.notifications_per_minute_by_priority;

  lock_screen_dismissal_enabled = from.lock_screen_dismissal_enabled;

  vehicle_make.assign_if_valid(from.vehicle_make);
  vehicle_model.assign_if_valid(from.vehicle_model);
  vehicle_year.assign_if_valid(from.vehicle_year);
  certificate.assign_if_valid(from.certificate);
}

Json::Value ModuleConfig::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("preloaded_pt", preloaded_pt, &result__);
  impl::WriteJsonField(
      "full_app_id_supported", full_app_id_supported, &result__);
  impl::WriteJsonField("exchange_after_x_ignition_cycles",
                       exchange_after_x_ignition_cycles,
                       &result__);
  impl::WriteJsonField(
      "exchange_after_x_kilometers", exchange_after_x_kilometers, &result__);
  impl::WriteJsonField(
      "exchange_after_x_days", exchange_after_x_days, &result__);
  impl::WriteJsonField(
      "timeout_after_x_seconds", timeout_after_x_seconds, &result__);
  impl::WriteJsonField(
      "seconds_between_retries", seconds_between_retries, &result__);
  impl::WriteJsonField("endpoints", endpoints, &result__);
  impl::WriteJsonField("endpoint_properties", endpoint_properties, &result__);
  impl::WriteJsonField("notifications_per_minute_by_priority",
                       notifications_per_minute_by_priority,
                       &result__);
  impl::WriteJsonField("vehicle_make", vehicle_make, &result__);
  impl::WriteJsonField("vehicle_model", vehicle_model, &result__);
  impl::WriteJsonField("vehicle_year", vehicle_year, &result__);
  impl::WriteJsonField("certificate", certificate, &result__);
  impl::WriteJsonField("preloaded_date", preloaded_date, &result__);
  impl::WriteJsonField("lock_screen_dismissal_enabled",
                       lock_screen_dismissal_enabled,
                       &result__);
  return result__;
}

bool ModuleConfig::is_valid() const {
  if (!preloaded_pt.is_valid()) {
    return false;
  }
  if (!full_app_id_supported.is_valid()) {
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
  if (!endpoint_properties.is_valid()) {
    return false;
  }
  if (!notifications_per_minute_by_priority.is_valid()) {
    return false;
  }
  if (!lock_screen_dismissal_enabled.is_valid()) {
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
  if (preloaded_pt.is_initialized()) {
    return false;
  }
  if (full_app_id_supported.is_initialized()) {
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

  if (endpoint_properties.is_initialized()) {
    return false;
  }

  if (notifications_per_minute_by_priority.is_initialized()) {
    return false;
  }
  if (lock_screen_dismissal_enabled.is_initialized()) {
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
    device_certificates.ReportErrors(
        &report__->ReportSubobject("device_certificates"));
  }
  if (!preloaded_pt.is_valid()) {
    preloaded_pt.ReportErrors(&report__->ReportSubobject("preloaded_pt"));
  }
  if (!full_app_id_supported.is_valid()) {
    full_app_id_supported.ReportErrors(
        &report__->ReportSubobject("full_app_id_supported"));
  }
  if (!exchange_after_x_ignition_cycles.is_valid()) {
    exchange_after_x_ignition_cycles.ReportErrors(
        &report__->ReportSubobject("exchange_after_x_ignition_cycles"));
  }
  if (!exchange_after_x_kilometers.is_valid()) {
    exchange_after_x_kilometers.ReportErrors(
        &report__->ReportSubobject("exchange_after_x_kilometers"));
  }
  if (!exchange_after_x_days.is_valid()) {
    exchange_after_x_days.ReportErrors(
        &report__->ReportSubobject("exchange_after_x_days"));
  }
  if (!timeout_after_x_seconds.is_valid()) {
    timeout_after_x_seconds.ReportErrors(
        &report__->ReportSubobject("timeout_after_x_seconds"));
  }
  if (!seconds_between_retries.is_valid()) {
    seconds_between_retries.ReportErrors(
        &report__->ReportSubobject("seconds_between_retries"));
  }
  if (!endpoints.is_valid()) {
    endpoints.ReportErrors(&report__->ReportSubobject("endpoints"));
  }
  if (!endpoint_properties.is_valid()) {
    endpoint_properties.ReportErrors(
        &report__->ReportSubobject("endpoint_properties"));
  }
  if (!notifications_per_minute_by_priority.is_valid()) {
    notifications_per_minute_by_priority.ReportErrors(
        &report__->ReportSubobject("notifications_per_minute_by_priority"));
  }
  if (!lock_screen_dismissal_enabled.is_valid()) {
    lock_screen_dismissal_enabled.ReportErrors(
        &report__->ReportSubobject("lock_screen_dismissal_enabled"));
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
    std::string validation_info =
        omitted_validation_info + PolicyTableTypeToString(GetPolicyTableType());
    rpc::ValidationReport* omitted_field_report;
    if (vehicle_make.is_initialized()) {
      omitted_field_report = &report__->ReportSubobject("vehicle_make");
      omitted_field_report->set_validation_info(validation_info);
    }
    if (vehicle_year.is_initialized()) {
      omitted_field_report = &report__->ReportSubobject("vehicle_year");
      omitted_field_report->set_validation_info(validation_info);
    }
    if (vehicle_model.is_initialized()) {
      omitted_field_report = &report__->ReportSubobject("vehicle_model");
      omitted_field_report->set_validation_info(validation_info);
    }
  }
}

void ModuleConfig::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  preloaded_pt.SetPolicyTableType(pt_type);
  full_app_id_supported.SetPolicyTableType(pt_type);
  exchange_after_x_ignition_cycles.SetPolicyTableType(pt_type);
  exchange_after_x_kilometers.SetPolicyTableType(pt_type);
  exchange_after_x_days.SetPolicyTableType(pt_type);
  timeout_after_x_seconds.SetPolicyTableType(pt_type);
  seconds_between_retries.SetPolicyTableType(pt_type);
  endpoints.SetPolicyTableType(pt_type);
  endpoint_properties.SetPolicyTableType(pt_type);
  notifications_per_minute_by_priority.SetPolicyTableType(pt_type);
  lock_screen_dismissal_enabled.SetPolicyTableType(pt_type);
  vehicle_make.SetPolicyTableType(pt_type);
  vehicle_model.SetPolicyTableType(pt_type);
  vehicle_year.SetPolicyTableType(pt_type);
}

// MessageString methods
MessageString::MessageString() : CompositeType(kUninitialized) {}

MessageString::~MessageString() {}

MessageString::MessageString(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , line1(impl::ValueMember(value__, "line1"))
    , line2(impl::ValueMember(value__, "line2"))
    , tts(impl::ValueMember(value__, "tts"))
    , label(impl::ValueMember(value__, "label"))
    , textBody(impl::ValueMember(value__, "textBody")) {}

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
const std::string MessageLanguages::default_language_("en-us");

MessageLanguages::MessageLanguages() : CompositeType(kUninitialized) {}

MessageLanguages::MessageLanguages(const Languages& languages)
    : CompositeType(kUninitialized), languages(languages) {}

MessageLanguages::~MessageLanguages() {}

MessageLanguages::MessageLanguages(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , languages(impl::ValueMember(value__, "languages")) {}

Json::Value MessageLanguages::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("languages", languages, &result__);
  return result__;
}

bool MessageLanguages::is_valid() const {
  if (!languages.is_valid()) {
    return false;
  }
  // Each RPC must have message in english
  if (languages.end() == languages.find(default_language_)) {
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
      std::string validation_info =
          omitted_validation_info +
          PolicyTableTypeToString(GetPolicyTableType());
      report__->ReportSubobject("languages")
          .set_validation_info(validation_info);
    }
  }
  if (!languages.is_valid()) {
    languages.ReportErrors(&report__->ReportSubobject("languages"));
  }
  if (languages.end() == languages.find(default_language_)) {
    report__->set_validation_info(
        "this message does not support the default language '" +
        default_language_ + "'");
  }
}

void MessageLanguages::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  languages.SetPolicyTableType(pt_type);
}

// ConsumerFriendlyMessages methods
ConsumerFriendlyMessages::ConsumerFriendlyMessages()
    : CompositeType(kUninitialized) {}

ConsumerFriendlyMessages::ConsumerFriendlyMessages(const std::string& version)
    : CompositeType(kUninitialized), version(version) {}

ConsumerFriendlyMessages::~ConsumerFriendlyMessages() {}

ConsumerFriendlyMessages::ConsumerFriendlyMessages(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , version(impl::ValueMember(value__, "version"))
    , messages(impl::ValueMember(value__, "messages")) {}

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

void ConsumerFriendlyMessages::ReportErrors(
    rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!version.is_valid()) {
    version.ReportErrors(&report__->ReportSubobject("version"));
  }
  if (PT_SNAPSHOT == GetPolicyTableType()) {
    if (messages.is_initialized()) {
      std::string validation_info =
          omitted_validation_info +
          PolicyTableTypeToString(GetPolicyTableType());
      report__->ReportSubobject("messages")
          .set_validation_info(validation_info);
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
ModuleMeta::ModuleMeta() : CompositeType(kUninitialized) {}

ModuleMeta::~ModuleMeta() {}

ModuleMeta::ModuleMeta(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , ccpu_version(impl::ValueMember(value__, "ccpu_version"))
    , language(impl::ValueMember(value__, "language"))
    , wers_country_code(impl::ValueMember(value__, "wers_country_code"))
    , pt_exchanged_at_odometer_x(
          impl::ValueMember(value__, "pt_exchanged_at_odometer_x"))
    , pt_exchanged_x_days_after_epoch(
          impl::ValueMember(value__, "pt_exchanged_x_days_after_epoch"))
    , ignition_cycles_since_last_exchange(
          impl::ValueMember(value__, "ignition_cycles_since_last_exchange")) {}

Json::Value ModuleMeta::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("ccpu_version", ccpu_version, &result__);
  impl::WriteJsonField("language", language, &result__);
  impl::WriteJsonField("wers_country_code", wers_country_code, &result__);
  impl::WriteJsonField(
      "pt_exchanged_at_odometer_x", pt_exchanged_at_odometer_x, &result__);
  impl::WriteJsonField("pt_exchanged_x_days_after_epoch",
                       pt_exchanged_x_days_after_epoch,
                       &result__);
  impl::WriteJsonField("ignition_cycles_since_last_exchange",
                       ignition_cycles_since_last_exchange,
                       &result__);
  return result__;
}

bool ModuleMeta::is_valid() const {
  if (struct_empty()) {
    return initialization_state__ == kInitialized && Validate();
  }
  if (!ccpu_version.is_valid()) {
    return false;
  }
  if (!language.is_valid()) {
    return false;
  }
  if (!wers_country_code.is_valid()) {
    return false;
  }
  if (!pt_exchanged_at_odometer_x.is_valid()) {
    return false;
  }
  if (!pt_exchanged_x_days_after_epoch.is_valid()) {
    return false;
  }
  if (!ignition_cycles_since_last_exchange.is_valid()) {
    return false;
  }
  return Validate();
}

bool ModuleMeta::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}

bool ModuleMeta::struct_empty() const {
  if (ccpu_version.is_initialized()) {
    return false;
  }
  if (language.is_initialized()) {
    return false;
  }

  if (wers_country_code.is_initialized()) {
    return false;
  }
  if (pt_exchanged_at_odometer_x.is_initialized()) {
    return false;
  }

  if (pt_exchanged_x_days_after_epoch.is_initialized()) {
    return false;
  }
  if (ignition_cycles_since_last_exchange.is_initialized()) {
    return false;
  }
  return true;
}

void ModuleMeta::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!ccpu_version.is_valid()) {
    ccpu_version.ReportErrors(&report__->ReportSubobject("ccpu_version"));
  }
  if (!language.is_valid()) {
    language.ReportErrors(&report__->ReportSubobject("language"));
  }
  if (!wers_country_code.is_valid()) {
    wers_country_code.ReportErrors(
        &report__->ReportSubobject("wers_country_code"));
  }
  if (!pt_exchanged_at_odometer_x.is_valid()) {
    pt_exchanged_at_odometer_x.ReportErrors(
        &report__->ReportSubobject("pt_exchanged_at_odometer_x"));
  }
  if (!pt_exchanged_x_days_after_epoch.is_valid()) {
    pt_exchanged_x_days_after_epoch.ReportErrors(
        &report__->ReportSubobject("pt_exchanged_x_days_after_epoch"));
  }
  if (!ignition_cycles_since_last_exchange.is_valid()) {
    ignition_cycles_since_last_exchange.ReportErrors(
        &report__->ReportSubobject("ignition_cycles_since_last_exchange"));
  }
}

void ModuleMeta::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  ccpu_version.SetPolicyTableType(pt_type);
  language.SetPolicyTableType(pt_type);
  wers_country_code.SetPolicyTableType(pt_type);
  pt_exchanged_at_odometer_x.SetPolicyTableType(pt_type);
  pt_exchanged_x_days_after_epoch.SetPolicyTableType(pt_type);
  ignition_cycles_since_last_exchange.SetPolicyTableType(pt_type);
}

// AppLevel methods
AppLevel::AppLevel() : CompositeType(kUninitialized) {}

AppLevel::AppLevel(uint16_t minutes_in_hmi_full,
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
                   uint16_t count_of_run_attempts_while_revoked)
    : CompositeType(kUninitialized)
    , minutes_in_hmi_full(minutes_in_hmi_full)
    , app_registration_language_gui(app_registration_language_gui)
    , app_registration_language_vui(app_registration_language_vui)
    , minutes_in_hmi_limited(minutes_in_hmi_limited)
    , minutes_in_hmi_background(minutes_in_hmi_background)
    , minutes_in_hmi_none(minutes_in_hmi_none)
    , count_of_user_selections(count_of_user_selections)
    , count_of_rejections_sync_out_of_memory(
          count_of_rejections_sync_out_of_memory)
    , count_of_rejections_nickname_mismatch(
          count_of_rejections_nickname_mismatch)
    , count_of_rejections_duplicate_name(count_of_rejections_duplicate_name)
    , count_of_rejected_rpc_calls(count_of_rejected_rpc_calls)
    , count_of_rpcs_sent_in_hmi_none(count_of_rpcs_sent_in_hmi_none)
    , count_of_removals_for_bad_behavior(count_of_removals_for_bad_behavior)
    , count_of_tls_errors(count_of_tls_errors)
    , count_of_run_attempts_while_revoked(count_of_run_attempts_while_revoked) {
}

AppLevel::~AppLevel() {}

AppLevel::AppLevel(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , minutes_in_hmi_full(impl::ValueMember(value__, "minutes_in_hmi_full"))
    , app_registration_language_gui(
          impl::ValueMember(value__, "app_registration_language_gui"))
    , app_registration_language_vui(
          impl::ValueMember(value__, "app_registration_language_vui"))
    , minutes_in_hmi_limited(
          impl::ValueMember(value__, "minutes_in_hmi_limited"))
    , minutes_in_hmi_background(
          impl::ValueMember(value__, "minutes_in_hmi_background"))
    , minutes_in_hmi_none(impl::ValueMember(value__, "minutes_in_hmi_none"))
    , count_of_user_selections(
          impl::ValueMember(value__, "count_of_user_selections"))
    , count_of_rejections_sync_out_of_memory(
          impl::ValueMember(value__, "count_of_rejections_sync_out_of_memory"))
    , count_of_rejections_nickname_mismatch(
          impl::ValueMember(value__, "count_of_rejections_nickname_mismatch"))
    , count_of_rejections_duplicate_name(
          impl::ValueMember(value__, "count_of_rejections_duplicate_name"))
    , count_of_rejected_rpc_calls(
          impl::ValueMember(value__, "count_of_rejected_rpc_calls"))
    , count_of_rpcs_sent_in_hmi_none(
          impl::ValueMember(value__, "count_of_rpcs_sent_in_hmi_none"))
    , count_of_removals_for_bad_behavior(
          impl::ValueMember(value__, "count_of_removals_for_bad_behavior"))
    , count_of_tls_errors(impl::ValueMember(value__, "count_of_tls_errors"))
    , count_of_run_attempts_while_revoked(
          impl::ValueMember(value__, "count_of_run_attempts_while_revoked")) {}

Json::Value AppLevel::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("count_of_TLS_errors", count_of_tls_errors, &result__);
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
    std::string validation_info =
        omitted_validation_info + PolicyTableTypeToString(GetPolicyTableType());
    report__->set_validation_info(validation_info);
  }
}

// UsageAndErrorCounts methods
UsageAndErrorCounts::UsageAndErrorCounts() : CompositeType(kUninitialized) {}

UsageAndErrorCounts::~UsageAndErrorCounts() {}
UsageAndErrorCounts::UsageAndErrorCounts(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , app_level(impl::ValueMember(value__, "app_level")) {}
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
    std::string validation_info =
        omitted_validation_info + PolicyTableTypeToString(GetPolicyTableType());
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
DeviceParams::DeviceParams() : CompositeType(kUninitialized) {}

DeviceParams::~DeviceParams() {}

DeviceParams::DeviceParams(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject)) {}

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

// VehicleDataItem methods
VehicleDataItem::VehicleDataItem() : CompositeType(kUninitialized) {}

VehicleDataItem::VehicleDataItem(const VehicleDataItem& vehicle_data)
    : CompositeType(vehicle_data.initialization_state__)
    , name(vehicle_data.name)
    , type(vehicle_data.type)
    , key(vehicle_data.key)
    , mandatory(vehicle_data.mandatory)
    , params(vehicle_data.params)
    , array(vehicle_data.array)
    , since(vehicle_data.since)
    , until(vehicle_data.until)
    , removed(vehicle_data.removed)
    , deprecated(vehicle_data.deprecated)
    , minvalue(vehicle_data.minvalue)
    , maxvalue(vehicle_data.maxvalue)
    , minsize(vehicle_data.minsize)
    , maxsize(vehicle_data.maxsize)
    , minlength(vehicle_data.minlength)
    , maxlength(vehicle_data.maxlength) {}

VehicleDataItem::VehicleDataItem(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , name(impl::ValueMember(value__, "name"))
    , type(impl::ValueMember(value__, "type"))
    , key(impl::ValueMember(value__, "key"))
    , mandatory(impl::ValueMember(value__, "mandatory"))
    , params(impl::ValueMember(value__, "params"))
    , array(impl::ValueMember(value__, "array"))
    , since(impl::ValueMember(value__, "since"))
    , until(impl::ValueMember(value__, "until"))
    , removed(impl::ValueMember(value__, "removed"))
    , deprecated(impl::ValueMember(value__, "deprecated"))
    , minvalue(impl::ValueMember(value__, "minvalue"))
    , maxvalue(impl::ValueMember(value__, "maxvalue"))
    , minsize(impl::ValueMember(value__, "minsize"))
    , maxsize(impl::ValueMember(value__, "maxsize"))
    , minlength(impl::ValueMember(value__, "minlength"))
    , maxlength(impl::ValueMember(value__, "maxlength")) {}

VehicleDataItem::~VehicleDataItem() {}

const std::string VehicleDataItem::kInteger = "Integer";
const std::string VehicleDataItem::kStruct = "Struct";
const std::string VehicleDataItem::kString = "String";
const std::string VehicleDataItem::kFloat = "Float";
const std::string VehicleDataItem::kDouble = "Double";
const std::string VehicleDataItem::kBoolean = "Boolean";

const std::vector<std::string> VehicleDataItem::kPODTypes = {
    kInteger, kFloat, kDouble, kString, kBoolean};

Json::Value VehicleDataItem::ToJsonValue() const {
  Json::Value ret(Json::objectValue);
  impl::WriteJsonField("name", name, &ret);
  impl::WriteJsonField("type", type, &ret);
  impl::WriteJsonField("key", key, &ret);
  impl::WriteJsonField("array", array, &ret);
  impl::WriteJsonField("mandatory", mandatory, &ret);
  impl::WriteJsonField("params", params, &ret);
  impl::WriteJsonField("since", since, &ret);
  impl::WriteJsonField("until", until, &ret);
  impl::WriteJsonField("removed", removed, &ret);
  impl::WriteJsonField("deprecated", deprecated, &ret);
  impl::WriteJsonField("minvalue", minvalue, &ret);
  impl::WriteJsonField("maxvalue", maxvalue, &ret);
  impl::WriteJsonField("minsize", minsize, &ret);
  impl::WriteJsonField("maxsize", maxsize, &ret);
  impl::WriteJsonField("minlength", minlength, &ret);
  impl::WriteJsonField("maxlength", maxlength, &ret);
  return ret;
}

bool VehicleDataItem::operator==(const VehicleDataItem& vd) {
  return (name == vd.name && type == vd.type && key == vd.key &&
          mandatory == vd.mandatory && params == vd.params &&
          array == vd.array && since == vd.since && until == vd.until &&
          removed == vd.removed && deprecated == vd.deprecated &&
          minvalue == vd.minvalue && maxvalue == vd.maxvalue &&
          minsize == vd.minsize && maxsize == vd.maxsize &&
          minlength == vd.minlength && maxlength == vd.maxlength);
}

bool VehicleDataItem::is_valid() const {
  if (!name.is_valid()) {
    return false;
  }
  if (!type.is_valid()) {
    return false;
  }
  if (!key.is_valid()) {
    return false;
  }
  if (!array.is_valid()) {
    return false;
  }
  if (!mandatory.is_valid()) {
    return false;
  }
  if (!params.is_valid()) {
    return false;
  }
  if (!since.is_valid()) {
    return false;
  }
  if (!until.is_valid()) {
    return false;
  }
  if (!removed.is_valid()) {
    return false;
  }
  if (!deprecated.is_valid()) {
    return false;
  }
  if (!minvalue.is_valid()) {
    return false;
  }
  if (!maxvalue.is_valid()) {
    return false;
  }
  if (!minsize.is_valid()) {
    return false;
  }
  if (!maxsize.is_valid()) {
    return false;
  }
  if (!minlength.is_valid()) {
    return false;
  }
  if (!maxlength.is_valid()) {
    return false;
  }
  return Validate();
}

bool VehicleDataItem::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (struct_not_empty());
}

bool VehicleDataItem::struct_not_empty() const {
  if (!name.is_initialized()) {
    return false;
  }
  if (!type.is_initialized()) {
    return false;
  }
  if (!key.is_initialized()) {
    return false;
  }
  if (!array.is_initialized()) {
    return false;
  }
  if (!mandatory.is_initialized()) {
    return false;
  }
  if (!params.is_initialized()) {
    return false;
  }
  if (!since.is_initialized()) {
    return false;
  }
  if (!until.is_initialized()) {
    return false;
  }
  if (!removed.is_initialized()) {
    return false;
  }
  if (!deprecated.is_initialized()) {
    return false;
  }
  if (!minvalue.is_initialized()) {
    return false;
  }
  if (!maxvalue.is_initialized()) {
    return false;
  }
  if (!minsize.is_initialized()) {
    return false;
  }
  if (!maxsize.is_initialized()) {
    return false;
  }
  if (!minlength.is_initialized()) {
    return false;
  }
  if (!maxlength.is_initialized()) {
    return false;
  }
  return true;
}

void VehicleDataItem::ReportErrors(rpc::ValidationReport* report__) const {
  if (!struct_not_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  if (!name.is_valid()) {
    name.ReportErrors(&report__->ReportSubobject("name"));
  }
  if (!ValidateNaming(std::string(name))) {
    report__->set_validation_info(
        "Invalid name values [" + std::string(name) +
        "]. It should not contain spaces or invalid chars.");
  }
  if (!type.is_valid()) {
    type.ReportErrors(&report__->ReportSubobject("type"));
  }
  if (type.is_initialized() && !ValidateTypes()) {
    report__->set_validation_info("Unknown type [" + std::string(name) +
                                  "]: '" + std::string(type) + "'.");
  }
  if (!key.is_valid()) {
    key.ReportErrors(&report__->ReportSubobject("key"));
  }
  if (!ValidateNaming(std::string(key))) {
    report__->set_validation_info(
        "Invalid key values [" + std::string(key) +
        "]. It should not contain spaces or invalid chars.");
  }
  if (!array.is_valid()) {
    array.ReportErrors(&report__->ReportSubobject("array"));
  }
  if (!mandatory.is_valid()) {
    mandatory.ReportErrors(&report__->ReportSubobject("mandatory"));
  }
  if (!params.is_valid()) {
    params.ReportErrors(&report__->ReportSubobject("params"));
  }
  if (!since.is_valid()) {
    since.ReportErrors(&report__->ReportSubobject("since"));
  }
  if (!until.is_valid()) {
    until.ReportErrors(&report__->ReportSubobject("until"));
  }
  if (!removed.is_valid()) {
    removed.ReportErrors(&report__->ReportSubobject("removed"));
  }
  if (!deprecated.is_valid()) {
    deprecated.ReportErrors(&report__->ReportSubobject("deprecated"));
  }
  if (!minvalue.is_valid()) {
    minvalue.ReportErrors(&report__->ReportSubobject("minvalue"));
  }
  if (!maxvalue.is_valid()) {
    maxvalue.ReportErrors(&report__->ReportSubobject("maxvalue"));
  }
  if (!minsize.is_valid()) {
    minsize.ReportErrors(&report__->ReportSubobject("minsize"));
  }
  if (!maxsize.is_valid()) {
    maxsize.ReportErrors(&report__->ReportSubobject("maxsize"));
  }
  if (!minlength.is_valid()) {
    minlength.ReportErrors(&report__->ReportSubobject("minlength"));
  }
  if (!maxlength.is_valid()) {
    maxlength.ReportErrors(&report__->ReportSubobject("maxlength"));
  }
}

void VehicleDataItem::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  name.SetPolicyTableType(pt_type);
  type.SetPolicyTableType(pt_type);
  key.SetPolicyTableType(pt_type);
  array.SetPolicyTableType(pt_type);
  mandatory.SetPolicyTableType(pt_type);
  params.SetPolicyTableType(pt_type);
  since.SetPolicyTableType(pt_type);
  until.SetPolicyTableType(pt_type);
  removed.SetPolicyTableType(pt_type);
  deprecated.SetPolicyTableType(pt_type);
  minvalue.SetPolicyTableType(pt_type);
  maxvalue.SetPolicyTableType(pt_type);
  minsize.SetPolicyTableType(pt_type);
  maxsize.SetPolicyTableType(pt_type);
  minlength.SetPolicyTableType(pt_type);
  maxlength.SetPolicyTableType(pt_type);
}

bool VehicleDataItem::ValidateNaming(std::string str) const {
  auto contains_spec_chars = [](std::string str) {
    const auto invalid_chars = "!@#$%^&*";
    return str.npos != str.find_first_of(invalid_chars);
  };

  auto contains_spaces = [](std::string str) {
    const auto found_space =
        std::find_if(str.begin(), str.end(), [](unsigned char ch) {
          return std::isspace(ch);
        });

    return found_space != str.end();
  };

  auto empty_string = [](std::string str) {
    str.erase(std::remove_if(str.begin(),
                             str.end(),
                             [](unsigned char ch) { return std::isspace(ch); }),
              str.end());
    return str.length() < 1;
  };

  return !empty_string(str) && !contains_spaces(str) &&
         !contains_spec_chars(str);
}

bool VehicleDataItem::ValidateTypes() const {
  if (VehicleDataItem::kStruct == std::string(type)) {
    return params.is_initialized() && !(params->empty()) && params.is_valid();
  }
  // params should be empty for POD types and for enum values
  return (!(params.is_initialized()) || params->empty());
}

bool VehicleDataItem::IsPrimitiveType() const {
  return helpers::in_range(kPODTypes, std::string(type));
}

// VehicleData methods
VehicleData::VehicleData() : CompositeType(kUninitialized) {}

VehicleData::VehicleData(const VehicleData& vehicle_data)
    : CompositeType(vehicle_data.initialization_state__)
    , schema_version(vehicle_data.schema_version)
    , schema_items(vehicle_data.schema_items) {}

VehicleData::VehicleData(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , schema_version(impl::ValueMember(value__, "schema_version"))
    , schema_items(impl::ValueMember(value__, "schema_items")) {}

VehicleData::~VehicleData() {}

bool VehicleData::is_valid() const {
  if (!schema_version.is_valid()) {
    return false;
  }
  if (!schema_items.is_valid()) {
    return false;
  }
  return Validate();
}

bool VehicleData::is_initialized() const {
  return (initialization_state__ != kUninitialized) || (!struct_empty());
}

bool VehicleData::struct_empty() const {
  if (schema_version.is_initialized()) {
    return false;
  }
  if (schema_items.is_initialized()) {
    return false;
  }
  return true;
}

Json::Value VehicleData::ToJsonValue() const {
  Json::Value ret(Json::objectValue);
  impl::WriteJsonField("schema_version", schema_version, &ret);
  impl::WriteJsonField("schema_items", schema_items, &ret);
  return ret;
}

void VehicleData::ReportErrors(rpc::ValidationReport* report__) const {
  if (struct_empty()) {
    rpc::CompositeType::ReportErrors(report__);
  }
  const auto pt_type = GetPolicyTableType();
  const auto pt_type_str = PolicyTableTypeToString(pt_type);
  std::string validation_info = "";

  if (PT_SNAPSHOT == pt_type) {
    if (schema_items.is_initialized()) {
      validation_info +=
          "; schema_items " + omitted_validation_info + pt_type_str;
    }
    if (!schema_version.is_initialized()) {
      validation_info +=
          "; schema_version " + required_validation_info + pt_type_str;
    }
    report__->set_validation_info(validation_info);
  }

  if (PT_UPDATE == pt_type || PT_PRELOADED == pt_type) {
    if ((schema_version.is_initialized() && !schema_items.is_initialized()) ||
        (!schema_version.is_initialized() && schema_items.is_initialized())) {
      validation_info += "; schema_version and schema_items " +
                         required_validation_info + pt_type_str;
    }
    report__->set_validation_info(validation_info);
  }

  if (!schema_version.is_valid()) {
    schema_version.ReportErrors(&report__->ReportSubobject("schema_version"));
  }
  if (!schema_items.is_valid()) {
    schema_items.ReportErrors(&report__->ReportSubobject("schema_items"));
  }
}

void VehicleData::SetPolicyTableType(PolicyTableType pt_type) {
  CompositeType::SetPolicyTableType(pt_type);
  schema_version.SetPolicyTableType(pt_type);
  schema_items.SetPolicyTableType(pt_type);
}

// PolicyTable methods
PolicyTable::PolicyTable() : CompositeType(kUninitialized) {}

PolicyTable::PolicyTable(
    const ApplicationPoliciesSection& app_policies_section,
    const FunctionalGroupings& functional_groupings,
    const ConsumerFriendlyMessages& consumer_friendly_messages,
    const ModuleConfig& module_config)
    : CompositeType(kUninitialized)
    , app_policies_section(app_policies_section)
    , functional_groupings(functional_groupings)
    , consumer_friendly_messages(consumer_friendly_messages)
    , module_config(module_config) {}

PolicyTable::~PolicyTable() {}

PolicyTable::PolicyTable(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , app_policies_section(impl::ValueMember(value__, "app_policies"))
    , functional_groupings(impl::ValueMember(value__, "functional_groupings"))
    , consumer_friendly_messages(
          impl::ValueMember(value__, "consumer_friendly_messages"))
    , module_config(impl::ValueMember(value__, "module_config"))
    , module_meta(impl::ValueMember(value__, "module_meta"))
    , usage_and_error_counts(
          impl::ValueMember(value__, "usage_and_error_counts"))
    , device_data(impl::ValueMember(value__, "device_data"))
    , vehicle_data(impl::ValueMember(value__, "vehicle_data")) {}

Json::Value PolicyTable::ToJsonValue() const {
  Json::Value result__(Json::objectValue);
  impl::WriteJsonField("app_policies", app_policies_section, &result__);
  impl::WriteJsonField("functional_groupings", functional_groupings, &result__);
  impl::WriteJsonField(
      "consumer_friendly_messages", consumer_friendly_messages, &result__);
  impl::WriteJsonField("module_config", module_config, &result__);
  impl::WriteJsonField("module_meta", module_meta, &result__);
  impl::WriteJsonField(
      "usage_and_error_counts", usage_and_error_counts, &result__);
  impl::WriteJsonField("device_data", device_data, &result__);
  impl::WriteJsonField("vehicle_data", vehicle_data, &result__);
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
  if (!vehicle_data.is_valid()) {
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
  if (vehicle_data.is_initialized()) {
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
    std::string validation_info =
        omitted_validation_info + PolicyTableTypeToString(GetPolicyTableType());

    if (device_data.is_initialized()) {
      report__->ReportSubobject("device_data")
          .set_validation_info(validation_info);
    }
  }
  if (!app_policies_section.is_valid()) {
    app_policies_section.ReportErrors(
        &report__->ReportSubobject("app_policies"));
  }
  if (!functional_groupings.is_valid()) {
    functional_groupings.ReportErrors(
        &report__->ReportSubobject("functional_groupings"));
  }
  if (!consumer_friendly_messages.is_valid()) {
    consumer_friendly_messages.ReportErrors(
        &report__->ReportSubobject("consumer_friendly_messages"));
  }
  if (!module_config.is_valid()) {
    module_config.ReportErrors(&report__->ReportSubobject("module_config"));
  }
  if (!module_meta.is_valid()) {
    module_meta.ReportErrors(&report__->ReportSubobject("module_meta"));
  }
  if (!usage_and_error_counts.is_valid()) {
    usage_and_error_counts.ReportErrors(
        &report__->ReportSubobject("usage_and_error_counts"));
  }
  if (!device_data.is_valid()) {
    device_data.ReportErrors(&report__->ReportSubobject("device_data"));
  }
  if (!vehicle_data.is_valid()) {
    vehicle_data.ReportErrors(&report__->ReportSubobject("vehicle_data"));
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
  vehicle_data.SetPolicyTableType(pt_type);
}

// Table methods
Table::Table() : CompositeType(kUninitialized) {}

Table::Table(const PolicyTable& policy_table)
    : CompositeType(kUninitialized), policy_table(policy_table) {}

Table::~Table() {}

Table::Table(const Json::Value* value__)
    : CompositeType(InitHelper(value__, &Json::Value::isObject))
    , policy_table(impl::ValueMember(value__, "policy_table")) {}

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
