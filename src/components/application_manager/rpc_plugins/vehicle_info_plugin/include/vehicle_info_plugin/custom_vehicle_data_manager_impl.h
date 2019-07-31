#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_VEHICLE_DATA_VALIDATION_MANAGER_IMPL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_VEHICLE_DATA_VALIDATION_MANAGER_IMPL_H

#include <boost/optional.hpp>

#include "smart_objects/smart_object.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager.h"

namespace rpc {
namespace policy_table_interface_base {
class VehicleDataItem;
}
}  // namespace rpc

namespace policy {
class VehicleDataItemProvider;
}

namespace vehicle_info_plugin {
namespace policy_table = rpc::policy_table_interface_base;

typedef boost::optional<const policy_table::VehicleDataItem> OptionalDataItem;
typedef std::set<std::string> VehicleInfoSubscriptions;

class CustomVehicleDataManagerImpl : public CustomVehicleDataManager {
 public:
  CustomVehicleDataManagerImpl(
      policy::VehicleDataItemProvider& vehicle_data_provider);
  bool ValidateVehicleDataItems(
      const smart_objects::SmartObject& msg_params) OVERRIDE;

  virtual smart_objects::SmartObject CreateHMIMessageParams(
      const std::set<std::string>& item_names) OVERRIDE;

  std::string GetVehicleDataItemType(
      const std::string& vehicle_data_item_name) const OVERRIDE;

  virtual void CreateMobileMessageParams(
      smart_objects::SmartObject& msg_params) OVERRIDE;

  bool IsVehicleDataName(const std::string& name) OVERRIDE;

  bool IsVehicleDataKey(const std::string& key) OVERRIDE;

 private:
  bool ValidateVehicleDataItem(
      const smart_objects::SmartObject& item,
      const policy_table::VehicleDataItem& item_schema);

  /**
   * @brief ValidateRPCSpecEnumVehicleDataItem validate custom item in case it
   * `type` of the item is enum from RPCspec
   * @param item item to validate
   * @param item_schema schema for item validation
   * @return true if item is valid according schema, false if not
   */
  bool ValidateRPCSpecEnumVehicleDataItem(
      const smart_objects::SmartObject& item,
      const policy_table::VehicleDataItem& item_schema);

  /**
   * @brief ValidatePODTypeItem validate custom item in case it
   * `type` of the item Plain Old Data (POD)
   * @param item item to validate
   * @param item_schema schema for item validation
   * @return true if item is valid according schema, false if not
   */
  bool ValidatePODTypeItem(const smart_objects::SmartObject& item,
                           const policy_table::VehicleDataItem& item_schema);

  bool ValidateStructTypeItem(
      const ns_smart_device_link::ns_smart_objects::SmartObject& item,
      const policy_table::VehicleDataItem& item_schema);

  const OptionalDataItem FindSchemaByNameNonRecursive(
      const std::string& name) const;
  const OptionalDataItem FindSchemaByKeyNonRecursive(
      const std::string& key) const;
  const OptionalDataItem FindSchemaByNameRecursive(
      const std::string& name) const;
  const OptionalDataItem FindSchemaByKeyRecursive(
      const std::string& name) const;

  policy::VehicleDataItemProvider& vehicle_data_provider_;
};

}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_VEHICLE_DATA_VALIDATION_MANAGER_IMPL_H
