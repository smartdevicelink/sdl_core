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

#include "gtest/gtest.h"
#include <string>
#include <memory>
#include "resumption/last_state.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace resumption {

using namespace ::resumption;
using namespace ::Json;

class LastStateTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    file_system::DeleteFile("./app_info_storage");
    last_state_ = std::auto_ptr<resumption::LastState>(
          new resumption::LastState("app_storage_folder", "app_info_storage"));
    ASSERT_TRUE(file_system::CreateFile("./app_info.dat"));
    profile::Profile::instance()->UpdateValues();
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(file_system::DeleteFile("./app_info.dat"));
  }
  std::auto_ptr<resumption::LastState> last_state_;
};

TEST_F(LastStateTest, Basic) {
  Value& dictionary = last_state_->dictionary;
  const std::string empty_dictionary = "null\n";
  EXPECT_EQ(empty_dictionary, dictionary.toStyledString());
}

TEST_F(LastStateTest, SetGetData) {
  {
    Value& dictionary = last_state_->dictionary;
    Value bluetooth_info = dictionary["TransportManager"]["BluetoothAdapter"];
    const std::string empty_bluetooth = "null\n";
    EXPECT_EQ(empty_bluetooth, bluetooth_info.toStyledString());

    Value tcp_adapter_info =
        dictionary["TransportManager"]["TcpAdapter"]["devices"];
    const std::string no_devices = "null\n";
    EXPECT_EQ(no_devices, tcp_adapter_info.toStyledString());

    Value resumption_time = dictionary["resumption"]["last_ign_off_time"];
    EXPECT_EQ("null\n", resumption_time.toStyledString());

    Value resumption_list = dictionary["resumption"]["resume_app_list"];
    EXPECT_EQ("null\n", resumption_list.toStyledString());

    Value test_value;
    test_value["name"] = "test_device";

    last_state_
        ->dictionary["TransportManager"]["TcpAdapter"]["devices"] = test_value;
    last_state_
        ->dictionary["TransportManager"]["BluetoothAdapter"]["devices"] =
        "bluetooth_device";
    last_state_->SaveToFileSystem();
  }
  Value& dictionary = last_state_->dictionary;
  Value bluetooth_info = dictionary["TransportManager"]["BluetoothAdapter"];
  Value tcp_adapter_info = dictionary["TransportManager"]["TcpAdapter"];
  EXPECT_EQ("{\n   \"devices\" : \"bluetooth_device\"\n}\n",
            bluetooth_info.toStyledString());
  EXPECT_EQ(
      "{\n   \"devices\" : {\n      \"name\" : \"test_device\"\n   }\n}\n",
      tcp_adapter_info.toStyledString());
}

}  // namespace resumption
}  // namespace components
}  // namespace test
