#include "sdl_rpc_plugin/extensions/get_system_capability_app_extension.h"

namespace sdl_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "GetSystemCapabilitiesAppExtension")

namespace app_mngr_ = application_manager;
unsigned SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID = 200;

SystemCapabilityAppExtension::SystemCapabilityAppExtension(
    sdl_rpc_plugin::SDLRPCPlugin& plugin, app_mngr_::Application& app)
    : app_mngr_::AppExtension(
          SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID)
    , plugin_(plugin)
    , app_(app) {
  LOG4CXX_AUTO_TRACE(logger_);
}

SystemCapabilityAppExtension::~SystemCapabilityAppExtension() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool SystemCapabilityAppExtension::subscribeTo(
    const SystemCapabilityType system_capability_type) {
  LOG4CXX_DEBUG(logger_, system_capability_type);
  return subscribed_data_.insert(system_capability_type).second;
}

bool SystemCapabilityAppExtension::unsubscribeFrom(
    const SystemCapabilityType system_capability_type) {
  LOG4CXX_DEBUG(logger_, system_capability_type);
  auto it = subscribed_data_.find(system_capability_type);
  if (it != subscribed_data_.end()) {
    subscribed_data_.erase(it);
    return true;
  }
  return false;
}

void SystemCapabilityAppExtension::unsubscribeFrom() {
  LOG4CXX_AUTO_TRACE(logger_);
  subscribed_data_.clear();
}

bool SystemCapabilityAppExtension::isSubscribedTo(
    const SystemCapabilityType system_capability_type) const {
  LOG4CXX_DEBUG(logger_, system_capability_type);
  return subscribed_data_.find(system_capability_type) !=
         subscribed_data_.end();
}

SystemCapabilitySubscriptions SystemCapabilityAppExtension::Subscriptions() {
  return subscribed_data_;
}

void SystemCapabilityAppExtension::SaveResumptionData(
    ns_smart_device_link::ns_smart_objects::SmartObject& resumption_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* application_system_capability = "systemCapability";

  resumption_data[application_system_capability] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  int i = 0;
  for (const auto& subscription : subscribed_data_) {
    resumption_data[application_system_capability][i] = subscription;
    i++;
  }
}

void SystemCapabilityAppExtension::ProcessResumption(
    const smart_objects::SmartObject& resumption_data) {
  LOG4CXX_AUTO_TRACE(logger_);

  const char* application_system_capability = "systemCapability";
  if (resumption_data.keyExists(application_system_capability)) {
    const smart_objects::SmartObject& subscriptions_ivi =
        resumption_data[application_system_capability];
    for (size_t i = 0; i < subscriptions_ivi.length(); ++i) {
      SystemCapabilityType ivi =
          static_cast<SystemCapabilityType>((resumption_data[i]).asInt());
      subscribeTo(ivi);
    }
    // plugin_.ProcessResumptionSubscription(app_, *this);
  }
}

SystemCapabilityAppExtension& SystemCapabilityAppExtension::ExtractExtension(
    app_mngr_::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto ext_ptr = app.QueryInterface(
      SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID);
  DCHECK(ext_ptr);
  DCHECK(dynamic_cast<SystemCapabilityAppExtension*>(ext_ptr.get()));
  auto vi_app_extension =
      std::static_pointer_cast<SystemCapabilityAppExtension>(ext_ptr);
  DCHECK(vi_app_extension);
  return *vi_app_extension;
}
}