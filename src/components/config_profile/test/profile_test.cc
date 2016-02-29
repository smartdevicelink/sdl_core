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

#include <algorithm>
#include <vector>

#include "gtest/gtest.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "utils/threads/thread.h"

namespace test {
namespace components {
namespace profile {

using namespace ::profile;

class ProfileTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    profile::Profile::destroy();
  }
  virtual void TearDown() {
    profile::Profile::destroy();
  }

};

TEST_F(ProfileTest, SingletonProfile) {
  Profile::instance();
  EXPECT_TRUE(Profile::exists());
  profile::Profile::destroy();
  EXPECT_FALSE(Profile::exists());
}

TEST_F(ProfileTest, UpdateConfigWithDefaultFile) {
  // Default value
  uint32_t thread_min_stack_size = threads::Thread::kMinStackSize;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());
  std::string vr_help_title_ = "";
  EXPECT_EQ(vr_help_title_, Profile::instance()->vr_help_title());
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  profile::Profile::instance()->UpdateValues();
  // Value was updated
  thread_min_stack_size = 20480;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
  EXPECT_TRUE(profile::Profile::instance()->enable_policy());
  vr_help_title_ = "Available Vr Commands List";
  EXPECT_EQ(vr_help_title_, Profile::instance()->vr_help_title());
}

TEST_F(ProfileTest, SetConfigFileWithoutCallUpdate) {
  // Default value
  uint32_t thread_min_stack_size = threads::Thread::kMinStackSize;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());
  std::string vr_help_title_ = "";
  EXPECT_EQ(vr_help_title_, Profile::instance()->vr_help_title());
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  // Value was updated
  thread_min_stack_size = 20480;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
  EXPECT_TRUE(profile::Profile::instance()->enable_policy());
  vr_help_title_ = "Available Vr Commands List";
  EXPECT_EQ(vr_help_title_, Profile::instance()->vr_help_title());
}

TEST_F(ProfileTest, SetConfigFileWithUpdate) {
  // Default value
  uint32_t thread_min_stack_size = threads::Thread::kMinStackSize;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  // Value was updated
  thread_min_stack_size = 20480;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Update config file again
  profile::Profile::instance()->UpdateValues();

  // Value should be the same
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
}

TEST_F(ProfileTest, UpdateManyTimesDefaultFile) {
  //using for check logger's work - core dump when this test was started and log4cxx exists in test derictory
  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());
  // Update config many times
  for (int i = 0; i < 10; i++) {
    profile::Profile::instance()->UpdateValues();
  }
}

TEST_F(ProfileTest, UpdateIntValues) {
  // Default value
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());
  uint32_t thread_min_stack_size = threads::Thread::kMinStackSize;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Set config file with default name
  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  // Value changes
  thread_min_stack_size = 20480;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Update config file again
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  // Value changes
  thread_min_stack_size = 21000;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Update config file again
  profile::Profile::instance()->UpdateValues();
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  // Value should be the same
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Set config file with default name again
  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  // Value should be changed
  thread_min_stack_size = 20480;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
}

TEST_F(ProfileTest, UpdateBoolValues) {
  // Default values
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());
  EXPECT_TRUE(profile::Profile::instance()->launch_hmi());
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());

  // Set config file
  Profile::instance()->config_file_name("smartDeviceLink.ini");
  // Check values
  EXPECT_TRUE(profile::Profile::instance()->launch_hmi());
  EXPECT_TRUE(profile::Profile::instance()->enable_policy());
  EXPECT_FALSE(profile::Profile::instance()->is_redecoding_enabled());

  // Update config file again
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_TRUE(profile::Profile::instance()->launch_hmi());
  EXPECT_TRUE(profile::Profile::instance()->enable_policy());
  EXPECT_FALSE(profile::Profile::instance()->is_redecoding_enabled());

  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  // Parameters after updating
  EXPECT_FALSE(profile::Profile::instance()->launch_hmi());
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());
  EXPECT_TRUE(profile::Profile::instance()->is_redecoding_enabled());

  // Update config file
  profile::Profile::instance()->UpdateValues();

  // Parameters are same
  EXPECT_FALSE(profile::Profile::instance()->launch_hmi());
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());
  EXPECT_TRUE(profile::Profile::instance()->is_redecoding_enabled());
}

