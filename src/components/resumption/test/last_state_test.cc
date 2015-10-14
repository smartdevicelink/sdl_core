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
#include "resumption/last_state.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace resumption {

using namespace ::resumption;
using namespace ::Json;

class LastStateTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    ASSERT_TRUE(::file_system::CreateFile("./app_info.dat"));
    ::profile::Profile::instance()->UpdateValues();
  }

  virtual void TearDown() {
    EXPECT_TRUE(::file_system::DeleteFile("./app_info.dat"));
  }
};

TEST_F(LastStateTest, Basic) {
  Value& dictionary = LastState::instance()->dictionary;
  EXPECT_EQ("null\n", dictionary.toStyledString());
}

TEST_F(LastStateTest, SetGetData) {
  {
    Value& dictionary = LastState::instance()->dictionary;
    Value bluetooth_info = dictionary["TransportManager"]["BluetoothAdapter"];
    EXPECT_EQ("null\n", bluetooth_info.toStyledString());

    Value tcp_adapter_info =
        dictionary["TransportManager"]["TcpAdapter"]["devices"];
    EXPECT_EQ("null\n", tcp_adapter_info.toStyledString());

    Value resumption_time = dictionary["resumption"]["last_ign_off_time"];
    EXPECT_EQ("null\n", resumption_time.toStyledString());

    Value resumption_list = dictionary["resumption"]["resume_app_list"];
    EXPECT_EQ("null\n", resumption_list.toStyledString());

    Value test_value;
    test_value["name"] = "test_device";

    LastState::instance()
        ->dictionary["TransportManager"]["TcpAdapter"]["devices"] = test_value;
    LastState::instance()
        ->dictionary["TransportManager"]["BluetoothAdapter"]["devices"] =
        "bluetooth_device";
    LastState::instance()->SaveToFileSystem();
  }

  Value& dictionary = LastState::instance()->dictionary;

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
