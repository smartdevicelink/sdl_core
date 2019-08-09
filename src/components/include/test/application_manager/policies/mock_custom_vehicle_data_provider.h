#ifndef MOCK_CUSTOM_VEHICLE_DATA_MANAGER_H
#define MOCK_CUSTOM_VEHICLE_DATA_MANAGER_H

#include "application_manager/policies/custom_vehicle_data_provider.h"
#include "gmock/gmock.h"

namespace test {
namespace components {
namespace policy_test {

class MockCustomVehicleDataProvider : public policy::VehicleDataItemProvider {
 public:
  MOCK_CONST_METHOD0(
      GetVehicleDataItems,
      const std::vector<rpc::policy_table_interface_base::VehicleDataItem>());
};

}  // namespace policy_test
}  // namespace components
}  // namespace test
#endif  // MOCK_CUSTOM_VEHICLE_DATA_MANAGER_H
