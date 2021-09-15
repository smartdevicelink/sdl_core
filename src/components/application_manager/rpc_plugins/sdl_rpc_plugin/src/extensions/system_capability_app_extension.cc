#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "application_manager/smart_object_keys.h"

namespace sdl_rpc_plugin {
SDL_CREATE_LOG_VARIABLE("GetSystemCapabilitiesAppExtension")

namespace app_mngr_ = application_manager;
namespace strings = app_mngr::strings;

const app_mngr_::AppExtensionUID
    SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID = 200;

SystemCapabilityAppExtension::SystemCapabilityAppExtension(
    sdl_rpc_plugin::SDLRPCPlugin& plugin, app_mngr_::Application& app)
    : app_mngr_::AppExtension(
          SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID)
    , plugin_(plugin)
    , app_(app) {}

SystemCapabilityAppExtension::~SystemCapabilityAppExtension() {}

bool SystemCapabilityAppExtension::SubscribeTo(
    const SystemCapabilityType system_capability_type) {
  SDL_LOG_INFO("Subscribing to System Capability " << system_capability_type);
  return subscribed_data_.insert(system_capability_type).second;
}

bool SystemCapabilityAppExtension::UnsubscribeFrom(
    const SystemCapabilityType system_capability_type) {
  SDL_LOG_INFO("Unsubscribing from System Capability "
               << system_capability_type);
  auto it = subscribed_data_.find(system_capability_type);
  if (it != subscribed_data_.end()) {
    subscribed_data_.erase(it);
    return true;
  }
  return false;
}

void SystemCapabilityAppExtension::UnsubscribeFromAll() {
  SDL_LOG_INFO("Unsubscribing from ALL System Capabilities");
  subscribed_data_.clear();
}

bool SystemCapabilityAppExtension::IsSubscribedTo(
    const SystemCapabilityType system_capability_type) const {
  SDL_LOG_DEBUG(system_capability_type);
  return subscribed_data_.find(system_capability_type) !=
         subscribed_data_.end();
}

SystemCapabilitySubscriptions SystemCapabilityAppExtension::Subscriptions() {
  return subscribed_data_;
}

void SystemCapabilityAppExtension::SaveResumptionData(
    smart_objects::SmartObject& resumption_data) {
  SDL_LOG_AUTO_TRACE();

  resumption_data[strings::system_capability] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  int i = 0;
  for (const auto& subscription : subscribed_data_) {
    resumption_data[strings::system_capability][i] = subscription;
    ++i;
  }
}

void SystemCapabilityAppExtension::ProcessResumption(
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& subscriptions =
      saved_app[strings::application_subscriptions];

  if (saved_app.keyExists(strings::system_capability)) {
    const auto& system_capability_subscriptions =
        subscriptions[strings::system_capability];
    for (size_t i = 0; i < system_capability_subscriptions.length(); ++i) {
      SystemCapabilityType capability_type = static_cast<SystemCapabilityType>(
          (system_capability_subscriptions[i]).asInt());
      SubscribeTo(capability_type);
    }
  }
}

void SystemCapabilityAppExtension::RevertResumption(
    const smart_objects::SmartObject& subscriptions) {
  SDL_LOG_AUTO_TRACE();

  // No need to revert subscriptions here when an app gets RESUME_FAILED
  // System capability subscriptions should still be active
  UNUSED(subscriptions);
}

SystemCapabilityAppExtension& SystemCapabilityAppExtension::ExtractExtension(
    app_mngr_::Application& app) {
  SDL_LOG_AUTO_TRACE();
  auto ext_ptr = app.QueryInterface(
      SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID);
  DCHECK(ext_ptr);
  DCHECK(dynamic_cast<SystemCapabilityAppExtension*>(ext_ptr.get()));
  auto app_extension =
      std::static_pointer_cast<SystemCapabilityAppExtension>(ext_ptr);
  DCHECK(app_extension);
  return *app_extension;
}
}  // namespace sdl_rpc_plugin
