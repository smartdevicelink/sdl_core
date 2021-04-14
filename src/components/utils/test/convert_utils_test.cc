
/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "utils/convert_utils.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace utils_test {

using namespace ::utils;

TEST(ConvertUtilsTest, ConvertInt64ToLongLongInt_CorrectValue) {
  int64_t value_to_convert = 42;
  EXPECT_EQ(typeid(long long int),
            typeid(ConvertInt64ToLongLongInt(value_to_convert)));
}

TEST(ConvertUtilsTest, ConvertLongLongIntToInt64_CorrectValue) {
  long long int value_to_convert = 42;
  EXPECT_EQ(typeid(int64_t),
            typeid(ConvertLongLongIntToInt64(value_to_convert)));
}

TEST(ConvertUtilsTest, ConvertUInt64ToLongLongUInt_CorrectValue) {
  uint64_t value_to_convert = 42;
  EXPECT_EQ(typeid(unsigned long long int),
            typeid(ConvertUInt64ToLongLongUInt(value_to_convert)));
}

TEST(ConvertUtilsTest, ConvertLongLongUIntToUInt64_CorrectValue) {
  unsigned long long int value_to_convert = 42;
  EXPECT_EQ(typeid(uint64_t),
            typeid(ConvertLongLongUIntToUInt64(value_to_convert)));
}

TEST(ConvertUtilsTest, ConvertBinaryDataToString_ValidCharacteres_CorrectText) {
  const uint8_t data[] = {'s', 'u', 'c', 'c', 'e', 's', 's'};
  const std::string convertion_result = "success";
  const size_t data_size = 7;
  EXPECT_EQ(convertion_result, ConvertBinaryDataToString(data, data_size));
}

TEST(ConvertUtilsTest,
     ConvertBinaryDataToString_NotValidCharacters_CorrectText) {
  const size_t data_size = 7;
  uint8_t data[data_size];
  data[0] = 0u;
  const std::string is_raw_data = "is raw data";
  EXPECT_EQ(is_raw_data, ConvertBinaryDataToString(data, data_size));
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
