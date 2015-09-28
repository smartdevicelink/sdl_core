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
#include "gmock/gmock.h"
#include "custom_string.h"

namespace custom_string = utils::custom_string;

namespace test {
namespace components {
namespace utils {

void CreateMultibyteString(std::string& mbstr) {
  const int kSizeStr = 9;
  uint8_t array[] = {
      0xD0, 0xA2, 0xD0, 0xB5, 0xD1, 0x81, 0xD1, 0x82, 0x0};  //Тест
  char str[kSizeStr];
  memcpy(str, array, kSizeStr);
  mbstr = str;
}

void CreateMultibyteStringWithASCII(std::string& mbstr) {
  const int kSizeStr = 12;
  uint8_t array[] = {0xD0,
                     0xA2,
                     0xD0,
                     0xB5,
                     0xD1,
                     0x81,
                     0xD1,
                     0x82,
                     0x61,
                     0x62,
                     0x63,
                     0x0};  //Тестabc
  char str[kSizeStr];
  memcpy(str, array, kSizeStr);
  mbstr = str;
}

TEST(CustomStringTest,
     AddASCIIStringToCustomString_ExpectCorrectSizeAndIsASCIIStringEQTrue) {
  std::string str("Test string");
  custom_string::CustomString obj(str);
  ASSERT_EQ(str.size(), obj.size());
  ASSERT_EQ(str.length(), obj.length());
  ASSERT_TRUE(obj.is_ascii_string());
}

TEST(CustomStringTest,
     AddEmptyToCustomString_ExpectCorrectSizeAndIsASCIIStringEQTrue) {
  std::string str;
  custom_string::CustomString obj(str);
  ASSERT_EQ(str.size(), obj.size());
  ASSERT_EQ(str.length(), obj.length());
  ASSERT_TRUE(obj.is_ascii_string());
  ASSERT_TRUE(obj.empty());
}

TEST(CustomStringTest,
     AddUTF8StringToCustomString_ExpectCorrectSizeAndIsASCIIStringEQFalse) {
  std::string utf_str;
  CreateMultibyteString(utf_str);
  custom_string::CustomString obj(utf_str);
  ASSERT_EQ(obj.size(), 4);
  ASSERT_EQ(obj.length(), 4);
  ASSERT_FALSE(obj.is_ascii_string());
}

TEST(
    CustomStringTest,
    AddMixingUTF8WithASCIIStringToCustomString_ExpectCorrectSizeAndIsASCIIStringEQFalse) {
  std::string utf_str;
  CreateMultibyteStringWithASCII(utf_str);
  custom_string::CustomString obj(utf_str);
  ASSERT_EQ(obj.size(), 7);
  ASSERT_EQ(obj.length(), 7);
  ASSERT_FALSE(obj.is_ascii_string());
}

TEST(CustomStringTest,
     AddUTF8StringToCustomString_ExpectCorrectConvertingToWString) {
  std::string utf_str;
  CreateMultibyteString(utf_str);
  custom_string::CustomString obj(utf_str);
  std::wstring wstr_first(obj.AsWString());
  std::wstring wstr_second(L"Тест");
  ASSERT_TRUE(wstr_first == wstr_second);
}

TEST(
    CustomStringTest,
    AddMixingUTF8WithASCIIStringToCustomString_ExpectCorrectConvertingToWString) {
  std::string utf_str;
  CreateMultibyteStringWithASCII(utf_str);
  custom_string::CustomString obj(utf_str);
  std::wstring wstr_first(obj.AsWString());
  std::wstring wstr_second(L"Тестabc");
  ASSERT_TRUE(wstr_first == wstr_second);
}

TEST(
    CustomStringTest,
    AddSameMultiByteStringsToCustomString_ExpectCorrectCaseSensetiveComparing) {
  std::string utf_str;
  CreateMultibyteString(utf_str);
  custom_string::CustomString obj(utf_str), obj_1(utf_str);
  ASSERT_TRUE(obj == obj_1);
  ASSERT_TRUE(obj == utf_str);
  ASSERT_EQ(0, obj.compare(utf_str.c_str()));
}

TEST(
    CustomStringTest,
    AddDiferenceMultiByteStringsToCustomString_ExpectCorrectCaseSensetiveComparing) {
  std::string utf_str, utf_str1;
  CreateMultibyteStringWithASCII(utf_str);
  CreateMultibyteString(utf_str1);
  custom_string::CustomString obj(utf_str), obj_1(utf_str1);
  ASSERT_FALSE(obj == obj_1);
  ASSERT_FALSE(obj == utf_str1);
  ASSERT_TRUE(obj.compare(utf_str1.c_str()) != 0);
}

TEST(CustomStringTest,
     AddSameAsciiStringsToCustomString_ExpectCorrectCaseInsensitiveComparing) {
  std::string str("hello"), str1("HellO");
  custom_string::CustomString obj(str), obj_1(str1);
  ASSERT_TRUE(obj.CompareIgnoreCase(obj_1));
  ASSERT_TRUE(obj.CompareIgnoreCase(str1.c_str()));
}

TEST(
    CustomStringTest,
    AddDiferenceAsciiStringsToCustomString_ExpectCorrectCaseInsensitiveComparing) {
  std::string str("hello"), str1("HellO12");
  custom_string::CustomString obj(str), obj_1(str1);
  ASSERT_FALSE(obj.CompareIgnoreCase(obj_1));
  ASSERT_FALSE(obj.CompareIgnoreCase(str1.c_str()));
}

TEST(
    CustomStringTest,
    AddDiferenceMultiByteStringsToCustomString_ExpectCorrectCaseInsensitiveComparing) {
  std::string str, str1;
  CreateMultibyteStringWithASCII(str);
  CreateMultibyteString(str1);
  custom_string::CustomString obj(str), obj_1(str1);
  ASSERT_FALSE(obj.CompareIgnoreCase(obj_1));
  ASSERT_FALSE(obj.CompareIgnoreCase(str1.c_str()));
}

TEST(
    CustomStringTest,
    AddSameMultiByteStringsToCustomString_ExpectCorrectCaseInsensitiveComparing) {
  std::string utf_str, utf_str1;
  CreateMultibyteString(utf_str);
  const int kSizeStr = 9;
  uint8_t array[] = {
      0xD1, 0x82, 0xD0, 0xB5, 0xD1, 0x81, 0xD0, 0xA2, 0x0};  //тесТ
  char str[kSizeStr];
  memcpy(str, array, kSizeStr);
  utf_str1 = str;
  custom_string::CustomString obj(utf_str), obj_1(utf_str1);
  ASSERT_TRUE(obj.CompareIgnoreCase(obj_1));
  ASSERT_TRUE(obj.CompareIgnoreCase(str));
}

}  // namespace utils
}  // namespace components
}  // namespace test
