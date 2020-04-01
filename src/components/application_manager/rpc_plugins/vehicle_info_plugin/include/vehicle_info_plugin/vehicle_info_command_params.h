#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_COMMAND_PARAMS_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_COMMAND_PARAMS_H

namespace application_manager {
class ApplicationManager;
class HMICapabilities;
namespace rpc_service {
class RPCService;
}
class ApplicationManager;
}  // namespace application_manager

namespace policy {
class PolicyHandlerInterface;
}

namespace vehicle_info_plugin {
namespace app_mngr = application_manager;

class CustomVehicleDataManager;

struct VehicleInfoCommandParams {
  app_mngr::ApplicationManager& application_manager_;
  app_mngr::rpc_service::RPCService& rpc_service_;
  app_mngr::HMICapabilities& hmi_capabilities_;
  policy::PolicyHandlerInterface& policy_handler_;
  CustomVehicleDataManager& custom_vehicle_data_manager_;
};
}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_COMMAND_PARAMS_H