TEST_F(ProfileTest, UpdateStringValue) {
  // Default values
  std::string config_folder = "";
  std::string tts_delimiter_ = "";
  std::string vr_help_title_ = "";
  std::string server_address = "127.0.0.1";
  std::string app_resourse_folder = "";
  EXPECT_EQ(config_folder, Profile::instance()->app_resourse_folder());
  EXPECT_EQ(server_address, profile::Profile::instance()->server_address());

  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  // Check values
  config_folder = file_system::CurrentWorkingDirectory();
  EXPECT_EQ(config_folder, Profile::instance()->app_resourse_folder());
  tts_delimiter_ = ",";
  EXPECT_EQ(tts_delimiter_, Profile::instance()->tts_delimiter());
  vr_help_title_ = "Available Vr Commands List";
  EXPECT_EQ(vr_help_title_, Profile::instance()->vr_help_title());

  EXPECT_EQ(server_address, profile::Profile::instance()->server_address());
  app_resourse_folder = file_system::CurrentWorkingDirectory();
  EXPECT_EQ(app_resourse_folder, Profile::instance()->app_resourse_folder());

  // Update config file
  profile::Profile::instance()->UpdateValues();

  // Values are the same
  EXPECT_EQ(config_folder, Profile::instance()->app_resourse_folder());
  EXPECT_EQ(tts_delimiter_, Profile::instance()->tts_delimiter());
  EXPECT_EQ(vr_help_title_, Profile::instance()->vr_help_title());
  EXPECT_EQ(server_address, profile::Profile::instance()->server_address());
  EXPECT_EQ(app_resourse_folder, Profile::instance()->app_resourse_folder());
}

TEST_F(ProfileTest, UpdateInt_ValueAppearsInFileTwice) {
  // Default values
  uint32_t server_port = 8087;
  EXPECT_EQ(server_port, Profile::instance()->server_port());
  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());
  // Update config file
  // First server_port = 8088
  server_port = 8088;
  EXPECT_EQ(server_port, Profile::instance()->server_port());
}

TEST_F(ProfileTest, UpdateBool_ValueAppearsInFileTwice) {
  // Default values
  EXPECT_FALSE(Profile::instance()->is_mixing_audio_supported());
  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());
  // Update config file
  // First value is false
  EXPECT_FALSE(profile::Profile::instance()->is_mixing_audio_supported());
}

TEST_F(ProfileTest, UpdateVectorOfString_ValueAppearsInFileTwice) {
  // Default values
  std::vector < std::string > time_out_promt;
  EXPECT_EQ(time_out_promt, Profile::instance()->time_out_promt());
  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());
  // Update config file
  time_out_promt.push_back("Please say a command,");
  EXPECT_EQ(time_out_promt, Profile::instance()->time_out_promt());
}

TEST_F(ProfileTest, UpdateString_ValueAppearsInFileTwice) {
  // Default values
  std::string recording_file_name = "record.wav";
  EXPECT_EQ(recording_file_name, Profile::instance()->recording_file_name());
  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());
  // Update config file
  recording_file_name = "video.wav";
  EXPECT_EQ(recording_file_name, Profile::instance()->recording_file_name());
}

TEST_F(ProfileTest, UpdatePairsValue) {
  // Default values
  std::pair < uint32_t, int32_t > value;
  value.first = 0;
  value.second = 0;
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());

  Profile::instance()->config_file_name("smartDeviceLink.ini");
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  value.first = 5;
  value.second = 1;
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());
}

// Section with negative tests

