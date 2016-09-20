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

#include "utils/custom_string.h"
#include <cwchar>
#include <cstdlib>
#include <clocale>
#include <cwctype>
#include <new>
#include <algorithm>
#include <string.h>
#include "utils/logger.h"
#include "utils/macro.h"

namespace {
namespace custom_str = utils::custom_string;

// Calculates amount of characters in UTF string
size_t CalculateLengthOfString(const char* str) {
  size_t length_of_string = 0;
  for (size_t i = 0; str[i] != '\0'; ++i) {
    if ((str[i] & custom_str::kHigestByteOfUTF8Byte2) !=
        custom_str::kByteOfUTF8) {
      ++length_of_string;
    }
  }
  return length_of_string;
}

// Converts string to unicode string.
std::wstring ConvertUTFToWString(const char* str) {
  size_t size = CalculateLengthOfString(str);
  std::vector<wchar_t> wchar_array(size + 1, L'\0');

  std::string current_locale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "");  // system locale
  mbstowcs(&(wchar_array.front()), str, size);
  setlocale(LC_ALL, current_locale.c_str());
  return std::wstring(&(wchar_array.front()));
}

// Converts string to lower case unicode string.
void ConvertWStringToLowerCase(std::wstring& str) {
  const std::string current_locale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "");
  std::transform(str.begin(), str.end(), str.begin(), towlower);
  setlocale(LC_ALL, current_locale.c_str());
}
}

namespace utils {
namespace custom_string {

CustomString::CustomString() : amount_characters_(0), is_ascii_string_(true) {}

CustomString::CustomString(const std::string& str)
    : mb_string_(str), amount_characters_(0), is_ascii_string_(true) {
  InitData();
}

CustomString::CustomString(const char* str)
    : mb_string_(str), amount_characters_(0), is_ascii_string_(true) {
  InitData();
}

CustomString::CustomString(size_t n, char c)
    : mb_string_(n, c), amount_characters_(0), is_ascii_string_(true) {
  InitData();
}

size_t CustomString::size() const {
  return amount_characters_;
}

size_t CustomString::length() const {
  return amount_characters_;
}

size_t CustomString::length_bytes() const {
  return mb_string_.size();
}

bool CustomString::is_ascii_string() const {
  return is_ascii_string_;
}

bool CustomString::empty() const {
  return 0 == amount_characters_;
}

bool CustomString::operator==(const CustomString& str) const {
  return mb_string_ == str.mb_string_;
}

bool CustomString::operator==(const std::string& str) const {
  return mb_string_ == str;
}

CustomString& CustomString::operator=(const char* str) {
  mb_string_ = str;
  InitData();
  return *this;
}

CustomString CustomString::operator+(const CustomString& str) const {
  return *this + str.AsMBString();
}

CustomString CustomString::operator+(const std::string& str) const {
  if (!str.empty()) {
    CustomString result_str(mb_string_ + str);
    return result_str;
  }
  return *this;
}

char CustomString::at(size_t pos) const {
  DCHECK_OR_RETURN((is_ascii_string_ && pos < amount_characters_), '\0');
  return mb_string_.at(pos);
}

int CustomString::compare(const char* str) const {
  return mb_string_.compare(str);
}

int CustomString::compare(const std::string& str) const {
  return mb_string_.compare(str);
}

bool CustomString::CompareIgnoreCase(const CustomString& str) const {
  if (is_ascii_string() && str.is_ascii_string()) {
    return !strcasecmp(c_str(), str.c_str());
  }
  std::wstring wstr_first(ConvertUTFToWString(mb_string_.c_str()));
  std::wstring wstr_second(ConvertUTFToWString(str.c_str()));
  ConvertWStringToLowerCase(wstr_first);
  ConvertWStringToLowerCase(wstr_second);
  return wstr_first == wstr_second;
}

bool CustomString::CompareIgnoreCase(const char* str) const {
  return CompareIgnoreCase(CustomString(str));
}

const char* CustomString::c_str() const {
  return mb_string_.c_str();
}

std::wstring CustomString::ToWString() const {
  return ConvertUTFToWString(mb_string_.c_str());
}

std::string CustomString::AsMBString() const {
  return mb_string_;
}

std::wstring CustomString::ToWStringLowerCase() const {
  std::wstring wstr(ConvertUTFToWString(mb_string_.c_str()));
  ConvertWStringToLowerCase(wstr);
  return wstr;
}

void CustomString::InitData() {
  if (mb_string_.empty()) {
    return;
  }
  amount_characters_ = CalculateLengthOfString(mb_string_.c_str());
  is_ascii_string_ = amount_characters_ == mb_string_.size();
}

}  // namespace custom_string
}  // namespace utils
