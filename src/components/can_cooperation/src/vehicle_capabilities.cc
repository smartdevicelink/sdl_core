/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <fstream>
#include "can_cooperation/vehicle_capabilities.h"
#include "can_cooperation/can_module_constants.h"
#include "functional_module/settings.h"

namespace can_cooperation {

CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleCapabilities")

using message_params::kInteriorVehicleDataCapabilities;
using message_params::kModuleZone;

VehicleCapabilities::VehicleCapabilities()
    : kDefaultPath_(
          "./plugins/Remote_Control/InteriorVehicleDataCapabilities.json") {
  std::string path_to_file;
  functional_modules::Settings settings;
  if (!settings.ReadParameter(
          "Remote Control", "InteriorVDCapabilitiesFile", &path_to_file)) {
    path_to_file = kDefaultPath_;
  }
  LOG4CXX_DEBUG(logger_, "Reading capabilities from " << path_to_file);

  std::ifstream in(path_to_file);
  if (!in.is_open() && path_to_file != kDefaultPath_) {
    in.open(kDefaultPath_);
  }

  if (in.is_open()) {
    Json::Reader reader;
    if (!reader.parse(in, VehicleCapabilities::capabilities_, false)) {
      capabilities_ = Json::Value(Json::ValueType::nullValue);
    }
  }
}

Json::Value VehicleCapabilities::capabilities() const {
  if (capabilities_.type() == Json::ValueType::objectValue) {
    return capabilities_[kInteriorVehicleDataCapabilities];
  }
  return capabilities_;
}

Json::Value VehicleCapabilities::capabilities(const Json::Value& zone) const {
  if (capabilities_.type() == Json::ValueType::objectValue) {
    Json::Value result(Json::ValueType::arrayValue);
    for (Json::Value::iterator it =
             capabilities_[kInteriorVehicleDataCapabilities].begin();
         capabilities_[kInteriorVehicleDataCapabilities].end() != it;
         ++it) {
      if ((*it)[kModuleZone] == zone) {
        result.append(*it);
      }
    }
    return result;
  }
  return Json::Value();
}

}  // namespace can_cooperation