TEST_F(ProfileTest, PairsValueEmpty) {
  // Default values
  std::pair < uint32_t, int32_t > value;
  value.first = 0;
  value.second = 0;
  EXPECT_EQ(value, Profile::instance()->read_did_frequency());

  Profile::instance()->config_file_name("smartDeviceLink_invalid_pairs.ini");
  EXPECT_EQ("smartDeviceLink_invalid_pairs.ini",
            Profile::instance()->config_file_name());

  // Default values
  value.first = 5;
  value.second = 1;
  EXPECT_EQ(value, Profile::instance()->read_did_frequency());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_EQ(value, Profile::instance()->read_did_frequency());
}

TEST_F(ProfileTest, CharValueInPairInsteadOfInt) {
  std::pair < uint32_t, int32_t > value;
  value.first = 0;
  value.second = 0;
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());

  Profile::instance()->config_file_name("smartDeviceLink_invalid_string.ini");
  EXPECT_EQ("smartDeviceLink_invalid_string.ini",
            Profile::instance()->config_file_name());

  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());
}

TEST_F(ProfileTest, EmptyValuesInPair) {
  Profile::instance()->config_file_name("smartDeviceLink_invalid_pairs.ini");
  EXPECT_EQ("smartDeviceLink_invalid_pairs.ini",
            Profile::instance()->config_file_name());

  std::pair < uint32_t, int32_t > value;
  value.first = 0;
  value.second = 0;
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_EQ(value, Profile::instance()->get_vehicle_data_frequency());
}

TEST_F(ProfileTest, IntInsteadOfPair) {
  // Default values
  std::pair < uint32_t, int32_t > value;
  value.first = 0;
  value.second = 0;
  EXPECT_EQ(value, Profile::instance()->start_stream_retry_amount());

  Profile::instance()->config_file_name("smartDeviceLink_invalid_pairs.ini");
  EXPECT_EQ("smartDeviceLink_invalid_pairs.ini",
            Profile::instance()->config_file_name());
  // Ini file includes only one element
  value.first = 9;
  value.second = 0;
  EXPECT_EQ(value, Profile::instance()->start_stream_retry_amount());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_EQ(value, Profile::instance()->start_stream_retry_amount());
}

TEST_F(ProfileTest, WrongIntValue) {
  // Default value
  uint32_t heart_beat_timeout = 0;
  EXPECT_EQ(heart_beat_timeout, Profile::instance()->heart_beat_timeout());

  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_invalid_int.ini");
  EXPECT_EQ("smartDeviceLink_invalid_int.ini",
            Profile::instance()->config_file_name());

  // Value in file includes letters. Check that value is default
  heart_beat_timeout = 0;
  EXPECT_EQ(heart_beat_timeout, Profile::instance()->heart_beat_timeout());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  EXPECT_EQ(heart_beat_timeout, Profile::instance()->heart_beat_timeout());
}

TEST_F(ProfileTest, WrongMaxIntValue) {
  // Default value
  uint32_t maxvalue = 2000000000;
  EXPECT_EQ(maxvalue, Profile::instance()->max_cmd_id());

  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_invalid_int.ini");
  EXPECT_EQ("smartDeviceLink_invalid_int.ini",
            Profile::instance()->config_file_name());

  // Value in file is more than could be saved.
  // Check that value is default
  EXPECT_EQ(maxvalue, Profile::instance()->max_cmd_id());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  EXPECT_EQ(maxvalue, Profile::instance()->max_cmd_id());
}

TEST_F(ProfileTest, WrongMinIntValue) {
  // Default value
  uint32_t minvalue = threads::Thread::kMinStackSize;
  EXPECT_EQ(minvalue, Profile::instance()->thread_min_stack_size());

  uint16_t server_port = 8087;
  EXPECT_EQ(server_port, Profile::instance()->server_port());

  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_invalid_int.ini");
  EXPECT_EQ("smartDeviceLink_invalid_int.ini",
            Profile::instance()->config_file_name());

  //File include 0, value should be lefted as default
  EXPECT_EQ(minvalue, Profile::instance()->thread_min_stack_size());
  //File include -1, value should be lefted as default
  EXPECT_EQ(server_port, Profile::instance()->server_port());

  // Update config file
  profile::Profile::instance()->UpdateValues();

  // Default value should be lefted
  EXPECT_EQ(minvalue, Profile::instance()->thread_min_stack_size());
  EXPECT_EQ(server_port, Profile::instance()->server_port());
}

