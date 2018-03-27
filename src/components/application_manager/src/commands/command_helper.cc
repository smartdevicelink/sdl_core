/*
 Copyright (c) 2017, Ford Motor Company
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

#include <algorithm>
#include <string>

#include "application_manager/commands/command_helper.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"
#include "utils/make_shared.h"

namespace application_manager {
namespace commands {
namespace command_helper {

CREATE_LOGGERPTR_GLOBAL(logger_, "Commands")

namespace {

struct DisallowedParamsInserter {
  DisallowedParamsInserter(smart_objects::SmartObject& response,
                           mobile_apis::VehicleDataResultCode::eType code)
      : response_(response), code_(code) {}

  bool operator()(const std::string& param) {
    using namespace application_manager;
    const VehicleData& vehicle_data = MessageHelper::vehicle_data();
    VehicleData::const_iterator it = vehicle_data.find(param);
    if (vehicle_data.end() != it) {
      smart_objects::SmartObjectSPtr disallowed_param =
          utils::MakeShared<smart_objects::SmartObject>(
              smart_objects::SmartType_Map);
      (*disallowed_param)[strings::data_type] = (*it).second;
      (*disallowed_param)[strings::result_code] = code_;
      response_[strings::msg_params][param.c_str()] = *disallowed_param;
      return true;
    }
    return false;
  }

 private:
  smart_objects::SmartObject& response_;
  mobile_apis::VehicleDataResultCode::eType code_;
};

void AddDissalowedParameterToInfoString(const std::string& param,
                                        std::string& info) {
  // prepare disallowed params enumeration for response info string
  if (info.empty()) {
    info = "\'" + param + "\'";
  } else {
    info = info + "," + " " + "\'" + param + "\'";
  }
}

}  // namespace

void AddDisallowedParameters(const CommandParametersPermissions& permissions,
                             smart_objects::SmartObject& response) {
  LOG4CXX_AUTO_TRACE(logger_);

  DisallowedParamsInserter disallowed_inserter(
      response, mobile_apis::VehicleDataResultCode::VDRC_USER_DISALLOWED);
  std::for_each(permissions.disallowed_params.begin(),
                permissions.disallowed_params.end(),
                disallowed_inserter);

  DisallowedParamsInserter undefined_inserter(
      response, mobile_apis::VehicleDataResultCode::VDRC_DISALLOWED);
  std::for_each(permissions.undefined_params.begin(),
                permissions.undefined_params.end(),
                undefined_inserter);
}

void AddDisallowedParametersToInfo(
    const CommandParametersPermissions& permissions,
    smart_objects::SmartObject& response) {
  LOG4CXX_AUTO_TRACE(logger_);

  std::string info;
  RPCParams::const_iterator it = permissions.disallowed_params.begin();
  for (; it != permissions.disallowed_params.end(); ++it) {
    AddDissalowedParameterToInfoString((*it), info);
  }

  it = permissions.undefined_params.begin();
  for (; it != permissions.undefined_params.end(); ++it) {
    AddDissalowedParameterToInfoString((*it), info);
  }

  if (!info.empty()) {
    const uint32_t params_count = permissions.disallowed_params.size() +
                                  permissions.undefined_params.size();
    info += params_count > 1 ? " parameters are " : " parameter is ";
    info += "disallowed by Policies";

    if (!response[strings::msg_params][strings::info].asString().empty()) {
      // If we already have info add info about disallowed params to it
      response[strings::msg_params][strings::info] =
          response[strings::msg_params][strings::info].asString() + " " + info;
    } else {
      response[strings::msg_params][strings::info] = info;
    }
  }
}

void RemoveDisallowedParameters(
    const CommandParametersPermissions& permissions,
    CommandParametersPermissions& removed_permissions,
    smart_objects::SmartObject& msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);

  // Remove from request all disallowed parameters
  RPCParams::const_iterator it_disallowed =
      permissions.disallowed_params.begin();
  RPCParams::const_iterator it_disallowed_end =
      permissions.disallowed_params.end();
  for (; it_disallowed != it_disallowed_end; ++it_disallowed) {
    if (msg_params.keyExists(*it_disallowed)) {
      const std::string key = *it_disallowed;
      msg_params.erase(key);
      removed_permissions.disallowed_params.insert(key);
      LOG4CXX_INFO(logger_,
                   "Following parameter is disallowed by user: " << key);
    }
  }

  // Remove from request all undefined yet parameters
  RPCParams::const_iterator it_undefined = permissions.undefined_params.begin();
  RPCParams::const_iterator it_undefined_end =
      permissions.undefined_params.end();
  for (; it_undefined != it_undefined_end; ++it_undefined) {
    if (msg_params.keyExists(*it_undefined)) {
      const std::string key = *it_undefined;
      msg_params.erase(key);
      removed_permissions.undefined_params.insert(key);
      LOG4CXX_INFO(logger_,
                   "Following parameter is disallowed by policy: " << key);
    }
  }

  // Remove from request all parameters missed in allowed
  const VehicleData& vehicle_data = MessageHelper::vehicle_data();

  VehicleData::const_iterator it_vehicle_data = vehicle_data.begin();
  VehicleData::const_iterator it_vehicle_data_end = vehicle_data.end();
  for (; it_vehicle_data != it_vehicle_data_end; ++it_vehicle_data) {
    const std::string key = it_vehicle_data->first;
    if (msg_params.keyExists(key) &&
        permissions.allowed_params.end() ==
            std::find(permissions.allowed_params.begin(),
                      permissions.allowed_params.end(),
                      key)) {
      msg_params.erase(key);
      removed_permissions.undefined_params.insert(key);
      LOG4CXX_INFO(logger_,
                   "Following parameter is not found among allowed parameters '"
                       << key << "' and will be treated as disallowed.");
    }
  }
}

bool HasDisallowedParams(const CommandParametersPermissions& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  return ((!permissions.disallowed_params.empty()) ||
          (!permissions.undefined_params.empty()));
}

}  // namespace command_helper
}  // namespace commands
}  // namespace application_manager
