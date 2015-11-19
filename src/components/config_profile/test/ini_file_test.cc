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
#include "config_profile/ini_file.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace profile {

using namespace ::profile;

TEST(IniFileTest, WriteItemReadItem) {
  // Write line in chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "Chapter";
  const char * item = "Test_item";
  const char * value = "test_value";
  const bool write_result = ini_write_value(fname, chapter, item, value,
                                            INI_FLAG_ITEM_UP_CREA);
  EXPECT_TRUE(write_result);
  char search_value[INI_LINE_LEN] = {0};
  const bool read_result = ini_read_value(fname, chapter, item, search_value);
  const char *res = search_value;

  EXPECT_TRUE(read_result);
  EXPECT_STREQ(res, value);
  EXPECT_TRUE(file_system::DeleteFile("./test_ini_file.ini"));
}

TEST(IniFileTest, WriteItemWithoutValueReadItem) {
  // Write line in chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "Chapter";
  const char * test_item = "Test_item";
  const char * value = "";
  const bool write_result = ini_write_value(fname, chapter, test_item, value,
                                            INI_FLAG_ITEM_UP_CREA);
  EXPECT_TRUE(write_result);

  // Read value from file
  char search_value[INI_LINE_LEN] = {0};
  const bool read_result = ini_read_value(fname, chapter, test_item, search_value);
  const char *res = search_value;
  EXPECT_TRUE(read_result);
  EXPECT_STREQ(res, value);

  EXPECT_TRUE(file_system::DeleteFile("./test_ini_file.ini"));
}

TEST(IniFileTest, WriteSameItemInDifferentChapters) {
  // Write line in chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter1 = "Chapter1";
  const char * test_item = "Test_item";
  const char * value = "test_value";
  const bool write_result = ini_write_value(fname, chapter1, test_item, value,
                                            INI_FLAG_ITEM_UP_CREA);
  EXPECT_TRUE(write_result);
  char search_value[INI_LINE_LEN] = {0};
  const bool read_result = ini_read_value(fname, chapter1, test_item, search_value);
  const char *res = search_value;
  EXPECT_TRUE(read_result);
  EXPECT_STREQ(res, value);

  // Create new chapter and write the same value
  const char *chapter2 = "Chapter2";

  const bool write_result2 = ini_write_value(fname, chapter2, test_item, value,
                                             INI_FLAG_ITEM_UP_CREA);

  EXPECT_TRUE(write_result2);

  char value2[INI_LINE_LEN] = "test_value";
  const bool read_result2 = ini_read_value(fname, chapter2, test_item, value2);
  const char *res2 = value2;
  EXPECT_TRUE(read_result2);
  EXPECT_STREQ(res2, res);
  EXPECT_TRUE(file_system::DeleteFile("./test_ini_file.ini"));
}

TEST(IniFileTest, RewriteItem) {
  // Write line in chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "Chapter";
  const char * item = "Test_item";
  const char * value = "test_value";
  bool write_result = ini_write_value(fname, chapter, item, value,
                                      INI_FLAG_ITEM_UP_CREA);

  EXPECT_TRUE(write_result);
  char search_value[INI_LINE_LEN] = {0};
  bool read_result = ini_read_value(fname, chapter, item, search_value);
  const char *res = search_value;
  EXPECT_TRUE(read_result);
  EXPECT_STREQ(res, value);

  // Write item again
  const char * newvalue = "new_test_value";
  write_result = ini_write_value(fname, chapter, item, newvalue,
                                 INI_FLAG_ITEM_UP_CREA);

  EXPECT_TRUE(write_result);
  char new_search_value[INI_LINE_LEN] = {0};
  read_result = ini_read_value(fname, chapter, item, new_search_value);
  const char *new_res = new_search_value;
  EXPECT_TRUE(read_result);
  EXPECT_STREQ(new_res, newvalue);
  EXPECT_TRUE(file_system::DeleteFile("./test_ini_file.ini"));
}