TEST_F(ProfileTest, CheckCorrectValueWhenOtherValueInvalid) {
  // Default value
  uint32_t maxvalue = 2000000000;
  EXPECT_EQ(maxvalue, Profile::instance()->max_cmd_id());

  uint32_t thread_min_stack_size = threads::Thread::kMinStackSize;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  // Check that value is default
  maxvalue = 2000000000;
  EXPECT_EQ(maxvalue, Profile::instance()->max_cmd_id());

  // Other value is correct
  thread_min_stack_size = 21000;
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());

  // Update config file
  profile::Profile::instance()->UpdateValues();

  // In file the number is bigger than can be, default value should be lefted
  EXPECT_EQ(maxvalue, Profile::instance()->max_cmd_id());
  EXPECT_EQ(thread_min_stack_size,
            Profile::instance()->thread_min_stack_size());
}

TEST_F(ProfileTest, PairsValueInsteadOfInt) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_invalid_int.ini");
  EXPECT_EQ("smartDeviceLink_invalid_int.ini",
            Profile::instance()->config_file_name());
  // Get first number
  uint32_t list_files_in_none = 5;
  EXPECT_EQ(list_files_in_none, Profile::instance()->list_files_in_none());

  // Update config file
  profile::Profile::instance()->UpdateValues();
  // Values are same
  EXPECT_EQ(list_files_in_none, Profile::instance()->list_files_in_none());
}

TEST_F(ProfileTest, StringValueIncludeSlashesAndRussianLetters) {
  // Default values
  std::string config_folder = "";
  EXPECT_EQ(config_folder, Profile::instance()->app_resourse_folder());
  std::string tts_delimiter_ = "";
  std::string app_resourse_folder = "";
  std::string app_storage_folder = "";

  std::string current_dir =  file_system::CurrentWorkingDirectory();
  Profile::instance()->config_file_name("smartDeviceLink_invalid_string.ini");
  EXPECT_EQ("smartDeviceLink_invalid_string.ini",
            Profile::instance()->config_file_name());

  // Check values
  config_folder = "///";
  EXPECT_EQ(config_folder, Profile::instance()->app_config_folder());
  tts_delimiter_ = "coma and point";
  EXPECT_EQ(tts_delimiter_, Profile::instance()->tts_delimiter());
  std::string server_address = "127.0.0.1 + слово";
  EXPECT_EQ(server_address, profile::Profile::instance()->server_address());
  app_resourse_folder = "/new folder/";
  EXPECT_EQ(current_dir+app_resourse_folder, Profile::instance()->app_resourse_folder());
  app_storage_folder = "/\" \"";
  EXPECT_EQ(current_dir+app_storage_folder, Profile::instance()->app_storage_folder());

  // Update config file
  profile::Profile::instance()->UpdateValues();

  // Values are the same
  EXPECT_EQ(config_folder, Profile::instance()->app_config_folder());
  EXPECT_EQ(tts_delimiter_, Profile::instance()->tts_delimiter());
  EXPECT_EQ(server_address, profile::Profile::instance()->server_address());
  EXPECT_EQ(current_dir+app_resourse_folder, Profile::instance()->app_resourse_folder());
}

