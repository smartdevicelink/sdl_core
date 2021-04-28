/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "app_service_rpc_plugin/app_service_rpc_plugin.h"
#include "application_manager/include/application_manager/smart_object_keys.h"

SDL_CREATE_LOG_VARIABLE("AppServiceRpcPlugin")

namespace app_service_rpc_plugin {

const AppExtensionUID AppServiceAppExtensionUID = 455;

AppServiceAppExtension::AppServiceAppExtension(
    AppServiceRpcPlugin& plugin, application_manager::Application& app)
    : app_mngr::AppExtension(AppServiceAppExtensionUID)
    , plugin_(plugin)
    , app_(app) {
  SDL_LOG_AUTO_TRACE();
}

AppServiceAppExtension::~AppServiceAppExtension() {
  SDL_LOG_AUTO_TRACE();
}

bool AppServiceAppExtension::SubscribeToAppService(
    const std::string app_service_type) {
  SDL_LOG_DEBUG("Subscribe to app service: " << app_service_type);
  return subscribed_data_.insert(app_service_type).second;
}

bool AppServiceAppExtension::UnsubscribeFromAppService(
    const std::string app_service_type) {
  SDL_LOG_DEBUG(app_service_type);
  auto it = subscribed_data_.find(app_service_type);
  if (it != subscribed_data_.end()) {
    subscribed_data_.erase(it);
    return true;
  }
  return false;
}

void AppServiceAppExtension::UnsubscribeFromAppService() {
  SDL_LOG_AUTO_TRACE();
  subscribed_data_.clear();
}

bool AppServiceAppExtension::IsSubscribedToAppService(
    const std::string app_service_type) const {
  SDL_LOG_DEBUG("isSubscribedToAppService for type: " << app_service_type);
  return subscribed_data_.find(app_service_type) != subscribed_data_.end();
}

AppServiceSubscriptions AppServiceAppExtension::Subscriptions() {
  return subscribed_data_;
}

void AppServiceAppExtension::SaveResumptionData(
    smart_objects::SmartObject& resumption_data) {
  resumption_data[app_mngr::hmi_interface::app_service] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  int i = 0;
  for (const auto& subscription : subscribed_data_) {
    resumption_data[app_mngr::hmi_interface::app_service][i] = subscription;
    ++i;
  }
}

void AppServiceAppExtension::ProcessResumption(
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  const auto& subscriptions =
      saved_app[application_manager::strings::application_subscriptions];

  if (subscriptions.keyExists(app_mngr::hmi_interface::app_service)) {
    const smart_objects::SmartObject& subscriptions_app_services =
        subscriptions[app_mngr::hmi_interface::app_service];
    for (size_t i = 0; i < subscriptions_app_services.length(); ++i) {
      std::string service_type = subscriptions_app_services[i].asString();
      SubscribeToAppService(service_type);
    }
  }
}

void AppServiceAppExtension::RevertResumption(
    const smart_objects::SmartObject& resumption_data) {
  SDL_LOG_AUTO_TRACE();

  UNUSED(resumption_data);
  // ToDo: implementation is blocked by
  // https://github.com/smartdevicelink/sdl_core/issues/3470
}

AppServiceAppExtension& AppServiceAppExtension::ExtractASExtension(
    application_manager::Application& app) {
  auto ext_ptr = app.QueryInterface(AppServiceAppExtensionUID);
  DCHECK(ext_ptr);
  DCHECK(dynamic_cast<AppServiceAppExtension*>(ext_ptr.get()));
  auto vi_app_extension =
      std::static_pointer_cast<AppServiceAppExtension>(ext_ptr);
  DCHECK(vi_app_extension);
  return *vi_app_extension;
}
}  // namespace app_service_rpc_plugin
