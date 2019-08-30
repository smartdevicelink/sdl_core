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
namespace plugin_manager = application_manager::plugin_manager;

typedef boost::optional<const policy_table::VehicleDataItem> OptionalDataItem;
typedef std::set<std::string> VehicleInfoSubscriptions;

class CustomVehicleDataManagerImpl : public CustomVehicleDataManager {
 public:
  CustomVehicleDataManagerImpl(
      policy::VehicleDataItemProvider& vehicle_data_provider,
      application_manager::rpc_service::RPCService& rpc_service);

  virtual smart_objects::SmartObject CreateHMIMessageParams(
      const std::set<std::string>& item_names) OVERRIDE;

  std::string GetVehicleDataItemType(
      const std::string& vehicle_data_item_name) const OVERRIDE;

  virtual void CreateMobileMessageParams(
      smart_objects::SmartObject& msg_params) OVERRIDE;

  void OnPolicyEvent(plugin_manager::PolicyEvent policy_event) OVERRIDE;

 private:
  class RPCParams {
   public:
    RPCParams() {}
    ~RPCParams() {}

    void addBoolParam(
        const std::pair<std::string, smart_objects::SMember>& param) {
      rpc_params_bool_.insert(param);
    }
    void addVDRParam(
        const std::pair<std::string, smart_objects::SMember>& param) {
      rpc_params_vdr_.insert(param);
    }
    void addParam(const std::pair<std::string, smart_objects::SMember>& param) {
      rpc_params_.insert(param);
    }

    const std::map<std::string, smart_objects::SMember>& getBoolParams() {
      return rpc_params_bool_;
    }
    const std::map<std::string, smart_objects::SMember>& getVDRParams() {
      return rpc_params_vdr_;
    }
    const std::map<std::string, smart_objects::SMember>& getParams() {
      return rpc_params_;
    }

   private:
    std::map<std::string, smart_objects::SMember> rpc_params_bool_;
    std::map<std::string, smart_objects::SMember> rpc_params_vdr_;
    std::map<std::string, smart_objects::SMember> rpc_params_;
  };

  /**
   * @brief Updates vehicle data schemas according to policy update.
   */
  void UpdateVehicleDataItems();

  const OptionalDataItem FindSchemaByNameNonRecursive(
      const std::string& name) const;
  const OptionalDataItem FindSchemaByKeyNonRecursive(
      const std::string& key) const;
  const OptionalDataItem FindSchemaByNameRecursive(
      const std::string& name) const;
  const OptionalDataItem FindSchemaByKeyRecursive(
      const std::string& name) const;

  policy::VehicleDataItemProvider& vehicle_data_provider_;
  application_manager::rpc_service::RPCService& rpc_service_;
};

}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_VEHICLE_DATA_VALIDATION_MANAGER_IMPL_H
