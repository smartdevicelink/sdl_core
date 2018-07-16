/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <algorithm>
#include <sstream>
#include <memory>
#include "json/json.h"
#include "gtest/gtest.h"
#include "utils/macro.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "utils/date_time.h"
#include "resumption/last_state_impl.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/mock_app_launch_settings.h"
#include "application_manager/app_launch/app_launch_data_json.h"

namespace test {
namespace components {
namespace test_app_launch {

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;

namespace am = application_manager;
using namespace Json;
using namespace file_system;

using namespace app_launch;

const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppStorageFile = "./app_info.dat";
const std::string kAppInfoStorage = "app_info_storage";

class AppLaunchDataJsonTest : public ::testing::Test {
 private:
  virtual void SetUp() {
    ::file_system::DeleteFile(kAppStorageFile);
    test_last_state_ = std::unique_ptr<resumption::LastState>(
        new resumption::LastStateImpl(kAppStorageFolder, kAppInfoStorage));
    ASSERT_TRUE(::file_system::CreateFile(kAppStorageFile));

    NiceMock<app_launch_test::MockAppLaunchSettings> mock_app_launch_settings_;
    ON_CALL(mock_app_launch_settings_, max_number_of_ios_device())
        .WillByDefault(Return(15u));

    res_json_.reset(
        new AppLaunchDataJson(mock_app_launch_settings_, *test_last_state_));
  }

 public:
  void TearDown() OVERRIDE {
    res_json_.get()->Clear();
  }

  static void SetUpTestCase() {}

  static void TearDownTestCase() {
    ::file_system::DeleteFile(kAppStorageFile);
    ::file_system::RemoveDirectory(kAppStorageFolder);
  }

  AppLaunchDataJson* res_json() {
    return res_json_.get();
  }

  void AddApplicationDataWithIncreaseTable(const ApplicationData& data);
  void AddApplicationDataWithoutIncreaseTable(const ApplicationData& data);
  TimevalStruct GetApplicationData_EXPECT_TRUE(const ApplicationData& in_data,
                                               ApplicationData& out_data);
  void GetApplicationData_EXPECT_FALSE(const ApplicationData& in_data);
  std::string AddCounter(const std::string& inp, int32_t val);

