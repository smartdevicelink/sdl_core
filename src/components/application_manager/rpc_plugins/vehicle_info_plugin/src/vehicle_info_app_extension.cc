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

CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPlugin")

namespace vehicle_info_plugin {

VehicleInfoAppExtension::VehicleInfoAppExtension(application_manager::AppExtensionUID uid)
  : app_mngr::AppExtension(uid) {
  LOG4CXX_AUTO_TRACE(logger_);
}

VehicleInfoAppExtension::~VehicleInfoAppExtension() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void VehicleInfoAppExtension::subscribeToVehicleInfo(
      const std::string& moduleType) {
  LOG4CXX_DEBUG(logger_, moduleType);
  subscribed_modules_.insert(moduleType);
}

void VehicleInfoAppExtension::unsubscribeFromVehicleInfo(
      const std::string& moduleType) {
  LOG4CXX_DEBUG(logger_, moduleType);
  auto it = subscribed_modules_.find(moduleType);
  if (it != subscribed_modules_.end()) {
    subscribed_modules_.erase(it);
  }
}

void VehicleInfoAppExtension::unsubscribeFromVehicleInfo() {
  LOG4CXX_AUTO_TRACE(logger_);
  subscribed_modules_.clear();
}

bool VehicleInfoAppExtension::isSubscribedToVehicleInfo(
      const std::string& moduleType) const {
  LOG4CXX_DEBUG(logger_, moduleType);
  return subscribed_modules_.find(moduleType) !=
      subscribed_modules_.end();
}

}
