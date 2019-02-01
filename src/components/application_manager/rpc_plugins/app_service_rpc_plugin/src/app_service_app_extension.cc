/*
 Copyright (c) 2018, Ford Motor Company
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

#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "app_service_rpc_plugin/app_service_rpc_plugin.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceRpcPlugin")

namespace app_service_rpc_plugin {

unsigned AppServiceAppExtension::AppServiceAppExtensionUID = 455;

AppServiceAppExtension::AppServiceAppExtension(
    AppServiceRpcPlugin& plugin, application_manager::Application& app)
    : app_mngr::AppExtension(AppServiceAppExtension::AppServiceAppExtensionUID)
    , plugin_(plugin)
    , app_(app) {
  LOG4CXX_AUTO_TRACE(logger_);
}

AppServiceAppExtension::~AppServiceAppExtension() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool AppServiceAppExtension::subscribeToAppService(
    const std::string app_service_type) {
  LOG4CXX_DEBUG(logger_, "Subscribe to app service: " << app_service_type);
  return subscribed_data_.insert(app_service_type).second;
}

bool AppServiceAppExtension::unsubscribeFromAppService(
    const std::string app_service_type) {
  LOG4CXX_DEBUG(logger_, app_service_type);
  auto it = subscribed_data_.find(app_service_type);
  if (it != subscribed_data_.end()) {
    subscribed_data_.erase(it);
    return true;
  }
  return false;
}

void AppServiceAppExtension::unsubscribeFromAppService() {
  LOG4CXX_AUTO_TRACE(logger_);
  subscribed_data_.clear();
}

bool AppServiceAppExtension::isSubscribedToAppService(
    const std::string app_service_type) const {
  LOG4CXX_DEBUG(logger_, app_service_type);
  return subscribed_data_.find(app_service_type) != subscribed_data_.end();
}

AppServiceSubscriptions AppServiceAppExtension::Subscriptions() {
  return subscribed_data_;
}

void AppServiceAppExtension::SaveResumptionData(
    smart_objects::SmartObject& resumption_data) {
  // todo
}

void AppServiceAppExtension::ProcessResumption(
    const smart_objects::SmartObject& resumption_data) {
  // todo
}

AppServiceAppExtension& AppServiceAppExtension::ExtractVIExtension(
    application_manager::Application& app) {
  auto ext_ptr =
      app.QueryInterface(AppServiceAppExtension::AppServiceAppExtensionUID);
  DCHECK(ext_ptr);
  DCHECK(dynamic_cast<AppServiceAppExtension*>(ext_ptr.get()));
  auto vi_app_extension =
      std::static_pointer_cast<AppServiceAppExtension>(ext_ptr);
  DCHECK(vi_app_extension);
  return *vi_app_extension;
}
}
