/*
 Copyright (c) 2013, Ford Motor Company
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

#include "can_cooperation/can_app_extension.h"

namespace can_cooperation {
CANAppExtension::CANAppExtension(application_manager::AppExtensionUID uid)
    : AppExtension(uid)
    , is_control_given_(false)
    , is_on_driver_device_(false) {}

bool CANAppExtension::IsControlGiven() const {
  return is_control_given_;
}

void CANAppExtension::GiveControl(bool is_control_given) {
  is_control_given_ = is_control_given;
}

void CANAppExtension::SubscribeToInteriorVehicleData(
    const Json::Value& module_type) {
  subscribed_interior_vehicle_data_.insert(module_type);
}

void CANAppExtension::UnsubscribeFromInteriorVehicleData(
    const Json::Value& module_type) {
  subscribed_interior_vehicle_data_.erase(module_type);
}

bool CANAppExtension::IsSubscibedToInteriorVehicleData(
    const Json::Value& module_type) {
  std::set<Json::Value>::iterator it =
      subscribed_interior_vehicle_data_.find(module_type);

  return (it != subscribed_interior_vehicle_data_.end());
}

CANAppExtension::~CANAppExtension() {}
}  //  namespace can_cooperation