TEST_F(ProfileTest, StringUpperBoundValue) {
  // Default values
  std::string vr_help_title = "";
  std::string recording_file_name = "record.wav";
  EXPECT_EQ(vr_help_title, Profile::instance()->vr_help_title());
  EXPECT_EQ(recording_file_name, Profile::instance()->recording_file_name());

  Profile::instance()->config_file_name("smartDeviceLink_invalid_string.ini");
  EXPECT_EQ("smartDeviceLink_invalid_string.ini",
            Profile::instance()->config_file_name());

  // Total count of elements in ini file's string will be less 512
  vr_help_title =
      "0/0/0/1/2345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890abc!def@ghi";
  EXPECT_EQ(vr_help_title, Profile::instance()->vr_help_title());
  EXPECT_NE(vr_help_title, Profile::instance()->recording_file_name());
  recording_file_name =
      "0/0/0/1/2345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012345678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHIJKL+MNO|PQR~STU{}WXY[]Z,01234567890abc";
  EXPECT_EQ(recording_file_name, Profile::instance()->recording_file_name());
  // Update config file
  profile::Profile::instance()->UpdateValues();
  // Values are the same
  EXPECT_EQ(vr_help_title, Profile::instance()->vr_help_title());
  EXPECT_EQ(recording_file_name, Profile::instance()->recording_file_name());
}

TEST_F(ProfileTest, CapitalLetterInBoolValue) {
  // Default values
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());
  EXPECT_TRUE(profile::Profile::instance()->launch_hmi());
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());

  // Set config file
  Profile::instance()->config_file_name("smartDeviceLink.ini");
  // Check values
  EXPECT_TRUE(profile::Profile::instance()->launch_hmi());
  EXPECT_TRUE(profile::Profile::instance()->enable_policy());
  EXPECT_FALSE(profile::Profile::instance()->is_redecoding_enabled());

  // Change config file
  Profile::instance()->config_file_name("smartDeviceLink_invalid_boolean.ini");
  EXPECT_EQ("smartDeviceLink_invalid_boolean.ini",
            Profile::instance()->config_file_name());

  // Parameters after updating
  // Parameter launch_hmi = True
  EXPECT_FALSE(profile::Profile::instance()->launch_hmi());
  // EnablePolicy = TRUE
  EXPECT_FALSE(profile::Profile::instance()->enable_policy());
  // EnableRedecoding = FALSE
  EXPECT_FALSE(profile::Profile::instance()->is_redecoding_enabled());
}

TEST_F(ProfileTest, CheckReadStringValue) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  std::string app_resourse_folder_;
  profile::Profile::instance()->ReadStringValue(
      &app_resourse_folder_, file_system::CurrentWorkingDirectory().c_str(),
      "MAIN", "AppResourceFolder");
  // Get default value
  EXPECT_EQ(app_resourse_folder_, file_system::CurrentWorkingDirectory());

  // Get value from file
  std::string server_address;
  profile::Profile::instance()->ReadStringValue(&server_address, "", "HMI",
                                                "ServerAddress");
  EXPECT_EQ("127.0.0.1", server_address);
}

TEST_F(ProfileTest, CheckReadBoolValue) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  bool enable_policy;
  profile::Profile::instance()->ReadBoolValue(&enable_policy, false, "Policy",
                                              "EnablePolicy");
  EXPECT_FALSE(enable_policy);

  // Change config back
  profile::Profile::destroy();
  EXPECT_EQ("smartDeviceLink.ini", Profile::instance()->config_file_name());

  //get default value
  bool launch_hmi;
  profile::Profile::instance()->ReadBoolValue(&launch_hmi, true, "HMI",
                                              "LaunchHMI");
  EXPECT_TRUE(launch_hmi);
}

TEST_F(ProfileTest, CheckReadIntValue) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  int server_port = 0;
  profile::Profile::instance()->ReadIntValue(&server_port, 0, "HMI",
                                             "ServerPort");

  EXPECT_EQ(8088, server_port);
}

TEST_F(ProfileTest, CheckIntContainer) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  bool isread = false;
  std::vector<int> diagmodes_list =
      profile::Profile::instance()->ReadIntContainer("MAIN",
                                                     "SupportedDiagModes",
                                                     &isread);
  EXPECT_TRUE(isread);

  std::vector<int>::iterator diag_mode = std::find(diagmodes_list.begin(),
                                                 diagmodes_list.end(), 0x12);

  // This element doesn't appear in list
  EXPECT_EQ(diag_mode, diagmodes_list.end());

  // List includes 0x01
  diag_mode = std::find(diagmodes_list.begin(), diagmodes_list.end(), 0x01);
  EXPECT_EQ(diag_mode, diagmodes_list.begin());

  // List includes 0x03
  std::vector<int>::iterator element_mode = diagmodes_list.begin();
  element_mode++;
  element_mode++;

  diag_mode = std::find(diagmodes_list.begin(), diagmodes_list.end(), 0x03);
  EXPECT_EQ(diag_mode, element_mode);
}