  std::unique_ptr<resumption::LastState> test_last_state_;
  std::unique_ptr<AppLaunchDataJson> res_json_;
  void SetTimestamp(const ApplicationData& in_data, TimevalStruct& timestamp);
};

void AppLaunchDataJsonTest::AddApplicationDataWithIncreaseTable(
    const ApplicationData& data) {
  uint32_t sizeBeforeAdding = res_json()->GetCurentNumberOfAppData();

  EXPECT_TRUE(res_json()->AddApplicationData(data));

  uint32_t sizeAfterAdding = res_json()->GetCurentNumberOfAppData();

  EXPECT_EQ(sizeBeforeAdding + 1u, sizeAfterAdding);
}

void AppLaunchDataJsonTest::AddApplicationDataWithoutIncreaseTable(
    const ApplicationData& data) {
  uint32_t sizeBeforeAdding = res_json()->GetCurentNumberOfAppData();

  EXPECT_TRUE(res_json()->AddApplicationData(data));

  uint32_t sizeAfterAdding = res_json()->GetCurentNumberOfAppData();

  EXPECT_EQ(sizeBeforeAdding, sizeAfterAdding);
}

TimevalStruct AppLaunchDataJsonTest::GetApplicationData_EXPECT_TRUE(
    const ApplicationData& in_data, ApplicationData& out_data) {
  uint32_t sizeBeforeGetting = res_json()->GetCurentNumberOfAppData();

  int32_t index = NotFound;
  Json::Value& json_data_list =
      res_json()->GetApplicationListAndIndex(in_data, index);
  EXPECT_FALSE(index == NotFound);

  uint32_t sizeAfterGetting = res_json()->GetCurentNumberOfAppData();

  EXPECT_EQ(sizeBeforeGetting, sizeAfterGetting);
  out_data.device_mac_ =
      json_data_list[index][am::strings::device_id].asString();
  out_data.mobile_app_id_ =
      json_data_list[index][am::strings::app_id].asString();
  out_data.bundle_id_ =
      json_data_list[index][am::strings::bundle_id].asString();
  // time stamp
  TimevalStruct tmVal = {0};
  tmVal.tv_sec =
      json_data_list[index][am::strings::app_launch_last_session].asUInt64();
  return tmVal;
}

void AppLaunchDataJsonTest::GetApplicationData_EXPECT_FALSE(
    const ApplicationData& in_data) {
  uint32_t sizeBeforeGetting = res_json()->GetCurentNumberOfAppData();

  int32_t index = NotFound;
  res_json()->GetApplicationListAndIndex(in_data, index);
  EXPECT_TRUE(index == NotFound);

  uint32_t sizeAfterGetting = res_json()->GetCurentNumberOfAppData();

  EXPECT_EQ(sizeBeforeGetting, sizeAfterGetting);
}

void AppLaunchDataJsonTest::SetTimestamp(const ApplicationData& in_data,
                                         TimevalStruct& timestamp) {
  uint32_t sizeBeforeGetting = res_json()->GetCurentNumberOfAppData();

  int32_t index = NotFound;
  Json::Value& json_data_list =
      res_json()->GetApplicationListAndIndex(in_data, index);
  EXPECT_FALSE(index == NotFound);

  uint32_t sizeAfterGetting = res_json()->GetCurentNumberOfAppData();

  EXPECT_FALSE(index == NotFound);

  EXPECT_EQ(sizeBeforeGetting, sizeAfterGetting);
  // time stamp
  json_data_list[index][am::strings::app_launch_last_session] =
      static_cast<Json::Value::UInt64>(timestamp.tv_sec);
}

std::string AppLaunchDataJsonTest::AddCounter(const std::string& inp,
                                              int32_t val) {
  std::stringstream ss;
  ss << inp << val;
  return ss.str();
}

TEST_F(AppLaunchDataJsonTest, SaveAndGetData) {
  ApplicationData data("mobile_app_id", "bundle_id", "device_mac");
  AddApplicationDataWithIncreaseTable(data);
  ApplicationData recoveredData("", "", "");
  GetApplicationData_EXPECT_TRUE(data, recoveredData);
  EXPECT_TRUE(data == recoveredData);
}

TEST_F(AppLaunchDataJsonTest, NotAddEmptyAppData) {
  ApplicationData data1("", "bundle_id", "device_mac");
  ApplicationData data2("mobile_app_id", "", "device_mac");
  ApplicationData data3("mobile_app_id", "bundle_id", "");
  ApplicationData data4("", "", "");
  EXPECT_FALSE(res_json()->AddApplicationData(data1));
  EXPECT_FALSE(res_json()->AddApplicationData(data2));
  EXPECT_FALSE(res_json()->AddApplicationData(data3));
  EXPECT_FALSE(res_json()->AddApplicationData(data4));
  EXPECT_EQ(0u, res_json()->GetCurentNumberOfAppData());
}

TEST_F(AppLaunchDataJsonTest, SaveOneAndGetAnotherData) {
  ApplicationData data("mobile_app_id", "bundle_id", "device_mac");
  ApplicationData recoverData = data;
  AddApplicationDataWithIncreaseTable(data);
  recoverData.device_mac_ += "test";
  GetApplicationData_EXPECT_FALSE(recoverData);
}

TEST_F(AppLaunchDataJsonTest, RefreshTimestamp) {
  ApplicationData data("mobile_app_id", "bundle_id", "device_mac");
  AddApplicationDataWithIncreaseTable(data);
  ApplicationData recoveredData("", "", "");
  TimevalStruct timestamp1 =
      GetApplicationData_EXPECT_TRUE(data, recoveredData);
  TimevalStruct tm = {0, 0};
  SetTimestamp(data, tm);
  TimevalStruct timestamp2 =
      GetApplicationData_EXPECT_TRUE(data, recoveredData);
  EXPECT_NE(timestamp1.tv_sec, timestamp2.tv_sec);
  AddApplicationDataWithoutIncreaseTable(data);  // again insert the same
  TimevalStruct timestamp3 =
      GetApplicationData_EXPECT_TRUE(data, recoveredData);
  EXPECT_TRUE(data == recoveredData);
  EXPECT_NE(timestamp2.tv_sec, timestamp3.tv_sec);
}

TEST_F(AppLaunchDataJsonTest, MaxCount) {
  const uint32_t max_ios_devs = res_json()->get_max_number_iOS_devs();
  for (uint32_t i = 0; i < max_ios_devs; i++) {
    ApplicationData data(AddCounter("mobile_app_id_", i),
                         AddCounter("bundle_id_", i),
                         "device_mac");
    AddApplicationDataWithIncreaseTable(data);
  }

  // insert new time stamp
  ApplicationData changedRecord("mobile_app_id_0", "bundle_id_0", "device_mac");
  TimevalStruct tm = {0, 0};
  SetTimestamp(changedRecord, tm);

  uint32_t size_max = res_json()->GetCurentNumberOfAppData();
  EXPECT_EQ(max_ios_devs, size_max);
  EXPECT_TRUE(res_json()->AddApplicationData(ApplicationData(
      "mobile_app_id_last", "bundle_id_last", "device_mac_last")));
  uint32_t size_after_max = res_json()->GetCurentNumberOfAppData();
  EXPECT_EQ(size_max, size_after_max);
  EXPECT_FALSE(res_json()->IsAppDataAlreadyExisted(changedRecord));
}

TEST_F(AppLaunchDataJsonTest, DeleteAllJsonDataTwice) {
  for (uint32_t i = 0; i < res_json()->get_max_number_iOS_devs(); i++) {
    ApplicationData data(AddCounter("mobile_app_id_", i),
                         AddCounter("bundle_id_", i),
                         "device_mac");
    AddApplicationDataWithIncreaseTable(data);
  }

  uint32_t full_size = res_json()->GetCurentNumberOfAppData();

  EXPECT_EQ(full_size, res_json()->get_max_number_iOS_devs());
  EXPECT_TRUE(res_json()->Clear());  // delete data
  EXPECT_EQ(0u, res_json()->GetCurentNumberOfAppData());
  EXPECT_TRUE(res_json()->Clear());  // second time delete data
  EXPECT_EQ(0u, res_json()->GetCurentNumberOfAppData());
}

namespace {
bool ApplicationDataComporator(const ApplicationDataPtr& left,
                               const ApplicationDataPtr& right) {
  return (left->device_mac_ < right->device_mac_ &&
          left->mobile_app_id_ < right->mobile_app_id_ &&
          left->bundle_id_ < right->bundle_id_);
}
}  // namespace

TEST_F(AppLaunchDataJsonTest, SelectMultipleData) {
  std::vector<ApplicationDataPtr> input_data1;
  std::vector<ApplicationDataPtr> input_data2;
  std::vector<ApplicationDataPtr> output_data1;
  std::vector<ApplicationDataPtr> output_data2;
  const std::string device_mac_1 = "device_mac_1";
  const std::string device_mac_2 = "device_mac_2";
  uint32_t half_part = res_json()->get_max_number_iOS_devs() / 2u;

  for (uint32_t i = 0; i < half_part; i++) {
    const std::string mobile_app_id = AddCounter("d1_mobile_app_id_", i);
    const std::string bundle_id = AddCounter("d1_bundle_id_", i);

    ApplicationDataPtr app_data = utils::MakeShared<ApplicationData>(
        mobile_app_id, bundle_id, device_mac_1);
    AddApplicationDataWithIncreaseTable(*app_data);
    input_data1.push_back(app_data);
  }

  for (uint32_t i = 0; i < half_part; i++) {
    const std::string mobile_app_id = AddCounter("d2_mobile_app_id_", i);
    const std::string bundle_id = AddCounter("d2_bundle_id_", i);

    ApplicationDataPtr app_data = utils::MakeShared<ApplicationData>(
        mobile_app_id, bundle_id, device_mac_2);
    AddApplicationDataWithIncreaseTable(*app_data);
    input_data2.push_back(app_data);
  }

  output_data1 = res_json()->GetApplicationDataByDevice(device_mac_1);
  output_data2 = res_json()->GetApplicationDataByDevice(device_mac_2);

  EXPECT_EQ(half_part, output_data1.size());
  EXPECT_EQ(half_part, output_data2.size());

  std::sort(
      output_data1.begin(), output_data1.end(), ApplicationDataComporator);
  std::sort(
      output_data2.begin(), output_data2.end(), ApplicationDataComporator);
  std::sort(input_data1.begin(), input_data1.end(), ApplicationDataComporator);
  std::sort(input_data2.begin(), input_data2.end(), ApplicationDataComporator);

  for (uint32_t i = 0; i < output_data1.size(); i++) {
    EXPECT_TRUE(*output_data1[i] == *input_data1[i]);
  }

  for (uint32_t i = 0; i < output_data2.size(); i++) {
    EXPECT_TRUE(*output_data2[i] == *input_data2[i]);
  }
}

}  // namespace app_launch
}  // namespace components
}  // namespace test
