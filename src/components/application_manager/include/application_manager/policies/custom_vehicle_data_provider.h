#ifndef CUSTOM_VEHICLE_DATA_PROVIDER_H
#define CUSTOM_VEHICLE_DATA_PROVIDER_H

#include <vector>

namespace rpc {
namespace policy_table_interface_base {
struct VehicleDataItem;
}
}  // namespace rpc

namespace policy {

class VehicleDataItemProvider {
 public:
  /**
   * @brief Gets vehicle data items
   * @return Structure with vehicle data items
   */
  virtual const std::vector<rpc::policy_table_interface_base::VehicleDataItem>
  GetVehicleDataItems() const = 0;
};
}  // namespace policy

#endif  // CUSTOM_VEHICLE_DATA_PROVIDER_H
