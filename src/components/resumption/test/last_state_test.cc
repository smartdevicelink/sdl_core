/*
 * Copyright (c) 2015, Ford Motor Company
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

#include <string>
#include "gtest/gtest.h"
#include "resumption/last_state.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace resumption_test {

using namespace ::resumption;

const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppInfoStorageFile = "app_info_storage";
class LastStateTest : public ::testing::Test {
 protected:
  LastStateTest()
      : empty_dictionary_("null\n")
      , app_info_dat_file_("app_info.dat")
      , last_state_(kAppStorageFolder, kAppInfoStorageFile) {}

  static void SetUpTestCase() {
    file_system::DeleteFile(kAppInfoStorageFile);
    file_system::RemoveDirectory(kAppStorageFolder, false);
  }
  void SetUp() OVERRIDE {
    ASSERT_TRUE(file_system::CreateFile(app_info_dat_file_));
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(file_system::DeleteFile((app_info_dat_file_)));
  }
  const std::string empty_dictionary_;
  const std::string app_info_dat_file_;

  resumption::LastState last_state_;
};

TEST_F(LastStateTest, Basic) {
  utils::json::JsonValue& dictionary = last_state_.dictionary();
  EXPECT_EQ("null\n", dictionary.ToJson(true));
}

TEST_F(LastStateTest, SetGetData) {
  {
    utils::json::JsonValue& dictionary = last_state_.dictionary();
    utils::json::JsonValue bluetooth_info =
        dictionary["TransportManager"]["BluetoothAdapter"];
    EXPECT_EQ("null\n", bluetooth_info.ToJson(true));

    utils::json::JsonValue tcp_adapter_info =
        dictionary["TransportManager"]["TcpAdapter"]["devices"];
    EXPECT_EQ("null\n", tcp_adapter_info.ToJson(true));

    utils::json::JsonValue resumption_time =
        dictionary["resumption"]["last_ign_off_time"];
    EXPECT_EQ("null\n", resumption_time.ToJson(true));

    utils::json::JsonValue resumption_list =
        dictionary["resumption"]["resume_app_list"];
    EXPECT_EQ("null\n", resumption_list.ToJson(true));

    utils::json::JsonValue test_value;
    test_value["name"] = "test_device";

    utils::json::JsonValue& save_dictionary = last_state_.dictionary();
    save_dictionary["TransportManager"]["TcpAdapter"]["devices"] = test_value;
    save_dictionary["TransportManager"]["BluetoothAdapter"]["devices"] =
        "bluetooth_device";
    last_state_.SetDictionary(save_dictionary);
    last_state_.SaveToFileSystem();
  }

  utils::json::JsonValue dictionary = last_state_.dictionary();

  utils::json::JsonValue bluetooth_info =
      dictionary["TransportManager"]["BluetoothAdapter"];
  utils::json::JsonValue tcp_adapter_info =
      dictionary["TransportManager"]["TcpAdapter"];
  EXPECT_EQ("{\n   \"devices\" : \"bluetooth_device\"\n}\n",
            bluetooth_info.ToJson(true));
  EXPECT_EQ(
      "{\n   \"devices\" : {\n      \"name\" : \"test_device\"\n   }\n}\n",
      tcp_adapter_info.ToJson(true));
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test
