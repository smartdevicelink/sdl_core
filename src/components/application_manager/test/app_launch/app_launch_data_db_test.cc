/*
 * Copyright (c) 2016, Ford Motor Company
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
#include <string>
#include <memory>
#include "gtest/gtest.h"
#include "utils/macro.h"
#include "sql_database.h"
#include "sql_query.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "application_manager/mock_app_launch_settings.h"
#include "application_manager/app_launch/app_launch_data_db.h"
#include "application_manager/app_launch/app_launch_sql_queries.h"

namespace test {
namespace components {
namespace app_launch_test {

using namespace file_system;
using namespace app_launch;

using ::testing::ReturnRef;
using ::testing::Return;
using ::testing::NiceMock;

namespace {
const std::string kEmptyString = "";
const std::string kMobileAppId = "mobile_app_id";
const std::string kBundleId = "bundle_id";
const std::string kDeviceMac = "device_mac";
}  // namespace

class AppLaunchDataDBTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    utils::dbms::SQLQuery query(test_db());
    EXPECT_TRUE(query.Prepare(kCreateSchema));
    EXPECT_TRUE(query.Exec());
    printf("Create table\n");
  }

  virtual void TearDown() {
    utils::dbms::SQLQuery query(test_db());
    EXPECT_TRUE(query.Prepare(kDropSchema));
    EXPECT_TRUE(query.Exec());
    printf("Delete table\n");
  }

  static void SetUpTestCase() {
    NiceMock<MockAppLaunchSettings> mock_app_launch_settings_;
    std::string curr_dir = file_system::CurrentWorkingDirectory();
    ON_CALL(mock_app_launch_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(curr_dir));
    ON_CALL(mock_app_launch_settings_, max_number_of_ios_device())
        .WillByDefault(Return(15u));
    ON_CALL(mock_app_launch_settings_, app_launch_max_retry_attempt())
        .WillByDefault(Return(5u));
    ON_CALL(mock_app_launch_settings_, app_launch_retry_wait_time())
        .WillByDefault(Return(500u));
    if (is_in_file) {
      res_db_.reset(
          new AppLaunchDataDB(mock_app_launch_settings_, In_File_Storage));
    } else {
      res_db_.reset(
          new AppLaunchDataDB(mock_app_launch_settings_, In_Memory_Storage));
    }
    test_db_ = (res_db_->db());

    EXPECT_TRUE(test_db()->Open());
    EXPECT_TRUE(test_db()->IsReadWrite());
  }

  // Memory keep and clear AppLaunchDataDb
  static utils::dbms::SQLDatabase* test_db_;

  static void TearDownTestCase() {
    DeleteFile(kDatabaseName + ".sqlite");
  }

  static utils::dbms::SQLDatabase* test_db() {
    return test_db_;
  }

  static std::auto_ptr<AppLaunchDataDB> res_db_;

  AppLaunchDataDB* res_db() {
    return res_db_.get();
  }

  const std::string kGetSsession =
      " SELECT * FROM `app_launch` WHERE `deviceMac` = ? AND `appID` = ? AND "
      "`bundleID` = ?;";
  const std::string kCheckTablesExist =
      "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND "
      "name='app_launch';";

  const std::string kInsertTimestamp =
      "UPDATE 'app_launch' "
      "SET 'last_session' = ' 1666-06-16 06:06:06.666 '"
      "WHERE `deviceMac` = ? AND `appID` = ? "
      "AND `bundleID` = ?;";

  // Write BD to file or memory
  static const bool is_in_file = false;

 public:
  void AddApplicationDataWithIncreaseTable(const ApplicationData& data);
  void AddApplicationDataWithoutIncreaseTable(const ApplicationData& data);
  std::string GetApplicationData(const ApplicationData& in_data,
                                 ApplicationData& out_data);
  std::string AddCounter(const std::string& inp, int32_t val);
  void AddApplicationData_Expect_FALSE(const ApplicationData& data);
};

utils::dbms::SQLDatabase* AppLaunchDataDBTest::test_db_ = NULL;
std::auto_ptr<AppLaunchDataDB> AppLaunchDataDBTest::res_db_;

void AppLaunchDataDBTest::AddApplicationDataWithIncreaseTable(
    const ApplicationData& data) {
  uint32_t sizeBeforeAdding = res_db()->GetCurentNumberOfAppData();
  EXPECT_TRUE(res_db()->AddApplicationData(data));

  uint32_t sizeAfterAdding = res_db()->GetCurentNumberOfAppData();
  EXPECT_EQ(sizeBeforeAdding + 1u, sizeAfterAdding);
}

void AppLaunchDataDBTest::AddApplicationDataWithoutIncreaseTable(
    const ApplicationData& data) {
  uint32_t sizeBeforeAdding = res_db()->GetCurentNumberOfAppData();
  EXPECT_TRUE(res_db()->AddApplicationData(data));

  uint32_t sizeAfterAdding = res_db()->GetCurentNumberOfAppData();
  EXPECT_EQ(sizeBeforeAdding, sizeAfterAdding);
}

std::string AppLaunchDataDBTest::GetApplicationData(
    const ApplicationData& in_data, ApplicationData& out_data) {
  utils::dbms::SQLQuery query(test_db());
  uint32_t sizeBeforeAdding = res_db()->GetCurentNumberOfAppData();
  EXPECT_TRUE(query.Prepare(kGetSsession));
  query.Bind(AppLaunchDataDB::device_mac_index, in_data.device_mac_);
  query.Bind(AppLaunchDataDB::application_id_index, in_data.mobile_app_id_);
  query.Bind(AppLaunchDataDB::bundle_id_index, in_data.bundle_id_);
  EXPECT_TRUE(query.Exec());

  uint32_t sizeAfterAdding = res_db()->GetCurentNumberOfAppData();

  EXPECT_EQ(sizeBeforeAdding, sizeAfterAdding);

  out_data.device_mac_ = query.GetString(AppLaunchDataDB::device_mac_index);
  out_data.mobile_app_id_ =
      query.GetString(AppLaunchDataDB::application_id_index);
  out_data.bundle_id_ = query.GetString(AppLaunchDataDB::bundle_id_index);
  // Timestamp
  return query.GetString(AppLaunchDataDB::timestamp_index);
}

std::string AppLaunchDataDBTest::AddCounter(const std::string& inp,
                                            int32_t val) {
  std::stringstream ss;
  ss << inp << "_" << val;
  return ss.str();
}

TEST_F(AppLaunchDataDBTest, Init) {
  utils::dbms::SQLQuery query_checks(test_db());
  EXPECT_TRUE(query_checks.Prepare(kCheckTablesExist));
  EXPECT_TRUE(query_checks.Exec());
  EXPECT_EQ(1u, query_checks.GetUInteger(0));
}

TEST_F(AppLaunchDataDBTest, SaveAndGetData) {
  ApplicationData data(kMobileAppId, kBundleId, kDeviceMac);
  AddApplicationDataWithIncreaseTable(data);
  ApplicationData recoveredData(kEmptyString, kEmptyString, kEmptyString);
  GetApplicationData(data, recoveredData);
  EXPECT_TRUE(data == recoveredData);
}

TEST_F(AppLaunchDataDBTest, NotAddEmptyAppData) {
  ApplicationData data1(kEmptyString, kBundleId, kDeviceMac);
  ApplicationData data2(kMobileAppId, kEmptyString, kDeviceMac);
  ApplicationData data3(kMobileAppId, kBundleId, kEmptyString);
  ApplicationData data4(kEmptyString, kEmptyString, kEmptyString);
  EXPECT_FALSE(res_db()->AddApplicationData(data1));
  EXPECT_FALSE(res_db()->AddApplicationData(data2));
  EXPECT_FALSE(res_db()->AddApplicationData(data3));
  EXPECT_FALSE(res_db()->AddApplicationData(data4));
  EXPECT_EQ(0u, res_db()->GetCurentNumberOfAppData());
}

TEST_F(AppLaunchDataDBTest, SaveOneAndGetAnotherData) {
  ApplicationData data(kMobileAppId, kBundleId, kDeviceMac);
  ApplicationData recoverData = data;
  AddApplicationDataWithIncreaseTable(data);
  recoverData.device_mac_ += "test";
  GetApplicationData(recoverData, recoverData);
  EXPECT_FALSE(data == recoverData);
}

TEST_F(AppLaunchDataDBTest, MaxCount) {
  const uint32_t max_ios_devs = res_db()->get_max_number_iOS_devs();

  for (uint32_t i = 0; i < max_ios_devs; i++) {
    ApplicationData data(
        AddCounter(kMobileAppId, i), AddCounter(kBundleId, i), kDeviceMac);
    AddApplicationDataWithIncreaseTable(data);
  }

  utils::dbms::SQLQuery query(test_db());
  ApplicationData changedRecord(
      AddCounter(kMobileAppId, 0), AddCounter(kBundleId, 0), kDeviceMac);
  EXPECT_TRUE(query.Prepare(kInsertTimestamp));
  query.Bind(AppLaunchDataDB::device_mac_index, changedRecord.device_mac_);
  query.Bind(AppLaunchDataDB::application_id_index,
             changedRecord.mobile_app_id_);
  query.Bind(AppLaunchDataDB::bundle_id_index, changedRecord.bundle_id_);
  EXPECT_TRUE(query.Exec());

  uint32_t size_max = res_db()->GetCurentNumberOfAppData();
  EXPECT_EQ(max_ios_devs, size_max);
  EXPECT_TRUE(res_db()->AddApplicationData(
      ApplicationData(AddCounter(kMobileAppId, max_ios_devs),
                      AddCounter(kBundleId, max_ios_devs),
                      kDeviceMac)));
  uint32_t size_after_max = res_db()->GetCurentNumberOfAppData();
  EXPECT_EQ(size_max, size_after_max);
  EXPECT_FALSE(res_db()->IsAppDataAlreadyExisted(changedRecord));
}

namespace {
bool ApplicationDataComporator(const ApplicationDataPtr& left,
                               const ApplicationDataPtr& right) {
  return (left->device_mac_ < right->device_mac_ &&
          left->mobile_app_id_ < right->mobile_app_id_ &&
          left->bundle_id_ < right->bundle_id_);
}
}  // namespace

TEST_F(AppLaunchDataDBTest, SelectMultipleData) {
  std::vector<ApplicationDataPtr> input_data1;
  std::vector<ApplicationDataPtr> input_data2;
  std::vector<ApplicationDataPtr> output_data1;
  std::vector<ApplicationDataPtr> output_data2;
  const std::string device_mac_1 = "device_mac_1";
  const std::string device_mac_2 = "device_mac_2";

  uint32_t half_of_max_number_iOS_devs =
      res_db()->get_max_number_iOS_devs() / 2u;

  for (uint32_t i = 0; i < half_of_max_number_iOS_devs; i++) {
    const std::string mobile_app_id = AddCounter("d1_mobile_app_id", i);
    const std::string bundle_id = AddCounter("d1_bundle_id", i);

    ApplicationDataPtr app_data = utils::MakeShared<ApplicationData>(
        mobile_app_id, bundle_id, device_mac_1);
    AddApplicationDataWithIncreaseTable(*app_data);
    input_data1.push_back(app_data);
  }

  for (uint32_t i = 0; i < half_of_max_number_iOS_devs; i++) {
    const std::string mobile_app_id = AddCounter("d2_mobile_app_id", i);
    const std::string bundle_id = AddCounter("d2_bundle_id", i);

    ApplicationDataPtr app_data = utils::MakeShared<ApplicationData>(
        mobile_app_id, bundle_id, device_mac_2);
    AddApplicationDataWithIncreaseTable(*app_data);
    input_data2.push_back(app_data);
  }

  output_data1 = res_db()->GetApplicationDataByDevice(device_mac_1);
  output_data2 = res_db()->GetApplicationDataByDevice(device_mac_2);

  EXPECT_EQ(half_of_max_number_iOS_devs, output_data1.size());
  EXPECT_EQ(half_of_max_number_iOS_devs, output_data2.size());

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

// Most be last cause after it AppLaunchDataDBTest
// requeste manual Init call
TEST_F(AppLaunchDataDBTest, DeleteAllTableDataTwice) {
  for (uint32_t i = 0; i < res_db()->get_max_number_iOS_devs(); i++) {
    ApplicationData data(
        AddCounter(kMobileAppId, i), AddCounter(kBundleId, i), kDeviceMac);
    AddApplicationDataWithIncreaseTable(data);
  }

  uint32_t full_size = res_db()->GetCurentNumberOfAppData();

  EXPECT_EQ(full_size, res_db()->get_max_number_iOS_devs());
  EXPECT_TRUE(res_db()->Clear());  // delete data
  EXPECT_EQ(0u, res_db()->GetCurentNumberOfAppData());
  EXPECT_TRUE(res_db()->Clear());  // second time delete data
  EXPECT_EQ(0u, res_db()->GetCurentNumberOfAppData());
}

}  // namespace app_launch_test
}  // namespace components
}  // namespace test
