/*
 * Copyright (c) 2018, Ford Motor Company
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
 * POSSIBILITY OF SUCH DAMAGE. */

#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/policies/mock_custom_vehicle_data_provider.h"
#include "gtest/gtest.h"
#include "test/application_manager/mock_application_manager.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager_impl.h"

namespace vehicle_info_plugin_test {

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using application_manager::MockMessageHelper;
namespace {
std::string kName = "rpm";
std::string kKey = "VD_RPM";
}  // namespace
class CustomVehicleDataManagerTest : public ::testing::Test {
 public:
  CustomVehicleDataManagerTest()
      : mock_message_helper_(
            *application_manager::MockMessageHelper::message_helper_mock()) {}

  void SetUp() OVERRIDE {}

  void InitValidationManager(const policy_table::VehicleDataItems& items) {
    using vehicle_info_plugin::CustomVehicleDataManagerImpl;

    ON_CALL(mock_custom_vehicle_data_provider_, GetVehicleDataItems())
        .WillByDefault(Return(items));
    ON_CALL(mock_custom_vehicle_data_provider_, GetRemovedVehicleDataItems())
        .WillByDefault(Return(policy_table::VehicleDataItems()));
    custom_vd_manager_.reset(new CustomVehicleDataManagerImpl(
        mock_custom_vehicle_data_provider_, mock_rpc_service_));
  }

  test::components::application_manager_test::MockApplicationManager
      app_manager_mock_;
  std::unique_ptr<vehicle_info_plugin::CustomVehicleDataManager>
      custom_vd_manager_;
  NiceMock<test::components::policy_test::MockCustomVehicleDataProvider>
      mock_custom_vehicle_data_provider_;
  NiceMock<test::components::application_manager_test::MockRPCService>
      mock_rpc_service_;
  MockMessageHelper& mock_message_helper_;
};

TEST_F(CustomVehicleDataManagerTest, CreateHMIMessageParamsEmptyItems) {
  policy_table::VehicleDataItems schema_items;
  InitValidationManager(schema_items);
  const std::set<std::string> items;
  const auto msg_params = custom_vd_manager_->CreateHMIMessageParams(items);
  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_TRUE(msg_params.empty());
}

TEST_F(CustomVehicleDataManagerTest, CreateHMIMessageParamsOnePODItem) {
  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = "Integer";
  rpm.key = "OEM_REF_RPM";
  rpm.mandatory = false;
  *rpm.array = false;
  rpm.params->mark_initialized();

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  const std::set<std::string> items = {kName};
  const auto msg_params = custom_vd_manager_->CreateHMIMessageParams(items);
  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_EQ(msg_params.length(), 1u);
  EXPECT_TRUE(msg_params.keyExists("OEM_REF_RPM"));
  auto key_value = msg_params["OEM_REF_RPM"];
  EXPECT_TRUE(key_value.asBool());
}

TEST_F(CustomVehicleDataManagerTest, CreateHMIMessageParamsSeveralPODItems) {
  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = "Integer";
  rpm.key = "OEM_REF_RPM";
  rpm.mandatory = false;
  *rpm.array = false;
  rpm.params->mark_initialized();

  policy_table::VehicleDataItem speed;
  speed.mark_initialized();
  speed.name = "speed";
  speed.type = "float";
  speed.key = "OEM_REF_SPEED";
  speed.mandatory = false;
  *speed.array = false;
  speed.params->mark_initialized();

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  schema_items.push_back(speed);
  InitValidationManager(schema_items);

  const std::set<std::string> items = {kName, "speed"};

  const auto msg_params = custom_vd_manager_->CreateHMIMessageParams(items);
  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_EQ(msg_params.length(), 2u);

  EXPECT_TRUE(msg_params.keyExists("OEM_REF_RPM"));
  auto rpm_key_value = msg_params["OEM_REF_RPM"];
  EXPECT_TRUE(rpm_key_value.asBool());

  EXPECT_TRUE(msg_params.keyExists("OEM_REF_SPEED"));
  auto speed_key_value = msg_params["OEM_REF_SPEED"];
  EXPECT_TRUE(speed_key_value.asBool());
}

TEST_F(CustomVehicleDataManagerTest,
       CreateHMIMessageParamsSchemaWithSeveralItemsRequestWithOneItem) {
  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = "Integer";
  rpm.key = "OEM_REF_RPM";
  rpm.mandatory = false;
  *rpm.array = false;
  rpm.params->mark_initialized();

  policy_table::VehicleDataItem speed;
  speed.mark_initialized();
  speed.name = "speed";
  speed.type = "float";
  speed.key = "OEM_REF_SPEED";
  speed.mandatory = false;
  *speed.array = false;
  speed.params->mark_initialized();

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  schema_items.push_back(speed);
  InitValidationManager(schema_items);

  const std::set<std::string> items = {kName};

  const auto msg_params = custom_vd_manager_->CreateHMIMessageParams(items);
  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_EQ(msg_params.length(), 1u);

  EXPECT_TRUE(msg_params.keyExists("OEM_REF_RPM"));
  auto rpm_key_value = msg_params["OEM_REF_RPM"];
  EXPECT_TRUE(rpm_key_value.asBool());
}

TEST_F(CustomVehicleDataManagerTest, CreateHMIMessageParamsStructType) {
  policy_table::VehicleDataItem alss;
  alss.mark_initialized();
  alss.name = "ambientLightSensorStatus";
  alss.type = "AmbientLightStatus";
  alss.key = "OEM_REF_AMB_LIGHT";
  alss.mandatory = false;
  alss.params->mark_initialized();

  policy_table::VehicleDataItem lss;
  lss.mark_initialized();
  lss.name = "LightSensorStatus";
  lss.type = "Struct";
  lss.key = "OEM_REF_SEN_LIGHT";
  lss.mandatory = false;
  lss.params->mark_initialized();
  lss.params->push_back(alss);

  policy_table::VehicleDataItem hbo;
  hbo.mark_initialized();
  hbo.name = "highBeamsOn";
  hbo.type = "Boolean";
  hbo.key = "OEM_REF_HIGH_BEAM";
  hbo.mandatory = true;
  hbo.params->mark_initialized();

  policy_table::VehicleDataItem lbo;
  lbo.mark_initialized();
  lbo.name = "lowBeamsOn";
  lbo.type = "Boolean";
  lbo.key = "OEM_REF_LOW_BEAM";
  lbo.mandatory = false;
  lbo.params->mark_initialized();

  policy_table::VehicleDataItem hls;
  hls.mark_initialized();
  hls.name = "headLampStatus";
  hls.type = "Struct";
  hls.key = "OEM_REF_HLSTATUS";
  hls.mandatory = false;
  hls.params->mark_initialized();
  hls.params->push_back(lss);
  hls.params->push_back(lbo);
  hls.params->push_back(hbo);

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(hls);
  InitValidationManager(schema_items);

  const std::set<std::string> items = {"headLampStatus"};

  const auto msg_params = custom_vd_manager_->CreateHMIMessageParams(items);
  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_EQ(msg_params.length(), 1u);

  EXPECT_TRUE(msg_params.keyExists("OEM_REF_HLSTATUS"));
  const auto head_lamp_status = msg_params["OEM_REF_HLSTATUS"];
  EXPECT_EQ(head_lamp_status.getType(), smart_objects::SmartType_Map);

  EXPECT_EQ(head_lamp_status.length(), 3u);

  EXPECT_TRUE(head_lamp_status.keyExists("OEM_REF_HIGH_BEAM"));
  EXPECT_TRUE(head_lamp_status.keyExists("OEM_REF_LOW_BEAM"));
  EXPECT_TRUE(head_lamp_status.keyExists("OEM_REF_SEN_LIGHT"));

  EXPECT_TRUE(head_lamp_status["OEM_REF_HIGH_BEAM"].asBool());
  EXPECT_TRUE(head_lamp_status["OEM_REF_LOW_BEAM"].asBool());
  auto light_sensor_status = head_lamp_status["OEM_REF_SEN_LIGHT"];

  EXPECT_EQ(light_sensor_status.length(), 1u);
  EXPECT_TRUE(light_sensor_status.keyExists("OEM_REF_AMB_LIGHT"));
  EXPECT_TRUE(light_sensor_status["OEM_REF_AMB_LIGHT"].asBool());
}

TEST_F(CustomVehicleDataManagerTest, CreateHMIMessageParamsNoSchemaForItem) {
  policy_table::VehicleDataItems schema_items;
  InitValidationManager(schema_items);

  const std::set<std::string> items = {kName};

  const auto msg_params = custom_vd_manager_->CreateHMIMessageParams(items);
  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_TRUE(msg_params.empty());
}

TEST_F(CustomVehicleDataManagerTest, CreateMobileMessageParamsStruct) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItems schema_items;

