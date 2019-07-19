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
    custom_vd_manager_.reset(
        new CustomVehicleDataManagerImpl(mock_custom_vehicle_data_provider_));
  }

  test::components::application_manager_test::MockApplicationManager
      app_manager_mock_;
  std::unique_ptr<vehicle_info_plugin::CustomVehicleDataManager>
      custom_vd_manager_;
  NiceMock<test::components::policy_test::MockCustomVehicleDataProvider>
      mock_custom_vehicle_data_provider_;
  MockMessageHelper& mock_message_helper_;
};

TEST_F(CustomVehicleDataManagerTest, SuccessOnEmptyItemsinMessage) {
  policy_table::VehicleDataItems items;
  InitValidationManager(items);
  smart_objects::SmartObject empty_msg_params(smart_objects::SmartType_Array);
  EXPECT_TRUE(custom_vd_manager_->ValidateVehicleDataItems(empty_msg_params));
}

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

TEST_F(CustomVehicleDataManagerTest, FailedSchemaForParamIsNotFound) {
  policy_table::VehicleDataItems schema_items;
  InitValidationManager(schema_items);
  smart_objects::SmartObject params;
  params[kName] = "VehicleDataItem";

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest, FailedItemValidation_WrongType) {
  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.mandatory = true;
  rpm.key = kKey;
  rpm.type = policy_table::VehicleDataItem::kInteger;
  ASSERT_TRUE(rpm.is_valid());

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject params;
  params[kName] = "VehicleDataItem";

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest, SuccesValidationOfVDItemEmptyArrayParam) {
  using smart_objects::SmartType_Array;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.mandatory = true;
  rpm.key = kKey;
  rpm.type = policy_table::VehicleDataItem::kInteger;
  *rpm.array = true;
  ASSERT_TRUE(rpm.is_valid());

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject params;
  params[kName] = smart_objects::SmartObject(SmartType_Array);

  EXPECT_TRUE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest, FailValidationArraySizeOutOfRange) {
  using smart_objects::SmartType_Array;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.mandatory = true;
  rpm.key = kKey;
  rpm.type = policy_table::VehicleDataItem::kInteger;
  rpm.maxsize = rpc::Optional<rpc::Integer<uint32_t, 0, UINT32_MAX> >(1);
  *rpm.array = true;
  ASSERT_TRUE(rpm.is_valid());

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject params;
  params[kName] = smart_objects::SmartObject(SmartType_Array);
  params[kName].asArray()->push_back(smart_objects::SmartObject(0));
  params[kName].asArray()->push_back(smart_objects::SmartObject(1));

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest, FailedValidationWhenSchemaInvalid) {
  std::string dummy_type = "dummy";
  int random_num = 0;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.mandatory = false;
  rpm.key = kKey;
  rpm.type = dummy_type;

  ASSERT_FALSE(rpm.is_valid());

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject params;
  params[kName] = random_num;
  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest,
       FailedValidationVDItemIntegerTypeWhenValueOutBoundaries) {
  auto minvalue = 1;
  auto maxvalue = 5;
  auto current_value = 10;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = policy_table::VehicleDataItem::kInteger;
  rpm.key = kKey;
  rpm.mandatory = false;
  *rpm.maxvalue = maxvalue;
  *rpm.minvalue = minvalue;
  ASSERT_TRUE(rpm.is_valid());

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject params;
  params[kName] = current_value;
  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest,
       SuccessIsVehicleDataNamePassedRPCSpecVehicleData) {
  namespace am = ::application_manager;
  am::VehicleData vehicle_data;
  std::string kSpeedName = "speed";
  vehicle_data.insert(am::VehicleData::value_type(
      kSpeedName, mobile_apis::VehicleDataType::VEHICLEDATA_SPEED));
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));
  policy_table::VehicleDataItems schema_items;
  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataName(kSpeedName));
}
TEST_F(CustomVehicleDataManagerTest,
       ValidateRPCSpecEnumVehicleDataItem_FAILED) {
  policy_table::VehicleDataItem custom_item;
  custom_item.mark_initialized();
  custom_item.name = "custom_name";
  custom_item.type = "SamplingRate";
  custom_item.mandatory = true;
  custom_item.params->mark_initialized();

  policy_table::VehicleDataItems vd_items;
  vd_items.push_back(custom_item);

  InitValidationManager(vd_items);
  smart_objects::SmartObject params;
  params["custom_name"] = "WRONG_VALUE";

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest,
       SuccesInitializedIsVehicleDataNamePassedCustomVehicleData) {
  namespace am = ::application_manager;
  am::VehicleData vehicle_data;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  policy_table::VehicleDataItem rpm;
  rpm.name = kName;
  rpm.key = kKey;
  rpm.mandatory = true;
  rpm.type = policy_table::VehicleDataItem::kString;
  rpm.mark_initialized();
  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataName(kName));
}

