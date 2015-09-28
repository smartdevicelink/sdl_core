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

#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "custom_string.h"

namespace custom_str = utils::custom_string;

namespace test {
namespace components {
namespace utils {

std::string CreateMultibyteString(uint8_t* array, size_t array_size) {
  return std::string(array, array + array_size);
}

class CustomStringTest : public ::testing::TestWithParam<std::string> {
 protected:
  static void SetUpTestCase() {
    const size_t kSizeStr = 8;
    uint8_t array[] = {0xD0,
                       0xA2,
                       0xD0,
                       0xB5,
                       0xD1,
                       0x81,
                       0xD1,
                       0x82};  // Array contains russian word "Тест"
    mbstring1_ = CreateMultibyteString(array, kSizeStr);
    mbstring2_ = mbstring1_ + "abc";
    amount_symbols_mbstring1_ = 4;  // amount of symbols from string mbstring1_
    amount_symbols_mbstring2_ = 7;  // amount of symbols from string mbstring2_
    amount_bytes_mbstring1_ = mbstring1_.size();
    amount_bytes_mbstring2_ = mbstring2_.size();
  }

 public:
  static std::string mbstring1_;  // String contains russian word "Тест"
  static std::string
      mbstring2_;  // String contains russian word with ASCII symbols "Тестabc"
  static size_t amount_symbols_mbstring1_;
  static size_t amount_symbols_mbstring2_;
  static size_t amount_bytes_mbstring1_;
  static size_t amount_bytes_mbstring2_;
};

std::string CustomStringTest::mbstring1_ = "";
std::string CustomStringTest::mbstring2_ = "";
size_t CustomStringTest::amount_symbols_mbstring1_ = 0;
size_t CustomStringTest::amount_symbols_mbstring2_ = 0;
size_t CustomStringTest::amount_bytes_mbstring1_ = 0;
size_t CustomStringTest::amount_bytes_mbstring2_ = 0;

TEST_F(CustomStringTest,
       AddASCIIStringToCustomString_ExpectCorrectSizeAndIsASCIIStringEQTrue) {
  std::string str("Test string");
  custom_str::CustomString obj(str);

  EXPECT_EQ(str.size(), obj.size());
  EXPECT_EQ(str.length(), obj.length());
  EXPECT_EQ(str.size(), obj.length_bytes());
  EXPECT_TRUE(obj.is_ascii_string());
  EXPECT_FALSE(obj.empty());
}

TEST_F(CustomStringTest,
       AddEmptyToCustomString_ExpectCorrectSizeAndIsASCIIStringEQTrue) {
  std::string str;
  custom_str::CustomString obj(str);

  EXPECT_EQ(str.size(), obj.size());
  EXPECT_EQ(str.length(), obj.length());
  EXPECT_EQ(str.size(), obj.length_bytes());
  EXPECT_TRUE(obj.is_ascii_string());
  EXPECT_TRUE(obj.empty());
}

TEST_F(CustomStringTest,
       AddEmptyToCustomString_ExpectCorrectWorkOfMethodAsMBString) {
  std::string str;
  custom_str::CustomString obj(str);
  EXPECT_TRUE(str == obj.AsMBString());
}

TEST_F(CustomStringTest,
       AddASCIIStringToCustomString_ExpectCorrectWorkOfMethodAsMBString) {
  std::string str("Test string");
  custom_str::CustomString obj(str);
  EXPECT_TRUE(str == obj.AsMBString());
}

TEST_F(CustomStringTest,
       AddASCIIStringToCustomString_ExpectCorrectWorkOfAppendOperator) {
  custom_str::CustomString obj("Test string");
  custom_str::CustomString obj1("abc");
  custom_str::CustomString obj_empty;
  std::string str("abc");
  std::string str_empty;
  std::string str_result("Test stringabc");
  std::string str_result1("Test string");
  custom_str::CustomString obj_result;
  obj_result = obj + obj1;
  EXPECT_TRUE(str_result == obj_result.AsMBString());
  obj_result = obj + obj_empty;
  EXPECT_TRUE(str_result1 == obj_result.AsMBString());
  obj_result = obj + str;
  EXPECT_TRUE(str_result == obj_result.AsMBString());
  obj_result = obj + str_empty;
  EXPECT_TRUE(str_result1 == obj_result.AsMBString());
}

TEST_F(CustomStringTest,
       AddEmptyStringToCustomString_ExpectCorrectWorkOfAppendOperator) {
  custom_str::CustomString obj_empty;
  custom_str::CustomString obj("abc");
  custom_str::CustomString obj_empty1;
  std::string str("abc");
  std::string str_empty;
  custom_str::CustomString obj_result;
  obj_result = obj_empty + obj;
  EXPECT_TRUE(str == obj_result.AsMBString());
  obj_result = obj_empty + obj_empty1;
  EXPECT_TRUE(str_empty == obj_result.AsMBString());
  obj_result = obj_empty + str;
  EXPECT_TRUE(str == obj_result.AsMBString());
  obj_result = obj_empty + str_empty;
  EXPECT_TRUE(str_empty == obj_result.AsMBString());
}

TEST_F(
    CustomStringTest,
    AddASCIIAndEmptyStringToCustomString_ExpectCorrectWorkOfCompareOperator) {
  custom_str::CustomString obj_empty;
  custom_str::CustomString obj_empty1;
  custom_str::CustomString obj("abc");
  custom_str::CustomString obj1("abc");
  std::string str_empty;
  std::string str("abc");

  EXPECT_TRUE(obj_empty == obj_empty1);
  EXPECT_FALSE(obj_empty == obj1);
  EXPECT_TRUE(obj_empty == str_empty);
  EXPECT_FALSE(obj_empty == str);
  EXPECT_TRUE(obj == obj1);
  EXPECT_TRUE(obj == str);
  EXPECT_FALSE(obj == str_empty);
}

TEST_F(CustomStringTest,
       AddASCIIAndEmptyStringToCustomString_ExpectCorrectWorkOfCompareMethod) {
  custom_str::CustomString obj_empty;
  custom_str::CustomString obj("abc");
  std::string str_empty;
  std::string str("abc");

  EXPECT_TRUE(obj_empty.compare(str_empty) == 0);
  EXPECT_TRUE(obj_empty.compare(str) != 0);
  EXPECT_TRUE(obj_empty.compare("") == 0);
  EXPECT_TRUE(obj_empty.compare("abc") != 0);
  EXPECT_TRUE(obj.compare(str_empty) != 0);
  EXPECT_TRUE(obj.compare(str) == 0);
  EXPECT_TRUE(obj.compare("") != 0);
  EXPECT_TRUE(obj.compare("abc") == 0);
}

TEST_F(CustomStringTest,
       AddASCIIStringToCustomString_ExpectCorrectWorkOfAtMethod) {
  std::string str("abc");
  custom_str::CustomString obj(str);
  for (size_t i = 0; i < obj.size(); ++i) {
    EXPECT_TRUE(obj.at(i) == str[i]);
  }
}

TEST_F(
    CustomStringTest,
    AddASCIIAndEmptyStringToCustomString_ExpectCorrectWorkOfCompareIgnoreCaseMethod) {
  custom_str::CustomString obj_empty;
  custom_str::CustomString obj_empty1;
  custom_str::CustomString obj("abc");
  custom_str::CustomString obj1("AbC");
  custom_str::CustomString obj2("AbCd");

  EXPECT_TRUE(obj_empty.CompareIgnoreCase(obj_empty1));
  EXPECT_FALSE(obj_empty.CompareIgnoreCase(obj));
  EXPECT_TRUE(obj.CompareIgnoreCase(obj1));
  EXPECT_TRUE(obj.CompareIgnoreCase(obj));
  EXPECT_FALSE(obj.CompareIgnoreCase(obj2));
  EXPECT_FALSE(obj.CompareIgnoreCase(obj_empty));
}

TEST_F(CustomStringTest,
       AddASCIIStringToCustomString_ExpectCorrectWorkOfToWStringMethod) {
  custom_str::CustomString obj("abc");
  std::wstring wstr(L"abc");
  EXPECT_TRUE(wstr == obj.ToWString());
}

TEST_F(CustomStringTest,
       AddUTF8StringToCustomString_ExpectCorrectSizeAndIsASCIIStringEQFalse) {
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  EXPECT_EQ(obj.size(), CustomStringTest::amount_symbols_mbstring1_);
  EXPECT_EQ(obj.length(), CustomStringTest::amount_symbols_mbstring1_);
  EXPECT_EQ(obj.length_bytes(), CustomStringTest::amount_bytes_mbstring1_);
  EXPECT_FALSE(obj.is_ascii_string());
}

TEST_F(
    CustomStringTest,
    AddMixingUTF8WithASCIIStringToCustomString_ExpectCorrectSizeAndIsASCIIStringEQFalse) {
  custom_str::CustomString obj(CustomStringTest::mbstring2_);
  EXPECT_EQ(obj.size(), CustomStringTest::amount_symbols_mbstring2_);
  EXPECT_EQ(obj.length(), CustomStringTest::amount_symbols_mbstring2_);
  EXPECT_EQ(obj.length_bytes(), CustomStringTest::amount_bytes_mbstring2_);
  EXPECT_FALSE(obj.is_ascii_string());
}

TEST_F(
    CustomStringTest,
    AddUTF8StringAndMixingUTF8WithASCIIToCustomString_ExpectCorrectWorkOfMethodAsMBString) {
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  custom_str::CustomString obj1(CustomStringTest::mbstring2_);
  EXPECT_TRUE(CustomStringTest::mbstring1_ == obj.AsMBString());
  EXPECT_TRUE(CustomStringTest::mbstring2_ == obj1.AsMBString());
}

TEST_F(CustomStringTest,
       AddUTF8StringToCustomString_ExpectCorrectConvertingToWString) {
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  custom_str::CustomString obj1(CustomStringTest::mbstring2_);
  std::wstring wstr1(L"Тест");
  std::wstring wstr2(L"Тестabc");
  EXPECT_TRUE(wstr1 == obj.ToWString());
  EXPECT_TRUE(wstr2 == obj1.ToWString());
}

TEST_F(
    CustomStringTest,
    AddSameMultiByteStringsToCustomString_ExpectCorrectCaseSensetiveComparing) {
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  custom_str::CustomString obj_1(CustomStringTest::mbstring1_);
  EXPECT_TRUE(obj == obj_1);
  EXPECT_TRUE(obj == CustomStringTest::mbstring1_);
  EXPECT_EQ(0, obj.compare(CustomStringTest::mbstring1_.c_str()));
  EXPECT_EQ(0, obj.compare(CustomStringTest::mbstring1_));
}

TEST_F(
    CustomStringTest,
    AddDiferenceMultiByteStringsToCustomString_ExpectCorrectCaseSensetiveComparing) {
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  custom_str::CustomString obj1(CustomStringTest::mbstring2_);
  EXPECT_FALSE(obj == obj1);
  EXPECT_FALSE(obj == CustomStringTest::mbstring2_);
  EXPECT_TRUE(obj.compare(CustomStringTest::mbstring2_) != 0);
}

TEST_F(
    CustomStringTest,
    AddDiferenceMultiByteStringsToCustomString_ExpectCorrectCaseInsensitiveComparing) {
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  custom_str::CustomString obj1(CustomStringTest::mbstring2_);
  EXPECT_FALSE(obj.CompareIgnoreCase(obj1));
  EXPECT_FALSE(obj.CompareIgnoreCase(CustomStringTest::mbstring2_.c_str()));
}

TEST_F(
    CustomStringTest,
    AddSameMultiByteStringsToCustomString_ExpectCorrectCaseInsensitiveComparing) {
  const size_t kSizeStr = 8;
  uint8_t array[] = {0xD1,
                     0x82,
                     0xD0,
                     0xB5,
                     0xD1,
                     0x81,
                     0xD0,
                     0xA2};  // String contains russian word "тесТ"
  std::string mbstring = CreateMultibyteString(array, kSizeStr);
  custom_str::CustomString obj(CustomStringTest::mbstring1_);
  custom_str::CustomString obj1(mbstring);
  EXPECT_TRUE(obj.CompareIgnoreCase(obj1));
  EXPECT_TRUE(obj.CompareIgnoreCase(mbstring.c_str()));
}

}  // namespace utils
}  // namespace components
}  // namespace test
