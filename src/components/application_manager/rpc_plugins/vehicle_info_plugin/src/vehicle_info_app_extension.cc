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

#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "vehicle_info_plugin/vehicle_info_plugin.h"

SDL_CREATE_LOG_VARIABLE("VehicleInfoPlugin")

namespace vehicle_info_plugin {
namespace strings = application_manager::strings;

unsigned VehicleInfoAppExtension::VehicleInfoAppExtensionUID = 146;

VehicleInfoAppExtension::VehicleInfoAppExtension(
    VehicleInfoPlugin& plugin, application_manager::Application& app)
    : app_mngr::AppExtension(
          VehicleInfoAppExtension::VehicleInfoAppExtensionUID)
    , subscribed_data_lock_(std::make_shared<sync_primitives::Lock>())
    , pending_subscriptions_lock_(std::make_shared<sync_primitives::Lock>())
    , plugin_(plugin)
    , app_(app) {
  SDL_LOG_AUTO_TRACE();
}

VehicleInfoAppExtension::~VehicleInfoAppExtension() {
  SDL_LOG_AUTO_TRACE();
}

bool VehicleInfoAppExtension::subscribeToVehicleInfo(
    const std::string& vehicle_data) {
  SDL_LOG_DEBUG(vehicle_data);
  sync_primitives::AutoLock lock(*subscribed_data_lock_);
  return subscribed_data_.insert(vehicle_data).second;
}

bool VehicleInfoAppExtension::unsubscribeFromVehicleInfo(
    const std::string& vehicle_data) {
  SDL_LOG_DEBUG(vehicle_data);
  sync_primitives::AutoLock lock(*subscribed_data_lock_);
  auto it = subscribed_data_.find(vehicle_data);
  if (it != subscribed_data_.end()) {
    subscribed_data_.erase(it);
    return true;
  }
  return false;
}

void VehicleInfoAppExtension::unsubscribeFromVehicleInfo() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(*subscribed_data_lock_);
  subscribed_data_.clear();
}

bool VehicleInfoAppExtension::isSubscribedToVehicleInfo(
    const std::string& vehicle_data) const {
  SDL_LOG_DEBUG(vehicle_data);
  sync_primitives::AutoLock lock(*subscribed_data_lock_);
  return subscribed_data_.find(vehicle_data) != subscribed_data_.end();
}

bool VehicleInfoAppExtension::isPendingSubscriptionToVehicleInfo(
    const std::string& vehicle_data) const {
  SDL_LOG_DEBUG(vehicle_data);
  sync_primitives::AutoLock lock(*pending_subscriptions_lock_);
  return pending_subscriptions_.find(vehicle_data) !=
         pending_subscriptions_.end();
}

const DataAccessor<VehicleInfoSubscriptions>
VehicleInfoAppExtension::Subscriptions() {
  DataAccessor<VehicleInfoSubscriptions> data_accessor(subscribed_data_,
                                                       subscribed_data_lock_);
  return data_accessor;
}

bool VehicleInfoAppExtension::AddPendingSubscription(
    const std::string& vehicle_data) {
  sync_primitives::AutoLock lock(*pending_subscriptions_lock_);
  return pending_subscriptions_.insert(vehicle_data).second;
}

bool VehicleInfoAppExtension::RemovePendingSubscription(
    const std::string& vehicle_data) {
  SDL_LOG_DEBUG(vehicle_data);
  sync_primitives::AutoLock lock(*pending_subscriptions_lock_);
  auto it = pending_subscriptions_.find(vehicle_data);
  if (it != pending_subscriptions_.end()) {
    pending_subscriptions_.erase(it);
    return true;
  }
  return false;
}

void VehicleInfoAppExtension::RemovePendingSubscriptions() {
  sync_primitives::AutoLock lock(*pending_subscriptions_lock_);
  pending_subscriptions_.clear();
}

const DataAccessor<VehicleInfoSubscriptions>
VehicleInfoAppExtension::PendingSubscriptions() {
  return DataAccessor<VehicleInfoSubscriptions>(pending_subscriptions_,
                                                pending_subscriptions_lock_);
}

void VehicleInfoAppExtension::SaveResumptionData(
    smart_objects::SmartObject& resumption_data) {
  resumption_data[strings::application_vehicle_info] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  int i = 0;
  sync_primitives::AutoLock lock(*subscribed_data_lock_);
  for (const auto& subscription : subscribed_data_) {
    resumption_data[strings::application_vehicle_info][i] = subscription;
    ++i;
  }
}

void VehicleInfoAppExtension::ProcessResumption(
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  if (!saved_app.keyExists(strings::application_subscriptions)) {
    SDL_LOG_DEBUG("application_subscriptions section is not exists");
    return;
  }

  const smart_objects::SmartObject& resumption_data =
      saved_app[strings::application_subscriptions];

  if (!resumption_data.keyExists(strings::application_vehicle_info)) {
    SDL_LOG_DEBUG("application_vehicle_info section is not exists");
    return;
  }

  const auto subscriptions_ivi =
      resumption_data[strings::application_vehicle_info].asArray();
  for (const auto& ivi : *subscriptions_ivi) {
    AddPendingSubscription(ivi.asString());
  }
  if (!subscriptions_ivi->empty()) {
    plugin_.ProcessResumptionSubscription(app_, *this);
  }
}

void VehicleInfoAppExtension::RevertResumption(
    const smart_objects::SmartObject& resumption_data) {
  SDL_LOG_AUTO_TRACE();

  unsubscribeFromVehicleInfo();

  if (!resumption_data.keyExists(
          application_manager::hmi_interface::vehicle_info)) {
    SDL_LOG_DEBUG("No resumption vahicle data subscription to revert");
    return;
  }
  const auto& resumption_vd_data =
      resumption_data[application_manager::hmi_interface::vehicle_info];

  std::set<std::string> ivi_subscriptions_to_revert;
  const auto ivi_subscriptions_keys = resumption_vd_data.enumerate();
  for (const auto& key : ivi_subscriptions_keys) {
    // Only boolean keys in subscriptions list are true vehicle data
    // subscriptions
    if (smart_objects::SmartType::SmartType_Boolean ==
        resumption_vd_data.getElement(key).getType()) {
      ivi_subscriptions_to_revert.insert(key);
    }
  }

  plugin_.RevertResumption(app_, ivi_subscriptions_to_revert);
}

VehicleInfoAppExtension& VehicleInfoAppExtension::ExtractVIExtension(
    application_manager::Application& app) {
  auto ext_ptr =
      app.QueryInterface(VehicleInfoAppExtension::VehicleInfoAppExtensionUID);
  DCHECK(ext_ptr);
  DCHECK(dynamic_cast<VehicleInfoAppExtension*>(ext_ptr.get()));
  auto vi_app_extension =
      std::static_pointer_cast<VehicleInfoAppExtension>(ext_ptr);
  DCHECK(vi_app_extension);
  return *vi_app_extension;
}
}  // namespace vehicle_info_plugin
