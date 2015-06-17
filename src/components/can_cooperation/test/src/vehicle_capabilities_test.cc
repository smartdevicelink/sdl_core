#include "gtest/gtest.h"
#include "can_cooperation/vehicle_capabilities.h"

namespace can_cooperation {

TEST(VehicleCapabilities, Load) {
  VehicleCapabilities* caps = VehicleCapabilities::instance();
  ASSERT_EQ(Json::ValueType::arrayValue, caps->capabilities().type());
}

TEST(VehicleCapabilities, GetCapabilities) {
  VehicleCapabilities* caps = VehicleCapabilities::instance();
  ASSERT_EQ(Json::ValueType::arrayValue, caps->capabilities().type());
  ASSERT_TRUE(6 == caps->capabilities().size());
}

TEST(VehicleCapabilities, GetSpecificCapabilities) {
  VehicleCapabilities* caps = VehicleCapabilities::instance();
  Json::Value value;
  value["col"] = 0;
  value["row"] = 0;
  value["level"] = 0;
  value["colspan"] = 2;
  value["rowspan"] = 2;
  value["levelspan"] = 1;
  ASSERT_EQ(Json::ValueType::arrayValue, caps->capabilities(value).type());
  ASSERT_TRUE(2 == caps->capabilities(value).size());

  value["row"] = 1;
  ASSERT_EQ(Json::ValueType::arrayValue, caps->capabilities(value).type());
  ASSERT_TRUE(1 == caps->capabilities(value).size());
}

}  // namespace can_cooperation
