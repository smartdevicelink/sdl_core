/**
 * Copyright (c) 2019, Ford Motor Company
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

#include "vehicle_info_plugin/custom_vehicle_data_manager_impl.h"
#include <strings.h>
#include <limits>
#include "policy/policy_table/types.h"
#include "vehicle_info_plugin/vehicle_data_item_schema.h"

#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/custom_vehicle_data_provider.h"
#include "interfaces/MOBILE_API_schema.h"
#include "policy/policy_table/policy_enum_schema_factory.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/helpers.h"
#include "utils/optional.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPlugin")

namespace vehicle_info_plugin {

template <typename T, typename U>
U GetOptional(const rpc::Optional<T>& optional, U def_val) {
  return optional.is_initialized() ? static_cast<U>(*optional) : def_val;
}

enum SearchMethod { RECURSIVE = 0, NON_RECURSIVE };

CustomVehicleDataManagerImpl::CustomVehicleDataManagerImpl(
    policy::VehicleDataItemProvider& vehicle_data_provider,
    application_manager::rpc_service::RPCService& rpc_service)
    : vehicle_data_provider_(vehicle_data_provider)
    , rpc_service_(rpc_service) {}

std::string CustomVehicleDataManagerImpl::GetVehicleDataItemType(
    const std::string& vehicle_data_item_name) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto& schema = FindSchemaByNameNonRecursive(vehicle_data_item_name);

  return schema.is_initialized() ? std::string(schema->type)
                                 : vehicle_data_item_name;
}

void CustomVehicleDataManagerImpl::CreateMobileMessageParams(
    smart_objects::SmartObject& msg_params) {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);

  typedef std::function<smart_objects::SmartObject(
      const smart_objects::SmartObject& input_params,
      SearchMethod search_method)>
      MobileMsgConstructor;

  MobileMsgConstructor fill_mobile_msg =
      [this, &fill_mobile_msg](
          const smart_objects::SmartObject& input_params,
          SearchMethod search_method) -> smart_objects::SmartObject {
    smart_objects::SmartObject out_params;

    const auto& items = input_params.enumerate();
    for (const auto& key : items) {
      auto schema = search_method == SearchMethod::RECURSIVE
                        ? FindSchemaByKeyRecursive(key)
                        : FindSchemaByKeyNonRecursive(key);
      if (!schema.is_initialized()) {
        LOG4CXX_DEBUG(logger_, "Schema for: " << key << " cannot be found");
        continue;
      }

      const auto& item_name = schema->name;
      if (policy_table::VehicleDataItem::kStruct == std::string(schema->type)) {
        const auto param =
            fill_mobile_msg(input_params[key], SearchMethod::RECURSIVE);
        if (!param.empty()) {
          out_params[item_name] = param;
          continue;
        }
      }

      out_params[item_name] = input_params[key];
    }

    return out_params;
  };

  const auto& rpc_spec_vehicle_data = MessageHelper::vehicle_data();
  const auto& item_names = msg_params.enumerate();
  smart_objects::SmartObject params_to_convert;

  for (const auto& item_key : item_names) {
    const auto& found_it = rpc_spec_vehicle_data.find(item_key);
    if (found_it == rpc_spec_vehicle_data.end()) {
      params_to_convert[item_key] = msg_params[item_key];
      msg_params.erase(item_key);
    }
  }

  const auto& mobile_msg_params =
      fill_mobile_msg(params_to_convert, SearchMethod::NON_RECURSIVE);
  for (const auto& name : mobile_msg_params.enumerate()) {
    msg_params[name] = mobile_msg_params[name];
  }
}

smart_objects::SmartObject CustomVehicleDataManagerImpl::CreateHMIMessageParams(
    const std::set<std::string>& item_names) {
  typedef std::function<smart_objects::SmartObject(
      const policy_table::VehicleDataItem&)>
      ParamsConstructor;

  auto fill_param = [](ParamsConstructor& constructor,
                       const policy_table::VehicleDataItem& param,
                       smart_objects::SmartObject* out_params) {
    DCHECK_OR_RETURN_VOID(out_params)
    const auto param_key = std::string(param.key);
    const auto param_type = std::string(param.type);
    if (policy_table::VehicleDataItem::kStruct == param_type) {
      (*out_params)[param_key] = constructor(param);
    } else {
      (*out_params)[param_key] = true;
    }
  };

  ParamsConstructor fill_hmi_params =
      [&fill_hmi_params,
       fill_param](const policy_table::VehicleDataItem& item) {
        smart_objects::SmartObject out_params;
        DCHECK(policy_table::VehicleDataItem::kStruct ==
               std::string(item.type));
        for (auto& param : *(item.params)) {
          fill_param(fill_hmi_params, param, &out_params);
        }
        return out_params;
      };

  smart_objects::SmartObject out_params(smart_objects::SmartType_Map);
  for (const auto& name : item_names) {
    auto schema = FindSchemaByNameNonRecursive(name);
    if (schema.is_initialized()) {
      fill_param(fill_hmi_params, *schema, &out_params);
    }
  }

  return out_params;
}

template <typename Comparer>
const OptionalDataItem FindSchema(
    const std::vector<policy_table::VehicleDataItem>& oem_items,
    SearchMethod search_method,
    Comparer comparer) {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<policy_table::VehicleDataItem> items;
  for (const auto& item : oem_items) {
    if (comparer(item)) {
      items.push_back(item);
    }
    if (search_method == SearchMethod::RECURSIVE &&
        policy_table::VehicleDataItem::kStruct == std::string(item.type)) {
      auto found_in_params =
          FindSchema(*(item.params), search_method, comparer);
      if (found_in_params.is_initialized()) {
        items.push_back(*found_in_params);
      }
    }
  }
  std::sort(items.begin(),
            items.end(),
            [](const policy_table::VehicleDataItem& left,
               const policy_table::VehicleDataItem& right) {
              if (!right.since.is_initialized()) {
                return false;
              }
              if (!left.since.is_initialized()) {
                return true;
              }
              const std::string l = *left.since;
              const std::string r = *right.since;
              return std::stof(l.c_str()) > std::stof(r.c_str());
            });

  if (!items.empty()) {
    return OptionalDataItem(*items.begin());
  }

  return OptionalDataItem();
}

void CustomVehicleDataManagerImpl::UpdateVehicleDataItems() {
  using namespace ns_smart_device_link::ns_smart_objects;
  using namespace policy_table;

  enum class SMemberType {
    SMEMBER_MOBILE,
    SMEMBER_HMI,
    SMEMBER_BOOL_MOBILE,
    SMEMBER_BOOL_HMI,
    SMEMBER_VDR_MOBILE,
    SMEMBER_VDR_HMI
  };

  auto get_custom_vdi = [this]()
      -> std::vector<rpc::policy_table_interface_base::VehicleDataItem> {
    using namespace rpc::policy_table_interface_base;
    std::vector<VehicleDataItem> custom_vehicle_data_items;
    auto vehicle_data_items = vehicle_data_provider_.GetVehicleDataItems();
    for (const auto& item : vehicle_data_items) {
      policy_table::Parameter parameter_enum;
      if (!policy_table::EnumFromJsonString(item.name, &parameter_enum)) {
        custom_vehicle_data_items.push_back(item);
      }
    }
    return custom_vehicle_data_items;
  };

  auto vehicle_data_items = get_custom_vdi();

  auto get_ischema_item = [](VehicleDataItem item,
                             SMemberType type,
                             std::vector<SMember> history = {}) -> SMember {
    switch (type) {
      case SMemberType::SMEMBER_BOOL_MOBILE: {
        auto member_schema =
            CBoolSchemaItem::create(TSchemaItemParameter<bool>());
        return SMember(
            member_schema,
            false,  // root level items should not be mandatory
            item.since.is_initialized() ? std::string(*item.since) : "",
            item.until.is_initialized() ? std::string(*item.until) : "",
            bool(*item.deprecated),
            bool(*item.removed),
            history);
      }
      case SMemberType::SMEMBER_VDR_MOBILE: {
        // valid since struct_schema_items is not used in
        // InitStructSchemaItem_VehicleDataResult
        mobile_apis::MOBILE_API::TStructsSchemaItems mobile_struct_schema_items;
        auto member_schema =
            mobile_apis::MOBILE_API::InitStructSchemaItem_VehicleDataResult(
                mobile_struct_schema_items);
        return SMember(
            member_schema,
            false,  // root level items should not be mandatory
            item.since.is_initialized() ? std::string(*item.since) : "",
            item.until.is_initialized() ? std::string(*item.until) : "",
            bool(*item.deprecated),
            bool(*item.removed),
            history);
      }
      case SMemberType::SMEMBER_MOBILE: {
        TSchemaItemParameter<VehicleDataItem> tschema_item(item);
        auto member_schema = VehicleDataItemSchema::create(
            tschema_item, VehicleDataItemSchema::SchemaType::MOBILE);
        return SMember(
            member_schema,
            false,  // root level items should not be mandatory
            item.since.is_initialized() ? std::string(*item.since) : "",
            item.until.is_initialized() ? std::string(*item.until) : "",
            bool(*item.deprecated),
            bool(*item.removed),
            history);
      }
      case SMemberType::SMEMBER_BOOL_HMI: {
        auto member_schema =
            CBoolSchemaItem::create(TSchemaItemParameter<bool>());
        return SMember(member_schema,
                       false  // root level items should not be mandatory
        );
      }
      case SMemberType::SMEMBER_VDR_HMI: {
        // valid since struct_schema_items is not used in
        // InitStructSchemaItem_Common_VehicleDataResult
        hmi_apis::HMI_API::TStructsSchemaItems hmi_struct_schema_items;
        auto member_schema =
            hmi_apis::HMI_API::InitStructSchemaItem_Common_VehicleDataResult(
                hmi_struct_schema_items);
        return SMember(
            member_schema, false  // root level items should not be mandatory
        );
      }
      case SMemberType::SMEMBER_HMI: {
        TSchemaItemParameter<VehicleDataItem> tschema_item(item);
        auto member_schema = VehicleDataItemSchema::create(
            tschema_item, VehicleDataItemSchema::SchemaType::HMI);
        return SMember(
            member_schema, false  // root level items should not be mandatory
        );
      }
      default: {
        auto member_schema = CAlwaysFalseSchemaItem::create();
        return SMember(member_schema, false);
      }
    }
  };

  auto get_vehicle_data_history =
      [&vehicle_data_items](std::string name) -> std::vector<VehicleDataItem> {
    std::vector<VehicleDataItem> result;
    std::copy_if(vehicle_data_items.begin(),
                 vehicle_data_items.end(),
                 std::back_inserter(result),
                 [&name](VehicleDataItem item) { return item.name == name; });

    std::sort(result.begin(),
              result.end(),
              [](const policy_table::VehicleDataItem& left,
                 const policy_table::VehicleDataItem& right) {
                if (!right.since.is_initialized()) {
                  return false;
                }
                if (!left.since.is_initialized()) {
                  return true;
                }
                const std::string l = *left.since;
                const std::string r = *right.since;
                return std::stof(l.c_str()) > std::stof(r.c_str());
              });

    return result;
  };

  auto get_member_with_history = [&get_ischema_item](
                                     std::vector<VehicleDataItem> items,
                                     SMemberType type) -> SMember {
    std::vector<SMember> history = {};
    std::vector<SMemberType> types{SMemberType::SMEMBER_MOBILE,
                                   SMemberType::SMEMBER_VDR_MOBILE,
                                   SMemberType::SMEMBER_BOOL_MOBILE};
    if (helpers::in_range(types, type) && items.size() > 1) {
      auto history_iterator = items.begin() + 1;
      for (; history_iterator < items.end(); ++history_iterator) {
        const auto& item = (*history_iterator);
        history.push_back(get_ischema_item(item, type));
      }
    }

    auto latest_item = (*items.begin());
    return get_ischema_item(latest_item, type, history);
  };

  CustomVehicleDataManagerImpl::RPCParams mobile_params;
  CustomVehicleDataManagerImpl::RPCParams hmi_params;

  std::set<std::string> vehicle_data_names;
  for (const auto& item : vehicle_data_items) {
    vehicle_data_names.insert(item.name);
  }

  for (const auto& name : vehicle_data_names) {
    auto vehicle_data_history = get_vehicle_data_history(name);
    const auto& vdi = vehicle_data_history.begin();

    SMember member_bool_hmi = get_member_with_history(
        vehicle_data_history, SMemberType::SMEMBER_BOOL_HMI);
    SMember member_bool_mobile = get_member_with_history(
        vehicle_data_history, SMemberType::SMEMBER_BOOL_MOBILE);

    SMember member_vdr_hmi = get_member_with_history(
        vehicle_data_history, SMemberType::SMEMBER_VDR_HMI);
    SMember member_vdr_mobile = get_member_with_history(
        vehicle_data_history, SMemberType::SMEMBER_VDR_MOBILE);
    SMember member_hmi =
        get_member_with_history(vehicle_data_history, SMemberType::SMEMBER_HMI);
    SMember member_mobile = get_member_with_history(
        vehicle_data_history, SMemberType::SMEMBER_MOBILE);

    mobile_params.addBoolParam(
        std::pair<std::string, SMember>(vdi->name, member_bool_mobile));
    mobile_params.addVDRParam(
        std::pair<std::string, SMember>(vdi->name, member_vdr_mobile));
    mobile_params.addParam(
        std::pair<std::string, SMember>(vdi->name, member_mobile));

    hmi_params.addBoolParam(
        std::pair<std::string, SMember>(vdi->key, member_bool_hmi));
    hmi_params.addVDRParam(
        std::pair<std::string, SMember>(vdi->key, member_vdr_hmi));
    hmi_params.addParam(std::pair<std::string, SMember>(vdi->key, member_hmi));
  }

  std::vector<mobile_apis::FunctionID::eType> mobile_subscribe_functions{
      mobile_apis::FunctionID::SubscribeVehicleDataID,
      mobile_apis::FunctionID::UnsubscribeVehicleDataID};

  for (const auto& function_id : mobile_subscribe_functions) {
    rpc_service_.UpdateMobileRPCParams(function_id,
                                       mobile_apis::messageType::request,
                                       mobile_params.getBoolParams());
    rpc_service_.UpdateMobileRPCParams(function_id,
                                       mobile_apis::messageType::response,
                                       mobile_params.getVDRParams());
  }

  std::vector<hmi_apis::FunctionID::eType> hmi_subscribe_functions{
      hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData};

  for (const auto& function_id : hmi_subscribe_functions) {
    rpc_service_.UpdateHMIRPCParams(function_id,
                                    hmi_apis::messageType::request,
                                    hmi_params.getBoolParams());
    rpc_service_.UpdateHMIRPCParams(function_id,
                                    hmi_apis::messageType::response,
                                    hmi_params.getVDRParams());
  }

  rpc_service_.UpdateMobileRPCParams(mobile_apis::FunctionID::GetVehicleDataID,
                                     mobile_apis::messageType::request,
                                     mobile_params.getBoolParams());

  rpc_service_.UpdateMobileRPCParams(mobile_apis::FunctionID::GetVehicleDataID,
                                     mobile_apis::messageType::response,
                                     mobile_params.getParams());

  rpc_service_.UpdateMobileRPCParams(mobile_apis::FunctionID::OnVehicleDataID,
                                     mobile_apis::messageType::notification,
                                     mobile_params.getParams());

  rpc_service_.UpdateHMIRPCParams(
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData,
      hmi_apis::messageType::request,
      hmi_params.getBoolParams());

  rpc_service_.UpdateHMIRPCParams(
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData,
      hmi_apis::messageType::response,
      hmi_params.getParams());

  rpc_service_.UpdateHMIRPCParams(
      hmi_apis::FunctionID::VehicleInfo_OnVehicleData,
      hmi_apis::messageType::notification,
      hmi_params.getParams());
}

void CustomVehicleDataManagerImpl::OnPolicyEvent(
    plugin_manager::PolicyEvent policy_event) {
  using namespace plugin_manager;
  LOG4CXX_AUTO_TRACE(logger_);

  switch (policy_event) {
    case kApplicationPolicyUpdated:
      UpdateVehicleDataItems();
      break;
    case kApplicationsDisabled:
    default:
      return;
  }
}

const OptionalDataItem
CustomVehicleDataManagerImpl::FindSchemaByNameNonRecursive(
    const std::string& name) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto& oem_items = vehicle_data_provider_.GetVehicleDataItems();
  auto compare_by_name = [&name](const policy_table::VehicleDataItem& item) {
    return (name == std::string(item.name));
  };

  return FindSchema(oem_items, SearchMethod::NON_RECURSIVE, compare_by_name);
}

const OptionalDataItem CustomVehicleDataManagerImpl::FindSchemaByNameRecursive(
    const std::string& name) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto& oem_items = vehicle_data_provider_.GetVehicleDataItems();
  auto compare_by_name = [&name](const policy_table::VehicleDataItem& item) {
    return (name == std::string(item.name));
  };

  return FindSchema(oem_items, SearchMethod::RECURSIVE, compare_by_name);
}

const OptionalDataItem
CustomVehicleDataManagerImpl::FindSchemaByKeyNonRecursive(
    const std::string& key) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto& oem_items = vehicle_data_provider_.GetVehicleDataItems();
  auto compare_by_key = [&key](const policy_table::VehicleDataItem& item) {
    return (key == std::string(item.key));
  };

  return FindSchema(oem_items, SearchMethod::NON_RECURSIVE, compare_by_key);
}

const OptionalDataItem CustomVehicleDataManagerImpl::FindSchemaByKeyRecursive(
    const std::string& key) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto& oem_items = vehicle_data_provider_.GetVehicleDataItems();
  auto compare_by_key = [&key](const policy_table::VehicleDataItem& item) {
    return (key == std::string(item.key));
  };

  return FindSchema(oem_items, SearchMethod::RECURSIVE, compare_by_key);
}

}  // namespace vehicle_info_plugin