  policy_table::VehicleDataItem custom_vd_item11_struct;
  custom_vd_item11_struct.mark_initialized();
  custom_vd_item11_struct.name = "custom_vd_item11_struct";
  custom_vd_item11_struct.type = "Struct";
  custom_vd_item11_struct.key = "OEM_REF_STRUCT";
  custom_vd_item11_struct.mandatory = false;
  custom_vd_item11_struct.params->mark_initialized();

  policy_table::VehicleDataItem struct_element_1_int;
  struct_element_1_int.mark_initialized();
  struct_element_1_int.name = "struct_element_1_int";
  struct_element_1_int.type = "Integer";
  struct_element_1_int.key = "OEM_REF_STRUCT_1_INT";
  struct_element_1_int.mandatory = false;
  struct_element_1_int.params->mark_initialized();
  custom_vd_item11_struct.params->push_back(struct_element_1_int);
  schema_items.push_back(custom_vd_item11_struct);
  InitValidationManager(schema_items);

  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params["OEM_REF_STRUCT"] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  auto& oem_ref_struct = msg_params["OEM_REF_STRUCT"];
  oem_ref_struct["OEM_REF_STRUCT_1_INT"] = 100;
  custom_vd_manager_->CreateMobileMessageParams(msg_params);

  EXPECT_EQ(msg_params.getType(), smart_objects::SmartType_Map);
  EXPECT_TRUE(msg_params.keyExists("custom_vd_item11_struct"));

  auto& mobile_response_vd_item_11 = msg_params["custom_vd_item11_struct"];
  EXPECT_EQ(mobile_response_vd_item_11.getType(), smart_objects::SmartType_Map);
  EXPECT_TRUE(mobile_response_vd_item_11.keyExists("struct_element_1_int"));

  auto& mobile_response_element1 =
      mobile_response_vd_item_11["struct_element_1_int"];
  EXPECT_EQ(mobile_response_element1.getType(),
            smart_objects::SmartType_Integer);
  EXPECT_EQ(mobile_response_element1.asInt(), 100);
}
}  // namespace vehicle_info_plugin_test