TEST_F(ProfileTest, CheckVectorContainer) {
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  // Get diag_modes after updating
  const std::vector<uint32_t> &diag_modes = profile::Profile::instance()
      ->supported_diag_modes();

  bool isread = false;
  std::vector<int> diagmodes_list =
      profile::Profile::instance()->ReadIntContainer("MAIN",
                                                     "SupportedDiagModes",
                                                     &isread);
  EXPECT_TRUE(isread);
  // Compare with result of ReadIntContainer
  ASSERT_EQ(diag_modes.size(), diagmodes_list.size());
  bool isEqual = true;
  std::vector<int>::iterator iter = diagmodes_list.begin();

  for (std::vector<uint32_t>::const_iterator it = diag_modes.begin();
      it != diag_modes.end(); it++) {

    if ((uint32_t)(*iter) != (*it)) {
      isEqual = false;
      break;
    }
    iter++;
  }
  EXPECT_TRUE(isEqual);
}

TEST_F(ProfileTest, CheckStringContainer) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
            Profile::instance()->config_file_name());

  bool isread = false;
  std::vector < std::string > diagmodes_list = profile::Profile::instance()
      ->ReadStringContainer("MAIN", "SupportedDiagModes", &isread);
  EXPECT_TRUE(isread);

  std::vector<std::string>::iterator diag_mode =
      std::find(diagmodes_list.begin(), diagmodes_list.end(), "0x12");

  // This element doesn't appear in list
  EXPECT_EQ(diag_mode, diagmodes_list.end());

  // List includes 0x01
  diag_mode = std::find(diagmodes_list.begin(), diagmodes_list.end(), "0x01");
  EXPECT_EQ(diag_mode, diagmodes_list.begin());

  // List includes 0x03
  std::vector<std::string>::iterator element_mode = diagmodes_list.begin();
  element_mode++;
  element_mode++;
  diag_mode = std::find(diagmodes_list.begin(), diagmodes_list.end(), " 0x03");
  EXPECT_EQ(diag_mode, element_mode);
}

#ifdef ENABLE_SECURITY
TEST_F(ProfileTest, CheckIntContainerInSecurityData) {
  // Set new config file
  Profile::instance()->config_file_name("smartDeviceLink_test.ini");
  EXPECT_EQ("smartDeviceLink_test.ini",
      Profile::instance()->config_file_name());

  std::vector<int> force_unprotected_list =
  profile::Profile::instance()->ReadIntContainer(
      "Security Manager", "ForceUnprotectedService", NULL);

  std::vector<int> force_protected_list =
  profile::Profile::instance()->ReadIntContainer(
      "Security Manager", "ForceProtectedService", NULL);

  std::vector<int>::iterator res_unprotect = std::find(force_unprotected_list.begin(), force_unprotected_list.end(), 0x07);
  std::vector<int>::iterator res_protect = std::find(force_protected_list.begin(), force_protected_list.end(), 0x07);
  // This element doesn't appear in both lists
  EXPECT_EQ(res_unprotect, force_unprotected_list.end() );
  EXPECT_EQ(res_protect, force_protected_list.end() );

  // Both lists include 0
  res_unprotect = std::find(force_unprotected_list.begin(), force_unprotected_list.end(), 0);
  res_protect = std::find(force_protected_list.begin(), force_protected_list.end(), 0);
  EXPECT_EQ(res_unprotect, force_unprotected_list.begin() );
  EXPECT_EQ(res_protect, force_protected_list.begin() );
}
#endif

} // namespace profile
} // namespace components
} // namespace test