TEST_F(CustomVehicleDataManagerTest, ValidateVehicleDataItemyAPI_SUCCESS) {
  policy_table::VehicleDataItem custom_item;
  custom_item.mark_initialized();
  custom_item.name = "custom_name";
  custom_item.type = "SamplingRate";
  custom_item.mandatory = true;
  custom_item.params->mark_initialized();

  policy_table::VehicleDataItems vd_items;
  vd_items.push_back(custom_item);

  InitValidationManager(vd_items);
  smart_objects::SmartObject params;
  params["custom_name"] = "8KHZ";

  EXPECT_TRUE(custom_vd_manager_->ValidateVehicleDataItems(params));
}
TEST_F(CustomVehicleDataManagerTest,
       FailedInitializedIsVehicleDataNamePaseedInvalidVehicleData) {
  namespace am = ::application_manager;
  am::VehicleData vehicle_data;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  policy_table::VehicleDataItem rpm;
  rpm.name = kName;
  rpm.key = kKey;
  rpm.mandatory = false;
  rpm.type = policy_table::VehicleDataItem::kString;
  rpm.mark_initialized();
  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  InitValidationManager(schema_items);
  std::string dummy_name = "dummy";

  EXPECT_FALSE(custom_vd_manager_->IsVehicleDataName(dummy_name));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateVehicleDataItem_MandatoryParameterIsAbsent_Failed) {
  policy_table::VehicleDataItem custom_item;
  custom_item.mark_initialized();
  custom_item.name = "custom_name";
  custom_item.type = "SamplingRate";
  custom_item.mandatory = true;
  custom_item.params->mark_initialized();

  policy_table::VehicleDataItems vd_items;
  vd_items.push_back(custom_item);

  InitValidationManager(vd_items);
  smart_objects::SmartObject params;
  params["custom_name"] =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Null);

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest,
       FailedValidateVehicleDataItemsMandatoryFieldOfStructNotInitialized) {
  std::string kLboName = "lbo";
  std::string kLboKey = "VD_LBO";

  policy_table::VehicleDataItems schema_items;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = policy_table::VehicleDataItem::kStruct;
  rpm.key = kKey;
  rpm.mandatory = false;
  rpm.params->mark_initialized();

  policy_table::VehicleDataItem lbo;
  lbo.mark_initialized();
  lbo.name = kLboName;
  lbo.type = policy_table::VehicleDataItem::kString;
  lbo.key = kLboKey;
  lbo.mandatory = true;
  lbo.params->mark_initialized();

  (*rpm.params).push_back(lbo);

  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject item;
  item[kName] =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Map);

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(item));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateVehicleDataItem_TypeMismatchNotArray_Failed) {
  policy_table::VehicleDataItem custom_item;
  custom_item.mark_initialized();
  custom_item.name = "custom_name";
  custom_item.type = "SamplingRate";
  custom_item.mandatory = true;
  custom_item.params->mark_initialized();
  custom_item.array = rpc::Optional<rpc::Boolean>(true);

  policy_table::VehicleDataItems vd_items;
  vd_items.push_back(custom_item);

  InitValidationManager(vd_items);
  smart_objects::SmartObject params;
  params["custom_name"] = "SOME_VALUE";

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(params));
}

TEST_F(CustomVehicleDataManagerTest,
       FailedValidateVehicleDataItemMarkedInSchemaArrayButNotArray) {
  policy_table::VehicleDataItems schema_items;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = policy_table::VehicleDataItem::kString;
  rpm.key = kKey;
  rpm.mandatory = false;
  *rpm.array = true;
  rpm.params->mark_initialized();

  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject inner_item;
  double arbitary_double = 42.0;
  inner_item[kName] = arbitary_double;

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(inner_item));
}