TEST(IniFileTest, WriteTwoItemsInOneChapter) {
  // Write line in chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "Chapter";
  const char * item = "Test_item";
  const char * value1 = "test_value";

  bool write_result = ini_write_value(fname, chapter, item, value1,
                                      INI_FLAG_ITEM_UP_CREA);
  EXPECT_TRUE(write_result);

  // Write another line in the same chapter
  const char * item2 = "Test_item2";
  const char * value2 = "test_value2";

  write_result = ini_write_value(fname, chapter, item2, value2,
                                 INI_FLAG_ITEM_UP_CREA);
  EXPECT_TRUE(write_result);

  // Search both values
  char search_value[INI_LINE_LEN] = {0};
  bool read_result = ini_read_value(fname, chapter, item, search_value);
  const char *res = search_value;
  EXPECT_TRUE(read_result);
  EXPECT_STREQ(res, value1);

  char search_value2[INI_LINE_LEN] = {0};
  read_result = ini_read_value(fname, chapter, item2, search_value2);
  const char *res2 = search_value2;
  EXPECT_TRUE(read_result);
  EXPECT_STREQ(res2, value2);
  EXPECT_TRUE(file_system::DeleteFile("./test_ini_file.ini"));
}

TEST(IniFileTest, WriteEmptyItemWithValueReadItem) {
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "Chapter";
  const char * test_item = "";
  const char * value = "test_value";
  bool result = ini_write_value(fname, chapter, test_item, value,
                                INI_FLAG_ITEM_UP_CREA);
  EXPECT_FALSE(result);
}

TEST(IniFileTest, WriteEmptyItemWithEmptyValue_ExpectFalse) {
  // Write empty line in chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "Chapter";
  const char * test_item = "";
  const char * value = "";
  bool result = ini_write_value(fname, chapter, test_item, value,
                                INI_FLAG_ITEM_UP_CREA);
  EXPECT_FALSE(result);
}

TEST(IniFileTest, WriteItemInEmptyChapter_ExpectFalse) {
  // Write line in empty chapter
  const char * fname = "./test_ini_file.ini";
  const char *chapter = "";
  const char * test_item = "Test_item";
  const char * value = "test_value";
  bool result = ini_write_value(fname, chapter, test_item, value,
                                INI_FLAG_ITEM_UP_CREA);
  EXPECT_FALSE(result);
}

TEST(IniFileTest,ParseEmptyLine) {
  char line[INI_LINE_LEN] = {0};
  char val[INI_LINE_LEN] = {0};
  const char * tag = "HMI";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_NOTHING, result);
}

TEST(IniFileTest,ParseChapter) {
  const char * line = "[HMI]";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "HMI";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_RIGHT_CHAPTER, result);
}

TEST(IniFileTest,ParseChapterTagEmpty) {
  const char * line = "[HMI]";
  char val[INI_LINE_LEN] = {0};
  char tag[INI_LINE_LEN] = {0};

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_WRONG_CHAPTER, result);
}

TEST(IniFileTest,ParseChapterWithUppercaseTag) {
  const char * line = "[Security Manager]";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "SECURITY MANAGER";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_RIGHT_CHAPTER, result);
}

TEST(IniFileTest,ParseChapterWithLowcaseTag) {
  const char * line = "[Security Manager]";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "Security Manager";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_WRONG_CHAPTER, result);
}

TEST(IniFileTest,ParseWithWrongChapter) {
  const char * line = "[HMI]";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "MAIN";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_WRONG_CHAPTER, result);
}

TEST(IniFileTest,ParseLineWithItem) {
  char line[INI_LINE_LEN] = "LaunchHMI = true";
  char val[INI_LINE_LEN] = "";
  char tag[INI_LINE_LEN] = "LAUNCHHMI";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_RIGHT_ITEM, result);

  char check_val[INI_LINE_LEN] = "true";
  EXPECT_EQ(*check_val, *val);
}

TEST(IniFileTest,ParseLineWithoutItem) {
  const char * line = "LaunchHMI = ";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "LAUNCHHMI";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  const char *res = val;
  const char *check_val="";
  EXPECT_EQ(INI_RIGHT_ITEM, result);
  EXPECT_STREQ(check_val, res);
}

TEST(IniFileTest,ParseLineWithEmptytag) {
  const char * line = "LaunchHMI = true";
  char val[INI_LINE_LEN] = {0};
  char tag[INI_LINE_LEN] = {0};

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_WRONG_ITEM, result);
}

TEST(IniFileTest,ParseLineWithLowcaseTag) {
  const char * line = "LaunchHMI = true";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "LaunchHmi";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  EXPECT_EQ(INI_WRONG_ITEM, result);
}

TEST(IniFileTest,ParseLineWithComment) {
  const char * line = "; [HMI]";
  char val[INI_LINE_LEN] = {0};
  const char * tag = "HMI";

  Ini_search_id result;
  result = ini_parse_line(line, tag, val);
  const char *res = val;
  EXPECT_EQ(INI_REMARK, result);
  EXPECT_STREQ(line, res);
}

}  // namespace profile
}  // namespace components
}  // namespace test
