/**
 * @file permissions_calculator.cc
 * @brief Permissions calculator source file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <map>
#include "policies/permissions_calculator.h"
#include "policies/policy_table_schema.h"
#include "formatters/CSmartFactory.hpp"
#include "smart_objects/enum_schema_item.h"

namespace NsSmartDeviceLink {
namespace policies {
  
namespace so_ns = NsSmartDeviceLink::NsSmartObjects;
namespace jsonhandler_ns = NsSmartDeviceLink::NsJSONHandler;

log4cxx::LoggerPtr PermissionsCalculator::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("Policies"));

//----------------------------------------------------------------------------

PermissionResult::eType PermissionsCalculator::CalcPermissions(
    const so_ns::SmartObject& pt_object,
    const uint32_t app_id,
    const so_ns::SmartObject& rpc,
    const mobile_apis::HMILevel::eType hmi_status) {

  char app_id_string[16];
  sprintf(app_id_string, "%d", app_id);  
  std::vector<std::string> rpc_groups;
  const so_ns::SmartObject& app_policies_object =
      pt_object.getElement(PolicyTableSchema::kStrPolicyTable)
               .getElement(PolicyTableSchema::kStrAppPolicies);

  // look for <app_id> section within app_policies
  if ((true == app_policies_object.keyExists(std::string(app_id_string)))) {
    if (so_ns::SmartType::SmartType_Array ==
        app_policies_object.getElement(app_id_string)
                          .getElement(PolicyTableSchema::kStrGroups).getType()) {
      convertSmartArray2VectorStrings(
          app_policies_object.getElement(app_id_string)
                              .getElement(PolicyTableSchema::kStrGroups),
          rpc_groups);
    } else {
       LOG4CXX_ERROR(logger_,
        "Section app_policies|<app_id>|groups either not exists or not array");
    }  // look for default section within app_policies
  } else if (so_ns::SmartType::SmartType_Array ==
      app_policies_object.getElement(PolicyTableSchema::kStrDefault)
                          .getElement(PolicyTableSchema::kStrGroups).getType()) {
    convertSmartArray2VectorStrings(
        app_policies_object.getElement(PolicyTableSchema::kStrDefault)
                            .getElement(PolicyTableSchema::kStrGroups),
        rpc_groups);
  } else {
    LOG4CXX_ERROR(logger_,
      "Section app_policies|default|groups either not exists or not array");
  }

  if (rpc_groups.size() > 0) {
    return FindRPCsByGroups(pt_object, rpc_groups, rpc, hmi_status);
  }

  return PermissionResult::PERMISSION_NOK_DISALLOWED;
}

//----------------------------------------------------------------------------

Priority::eType PermissionsCalculator::GetPriority(
    const so_ns::SmartObject& pt_object,
    const uint32_t app_id) {
  // TODO(anyone): to implement
  return Priority::PRIORITY_NONE;
}

//----------------------------------------------------------------------------

PermissionResult::eType
  PermissionsCalculator::FindRPCsByGroups(
    const so_ns::SmartObject& pt_object,
    const std::vector<std::string> rpc_groups,
    const so_ns::SmartObject& rpc,
    const mobile_apis::HMILevel::eType hmi_status) {

  NsSmartDeviceLink::NsSmartObjects::SmartObject rpc_object(rpc);
  // unapply schema to convert FunctionID from enum (number) to string
  rpc_object.getSchema().unapplySchema(rpc_object);

  const so_ns::SmartObject &function_id =
      rpc_object.getElement(jsonhandler_ns::strings::S_PARAMS)
                .getElement(jsonhandler_ns::strings::S_FUNCTION_ID);
  if (function_id.asString().length() == 0) {
    return PermissionResult::PERMISSION_NOK_DISALLOWED;
  }

  const so_ns::SmartObject& functional_groupings_object = 
     pt_object.getElement(PolicyTableSchema::kStrPolicyTable)
              .getElement(PolicyTableSchema::kStrFunctionalGroupings);
  if (so_ns::SmartType::SmartType_Map !=
      functional_groupings_object.getType()) {
    return PermissionResult::PERMISSION_NOK_DISALLOWED;
  }

  for (std::vector<std::string>::const_iterator it = rpc_groups.begin() ;
      it != rpc_groups.end(); ++it) {
    if (functional_groupings_object.keyExists(*it)) {

      const so_ns::SmartObject& fgroup = functional_groupings_object.getElement(*it);
      if (fgroup.getElement(PolicyTableSchema::kStrRpcs)
                .keyExists(function_id.asString())) {

        std::string hmi_status_string;
        convertHMILevel2String(hmi_status, hmi_status_string);

        so_ns::SmartObject hmi_levels =
            fgroup.getElement(PolicyTableSchema::kStrRpcs)
                  .getElement(function_id.asString())
                  .getElement(PolicyTableSchema::kStrHmiLevels);
        for(uint16_t n = 0; n < hmi_levels.length(); n++) {
          if (hmi_levels[n].getType() == so_ns::SmartType::SmartType_String) {
            if (hmi_status_string.compare(hmi_levels[n].asString()) == 0) {
              return PermissionResult::PERMISSION_OK_ALLOWED;
            }
          }
        }
      } else continue;
    }
  }

  return PermissionResult::PERMISSION_NOK_DISALLOWED;
}

//----------------------------------------------------------------------------

void PermissionsCalculator::convertHMILevel2String(
    const mobile_apis::HMILevel::eType hmi_status,
    std::string& hmi_level_string) {

  const std::map<mobile_apis::HMILevel::eType, std::string> elementsStringRepresentation =
      so_ns::TEnumSchemaItem<mobile_apis::HMILevel::eType>::getEnumElementsStringRepresentation();

  typename std::map<mobile_apis::HMILevel::eType, std::string>::const_iterator i = elementsStringRepresentation.find(hmi_status);

  if (i != elementsStringRepresentation.end())
  {
      hmi_level_string = i->second;
  }
}

//----------------------------------------------------------------------------

void PermissionsCalculator::convertSmartArray2VectorStrings(
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& object,
    std::vector<std::string>& v_strings) {

  if (object.getType() == so_ns::SmartType::SmartType_Array) {
    for(uint32_t i = 0; i < object.length(); i++) {
      if (object.getType() == so_ns::SmartType::SmartType_String) {
        v_strings.push_back(object.asString());
      }
    }
  }
}

}  // namespace policies
}  // namespace NsSmartDeviceLink