TEST_F(CustomVehicleDataManagerTest,
       FailedValidateVehicleDataItemInnerElementDoesNotMatchSchema) {
  std::string kLboName = "lbo";
  std::string kLboKey = "VD_LBO";

  policy_table::VehicleDataItems schema_items;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = policy_table::VehicleDataItem::kStruct;
  rpm.key = kKey;
  rpm.mandatory = false;
  *rpm.array = true;
  rpm.params->mark_initialized();

  policy_table::VehicleDataItem lbo;
  lbo.mark_initialized();
  lbo.name = kLboName;
  lbo.type = policy_table::VehicleDataItem::kString;
  lbo.key = kLboKey;
  lbo.mandatory = true;
  *lbo.array = false;
  lbo.params->mark_initialized();

  (*rpm.params).push_back(lbo);

  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject item;
  item[kName] =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Array);
  smart_objects::SmartObject inner_item;
  int arbitary_integer = 42;
  inner_item[kName] = arbitary_integer;

  item[kName].asArray()->push_back(inner_item);

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(item));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateSchemaByNameAndReturnWithTheLatestSince) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItem rpm;
  rpm.name = kName;
  *rpm.since = "4.0";

  policy_table::VehicleDataItem rpm_later;
  rpm_later.mark_initialized();
  rpm_later.name = kName;
  *rpm_later.since = "4.2";

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  schema_items.push_back(rpm_later);

  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataName(kName));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateSchemaByNameAndReturnWithTheLatestSinceWhichIsEmpty) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItem rpm;
  rpm.name = kName;
  *rpm.since = "4.0";

  policy_table::VehicleDataItem rpm_later;
  rpm_later.name = kName;
  *rpm_later.since = "4.2";

  policy_table::VehicleDataItem rpm_last;
  rpm_last.mark_initialized();
  rpm_last.name = kName;

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  schema_items.push_back(rpm_last);
  schema_items.push_back(rpm_later);

  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataName(kName));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateSchemaByNameWithOnlyOneEmptySinceItem) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);

  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataName(kName));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateSchemaByKeyAndReturnWithTheLatestSince) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItem rpm;
  rpm.key = kKey;
  *rpm.since = "4.0";

  policy_table::VehicleDataItem rpm_later;
  rpm_later.mark_initialized();
  rpm_later.key = kKey;
  *rpm_later.since = "4.2";

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  schema_items.push_back(rpm_later);

  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataKey(kKey));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateSchemaByKeyAndReturnWithTheLatestSinceWhichIsEmpty) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItem rpm;
  rpm.key = kKey;
  *rpm.since = "4.0";

  policy_table::VehicleDataItem rpm_later;
  rpm_later.key = kKey;
  *rpm_later.since = "4.2";

  policy_table::VehicleDataItem rpm_last;
  rpm_last.mark_initialized();
  rpm_last.key = kKey;

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);
  schema_items.push_back(rpm_last);
  schema_items.push_back(rpm_later);

  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataKey(kKey));
}

TEST_F(CustomVehicleDataManagerTest,
       ValidateSchemaByKeyWithOnlyOneEmptySinceItem) {
  const application_manager::VehicleData kEmptyVehicleData;
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(kEmptyVehicleData));

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.key = kKey;

  policy_table::VehicleDataItems schema_items;
  schema_items.push_back(rpm);

  InitValidationManager(schema_items);

  EXPECT_TRUE(custom_vd_manager_->IsVehicleDataKey(kKey));

  rpm.type = policy_table::VehicleDataItem::kString;
  rpm.key = kKey;
  rpm.mandatory = false;

  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject item;
  int arbitary_integer = 42;
  item[kName] = arbitary_integer;

  EXPECT_FALSE(custom_vd_manager_->ValidateVehicleDataItems(item));
}

TEST_F(CustomVehicleDataManagerTest,
       SuccesValidatePODTypeItemPassedStringItem) {
  policy_table::VehicleDataItems schema_items;

  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = kName;
  rpm.type = policy_table::VehicleDataItem::kString;
  rpm.key = kKey;
  rpm.mandatory = false;

  schema_items.push_back(rpm);
  InitValidationManager(schema_items);

  smart_objects::SmartObject item;
  item[kName] = "Arbitary string";

  EXPECT_TRUE(custom_vd_manager_->ValidateVehicleDataItems(item));
}
}  // namespace vehicle_info_plugin_test
