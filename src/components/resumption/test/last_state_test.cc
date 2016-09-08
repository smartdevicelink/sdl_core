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
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include <json/writer.h>
#include <json/reader.h>

namespace test {
namespace components {
namespace resumption_test {

using ::utils::SharedPtr;
using ::utils::MakeShared;

namespace {
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppInfoStorageFile = "app_info_storage.dat";
const std::string kJsonStr = "{\"firstName\":\"name\"}";
}  // namespace

class LastStateTest : public ::testing::Test {
 protected:
  LastStateTest()
      : empty_dictionary_("null\n"), app_info_dat_file_(kAppInfoStorageFile) {}

  static void SetUpTestCase() {
    file_system::DeleteFile(kAppInfoStorageFile);
    file_system::RemoveDirectory(kAppStorageFolder, false);
  }
  void SetUp() OVERRIDE {
    ASSERT_TRUE(file_system::CreateFile(app_info_dat_file_));

    const std::vector<uint8_t> char_vector_pdata(kJsonStr.begin(),
                                                 kJsonStr.end());
    ASSERT_TRUE(file_system::Write(kAppInfoStorageFile, char_vector_pdata));
    last_state_ = utils::MakeShared<resumption::LastState>(kAppStorageFolder,
                                                           kAppInfoStorageFile);
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(file_system::DeleteFile((app_info_dat_file_)));
  }
  const std::string empty_dictionary_;
  const std::string app_info_dat_file_;

  ::utils::SharedPtr<resumption::LastState> last_state_;

  void CompactJson(std::string& str) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(str, root);
    Json::FastWriter writer;
    str = writer.write(root);
    if ('\n' == *(str.end() - 1)) {
      str.erase(str.end() - 1);
    }
  }
};

TEST_F(LastStateTest, Basic) {
  utils::json::JsonValue& dictionary = last_state_->dictionary();
  std::string str = dictionary.ToJson(true);
  CompactJson(str);
  EXPECT_EQ(kJsonStr, str);
}

TEST_F(LastStateTest, SetGetData) {
  {
    const std::string null_string = "null";
    utils::json::JsonValue& dictionary = last_state_->dictionary();
    utils::json::JsonValue bluetooth_info =
        dictionary["TransportManager"]["BluetoothAdapter"];
    std::string str = bluetooth_info.ToJson(true);
    CompactJson(str);
    EXPECT_EQ(null_string, str);

    utils::json::JsonValue tcp_adapter_info =
        dictionary["TransportManager"]["TcpAdapter"]["devices"];
    str = tcp_adapter_info.ToJson(true);
    CompactJson(str);
    EXPECT_EQ(null_string, str);

    utils::json::JsonValue resumption_time =
        dictionary["resumption"]["last_ign_off_time"];
    str = resumption_time.ToJson(true);
    CompactJson(str);
    EXPECT_EQ(null_string, str);

    utils::json::JsonValue resumption_list =
        dictionary["resumption"]["resume_app_list"];
    str = resumption_list.ToJson(true);
    CompactJson(str);
    EXPECT_EQ(null_string, str);

    utils::json::JsonValue test_value;
    test_value["name"] = "test_device";

    utils::json::JsonValue& save_dictionary = last_state_->dictionary();
    save_dictionary["TransportManager"]["TcpAdapter"]["devices"] = test_value;
    save_dictionary["TransportManager"]["BluetoothAdapter"]["devices"] =
        "bluetooth_device";
    last_state_->SetDictionary(save_dictionary);
    last_state_->SaveToFileSystem();
  }

  utils::json::JsonValue dictionary = last_state_->dictionary();

  utils::json::JsonValue bluetooth_info =
      dictionary["TransportManager"]["BluetoothAdapter"];
  utils::json::JsonValue tcp_adapter_info =
      dictionary["TransportManager"]["TcpAdapter"];
  std::string str = bluetooth_info.ToJson(true);
  CompactJson(str);
  const std::string bluetooth_device_str = "{\"devices\":\"bluetooth_device\"}";
  EXPECT_EQ(bluetooth_device_str, str);
  str = tcp_adapter_info.ToJson(true);
  CompactJson(str);
  const std::string test_device_str =
      "{\"devices\":{\"name\":\"test_device\"}}";
  EXPECT_EQ(test_device_str, str);
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test
